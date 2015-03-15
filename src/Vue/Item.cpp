//-------------------------------------------------------------------------------
/**
 * @file        Item.cpp
 * @brief       Représentation d'un élément grpahique
 *
 * @author      N.Jarnoux
 * @author      STS IRIS, Lycée Nicolas APPERT, ORVAULT (FRANCE)
 * @since       21/02/14
 * @version     1.0
 * @date        21/02/14
 *
 * Représente un élément graphique, cette classe sera dérivée en classe fille pour différencier tout les
 * types d'items.
 *
 * Fabrication  OpenOrganigram.pro
 *
 * @todo        Tout est à faire, la classe est codée au minimum syndicale pour tester d'autres fonctionnalités
 *
 * @bug         /
 */
//-------------------------------------------------------------------------------

//===== Headers standards =====
#include <QBrush>
#include <QPainter>
#include <QLinearGradient>
#include <QGraphicsView>
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QDrag>
#include <QMimeData>
#include <QApplication>


//=====   Headers Peros   =====
#include "Item.h"
#include "Scene.h"
#include "../Control/Instruction.h"
#include "Item/Item_Cond.h"
#include "../Control/Inst/Inst_Boucle.h"
#include "../Control/Inst/Inst_DebFin.h"
#include "../Control/Inst/Inst_Ope.h"
#include "../Control/Inst/Inst_Cond.h"
#include "../Control/Inst/Inst_ES.h"
#include "../Control/Inst/Inst_Rout.h"
#include "../Control/Inst/Inst_Tempo.h"
#include "../Control/Inst/Inst_Com.h"

/**
 * Constructeur
 *
 * @brief   Item::Item(QGraphicsItem *  pParent = 0)
 * @param   pParent     Pointeur vers l'objet Item (ou dérivé) parent.
 */
Item::Item(Instruction *   pInstRepresentee, Item * pItemPrec, QGraphicsItem *  pParent) :
    QGraphicsItem   (pParent),
    rect            (-TAILLE_H/2, -TAILLE_V/2, TAILLE_H, TAILLE_V),
    sTexte          (""),
    pItemPrec       (pItemPrec),
    pInstRepresentee(pInstRepresentee),
    nValEcartement  (0),
    nValAbaissement (0)
{
    //Parametrage des flags de selection et d'envois de signaux en ca de changement de geometrie
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

    //Application de l'effet d'ombre
    QGraphicsDropShadowEffect * pEffetSelection (new QGraphicsDropShadowEffect) ;

    pEffetSelection->setOffset(0, 0) ;
    pEffetSelection->setBlurRadius(10) ;

    this->setGraphicsEffect(pEffetSelection) ;
    this->setFlag(QGraphicsItem::ItemIsFocusable, true);
}

/**
 * Destructeur
 *
 * @brief   Item::Item()
 */
Item::~Item()
{
}

/**
 * Renvois le rectangle dans lequel doit être contenu tous les points et éléments à afficher. Ce rectangle
 * sert à la vue pour optimiser sont rendu.
 *
 * @brief   Item::boundingRect() const
 * @return  Le rectangle à afficher
 */
QRectF Item::boundingRect() const
{
    return rect;
}

/**
 * Mutateur pour le texte à afficher
 *
 * @brief   Item::setText(QString text)
 * @param   sText    le texte à afficher
 */
void Item::setTexte(QString sTexte)
{
    this->sTexte = sTexte;
}

/**
 * Accesseur indirect de l'id de l'instruction
 *
 * @brief   Item::getIdInst(
 * @see     Instruction
 */
unsigned int Item::getIdInst()
{
    return this->pInstRepresentee->getId() ;
}

TypeCommande Item::getTypeInst()
{
    return this->pInstRepresentee->getType() ;
}

/**
 * Mutateur de l'Id Prec
 *
 * @brief   Item::setItemPrec(Item * pItemPrec)
 * @see     Item
 */
void Item::setItemPrec(Item * pItemPrec)
{
    this->pItemPrec = pItemPrec ;
}

/**
 * Mutateur de l'Id Prec
 *
 * @brief   Item::setValEcartement(unsigned int nValEcartement)
 * @see     Item
 */
void Item::setValEcartement(unsigned int nValEcartement)
{
    if(this->nValEcartement != 0)
    {
        static_cast<Item_Cond *>(this->pItemPrec)->setValEcartementSortie(nValEcartement);
    }

    this->nValEcartement = nValEcartement ;

    if(this->nValAbaissement > 0)
    {
        static_cast<Item_Cond *>(this->pItemPrec)->setValEcartementSortie(nValEcartement);
    }
}

void Item::setValAbaissement(unsigned int nValAbaissement)
{
    this->nValAbaissement = nValAbaissement ;
}

