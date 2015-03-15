//ScenePlanDeCablage.h  1.0     14/12/14    N.Jarnoux
#ifndef SCENEPLANDECABLAGE_H
#define SCENEPLANDECABLAGE_H

class ScenePlanDeCablage;

//=== Hearders Standards ===
#include <QGraphicsScene>
#include <QGraphicsSceneDragDropEvent>
#include <QSettings>

//==== Headers  Persos ====
#include  "Photo.h"
#include "ItemBroche.h"

class ScenePlanDeCablage : public QGraphicsScene
{
    Q_OBJECT
public:
    ScenePlanDeCablage(QSettings* Conf, QObject* parent = 0); //Constructeur
    ~ScenePlanDeCablage(); //Destructeur

    void    setInterface(QString NomInterface); //Mutateur de l'interface à afficher

    void    Enregistrer();


private:
    Photo*              Interface;  //Photo de l'interface
    QString             NomInterface;
    QList<ItemBroche*>  ListeBroche;//Liste des broches possibles
    QSettings* Conf;                //Config generale
};

#endif // SCENEPLANDECABLAGE_H
