#include "f_AffichageFenetre.h"
#include "ui_f_AffichageFenetre.h"

f_AffichageFenetre::f_AffichageFenetre(QList<f_SubWindow *> ListeFenetre, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::f_AffichageFenetre),
    Model(new QStandardItemModel),
    ListeFenetre(ListeFenetre)
{
    ui->setupUi(this);

    ui->listView->setModel(Model);

    for(register int i = 0; i < ListeFenetre.length(); i++)
    {
        QStandardItem* NouvelItem = new QStandardItem(ListeFenetre[i]->windowTitle());
        NouvelItem->setCheckable(true);

        if(ListeFenetre[i]->isVisible())
            NouvelItem->setCheckState(Qt::Checked);
        else
            NouvelItem->setCheckState(Qt::Unchecked);

        NouvelItem->setData(i, Qt::UserRole);

        Model->appendRow(NouvelItem);
    }

    connect(Model, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(slot_itemChanged(QStandardItem*)));
}

f_AffichageFenetre::~f_AffichageFenetre()
{
    Model->deleteLater();
    delete ui;
}

void f_AffichageFenetre::slot_itemChanged(QStandardItem* item)
{
    if(item->checkState() == Qt::Checked)
        ListeFenetre[item->data(Qt::UserRole).toInt()]->setVisible(true);
    else
        ListeFenetre[item->data(Qt::UserRole).toInt()]->setVisible(false);
}
