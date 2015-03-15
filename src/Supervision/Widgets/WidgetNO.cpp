#include <QSettings>
#include <QDebug>

#include "WidgetNO.h"
#include "../f_SubWindow.h"

WidgetNO::WidgetNO(QString IOCard, QString Broche, QWidget *parent) :
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
    qDebug() << RetourOn;

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

WidgetNO::~WidgetNO()
{
}

void WidgetNO::RetourCommande(QByteArray Retour)
{
    if(Retour.left(6) == "VALUE=")
        Btn_OnOff->setChecked(QString(Retour.right(1)) == QVariant(RetourOn).toString());
}

QByteArray WidgetNO::SimulerCommande(QString Commande)
{
    QByteArray  Retour;

    if(!Commande.isEmpty())
    {
        if(Commande[0] == 'W')
        {
            if(Commande[3] == '1')
                Btn_OnOff->setChecked(true);
            else
                Btn_OnOff->setChecked(false);

            Retour += QString("DONE " + Commande);
        }
    }

    return Retour;
}

void WidgetNO::Superviser()
{
    EnvoyerDonneesW(CommandeSupervision);
}

void WidgetNO::slot_toggled(bool checked)
{
    if(checked)
    {
        EnvoyerDonneesW("W" + Broche.rightJustified(2, '0') + "1");
        LB_Retour->setText(TexteRetour[0]);
        LB_Image->setPixmap(QPixmap("./IOCard/" + IOCard + "/photoOn.png"));
    }
    else
    {
        EnvoyerDonneesW("W" + Broche.rightJustified(2, '0') + "0");
        LB_Retour->setText(TexteRetour[1]);
        LB_Image->setPixmap(QPixmap("./IOCard/" + IOCard + "/photoOff.png"));
    }
}