/**
 * Calcul les coordonnée réelle de l'item sur la scène en fonction des coordonnée dans le tableau et
 * refait un rendu de la scene complete.
 *
 * @brief   Item::Replacer()
 * @pre     Doit être inclu dans une scène
 */
void Item::Replacer()
{
    qreal   X   (0) ;
    qreal   Y   (0) ;

    if(this->pItemPrec != 0)
    {
        X = this->pItemPrec->x() + ESP_H * this->nValEcartement ;

        Y = this->pItemPrec->y() + ESP_V + ESP_V * this->nValAbaissement ;
    }

    QPointF NouvellePos(X, Y) ;

    if(NouvellePos != this->pos())
    {
        this->setPos(NouvellePos) ;
        this->itemChange(QGraphicsItem::ItemPositionChange, QVariant());
    }

}

/**
 * Retrouve le dernier item parent décalé
 *
 * @brief   Item::getItemParentDecale()
 * @return  le dernier item parent décalé, renvois 0 s'il n'y en a pas
 */
Item * Item::getItemParentDecale()
{
    Item *  Retour  (0) ;

    if(this->nValEcartement > 0)
    {
        Retour = this ;
    }
    else
    {
        if(this->pItemPrec != 0)
        {
            Retour = this->pItemPrec->getItemParentDecale() ;
        }
    }

    return Retour ;
}

/**
 * Gestion de l'évenement clic droit pour le menu contextuel. Cela affichera un petit menu contenant
 * quelques options telles que "Modifier" ou "Supprimer"
 *
 * @brief   Item::contextMenuEvent(QGraphicsSceneContextMenuEvent * pEvent)
 * @param   pEvent  L'evenemnt concerné
 */
void Item::contextMenuEvent(QGraphicsSceneContextMenuEvent *    pEvent)
{
    //création du menu
    QMenu menu;
    menu.addAction("Modifier");
    menu.addAction("Supprimer");
    menu.move(pEvent->screenPos());

    //Exécution du menu et récupération de l'action choisie
    QAction* selectedAction = menu.exec();


    //Traitement de l'action en question
    if(selectedAction != 0)
    {
        if(selectedAction->text() == "Modifier")
        {
            this->Modifier();
        }
        else
        {
            this->Supprimer();
        }
    }

    pEvent->accept();
}

/**
 * Gestion de l'évenement double clic pour lancer la modification de l'élément
 *
 * @brief   Item::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * pEvent)
 * @param   pEvent  L'evenemnt concerné
 */
void Item::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * pEvent)
{
    this->Modifier();
    pEvent->accept();
}

/**
 * GEstion de l'appui sur une touche du clavier
 * @brief Item::keyPressEvent
 * @param pEvent
 */
void Item::keyPressEvent(QKeyEvent *   pEvent)
{
    //Si 'est la touche suppr, on supprime l'item et instruction
    if(pEvent->key() == Qt::Key_Delete)
    {
        pEvent->accept();
        this->Supprimer();
    }
    else
    {
        //Sinon si c'est la touche entrée, o lance la modif
        if(pEvent->key() == Qt::Key_Return)
        {
            pEvent->accept();
            this->Modifier();
        }
    }
    QGraphicsItem::keyPressEvent(pEvent);
}

/**
 * Gestion du clic de souris
 * @brief Item::mousePressEvent
 * @param pEvent
 */
void Item::mousePressEvent(QGraphicsSceneMouseEvent * pEvent)
{
    //Si on clic gauche dessus, on met a jour la position de drag initiale
    if(pEvent->button() == Qt::LeftButton)
    {
        this->PosDebutDrag = pEvent->pos() ;
    }
    QGraphicsItem::mousePressEvent(pEvent) ;

}

/**
 * Gestion du mouvement de la souris
 * @brief Item::mouseMoveEvent
 * @param pEvent
 */
