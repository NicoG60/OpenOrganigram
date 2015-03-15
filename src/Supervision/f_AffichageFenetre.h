//f_AffichageFenetre    1.0     22/12/2014  N.Jarnoux
#ifndef F_AFFICHAGEFENETRE_H
#define F_AFFICHAGEFENETRE_H

class f_AffichageFenetre;

#include <QDialog>
#include <QStandardItemModel>

#include "f_SubWindow.h"

namespace Ui {
class f_AffichageFenetre;
}

class f_AffichageFenetre : public QDialog
{
    Q_OBJECT

public:
    explicit f_AffichageFenetre(QList<f_SubWindow*> ListeFenetre, QWidget *parent = 0);
    ~f_AffichageFenetre();

private slots:
    void slot_itemChanged(QStandardItem* item);

private:
    Ui::f_AffichageFenetre *ui;
    QStandardItemModel* Model;
    QList<f_SubWindow*> ListeFenetre;
};

#endif // F_AFFICHAGEFENETRE_H
