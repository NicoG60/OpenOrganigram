//-------------------------------------------------------------------------------
/**
 * @file        Vue.cpp
 * @brief       Vue d'une scène
 *
 * @author      N.Jarnoux
 * @author      STS IRIS, Lycée Nicolas APPERT, ORVAULT (FRANCE)
 * @since       21/02/14
 * @version     1.0
 * @date        21/02/14
 *
 * Widget affichant une scène, il ne sert que à l'affichage, c'est la scène qui gèrera les
 * coordonnée de tous les éléments.
 *
 * Fabrication  OpenOrganigram.pro
 *
 * @todo        Tout est à faire, la classe est codée au minimum syndicale pour tester d'autres fonctionnalités
 *
 * @bug         /
 */
//-------------------------------------------------------------------------------

//===== Headers standards =====
#include <QDateTime>
#include <QDebug>

//=====   Headers Peros   =====
#include "Vue.h"
#include "Item/Item_Com.h"
#include "Item/Item_Cond.h"
#include "Item/Item_DebFin.h"
#include "Item/Item_ES.h"
#include "Item/Item_Ope.h"
#include "Item/Item_Rout.h"
#include "Item/Item_Tempo.h"
#include "Item/Item_Boucle.h"

/**
 * Constructeur
 *
 * @brief   Vue::Vue(QObject *pParent = 0)
 * @param   pParent     Pointeur vers l'objet QObject (ou dérivé) parent.
 * @see     Vue::Constructeur()
 */
Vue::Vue(QWidget *pParent) :
    QGraphicsView(pParent),
    RoutineAffichee(0),
    pScene(0)
{
    this->Constructeur();
}

/**
 * Constructeur
 *
 * @brief   Vue::Vue(const Routine *    RoutineAffichee, QObject *pParent = 0)
 * @param   RoutineAffichee Pointeur vers la routine à analyser et à afficher
 * @param   pParent         Pointeur vers l'objet QObject (ou dérivé) parent.
 * @see     Vue::Constructeur()
 */
Vue::Vue(Routine *    RoutineAffichee, QWidget *  pParent) :
    QGraphicsView(pParent),
    RoutineAffichee(RoutineAffichee),
    pScene(0)
{
    this->Constructeur();
}

/**
 * Action commune au deux constructeurs
 *
 * @brief   Vue::Constructeur()
 */
void Vue::Constructeur()
{
    Scene * SceneAffichee   (new Scene(this));

    this->pScene = SceneAffichee ;

    this->acceptDrops() ;
    this->setScene(SceneAffichee);
    this->setRenderHint(QPainter::Antialiasing, true);
    this->setRenderHint(QPainter::TextAntialiasing, true);
    this->setOptimizationFlag(QGraphicsView::DontAdjustForAntialiasing, true);
}

/**
 * Calcul les coordonnée réelle de l'item sur la scène en fonction des coordonnée dans le tableau et
 * refait un rendu de la scene complete.
 *
 * @brief   Vue::getEcartementAbsolu(unsigned int nIdDepart)
 */
unsigned int Vue::getEcartementAbsolu(unsigned int nIdDepart)
{
    unsigned int nIdCourant (nIdDepart) ;
    unsigned int Retour     (0) ;

    while(nIdCourant != 0)
    {
        Item *          pItemCourant    (this->getItem(nIdCourant)) ;
        if(pItemCourant != NULL)
        {
            Retour      +=  pItemCourant->getValEcartement() ;
            nIdCourant  =   pItemCourant->getIdPrec() ;
        }
    }

    return Retour ;
}

/**
 * Raffraichi l'affichage
 *
 * @brief   Vue::Rafraichir()
 */
