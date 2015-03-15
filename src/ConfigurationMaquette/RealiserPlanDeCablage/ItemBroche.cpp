//--------------------------------------------------------------------------------------------------
/** @file       ItemBroche
 *  @brief      Représente une broche sur laquelle on peut connecter quelque chose
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
#include <QBrush>
#include <QPainter>
#include <QLinearGradient>
#include <QPen>
#include <QDrag>
#include <QMimeData>
#include <QMenu>
#include <QAction>
#include <QSettings>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMessageBox>

//==== Headers  perso ====
#include "ItemBroche.h"
#include "../f_DefinirActionsEtTests.h"

/**
 * Constructeur
 * @brief ItemBroche::ItemBroche
 * @param NomInterface : Nom de l'interface à laquelle il est rattaché
 * @param Conf : Config generale
 * @param parent : item parent
 */
ItemBroche::ItemBroche(QString NomInterface, QSettings *Conf, QGraphicsItem *parent) :
    QGraphicsItem   (parent),
    NomInterface    (NomInterface),
    AcceptDrag      (NORMAL),
    CapConnect      (0),
    Conf            (Conf)
{
    this->setAcceptDrops(true);
}

/**
 * Dessine la broche, si un capteur est connecté, il affiche la photo du capteur, sinon il affiche un cercle
 * @brief ItemBroche::paint
 * @param painter
 */
