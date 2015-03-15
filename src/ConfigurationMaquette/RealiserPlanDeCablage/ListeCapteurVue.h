//ListeCapteurVue.h     1.0     14/12/14    N.Jarnoux
#ifndef LISTECAPTEURVUE_H
#define LISTECAPTEURVUE_H

class ListeCapteurVue;

//=== Headers Standards ===
#include <QListView>

class ListeCapteurVue : public QListView
{
    Q_OBJECT
public:
    explicit ListeCapteurVue(QWidget *parent = 0); //Constructeur

protected:
    void        mousePressEvent(QMouseEvent *   pEvent);            //Evenement de clic de souris
    void        mouseMoveEvent(QMouseEvent *    pEvent);            //Evenement de deplacement

private:
    QPoint      PosDebutDrag; //Position de la souris en début de drag

};

#endif // LISTECAPTEURVUE_H