void Vue::Rafraichir()
{
    //S'il y a des instruction dans la routine
    if(this->RoutineAffichee->getListe().length() != 0)
    {
        //On supprime tout ce qu'il y a sur la scene
        this->pScene->RAZ();

        //On calcul la position des items
        this->CalculerEmplacementItem();

        //Préparation de la boucle de fleche
        QList<Fleche *>         ListeFleche ;

        QList<Instruction *>    ListeInstruction   (this->RoutineAffichee->getListe()) ;
        Instruction *           InstructionCourante (0) ;

        //Pour chaque instruction, on déclare la ou les fleche correspondante
        for(register int i = 0; i < ListeInstruction.length(); i++)
        {
            InstructionCourante = ListeInstruction[i] ;

            QList<unsigned int> ListeIdSuivant  (InstructionCourante->getIdSuivantParSortie()) ;

            if(ListeIdSuivant[0] != 0)
            {
                for(register int j = 0; j < ListeIdSuivant.length(); j++)
                {
                    unsigned int    IdInstCourante  (InstructionCourante->getId());
                    Item *          ItemDepart  (this->getItem(IdInstCourante)) ;
                    Item *          ItemArrivee (this->getItem(ListeIdSuivant[j])) ;

                    if(ItemDepart != NULL && ItemArrivee != NULL)
                    {
                        if(ItemDepart->getTypeInst() == COND && j > 0)
                        {
                            //Si c'est une condition vide
                            if(ItemArrivee->x() <= ItemDepart->x())
                            {
                                unsigned int EcartementMax   (0);

                                for(register int k = 0; k < ListeInstruction.length(); k++)
                                {
                                    Item* ItemCourant   (this->getItem(k));

                                    if(ItemCourant != 0)
                                    {
                                        Item* ItemParent    (ItemCourant->getItemPrec());

                                        if(ItemParent != 0)
                                        {
                                            bool SortieBoucle   (false);
                                            while(ItemParent->x() != ItemDepart->x() && !SortieBoucle)
                                            {
                                                Item* temp(ItemParent->getItemPrec());

                                                if(temp != 0)
                                                {
                                                    ItemParent = temp;
                                                }
                                                else
                                                {
                                                    SortieBoucle = true;
                                                }
                                            }

                                            if(ItemParent != 0)
                                            {
                                                if(ItemParent->y() > ItemDepart->y() && ItemParent->y() < ItemArrivee->y())
                                                {
                                                    unsigned int EcartementAbsolu (this->getEcartementAbsolu(ItemCourant->getIdInst()));
                                                    if(EcartementAbsolu > EcartementMax)
                                                    {
                                                        EcartementMax = EcartementAbsolu;
                                                    }
                                                }
                                            }

                                        }
                                    }

                                }

                                Item_Cond*  Condition   (static_cast<Item_Cond*>(ItemDepart));
                                Condition->setValEcartementSortie(EcartementMax);
                            }

                            ListeFleche.append(new Fleche(ItemDepart, ItemArrivee, true)) ;
                        }
                        else
                        {
                            ListeFleche.append(new Fleche(ItemDepart, ItemArrivee)) ;
                        }
                    }
                    else
                    {
                        qDebug() << "POINTEUR NULL" ;
                    }
                }
            }
        }

        //Une fois la liste établie, on l'affiche
        this->TracerFleche(ListeFleche) ;
    }

}

/**
 * Mutateur de la routine à afficher
 *
 * @brief   Vue::setRoutineAffichee(Routine *    RoutineAffichee)
 */
void Vue::setRoutineAffichee(Routine *    RoutineAffichee)
{
    this->RoutineAffichee = RoutineAffichee ;
}

/**
 * Renvois un pointeur vers l'item représentant l'instruction d'id nIdInst. S'il n'existe pas, il renvois un pointeur null.
 *
 * @brief   Vue::getItem(int nIdInst)
 * @param   nIdInst   Identifiant de l'instruction recherchée.
 */
Item * Vue::getItem(unsigned int nIdInst)
{
    return this->pScene->getItem(nIdInst) ;
}

/**
 * Renvois un pointeur vers un nouvel item créé en fonction du type donné
 *
 * @brief   Vue::newItem(TypeCommande Type)
 * @param   nIdInst   Identifiant de l'instruction recherchée.
 */
Item * Vue::newItem(Instruction *   pInst, Item *  pParent)
{
    Item *  pNewItem    (0) ;
    switch(pInst->getType())
    {
    case OPE :
        pNewItem = new Item_Ope(pInst, pParent) ;
        break ;

    case COND :
        pNewItem = new Item_Cond(pInst, pParent) ;
        break ;

    case ES :
        pNewItem = new Item_ES(pInst, pParent) ;
        break ;

    case ROUT :
        pNewItem = new Item_Rout(pInst, pParent) ;
        break ;

    case TEMPO :
        pNewItem = new Item_Tempo(pInst, pParent) ;
        break ;

    case COM :
        pNewItem = new Item_Com(pInst, pParent) ;
        break ;

    case DEBFIN :
        pNewItem = new Item_DebFin(pInst, pParent) ;
        break ;

    case BOUCLE:
        pNewItem = new Item_Boucle(pInst, pParent) ;
        break ;
    }

    return pNewItem ;
}

