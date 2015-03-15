//Photo.h   1.0     14/12/14    N.Jarnoux
#ifndef PHOTO_H
#define PHOTO_H

class Photo;

//=== Headers Standards ===
#include <QGraphicsItem>

class Photo : public QGraphicsItem
{
public:
    Photo(QString Chemin, QGraphicsItem * parent = 0); //Constructeur

    void paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/); //Dessine
    QRectF boundingRect() const;    //Rectangle l'entourant

private:
    QString Chemin; //Chemin vers la photo
};

#endif // PHOTO_H
