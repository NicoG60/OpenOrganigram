#ifndef WIDGETAI_H
#define WIDGETAI_H

class WidgetNI;

#include <QDial>
#include <QLCDNumber>

#include "../f_WidgetInterne.h"
#include "../../Interface/LibParser/muParser.h"


class WidgetAI : public f_WidgetInterne
{
    Q_OBJECT
public:
    explicit WidgetAI(QString IOCard, QString Broche, QWidget *parent = 0);
    ~WidgetAI();

    virtual inline  TypeFenetreSup      getType() { return AI; }
    virtual inline  QString             getCommandeSupervision() { return CommandeSupervision; }
    virtual         void                RetourCommande(QByteArray Retour);
    virtual         QByteArray          SimulerCommande(QString Commande);
    virtual         void                Superviser();

private slots:
    void    slot_valueChange(int i);

private:
    QHBoxLayout*    Layout;
    QDial*          Potar;
    QLCDNumber*     LCD_Retour;

    QHBoxLayout*    LayoutCalcul;
    QLabel*         LB_ValCalcul;
    QLabel*         LB_Unit;

    mu::Parser      Parser;
    QString         Expression;
    double          val;

    QString         CommandeSupervision;
};

#endif // WIDGETAI_H