/**
 * Calcul l'emplacement de tout les items
 *
 * @brief   Vue::CalculerEmplacementItem(unsigned int nIdDepart, unsigned int nEcartementRelatif)
 * @param   nIdDepart           L'identifiant de départ de placement (pour la récurcivité).
 * @param   nEcartementRelatif  L'écartement relatif au dernier item placé
 */
void Vue::CalculerEmplacementItem(unsigned int nIdDepart/*, unsigned int nEcartementRelatif*/)
{
    //Déclaration et initialisation des variables
    Item *              pItemCourant    (0) ;
    Item *              pItemPrec       (0) ;
    Instruction *       pInstCourante   (0) ;
    Instruction *       pInstPrec       (0) ;
    QList<unsigned int> ListeSuivDuPrec ;
    QList<unsigned int> ListeSuivCourant ;

    pInstCourante       = this->RoutineAffichee->getInstruction(nIdDepart) ;
    pItemPrec           = this->getItem(pInstCourante->getIdPrec()) ;
    ListeSuivCourant    = pInstCourante->getIdSuivantParSortie() ;
    pItemCourant        = this->getItem(nIdDepart) ;

    //Si on a affaire à un nouvel item
    if(pItemCourant == 0 && (pItemPrec != 0 || nIdDepart == 0))
    {
        pItemCourant = this->newItem(pInstCourante, pItemPrec) ;
        this->pScene->AjouterItem(pItemCourant);
    }

    //Si on a reussis à déclarer notre item
    if(pItemCourant != 0)
    {
        //On le replace
        pItemCourant->Replacer();

        //S'il y a un précédent
        if(pItemPrec != 0)
        {
            //On récupere la liste de ses suivant
            pInstPrec           = pItemPrec->getInstructionRepresentee() ;
            ListeSuivDuPrec     = pInstPrec->getIdSuivantParSortie() ;

            bool Verif (true);

            //Et pour chaque item suivant du précédent, on vérifie s'ils sont écarté ou non
            if(ListeSuivDuPrec.length() > 1)
            {
                if(ListeSuivDuPrec[1] == pInstCourante->getId() && ListeSuivDuPrec[0] != ListeSuivDuPrec[1])
                {
                    Verif = false ;
                }
            }

            //On calcul l'écartement
            this->EcarterItem(pItemCourant, Verif);

            //On calcul l'abaissement
            this->AbaisserItem(pItemCourant) ;
        }

       //S'il y a un suivant
        if(ListeSuivCourant[0] != 0)
        {
            //Pour chaque suivant
            for(register int i = ListeSuivCourant.length() - 1; i >= 0; i--)
            {
                //On reprend le même procédé
                this->CalculerEmplacementItem(ListeSuivCourant[i]) ;
            }
        }
    }
}

/**
 * Calcul l'écartement d'un item
 * @brief Vue::EcarterItem
 * @param ItemAEcarte
 * @param bVerif
 */
void Vue::EcarterItem(Item *  ItemAEcarte, bool bVerif)
{
    if(ItemAEcarte != 0)
    {
        //S'il ne s'agis pas que d'une vérification d'écartement et qu'on veut ajouter un ecartement de plus a un item
        if(!bVerif)
        {
            //On calcul l'ecartement
            unsigned int nValEcartement (0) ;

            nValEcartement = ItemAEcarte->getValEcartement() ;

            ItemAEcarte->setValEcartement(nValEcartement + 1) ;


        }

        //On replace l'item dans la scene
        ItemAEcarte->Replacer();

        //On recherche ensuite s'il y a déjà des item dans cette colone
        QPointF         PositionItem    (ItemAEcarte->pos()) ;
        QGraphicsItem * ItemIci (0) ;

        bool SortieBoucle   (false) ;

        while(ItemIci == 0 && PositionItem.y() > 0 && !SortieBoucle)
        {
            QList<QGraphicsItem *>  ListeItemIci    (this->scene()->items(PositionItem)) ;

            for(register int i = 0; i < ListeItemIci.length(); i++)
            {
                if(ListeItemIci[i] != ItemAEcarte)
                {
                    if(ListeItemIci[i] == ItemAEcarte->getItemPrec())
                    {
                        SortieBoucle = true;
                    }
                    else
                    {
                        ItemIci = ListeItemIci[i] ;
                    }
                }
            }

            qreal Y (PositionItem.y()- ESP_V) ;
            PositionItem.setY(Y);
        }

        //S'il y en a, on regarde s'il font parti d'une condition en cours ou finis
        if(ItemIci != 0)
        {
            Item *          pItemConverti       (static_cast<Item *>(ItemIci)) ;

            unsigned int    nIdItemSuivant      (pItemConverti->getInstructionRepresentee()->getIdSuivantParSortie().at(0)) ;
            Item *          pItemSuivant        (this->getItem(nIdItemSuivant)) ;
            QPointF         PositionItemAVirer  (pItemConverti->pos()) ;
            QPointF         PositionItemSuivant  ;

            if(pItemSuivant != 0)
            {
                PositionItemSuivant = pItemSuivant->pos();
            }
            else
            {
                PositionItemSuivant = PositionItemAVirer ;
            }

            //Recherche de la fin de condition
            while(PositionItemSuivant.x() >= PositionItemAVirer.x() && pItemSuivant != 0)
            {
                nIdItemSuivant  = pItemSuivant->getInstructionRepresentee()->getIdSuivantParSortie().at(0) ;
                pItemSuivant    = this->getItem(nIdItemSuivant) ;

                if(pItemSuivant != 0)
                {
                    PositionItemSuivant = pItemSuivant->pos();
                }
            }


            if(pItemSuivant == 0)
            {
                Item *  pItemParentDecale   (pItemConverti->getItemParentDecale()) ;

                this->EcarterItem(pItemParentDecale);
            }
        }

        QList<unsigned int> ListeSuivant    (ItemAEcarte->getInstructionRepresentee()->getIdSuivantParSortie());

        if(ListeSuivant.length() > 1)
        {
            this->EcarterItem(this->getItem(ListeSuivant[1]));
        }
    }

}

