//--------------------------------------------------------------------------------------------------
/** @file       CapteurActionneur
 *  @brief      Représente un capteur/actionneur
 *
 *  @author     N. Jarnoux
 *  @author     BTS IRIS, Lycée Nicolas APPPERT, ORVAULT (FRANCE)
 *  @since      2014-12-14
 *  @version    1.0
 *  @date       2014-12-14
 *
 *  Classe de gestion de l'IHM pour réaliser le plan de câblage qui consiste a déposer
 *  les capteurs sur tel ou tel prise de la carte easycon ou arduino.
 *
 */
//--------------------------------------------------------------------------------------------------
//=== Headers standards ===
#include <QSettings>

//==== Headers  persos ====
#include "CapteurActionneur.h"

/**
 * Constructeur, charge le capteur dans les attributs de l'objet
 * @brief CapteurActionneur::CapteurActionneur
 * @param Nom : Nom du capteur
 * @param Broche : broche où il branché
 * @param Carte : carte à laquelle il est connecté
 * @param Module : module auquel il appartient
 */
CapteurActionneur::CapteurActionneur(QString Nom, QString Broche, QString Carte, QString Module) :
    Photo   ("./IOCard/" + Nom + "/photo.png"),
    Nom     (Nom),
    Broche  (Broche),
    Carte   (Carte),
    Module  (Module)
{
    QSettings   Config("./IOCard/" + Nom + "/config.ini", QSettings::IniFormat);

    //Le Titre
    this->Titre = Config.value("TITRE/NOM").toString();

    //Tyoe
    this->Type = Config.value("PORT/Type").toString();

    //ARU
    this->ARU = Config.value("ARU/Code").toString();

    QStringList ListeGroupe (Config.childGroups());

    for(register int i = 0; i < ListeGroupe.length(); i++)
    {
        if(ListeGroupe[i].left(4) == "TEST")
        {
            TestArduino* NouveauTest = new TestArduino;

            NouveauTest->sNomTest = Config.value(ListeGroupe[i] + "/Nom").toString();
            NouveauTest->sCommande = Config.value(ListeGroupe[i] + "/Code").toStringList().join(',');
            NouveauTest->bActive = true;

            this->ListeTests.append(NouveauTest);
        }
        else if(ListeGroupe[i].left(6) == "ACTION")
        {
            ActionArduino* NouvelAction = new ActionArduino;

            NouvelAction->sNomAction = Config.value(ListeGroupe[i] + "/Nom").toString();
            NouvelAction->sCommande = Config.value(ListeGroupe[i] + "/Code").toStringList().join(',');
            NouvelAction->bActive = true;

            this->ListeActions.append(NouvelAction);
        }
    }

}

void CapteurActionneur::setTitre(QString Titre)
{
    this->Titre = Titre;
}

void CapteurActionneur::setModule(QString Module)
{
    this->Module = Module;
}

void CapteurActionneur::setARU(QString ARU)
{
    this->ARU = ARU;
}

void CapteurActionneur::AjouterTest(TestArduino* Test)
{
    if(Test != 0)
    {
        this->ListeTests.append(Test);
    }

}

void CapteurActionneur::SupprTest(TestArduino* Test)
{
    if(Test != 0)
    {
        this->ListeTests.removeOne(Test);
        delete Test;
        Test = 0;
    }
}

void CapteurActionneur::AjouterAction(ActionArduino* Action)
{
    if(Action != 0)
    {
        this->ListeActions.append(Action);
    }
}

void CapteurActionneur::SupprAction(ActionArduino* Action)
{
    if(Action != 0)
    {
        this->ListeActions.removeOne(Action);
        delete Action;
        Action = 0;
    }
}
