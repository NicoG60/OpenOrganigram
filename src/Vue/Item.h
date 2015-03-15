//  Item.h  1.0     21/02/14        N.Jarnoux
#ifndef ITEM_H
#define ITEM_H

#define TAILLE_H    (130)
#define TAILLE_V    (50)

#define ESP_H   (2*TAILLE_H - 20)
#define ESP_V   (2*TAILLE_V)

class Item ;

//===== Headers standards =====
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsDropShadowEffect>
#include <QKeyEvent>

//=====   Headers Peros   =====
#include "../Control/Instruction.h"

class Item : public QGraphicsItem
{
    public:
        explicit        Item(Instruction *   pInstRepresentee, Item * pItemPec = 0, QGraphicsItem *pParent = 0) ;//Constructeur
        virtual         ~Item();                                                            //Destructeur

                QRectF          boundingRect() const ;                                      //surcharge de boundingRect
                void            setTexte(QString sTexte) ;                                  //Texte à afficher dans l'item
                unsigned int    getIdInst() ;                                               //Accesseur de l'Id de l'instruction représentée
        inline  unsigned int    getIdPrec()             { return pItemPrec->getIdInst() ; } //Accesseur de l'Id de l'item precedent
                TypeCommande    getTypeInst() ;                                             //Retourne le type d'instruction contenue
                void            setItemPrec(Item * pItemPrec) ;                             //Mutateur du pointeur vers l'item precedent
        inline  Item *          getItemPrec()           { return pItemPrec ; }              //Accesseur de l'item precedent
        inline  unsigned int    getValEcartement()      { return nValEcartement ; }         //Accesseur de la valeur de l'écartement par rapport à son parent
        inline  unsigned int    getValAbaissement()     { return nValAbaissement ; }        //Accesseur de la valeur de l'abaissement par rapport à l'item parent
        inline  Instruction*    getInstructionRepresentee() { return pInstRepresentee ; }   //Accesseur vers l'instruction représentée par l'item
                void            setValEcartement(unsigned int nValEcartement) ;             //Mutateur de la valeur de l'écartement
                void            setValAbaissement(unsigned int nValAbaissement) ;           //Mutateur de la valeur d'abaissement
                void            Replacer() ;                                                //Calcul, à partir des coordonnées du parent, ses propre coordonnee
                Item *          getItemParentDecale() ;                                     //Retourne l'item parent ayant provoqué le décalage, renvois 0 sinon

    protected:
        void            contextMenuEvent(QGraphicsSceneContextMenuEvent *   pEvent) ;   //Gestion de l'évenement clic droit
        void            mouseDoubleClickEvent(QGraphicsSceneMouseEvent *    pEvent) ;   //Gestion du double clic
        void            keyPressEvent(QKeyEvent *   pEvent);
        void            mousePressEvent(QGraphicsSceneMouseEvent *   pEvent);            //Evenement de clic de souris
        void            mouseMoveEvent(QGraphicsSceneMouseEvent *    pEvent);            //Evenement de deplacement

        QVariant        itemChange(GraphicsItemChange change, const QVariant &value);   //Est appelé pour notifier d'un changement sur l'objet

        void            GererSelection(QPainter * pPainter) ;                           //Gestion de l'affichage de la selection

        void            Modifier() ;        //Ouvrira une fenetre de modification
        void            Supprimer() ;
        QRectF          rect ;              //Rectangle dans lequel s'affichera l'item
        QString         sTexte ;            //Texte à afficher dnas l'item
        Item *          pItemPrec ;         //L'item précédent celui ci
        QPointF         PosDebutDrag;

        Instruction *   pInstRepresentee ;  //L'instruction representee
        unsigned int    nValEcartement ;    //précise la valeur de l'écartement toujours par rapport à l'item parent (par défaut 1, 2 pour 2 fois l'écartement normal et ainsi de suite...)
        unsigned int    nValAbaissement ;   //précise la valeur d'abaissement d'un item, c'est à dire s'il doit être placé plus bas qu'il ne devrait pour faire correspondre les conditions.


};

#endif // ITEM_H
