//-------------------------------------------------------------------------------
/**
 * @file        f_SaisieNom.cpp
 * @brief       Boite de dialogue de saisie de nom
 *
 * @author      N.Jarnoux
 * @author      STS IRIS, Lycée Nicolas APPERT, ORVAULT (FRANCE)
 * @since       21/02/14
 * @version     1.0
 * @date        21/02/14
 *
 * Petite boite de dialogue demandant l'entrée d'une chaine de caractère.
 *
 * Fabrication  OpenOrganigram.pro
 *
 * @todo        Terminée et fonctionnelle. Utilisable par quiconque aura besoin de cette fonction.
 *
 * @bug         /
 */
//-------------------------------------------------------------------------------

#include "f_SaisieNom.h"
#include "ui_f_SaisieNom.h"


/**
 * Constructeur par défaut de la fenêtre, il a des valeurs par défaut de titre et de texte et est parfaitement utilisable.
 *
 * @brief   f_SaisieNom::f_SaisieNom
 * @param   parent Pointeur vers l'objet QObject ou dérivé parent.
 */
f_SaisieNom::f_SaisieNom(QWidget *pParent) :
    QDialog (pParent),
    ui      (new Ui::f_SaisieNom)
{
    this->ui->setupUi(this) ;
}

/**
 * Constructeur par défaut de la fenêtre, il a des valeurs par défaut de titre et de texte et est parfaitement utilisable.
 *
 * @brief   f_SaisieNom::f_SaisieNom
 * @param   Titre   Chaine de caractère pour le titre de la fenêtre
 * @param   Texte   Chaine de caractère se plaçant juste au dessus du champs de saise
 * @param   parent  Pointeur vers l'objet QObject ou dérivé parent.
 */
f_SaisieNom::f_SaisieNom(QString sTitre, QString sTexte, QWidget *  pParent) :
    QDialog (pParent),
    ui      (new Ui::f_SaisieNom)
{
    this->ui->setupUi(this) ;

    this->setWindowTitle(sTitre) ;
    this->ui->Lb_Texte->setText(sTexte) ;
}

/**
 * Destructeur de la fenêtre.
 *
 * @brief f_SaisieNom::~f_SaisieNom
 */
f_SaisieNom::~f_SaisieNom()
{
    delete this->ui ;
}

/**
 * Retourne la valeur saisie par l'utilisateur dans lke LineEdit.
 *
 * @brief   f_Saison::getSaisie
 * @return  Retourne la valeur de la saise
 */
QString f_SaisieNom::getSaisie()
{
    return this->ui->LE_Saisie->text() ;
}
