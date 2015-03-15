#ifndef WIDGETNO_H
#define WIDGETNO_H

class WidgetNO;

#include "../f_WidgetInterne.h"
#include "../Switch.h"

class WidgetNO : public f_WidgetInterne
{
    Q_OBJECT

public:
    explicit WidgetNO(QString IOCard, QString Broche, QWidget *parent = 0);
    ~WidgetNO();

    virtual inline  TypeFenetreSup      getType() { return NO; }
    virtual inline  QString             getCommandeSupervision() { return CommandeSupervision; }
    virtual         void                RetourCommande(QByteArray Retour);
    virtual         QByteArray          SimulerCommande(QString Commande);
    virtual         void                Superviser();


private slots:
    void slot_toggled(bool checked);

private:
    QHBoxLayout*    Layout;
    Switch*         Btn_OnOff;
    QLabel*         LB_Retour;

    QString         CommandeSupervision;
    int             RetourOn;
    QStringList     TexteRetour;

};

#endif // WIDGETNO_H
