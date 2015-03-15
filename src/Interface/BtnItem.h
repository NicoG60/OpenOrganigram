//  BtnItem.h       1.0     21/02/14     N.Jarnoux
#ifndef BTNITEM_H
#define BTNITEM_H

class BtnItem ;

//===== Headers standards =====
#include <QPushButton>
#include <QMouseEvent>

//=====   Headers persos  =====
#include "TypeCommun.h"
#include "../f_MainWindow.h"

class BtnItem : public QPushButton
{
    Q_OBJECT
    public:
        explicit    BtnItem(TypeCommande Type, f_MainWindow* f_Principale, QWidget *  parent = 0);  //Constructeur

    protected:
        void        mousePressEvent(QMouseEvent *   pEvent);            //Evenement de clic de souris
        void        mouseMoveEvent(QMouseEvent *    pEvent);            //Evenement de deplacement
        void        mouseReleaseEvent(QMouseEvent * pEvent);

    private:
        TypeCommande    Type;           //Type d'item à afficher
        QPoint          PosDebutDrag;   //Position de début de drag
        f_MainWindow *  f_Principale;   //pointeur vers la fenetre principale

};

#endif // BTNITEM_H
