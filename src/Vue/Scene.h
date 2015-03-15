//  Scene.h     1.0     21/02/14        N.Jarnoux
#ifndef SCENE_H
#define SCENE_H

class Scene ;

//===== Headers standards =====
#include <QGraphicsScene>
#include <QGraphicsSceneDragDropEvent>

//=====   Headers Peros   =====
#include "Item.h"
#include "Fleche.h"
#include "MarqueurDrop.h"

class Scene : public QGraphicsScene
{
    Q_OBJECT
    public:
        explicit    Scene(QObject * pParent = 0);                           //Constructeur
                    ~Scene() ;
        Item *      getItem(unsigned int nIdInst) ;                         //Renvois l'item à l'Id correspondant

        void        AjouterItem(Item * ItemAAjouter) ;
        void        AjouterFleche(Fleche * FlecheAAjouter) ;
        void        RAZ() ;

    protected:
        void        dragEnterEvent(QGraphicsSceneDragDropEvent *    pEvent);//Evenement d'entrée de drag
        void        dragLeaveEvent(QGraphicsSceneDragDropEvent *    pEvent);//Evenement de sortie de drag
        void        dragMoveEvent(QGraphicsSceneDragDropEvent * pEvent);    //Evenement de déplacement de drag
        void        dropEvent(QGraphicsSceneDragDropEvent * pEvent);        //Evenemetn de drop

    private:
        QList<Item *>   ListeItems ;    //Liste des items
        QList<Fleche *> ListeFleches ;  //Liste des fleche

        MarqueurDrop *  pMarqueur ;         //Marqueur de drop
        Fleche *        pFlecheMarqueur ;   //Fleche que le marqueur pointe

        void CalculerEmplacementMarqueur(QPointF Position) ;    //Calcul l'emplacement du marqueur
};

#endif // SCENE_H
