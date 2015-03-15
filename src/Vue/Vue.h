//  Vue.h   1.0     21/02/14    N.Jarnoux
#ifndef VUE_H
#define VUE_H

class Vue ;



//===== Headers standards =====
#include <QGraphicsView>

//=====   Headers Peros   =====
#include "../Control/Routine.h"
#include "Item.h"
#include "Scene.h"
#include "Fleche.h"

class Vue : public QGraphicsView
{
    Q_OBJECT
    public:
        explicit                    Vue(QWidget *  pParent = 0) ;                               //Constructeur par défaut
                                    Vue(Routine *   RoutineAffichee, QWidget *  pParent = 0) ;  //Constructeur avec argument

                         void       Constructeur() ;                                            //Actions communes aux constructeurs
                         void       Rafraichir() ;                                              //Rafraichis la vue de la routine.

        inline           Routine *  getRoutineAffichee()    { return RoutineAffichee ; }        //Acesseur de la routine à affichee
                         void       setRoutineAffichee(Routine *    RoutineAffichee) ;          //Mutateur de la routine à afficher
                 unsigned int       getEcartementAbsolu(unsigned int nIdDepert) ;               //Revois l'écartement absolu (par rapport au Depart) de l'item
                         Item *     getItem(unsigned int nIdInst) ;                             //Renvois un pointeur vers l'item d'id nIdInst
                         void       DeselectionnerItems();
    private:
                Routine *       RoutineAffichee ;                                   //Routine à afficher
                Scene *         pScene;
                Item *          newItem(Instruction *   pInst, Item *  pParent) ;   //Renvois un pointeur vers un item du type TYPE
                void            CalculerEmplacementItem(unsigned int nIdDepart = 0) ;  //Calcul l'emplacement des items
                void            EcarterItem(Item *  ItemAEcarte, bool Verif = false) ;  //Ecarte un item ou verifie simplement s'il est bien à la bonne place
                QList<Item*>    getItemPointantVers(Item * pItem) ;                     //Retourne la liste des item pointant vers celui passé en parametre
                void            AbaisserItem(Item * ItemAAbaisser) ;                    //Abaisse un item
                void            TracerFleche(QList<Fleche*> Liste) ;                //Trace la liste de flèche fournie dans la liste

};

#endif // VUE_H
