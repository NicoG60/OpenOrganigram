//--------------------------------------------------------------------------------------------------
/** @file       AssistantConfiguration.cpp
 *  @brief      Classe pour gerer l'assistant de création de projet
 *
 *  @author     Nicolas Jarnoux
 *  @author     BTS IRIS, Lycée Nicolas APPPERT, ORVAULT (FRANCE)
 *  @since      2014-02-21
 *  @version    1.0
 *  @date       2014-12-12
 *
 *  Classe gerant la bonne exécution de l'assistant de configuration de projet.
 *  Elle devra configurer un nouveau projet ou modifier une configuration existante.
 *
 */
//--------------------------------------------------------------------------------------------------
//=== Headers standards ===
#include <QFile>

//===   Headers persos  ===#
#include "AssistantConfiguration.h"
#include "f_ChoixControleur.h"
#include "f_ChoixInterface.h"
#include "f_ChoixModules.h"
#include "f_RealiserPlanCablage.h"

/**
 * Constructeur de l'assistant
 * @brief AssistantConfiguration::AssistantConfiguration
 * @param ModifPlan : Chemin vers le fichier à modifier. Si il s'agis du fichier Temp, c'est une nouvelle conf
 */
AssistantConfiguration::AssistantConfiguration(QString ModifPlan) :
    Status(true),
    Config(ModifPlan, QSettings::IniFormat)
{
    if(ModifPlan == "TempConfigArduino.ini")
        Config.clear();
}

/**
 * Destructeur
 * @brief AssistantConfiguration::~AssistantConfiguration
 */
AssistantConfiguration::~AssistantConfiguration()
{
}

/**
 * Lance les étapes de l'assistant
 * @brief AssistantConfiguration::LancerAssistant
 * @return True si tout s'est bien passé, False si il a été annulé
 */
bool AssistantConfiguration::LancerAssistant()
{
    this->Etape1();

    Config.sync();
    return this->Status;
}

void AssistantConfiguration::Etape1()
{
    int Reponse(0);

    f_ChoixControleur Etape1(&(this->Config));

    Reponse = Etape1.exec();

    if(Reponse == QDialog::Accepted)
    {
        this->Status = true;
        this->Etape2();
    }
    else
    {
        this->Status = false;
    }
}

void AssistantConfiguration::Etape2()
{
    int Reponse(0);

    f_ChoixInterface Etape2(&(this->Config), this->ListeInterfacesInstall);

    Reponse = Etape2.exec();

    switch(Reponse)
    {
    case QDialog::Accepted:
        this->ListeInterfacesInstall = Etape2.getListeInterfaceInstall();
        this->Status = true;
        this->Etape3();
        break;

    case QDialog::Rejected:
        this->Status = false;
        break;

    case 2:
        this->Etape1();
        break;
    }
}

void AssistantConfiguration::Etape3()
{
    int Reponse(0);

    f_ChoixModules Etape3(&(this->Config));

    Reponse = Etape3.exec();

    switch(Reponse)
    {
    case QDialog::Accepted:
        this->Status = true;
        this->Etape4();
        break;

    case QDialog::Rejected:
        this->Status = false;
        break;

    case 2:
        this->Etape2();
        break;
    }
}

void AssistantConfiguration::Etape4()
{
    int Reponse(0);

    f_RealiserPlanCablage Etape4(&(this->Config), this->ListeInterfacesInstall);

    Reponse = Etape4.exec();

    switch(Reponse)
    {
    case QDialog::Accepted:
        this->Status = true;
        break;

    case QDialog::Rejected:
        this->Status = false;
        break;

    case 2:
        this->Etape3();
        break;
    }
}