void Item::mouseMoveEvent(QGraphicsSceneMouseEvent *  pEvent)
{
    QPointF Vecteur  (pEvent->pos() - this->PosDebutDrag) ;
    int Distance    (Vecteur.manhattanLength()) ;

    //Si la distance est suffisante, on démarre la procédure de drag'n drop
    if(Distance > QApplication::startDragDistance() && this->getTypeInst() != DEBFIN)
    {
        QDrag* drag = new QDrag(this->scene()->views().at(0));

        QString     sNomFichier;
        QString     DescBrute;

        Inst_Ope*   Operation   (0);
        Inst_Cond*  Condison    (0);
        Inst_ES*    EntreeSortie(0);
        Inst_Rout*  AppelRoutine(0);
        Inst_Tempo* Tempo       (0);
        Inst_Com*   Commentaire (0);
        Inst_Boucle*  Boucle      (0);

        //En fonction du type d'item, on met l'icone qui va bien à côté de la, on récupere le pointeur vers l'instruction et la description brute en chaine de caractère
        switch(this->getTypeInst())
        {
        case OPE:
            sNomFichier = "iconop.png" ;
            Operation   = static_cast<Inst_Ope*>(this->pInstRepresentee);
            DescBrute   = Instruction::ConvertirVersChaine(Operation->getDesc());
            break;

        case COND:
            sNomFichier = "iconcond.png";
            Condison    = static_cast<Inst_Cond*>(this->pInstRepresentee);
            DescBrute   = Instruction::ConvertirVersChaine(Condison->getDesc());
            break;

        case ES:
            sNomFichier = "icones.png" ;
            EntreeSortie= static_cast<Inst_ES*>(this->pInstRepresentee);
            DescBrute   = Instruction::ConvertirVersChaine(EntreeSortie->getDesc());
            break;

        case ROUT:
            sNomFichier = "iconmeth.png" ;
            AppelRoutine= static_cast<Inst_Rout*>(this->pInstRepresentee);
            DescBrute   = Instruction::ConvertirVersChaine(AppelRoutine->getDesc());
            break;

        case TEMPO:
            sNomFichier = "icontemp.png" ;
            Tempo       = static_cast<Inst_Tempo*>(this->pInstRepresentee);
            DescBrute   = Instruction::ConvertirVersChaine(Tempo->getDesc());
            break;

        case COM:
            sNomFichier = "iconcom.png" ;
            Commentaire = static_cast<Inst_Com*>(this->pInstRepresentee);
            DescBrute   = Instruction::ConvertirVersChaine(Commentaire->getDesc());
            break;

        case BOUCLE:
            sNomFichier = "iconboucle.png" ;
            Boucle      = static_cast<Inst_Boucle*>(this->pInstRepresentee);
            DescBrute   = Instruction::ConvertirVersChaine(Boucle->getDesc());
            break;

        default:
            break;
        }

        //On place l'icon à côté de la souris
        QSize   TailleImageDrag (32, 32) ;
        QIcon   IconBouton      (":/Images/Items/" + sNomFichier) ;
        QPixmap ImageDrag       (IconBouton.pixmap(TailleImageDrag)) ;
        drag->setPixmap(ImageDrag);

        //On place les données MIME à traiter
        QMimeData * mime    (new QMimeData);
        QVariant    TypeItem((int)this->getTypeInst());
        mime->setText(QString("Deplacer ") + TypeItem.toChar() + " " + DescBrute);
        drag->setMimeData(mime);

        //On supprime l'item et on demarre le drag
        this->Supprimer();
        drag->exec();
    }
    else
    {
        QGraphicsItem::mouseMoveEvent(pEvent);
    }
}

/**
 * Gestion de l'affichage de la sélection d'item
 *
 * @brief   Item::GererSelection()
 */
void Item::GererSelection(QPainter * pPainter)
{
    if(this->isSelected())
    {
        QColor  CouleurSelect   (0x00, 0xC0, 0xFF, 0xFF) ;

        QPen    pen(CouleurSelect) ;
        pPainter->setPen(pen) ;
    }
}

/**
 * Gestion du changement de position d'un item
 *
 * @brief   Item::itemChange(GraphicsItemChange change, const QVariant &value)
 */
QVariant Item::itemChange(GraphicsItemChange change, const QVariant &value)
{
    //Si c'est la position d'un item qui change, on replace tous les items qui le suivents
    if(change == QGraphicsItem::ItemPositionChange)
    {
        QList<unsigned int> IdSuivant (this->pInstRepresentee->getIdSuivantParSortie()) ;

        for(register int i = 0; i < IdSuivant.length(); i++)
        {
            Item * pItemSuivant (qobject_cast<Scene *>(this->scene())->getItem(IdSuivant[i])) ;

            if(pItemSuivant != NULL)
            {
                pItemSuivant->Replacer() ;
            }
        }
    }

    return QGraphicsItem::itemChange(change, value) ;
}

/**
 * Lance la modification d'une instruction
 * @brief Item::Modifier
 */
void Item::Modifier()
{
    this->pInstRepresentee->Modifier() ;
    this->setSelected(false);

    if(this->pInstRepresentee->getType() == BOUCLE)
    {
        Inst_Boucle *     pInst_Boucle  (static_cast<Inst_Boucle *>(this->pInstRepresentee)) ;
        unsigned int    nIdPaire    (pInst_Boucle->getIdPaire());

        Vue *           pVue        (qobject_cast<Vue *>(this->scene()->views().at(0))) ;

        Item * pItemPaire   (pVue->getItem(nIdPaire)) ;

        pItemPaire->setSelected(true);
    }
}

/**
 * Supprime un item et l'instruction correspondante
 * @brief Item::Supprimer
 */
void Item::Supprimer()
{
    this->pInstRepresentee->Supprimer();
}