/**
 * Renvois la liste des items pointant vers celui passé en paramètre
 * @brief Vue::getItemPointantVers
 * @param pItem
 * @return
 */
QList<Item*> Vue::getItemPointantVers(Item * pItem)
{
    QList<Item*>            Retour ;
    QList<Instruction*>     ListeInst   (this->RoutineAffichee->getListe()) ;

    //Pour chaque item
    for(register int i = 0; i < ListeInst.length(); i++)
    {
        //On récupère la list de ses suivant
        QList<unsigned int> ListeInstSuivant    (ListeInst[i]->getIdSuivantParSortie());

        //Pour chaque suivant
        for(register int j = 0; j < ListeInstSuivant.length(); j++)
        {
            //Si il pointe vers l'item en parametre, on l'ajoute à la liste
            if(ListeInstSuivant[j] == pItem->getIdInst())
            {
                Item *  ItemPointant    (this->getItem(ListeInst[i]->getId())) ;

                if(ItemPointant != 0)
                {
                    if(!Retour.contains(ItemPointant) && ItemPointant->getIdInst() != 1)
                    {
                        Retour.append(ItemPointant);
                    }
                }
            }
        }
    }

    return Retour ;
}

/**
 * Calcul l'abaissement d'un item (cas d'un retour de condition où il y a plus d'instruction d'un côté que de l'autre)
 * @brief Vue::AbaisserItem
 * @param ItemAAbaisser
 */
void Vue::AbaisserItem(Item * ItemAAbaisser)
{
    QList<Item*>    ItemsPointant   (this->getItemPointantVers(ItemAAbaisser)) ;

    if(!ItemsPointant.isEmpty())
    {
        bool    bEstEnDessous   (true) ;

        for(register int i = 0; i < ItemsPointant.length(); i++)
        {
            if(ItemAAbaisser->pos().y() <= ItemsPointant[i]->pos().y())
            {
                bEstEnDessous = bEstEnDessous && false;
            }
        }

        if(!bEstEnDessous)
        {
            unsigned int ValAbaissement (0) ;
            ValAbaissement = ItemAAbaisser->getValAbaissement() ;
            ValAbaissement++;

            ItemAAbaisser->setValAbaissement(ValAbaissement);

            ItemAAbaisser->Replacer();

            this->AbaisserItem(ItemAAbaisser );
        }
    }
}

/**
 * Trace les fleche sur la scene
 * @brief Vue::TracerFleche
 * @param Liste
 */
void Vue::TracerFleche(QList<Fleche *> Liste)
{
    Scene * pScene  (static_cast<Scene *>(this->scene())) ;

    for(register int i = 0; i < Liste.length(); i++)
    {
        pScene->AjouterFleche(Liste[i]);
    }
}

/**
 * Deselectionne tous les items
 * @brief Vue::DeselectionnerItems
 */
void Vue::DeselectionnerItems()
{
    this->scene()->clearSelection();
}