void ItemBroche::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
    //Si aucun capteur n'est connecté
    if(CapConnect == 0)
    {
        //On dessine un cercle avec le nom de la broche
        QRectF rect (this->boundingRect());

        //Définition du dégradé
        QPointF         PremierPoint    (0, 0) ;
        QPointF         SecondPoint     (0, TAILLE_ITEM) ;
        QLinearGradient grad            (PremierPoint, SecondPoint) ;
        QColor          PremiereCouleur;
        QColor          SecondeCouleur;
        QString         Texte;

        //Gestion de l'affichage en cas de drag
        switch(AcceptDrag)
        {
        case NORMAL: //Cas normal, sans drag
            PremiereCouleur = QColor(0xFE, 0xFE, 0x98, 0xFF) ;
            SecondeCouleur  = QColor(0xC3, 0xC3, 0x67, 0xFF) ;
            Texte           = Nom;
            break;

        case ACCEPT: //Cas si le drag est acceptable
            PremiereCouleur = QColor(0x98, 0xFE, 0x98, 0xFF) ;
            SecondeCouleur  = QColor(0x62, 0xBA, 0x62, 0xFF) ;
            Texte           = "Oui";
            break;

        case REFUSE: //Cas si le drag est refusé
            PremiereCouleur = QColor(0xFE, 0x98, 0x98, 0xFF) ;
            SecondeCouleur  = QColor(0xBA, 0x62, 0x62, 0xFF) ;
            Texte           = "Non";
            break;
        }

        grad.setColorAt(0, PremiereCouleur);
        grad.setColorAt(1, SecondeCouleur);

        //Définition du brush
        QBrush brush(grad);
        painter->setBrush(brush);

        painter->drawEllipse(rect);

        painter->setPen(QPen());

        painter->drawText(rect, Qt::AlignCenter, Texte);
    }
    else //Sinon on affiche la photo
    {
        painter->drawPixmap(0, 0, this->CapConnect->getPhoto().scaled(this->boundingRect().size().toSize(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

/**
 * Rectangle entourant l'item
 * @brief ItemBroche::boundingRect
 * @return
 */
QRectF ItemBroche::boundingRect() const
{
    return QRectF(0, 0, TAILLE_ITEM, TAILLE_ITEM);
}

/**
 * Gestion de l'evenement d'arrivée d'un drag
 * @brief ItemBroche::dragEnterEvent
 * @param event : l'evenement
 */
void ItemBroche::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    //Si c'est un drag que l'on a initié, on le gere
    if(event->mimeData()->text().left(3) == "Cap")
    {
        event->accept();

        QSettings Config("./IOCard/" + event->mimeData()->text().mid(4) + "/config.ini", QSettings::IniFormat);

        //Si le drag est acceptable ou non sur cette broche
        if(this->TypeBroche.contains(Config.value("PORT/Type").toString()))
        {
            this->AcceptDrag = ACCEPT;
        }
        else
        {
            this->AcceptDrag = REFUSE;
        }

        //On update la vue
        QWidget* viewport = this->scene()->views().at(0)->viewport();
        viewport->update();
    }
    else //Si c'est un drag random, on renvois à la gestion parente
    {
        QGraphicsItem::dragEnterEvent(event);
    }
}

/**
 * Gestion de la sortie sur drag, on repasse en mode normal
 * @brief ItemBroche::dragLeaveEvent
 * @param event : l'evenement
 */
void ItemBroche::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    event->accept();

    this->AcceptDrag = NORMAL;

    QWidget* viewport = this->scene()->views().at(0)->viewport();
    viewport->update();
}

/**
 * Gestion du drop
 * @brief ItemBroche::dropEvent
 * @param event : l'evenement
 */
void ItemBroche::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    //Si c'est un drag que l'on a initié
    if(event->mimeData()->text().left(3) == "Cap" && this->AcceptDrag == ACCEPT)
    {
        //S'il n'y a pas de capteur déjà connecté
        if(CapConnect == 0)
        {
            this->DefinirCapteur(event->mimeData()->text().mid(4));
        }
        else if(QMessageBox::question(0, "Modification", "Voulez vous écraser le capteur déjà installé?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            delete this->CapConnect;
            this->CapConnect = 0;
            this->DefinirCapteur(event->mimeData()->text().mid(4));
        }
    }
    else //Si le drag est initié de façon random par le systeme, on renvois vers la gestion parente
    {
        QGraphicsItem::dragEnterEvent(event);
    }
}

/**
 * Evenement de double click, lance la modification de l'item
 * @brief ItemBroche::mouseDoubleClickEvent
 * @param event
 */
void ItemBroche::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{
    if(this->CapConnect != 0)
    {
        event->accept();
        f_DefinirActionsEtTests f_Modif(this->CapConnect, Conf);

        f_Modif.exec();
    }
}

void ItemBroche::contextMenuEvent(QGraphicsSceneContextMenuEvent * event)
{
    if(CapConnect != 0)
    {
        //création du menu
        QMenu menu;
        menu.addAction("Modifier");
        menu.addAction("Supprimer");
        menu.move(event->screenPos());

        //Exécution du menu et récupération de l'action choisie
        QAction* selectedAction = menu.exec();


        //Traitement de l'action en question
        if(selectedAction != 0)
        {
            if(selectedAction->text() == "Modifier")
            {
                f_DefinirActionsEtTests f_Modif(this->CapConnect, Conf);

                f_Modif.exec();
            }
            else
            {
                if(QMessageBox::question(0, "Suppression", "Etes-vous sûr de supprimer cet élément?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
                {
                    delete CapConnect;
                    CapConnect = 0;

                    this->AcceptDrag = NORMAL;

                    QWidget* viewport = this->scene()->views().at(0)->viewport();
                    viewport->update();
                }
            }
        }

        event->accept();
    }
}

/**
 * Définit le capteur connecté
 * @brief ItemBroche::DefinirCapteur
 * @param Mime
 */
void ItemBroche::DefinirCapteur(QString Mime)
{
    //Création de l'objet capteur
    CapteurActionneur* NouveauCA = new CapteurActionneur(Mime, Nom, NomInterface, "");

    //Si on peut accepter le drop en fonction du type
    if(TypeBroche.contains(NouveauCA->getType()))
    {
        //On l'ajoute réellement et on update la vue
        this->CapConnect = NouveauCA;

        QWidget* viewport = this->scene()->views().at(0)->viewport();
        viewport->update();

        //On lance l'interface de configuration du capteur
        f_DefinirActionsEtTests f_Modif(this->CapConnect, Conf);

        f_Modif.exec();
    }
    else
    {
        delete NouveauCA; //Si le drop n'est pas acceptable, on delete le nouveau capteur
    }
}

/**
 * Setter de l'id
 * @brief ItemBroche::setId
 * @param Id
 */
void ItemBroche::setId(unsigned int Id)
{
    if( Id > 0)
    {
        this->Id = Id;
    }
}

/**
 * Setter du nom
 * @brief ItemBroche::setNom
 * @param Nom
 */
void ItemBroche::setNom(QString Nom)
{
    if(!Nom.isEmpty())
    {
        this->Nom = Nom;
    }
}

/**
 * Setter du numero de broche numérique
 * @brief ItemBroche::setBrocheNum
 * @param BrocheNum
 */
void ItemBroche::setBrocheNum(int BrocheNum)
{
    if(BrocheNum > 0)
        this->BrocheNum = BrocheNum;
}

/**
 * Setter du numero de broche analogique
 * @brief ItemBroche::setBrocheAna
 * @param BrocheAna
 */
void ItemBroche::setBrocheAna(int BrocheAna)
{
    if(BrocheAna >= -1)
        this->BrocheAna = BrocheAna;
}

/**
 * Setter des type de broche
 * @brief ItemBroche::setTypeBroche
 * @param TypeBroche
 */
void ItemBroche::setTypeBroche(QStringList TypeBroche)
{
    this->TypeBroche = TypeBroche;
}

/**
 * Enregistre l'item dans la config
 * @brief ItemBroche::Enregistrer
 */
void ItemBroche::Enregistrer()
{
    if(CapConnect != 0)
    {
        QStringList ListeMot (NomInterface.toUpper().split(" "));
        QString NomGroupe;

        for(register int i = 0; i < ListeMot.length(); i++)
            NomGroupe += ListeMot[i];

        Conf->beginGroup(NomGroupe + "-BROCHE" + QVariant(Id).toString());

        Conf->setValue("Nom", Nom);
        Conf->setValue("Broche_Analogique", BrocheAna);
        Conf->setValue("Broche_Numerique", BrocheNum);
        Conf->setValue("Type", CapConnect->getType());
        Conf->setValue("Iocard", CapConnect->getNom());
        Conf->setValue("Commentaire", QVariant());
        Conf->setValue("ARU", CapConnect->getARU());
        Conf->setValue("Module", CapConnect->getModule());

        QList<TestArduino*> ListeTests (CapConnect->getListeTests());

        QString BrocheCode;
        QString BrocheMot;
        if(CapConnect->getType() != "MOT1")
        {

            if(BrocheAna > -1)
            {
                BrocheCode = QVariant(BrocheAna).toString().rightJustified(2, '0');
            }
            else
            {
                BrocheCode = QVariant(BrocheNum).toString().rightJustified(2, '0');
            }
        }
        else
        {
            if(Nom == "MOTA")
            {
                BrocheCode = "52";
                BrocheMot = "13";
            }
            else if(Nom == "MOTB")
            {
                BrocheCode = "51";
                BrocheMot = "12";
            }
        }

        for(register int i = 0; i < ListeTests.length(); i++)
        {
            Conf->setValue("Test" + QVariant(i+1).toString() + "_Nom", ListeTests[i]->sNomTest);
            Conf->setValue("Test" + QVariant(i+1).toString() + "_Code", ListeTests[i]->sCommande.replace('*', BrocheCode).replace('$', BrocheMot));
            Conf->setValue("Test" + QVariant(i+1).toString() + "_Enable", ListeTests[i]->bActive);
        }

        QList<ActionArduino*> ListeActions (CapConnect->getListeActions());

        for(register int i = 0; i < ListeActions.length(); i++)
        {
            Conf->setValue("Action" + QVariant(i+1).toString() + "_Nom", ListeActions[i]->sNomAction);
            Conf->setValue("Action" + QVariant(i+1).toString() + "_Code", ListeActions[i]->sCommande.replace('*', BrocheCode).replace('$', BrocheMot));
            Conf->setValue("Action" + QVariant(i+1).toString() + "_Enable", ListeActions[i]->bActive);
        }

        Conf->endGroup();
    }
}
