//--------------------------------------------------------------------------------------------------
/** @file       ScenePlanDeCablage
 *  @brief      Classe gerant l'affichage de l'éditeur de plan de cablage
 *
 *  @author     N. Jarnoux
 *  @author     BTS IRIS, Lycée Nicolas APPPERT, ORVAULT (FRANCE)
 *  @since      2014-12-14
 *  @version    1.0
 *  @date       2014-12-14
 *
 *  Classe gerant l'affichage de l'éditeur de plan de cablage
 *
 */
//--------------------------------------------------------------------------------------------------
//=== Headers standards ===
#include <QSettings>
#include <QDebug>

//==== Headers  Persos ====
#include "ScenePlanDeCablage.h"

/**
 * Constructeur
 * @brief ScenePlanDeCablage::ScenePlanDeCablage
 * @param Conf : La conf générale
 * @param parent : objet parent
 */
ScenePlanDeCablage::ScenePlanDeCablage(QSettings *Conf, QObject *parent) :
    QGraphicsScene  (parent),
    Interface       (0),
    Conf    (Conf)
{
}

/**
 * Destructeur
 * @brief ScenePlanDeCablage::~ScenePlanDeCablage
 */
ScenePlanDeCablage::~ScenePlanDeCablage()
{
    if(Interface != 0)
    {
        this->removeItem(Interface);
        delete Interface;
        Interface = 0;
    }

    for(register int i = 0; i < ListeBroche.length(); i++)
    {
        if(ListeBroche[i] != 0)
        {
            this->removeItem(ListeBroche[i]);
            delete ListeBroche[i];
            ListeBroche[i] = 0;
        }
    }
}

/**
 * Défniti l'interface à représenter
 * @brief ScenePlanDeCablage::setInterface
 * @param NomInterface
 */
void ScenePlanDeCablage::setInterface(QString NomInterface)
{
    this->NomInterface = NomInterface;

    //Photo Interface
    if(Interface != 0)
    {
        this->removeItem(Interface);
        delete Interface;
        Interface = 0;
    }

    Interface = new Photo("./Interfaces/" + NomInterface);
    this->addItem(Interface);

    //Liste Broche
    for(register int i = 0; i < ListeBroche.length(); i++)
    {
        if(ListeBroche[i] != 0)
        {
            this->removeItem(ListeBroche[i]);
            delete ListeBroche[i];
            ListeBroche[i] = 0;
        }
    }

    //Ouverture de la configuration de l'interface
    QSettings Config("./Interfaces/" + NomInterface + "/config.ini", QSettings::IniFormat);

    QStringList ListeBroche(Config.childGroups());

    for(register int i = 0; i < ListeBroche.length(); i++)
    {
        if(ListeBroche[i].left(6) == "BROCHE")
        {
            ItemBroche* NouvelItem(new ItemBroche(NomInterface, Conf));

            NouvelItem->setId(ListeBroche[i].mid(6).toInt());

            Config.beginGroup(ListeBroche[i]);
            NouvelItem->setNom(Config.value("Nom").toString());
            NouvelItem->setBrocheAna(Config.value("Broche_analogique").toInt());
            NouvelItem->setBrocheNum(Config.value("Broche_numerique").toInt());
            NouvelItem->setTypeBroche(Config.value("Type").toStringList());
            NouvelItem->setX(Config.value("X").toInt());
            NouvelItem->setY(Config.value("Y").toInt());
            Config.endGroup();

            this->addItem(NouvelItem);

            this->ListeBroche.append(NouvelItem);
        }
    }
}

void ScenePlanDeCablage::Enregistrer()
{
    for(register int i = 0; i < ListeBroche.length(); i++)
    {
        ListeBroche[i]->Enregistrer();
    }
}
