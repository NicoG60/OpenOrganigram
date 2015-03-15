//CapteurActionneur.h   1.0     14/12/14    N.Jarnoux
#ifndef CAPTEURACTIONNEUR_H
#define CAPTEURACTIONNEUR_H

class CapteurActionneur;

//=== Headers standards ===
#include <QString>
#include <QPixmap>

//==== Headers  persos ====
#include "../../TypeCommun.h"

class CapteurActionneur
{
public:
    CapteurActionneur(QString Nom, QString Broche, QString Carte, QString Module);  //Constructeur

    inline QPixmap getPhoto()   { return Photo; }   //Accesseur de la photo
    inline QString getNom()     { return Nom; }     //Accesseur du nom
    inline QString getTitre()   { return Titre; }   //Accesseur du titre
    inline QString getBroche()  { return Broche; }  //Accesseur de la broche
    inline QString getType()    { return Type; }    //Accesseur du type
    inline QString getCarte()   { return Carte; }   //Accesseur de la carte ou il est branché
    inline QString getModule()  { return Module; }  //Accesseur du module
    inline QString getARU()     { return ARU; }     //Accesseur d'arret d'urgence
    inline QList<TestArduino*>   getListeTests()     { return ListeTests; }     //Accesseur de la liste de tests
    inline QList<ActionArduino*> getListeActions()   { return ListeActions; }   //Accesseur de la liste d'actions

    void    setTitre(QString Titre);
    void    setModule(QString Module);
    void    setARU(QString ARU);
    void    AjouterTest(TestArduino* Test);
    void    SupprTest(TestArduino* Test);
    void    AjouterAction(ActionArduino* Action);
    void    SupprAction(ActionArduino* Action);

private:
    QPixmap Photo;

    QString Nom;
    QString Titre;
    QString Broche;
    QString Type;
    QString Carte;
    QString Module;
    QString ARU;

    QList<TestArduino*>      ListeTests;
    QList<ActionArduino*>    ListeActions;
};

#endif // CAPTEURACTIONNEUR_H
