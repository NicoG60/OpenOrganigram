//AssistantConfiguration.h  1.0 12/12/14    N.Jarnoux
#ifndef ASSISTANTCONFIGURATION_H
#define ASSISTANTCONFIGURATION_H

class AssistantConfiguration;

//===== Headers Standard ====
#include <QSettings>

class AssistantConfiguration
{
public:
    AssistantConfiguration(QString ModifPlan = QString("TempConfigArduino.ini")); //Constructeur
    ~AssistantConfiguration();  //Destructeur

    bool LancerAssistant(); //Lancement de l'assistant

private:
    void Etape1();
    void Etape2();
    void Etape3();
    void Etape4();

    QStringList ListeInterfacesInstall; //Liste des insterfaces installées pour la transition entre Etape 2 & 4
    bool        Status; //Status de l'avancée de l'assistant
    QSettings   Config; //Fichier de configuration pointé
};

#endif // ASSISTANTCONFIGURATION_H
