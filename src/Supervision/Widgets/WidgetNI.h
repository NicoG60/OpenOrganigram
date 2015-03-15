#ifndef WIDGETNI_H
#define WIDGETNI_H

class WidgetNI;

#include "../f_WidgetInterne.h"
#include "../Switch.h"

class WidgetNI : public f_WidgetInterne
{
    Q_OBJECT

public:
    explicit WidgetNI(QString IOCard, QString Broche, QWidget *parent = 0);
    ~WidgetNI();

    virtual inline  TypeFenetreSup      getType() { return NI; }
    virtual inline  QString             getCommandeSupervision() { return CommandeSupervision; }
    virtual         void                RetourCommande(QByteArray Retour);
    virtual         QByteArray          SimulerCommande(QString);
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

#endif // WIDGETNI_H
