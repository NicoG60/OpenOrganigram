//=====   En-Têtes Personnels   =====
#include "f_WidgetInterne.h"
#include "f_SubWindow.h"

f_WidgetInterne::f_WidgetInterne(QWidget *parent) :
    QWidget(parent),
    CommandeEtat("")
{

}

/** Permet de remonter l'envoi de commande à f_SubWindow
**/
void f_WidgetInterne::EnvoyerDonneesW(QString Commande)
{
    qobject_cast<f_SubWindow *>(this->parent())->EnvoyerDonneesSubW(Commande);
}

void f_WidgetInterne::setCommandeEtat(QString CommandeEtat)
{
    if(!CommandeEtat.isEmpty())
    {
        this->CommandeEtat = CommandeEtat;
    }
}
