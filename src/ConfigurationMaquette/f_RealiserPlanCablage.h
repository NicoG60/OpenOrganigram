//f_RealiserPlanCablage.h  1.0    2014-02-21  N. Jarnoux
#ifndef f_RealiserPlanCablage_H
#define f_RealiserPlanCablage_H
class f_RealiserPlanCablage ;

//=== Headers standards ===
#include <QDialog>
#include <QStandardItemModel>
#include <QSettings>


//===   Headers persos  ===
#include "RealiserPlanDeCablage/ListeCapteurVue.h"
#include "RealiserPlanDeCablage/ScenePlanDeCablage.h"

namespace Ui {
class f_RealiserPlanCablage ;
}

class f_RealiserPlanCablage : public QDialog
{
    Q_OBJECT

    public:
            explicit f_RealiserPlanCablage(QSettings* Config, QStringList ListeInterfaceInstall, QWidget *parent = 0) ;   // Constructeur
            ~f_RealiserPlanCablage() ;                              // Destructeur

private slots:
    void on_CB_Interface_currentIndexChanged(int index);

    void on_Btn_Prec_clicked();

    void on_Btn_Suiv_clicked();

private:
            Ui::f_RealiserPlanCablage * ui ;        // Interface graphique
            ListeCapteurVue*            ListeCap;   //Liste des capteur

            QSettings*                  Config;     //Pointeur vers la config

            QList<QStandardItemModel*>  Models;     //Liste des modeles listant les capteur
            QList<ScenePlanDeCablage*>  Scenes;     //Liste des scene affichant le plan


};

#endif // f_RealiserPlanCablage_H
