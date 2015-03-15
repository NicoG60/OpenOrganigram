//-------------------------------------------------------------------------------
/**
 * @file        BtnItem.cpp
 * @brief       Bouton pour inserer des éléments graphiques d'organigramme
 *
 * @author      N.Jarnoux
 * @author      STS IRIS, Lycée Nicolas APPERT, ORVAULT (FRANCE)
 * @since       21/02/14
 * @version     1.0
 * @date        21/02/14
 *
 * Bouton pour inserer des éléments grpahiques d'organigramme. Ce bouton est hérité du QPushButton
 * classique pour une gestion plus poussée des événements et pour déclencher correctement le drag'n drop.
 *
 * Fabrication  OpenOrganigram.pro
 *
 * @todo        Définir les MIME Data pour le drag'n drop => Binaire
 *
 * @bug         /
 */
//-------------------------------------------------------------------------------

//===== Headers standards =====
#include <QDebug>

//=====   Headers Peros   =====
#include "InterpreteurFichierIni.h"

/**
 * Constructeur
 * @brief InterpreteurFichierIni::InterpreteurFichierIni
 * @param Chemin    Chemin vers le fichier ini
 * @param pArduino  Connexion à arduino
 * @param parent    Objet parent
 */
InterpreteurFichierIni::InterpreteurFichierIni(QString Chemin, Arduino* pArduino, QObject *parent) :
    QObject     (parent),
    pArduino    (pArduino),
    FichierIni  (Chemin, QSettings::IniFormat),
    bAttente    (false),
    bErreur     (false)
{

    connect(&Timer, SIGNAL(timeout()), this, SLOT(slot_timeout()));
    connect(pArduino, SIGNAL(RetourCommandeGenerale(QByteArray)), this, SLOT(RetourCommande(QByteArray)));
}

/**
 * Destructeur
 * @brief InterpreteurFichierIni::~InterpreteurFichierIni
 */
InterpreteurFichierIni::~InterpreteurFichierIni()
{
}

/**
 * Interprete le fichier ini
 * @brief InterpreteurFichierIni::Interpreter
 */
void InterpreteurFichierIni::Interpreter()
{
    this->bAttente = false;

    //On récupère la liste des groupes
    QStringList ListeGroupe (FichierIni.childGroups());

    //Pour chaque groupe
    for(register int i = 0; i < ListeGroupe.length(); i++)
    {
        //S'il s'agis d'une description de connexion
        if(ListeGroupe[i].left(4) == "EASY" || ListeGroupe[i].left(7) == "ARDUINO")
        {
            //On récupere le type et le numero des broches
            QString     Type        (FichierIni.value(ListeGroupe[i] + "/Type").toString());
            QVariant    ValeurBrute (FichierIni.value(ListeGroupe[i] + "/Broche_Numerique"));

            //On split pour avoir la liste des broches
            QStringList ListeBroche (ValeurBrute.toStringList());

            //Pour chaque broche
            for(register int j = 0; j < ListeBroche.length(); j++)
            {
                //En fonction du type, on configure convenablement la maquette
                QString     CommandeCourante    ("E" + ListeBroche[j].rightJustified(2, '0'));
                if(Type == "NO")
                {
                    CommandeCourante += "1";
                }
                if(Type == "NI")
                {
                    CommandeCourante += "2";
                }
                if(Type == "AI")
                {
                    CommandeCourante += "3";
                }
                if(Type == "PWM")
                {
                    CommandeCourante += "4";
                }
                if(Type == "SRV")
                {
                    CommandeCourante += "5";
                }
                if(Type == "TC")
                {
                    CommandeCourante += "6";
                }
                if(Type.left(3) == "MOT")
                {
                    if(ListeBroche[j][0] == '1')
                    {
                        CommandeCourante += "4";
                    }
                    else if(ListeBroche[j][0] == '5')
                    {
                        CommandeCourante += "1";
                    }
                }
                if(Type != "I2C")
                {
                    ListeCommandeConfig.append(CommandeCourante);
                }

            }

        }
    }
    //On ajoute la commande de réinitialisation
    ListeCommandeConfig.append("Z");
    Timer.start(1); //On démarre le timer pour  exec toutes les commandes
}

/**
 * A chaque déclenchement du timer, exécute une commande
 * @brief InterpreteurFichierIni::slot_timeout
 */
void InterpreteurFichierIni::slot_timeout()
{
    //Si on est pas en attente
    if(!bAttente)
    {
        Timeout     = 0;
        //Si la liste de commande restante est supérieure à 1
        if(ListeCommandeConfig.length() > 1)
        {
            //On pop la premiere, on l'envois et si elle marche, on passe à la suite sinon, on a une erreur
            if(pArduino->EnvoyerDonnees(this->ListeCommandeConfig.takeFirst(), GEN))
            {
                bAttente    = true;
                Timeout     = 0;
            }
            else
            {
                bErreur     = true;
            }
        }
        else    //Quand on arrive à la derniere, c'est que c'est la réinitialisation de la maquette. on l'envois et on arrete le timer et on déclenche le signal de fin d'interpretation.
        {
            pArduino->EnvoyerDonnees(this->ListeCommandeConfig.takeFirst(), GEN);
            Timer.stop();
            emit FinConfigMaquette(this, !bErreur);
        }
    }
    else    //Si on attent un réponse on compte jusqu'à 1 seconde, après on abandonne la commande.
    {
        Timeout++;

        if(Timeout > 1000)
        {
            Timeout     = 0;
            bAttente    = false;
            bErreur     = true;
            qDebug() << "Timeout...";
            pArduino->AnnulerDerniereCommande(GEN);
        }
    }
}

/**
 * Retour de commande arduino
 * @brief InterpreteurFichierIni::RetourCommande
 * @param Retour
 */
void InterpreteurFichierIni::RetourCommande(QByteArray Retour)
{
    qDebug() << "INTERPRETEUR INI : " << Retour;
    if(Retour.left(4) == "DONE")
    {
        bAttente = false;
    }
}
