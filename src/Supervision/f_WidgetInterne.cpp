//=====   En-Têtes Personnels   =====
#include "f_WidgetInterne.h"
#include "f_SubWindow.h"

f_WidgetInterne::f_WidgetInterne(QString IOCard, QString Broche, QWidget *parent) :
    QWidget     (parent),
    LayoutV     (new QVBoxLayout),
    LB_Image    (new QLabel),
    LB_Titre    (new QLabel),
    IOCard      (IOCard),
    Broche      (Broche)
{
    this->setLayout(LayoutV);

    QHBoxLayout* LayoutImage    (new QHBoxLayout);
    LayoutImage->addStretch();
    LayoutImage->addWidget(LB_Image);
    LayoutImage->addStretch();

    LayoutV->addLayout(LayoutImage);
    LayoutV->addWidget(LB_Titre);
}

/** Permet de remonter l'envoi de commande à f_SubWindow
**/
void f_WidgetInterne::EnvoyerDonneesW(QString Commande)
{
    qobject_cast<f_SubWindow *>(this->parent())->EnvoyerDonneesSubW(Commande);
}
