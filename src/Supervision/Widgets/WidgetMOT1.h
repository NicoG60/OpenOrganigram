#ifndef WIDGETMOT1_H
#define WIDGETMOT1_H

class WidgetMOT1;

#include "../f_WidgetInterne.h"
#include "../Switch.h"

class WidgetMOT1 : public f_WidgetInterne
{
    Q_OBJECT

public:
    explicit WidgetMOT1(QString IOCard, QString Broche, QWidget *parent = 0);
    ~WidgetMOT1();

    virtual inline  TypeFenetreSup      getType() { return NO; }
    virtual inline  QString             getCommandeSupervision() { return CommandeSupervision; }
    virtual         void                RetourCommande(QByteArray Retour);
    virtual         QByteArray          SimulerCommande(QString Commande);
    virtual         void                Superviser();


private slots:

private:
    QHBoxLayout*    Layout;

    QString         CommandeSupervision;
    int             RetourOn;
    QStringList     TexteRetour;

};

#endif // WIDGETMOT1_H
