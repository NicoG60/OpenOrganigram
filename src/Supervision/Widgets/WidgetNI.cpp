#include <QSettings>
#include <QDebug>

#include "WidgetNI.h"
#include "../f_SubWindow.h"

WidgetNI::WidgetNI(QString IOCard, QString Broche, QWidget *parent) :
    f_WidgetInterne (IOCard, Broche, parent),
    Layout          (new QHBoxLayout),
    Btn_OnOff       (new Switch),
    LB_Retour       (new QLabel)
{
    //Configuration
    QSettings   Config("./IOCard/" + IOCard + "/config.ini", QSettings::IniFormat);

    QString     NomGroupeSup            (Config.value("SUP/BtnOn").toString());
    QString     ChaineBrute             (Config.value(NomGroupeSup + "/Code").toString().replace('*', Broche.rightJustified(2, '0')));
    QStringList SeparationCommandeRetour(ChaineBrute.split('='));

    CommandeSupervision = SeparationCommandeRetour[0];
    RetourOn = SeparationCommandeRetour[1].toInt();

    TexteRetour.append(Config.value("SUP/TxtOn").toString());
    TexteRetour.append(Config.value("SUP/TxtOff").toString());

    //Interface

    LB_Retour->setMinimumSize(150, 26);
    LB_Retour->setMaximumSize(150, 26);

    Layout->addStretch();
    Layout->addWidget(Btn_OnOff);
    Layout->addWidget(LB_Retour);
    Layout->addStretch();

    LayoutV->addLayout(Layout);
    LayoutV->addStretch();

    LB_Image->setPixmap(QPixmap("./IOCard/" + IOCard + "/photo.png"));
    LB_Titre->setText(Config.value("TITRE/NOM").toString());
    LB_Retour->setText(TexteRetour[1]);

    connect(Btn_OnOff, SIGNAL(toggled(bool)), this, SLOT(slot_toggled(bool)));
}

WidgetNI::~WidgetNI()
{
}

void WidgetNI::RetourCommande(QByteArray Retour)
{
    if(Retour.left(6) == "VALUE=")
        Btn_OnOff->setChecked(QString(Retour.right(1)) == QVariant(RetourOn).toString());
}

QByteArray WidgetNI::SimulerCommande(QString)
{
    QString     sRetourOn(QVariant(RetourOn).toString());
    QString     sRetourOff;

    if(RetourOn == 1)
        sRetourOff = "0";
    else
        sRetourOff = "1";

    QByteArray  Retour("VALUE=");
    if(Btn_OnOff->isChecked())
        Retour += sRetourOn;
    else
        Retour += sRetourOff;

    return Retour;
}

void WidgetNI::Superviser()
{
    EnvoyerDonneesW(CommandeSupervision);
}

void WidgetNI::slot_toggled(bool checked)
{
    if(checked)
        LB_Retour->setText(TexteRetour[0]);
    else
        LB_Retour->setText(TexteRetour[1]);
}
