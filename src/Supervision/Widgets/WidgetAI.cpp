#include <QSettings>

#include "WidgetAI.h"

WidgetAI::WidgetAI(QString IOCard, QString Broche, QWidget *parent) :
    f_WidgetInterne (IOCard, Broche, parent),
    Layout          (new QHBoxLayout),
    Potar           (new QDial),
    LCD_Retour      (new QLCDNumber),
    LayoutCalcul    (new QHBoxLayout),
    LB_ValCalcul    (new QLabel),
    LB_Unit         (new QLabel)
{
    QSettings   Config("./IOCard/" + IOCard + "/config.ini", QSettings::IniFormat);

    CommandeSupervision = Config.value("SUP/Commande").toString() + Broche.rightJustified(2, '0');

    Expression = Config.value("SUP/Expression").toString();
    QString Inconnue = Config.value("SUP/Inconnue").toString();

    if(!Expression.isEmpty() && !Inconnue.isEmpty())
    {
        Parser.DefineVar(Inconnue.toStdString().c_str(), &val);
        Parser.SetExpr(Expression.toStdString().c_str());
    }

    //Interface
    LCD_Retour->setMinimumSize(100, 25);
    LCD_Retour->setMaximumSize(100, 25);

    Potar->setMinimum(0);
    Potar->setMaximum(255);

    Layout->addStretch();
    Layout->addWidget(Potar);
    Layout->addWidget(LCD_Retour);
    Layout->addStretch();

    LayoutCalcul->addStretch();
    LayoutCalcul->addWidget(LB_ValCalcul);
    LayoutCalcul->addWidget(LB_Unit);
    LayoutCalcul->addStretch();

    LayoutV->addLayout(Layout);
    LayoutV->addLayout(LayoutCalcul);
    LayoutV->addStretch();

    LB_Image->setPixmap(QPixmap("./IOCard/" + IOCard + "/photo.png"));
    LB_Titre->setText(Config.value("TITRE/NOM").toString());
    LB_Unit->setText(Config.value("SUP/Unit").toString());

    connect(Potar, SIGNAL(valueChanged(int)), this, SLOT(slot_valueChange(int)));
}

WidgetAI::~WidgetAI()
{

}

void WidgetAI::RetourCommande(QByteArray Retour)
{
    if(Retour.left(6) == "VALUE=")
    {
        QList<QByteArray> Liste (Retour.split('='));

        Potar->setValue(QString(Liste[1]).toInt());
    }
}

QByteArray WidgetAI::SimulerCommande(QString Commande)
{
    QByteArray retour("VALUE=");

    if(!Commande.isEmpty())
    {
        if(Commande[0] == 'A')
        {
            retour += QVariant(Potar->value()).toString();
        }
        else if(Commande[0] == 'a')
        {
            retour += QVariant(Potar->value()*2).toString();
        }
    }

    return retour;
}

void WidgetAI::Superviser()
{
    EnvoyerDonneesW(CommandeSupervision);
}
void WidgetAI::slot_valueChange(int i)
{
    val = i;

    double aff(0);

    if(!Expression.isEmpty())
        aff = Parser.Eval();
    else
        aff = val;

    LB_ValCalcul->setText(QVariant(aff).toString());
    LCD_Retour->display(i);
}

