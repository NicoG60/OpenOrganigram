#include <QSettings>
#include <QDebug>

#include "WidgetMOT1.h"
#include "../f_SubWindow.h"

WidgetMOT1::WidgetMOT1(QString IOCard, QString Broche, QWidget *parent) :
    f_WidgetInterne (IOCard, Broche, parent),
    Layout          (new QHBoxLayout)
{
    //Configuration
    QSettings   Config("./IOCard/" + IOCard + "/config.ini", QSettings::IniFormat);

}

WidgetMOT1::~WidgetMOT1()
{
}

void WidgetMOT1::RetourCommande(QByteArray Retour)
{
}

QByteArray WidgetMOT1::SimulerCommande(QString Commande)
{

}

void WidgetMOT1::Superviser()
{
    EnvoyerDonneesW(CommandeSupervision);
}
