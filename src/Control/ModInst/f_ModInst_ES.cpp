//-------------------------------------------------------------------------------
/**
 * @file        f_ModInst_ES.cpp
 * @brief       Fenetre de modification d'une insturction
 *
 * @author      N.Jarnoux
 * @author      STS IRIS, Lycée Nicolas APPERT, ORVAULT (FRANCE)
 * @since       20/03/14
 * @version     1.0
 * @date        20/03/14
 *
 * Classe affichant une interface de modification des propriété d'une instruction "Entrée/Sortie"
 *
 * Fabrication  OpenOrganigram.pro
 *
 * @todo        Tester si la classe est fonctionnelle
 *
 * @bug         /
 */
//-------------------------------------------------------------------------------

//===== Headers standards =====

//=====   Headers Peros   =====
#include "f_ModInst_ES.h"
#include "ui_f_ModInst_ES.h"

/**
 * Constructeur
 *
 * @brief f_ModInst_ES::f_ModInst_ES
 * @param pInstructionAModif    Instruction à modifier
 * @param parent                Widget parent
 */
f_ModInst_ES::f_ModInst_ES(Inst_ES *pInstructionAModif, QWidget *parent) :
    QDialog(parent),
    pInstructionAModif(pInstructionAModif),
    ui(new Ui::f_ModInst_ES)
{
    ui->setupUi(this);

    //On efface la combo
    this->ui->comboBox->clear();

    //On rempli la liste
    QList<ActionArduino>  ListeActionPossible (pInstructionAModif->getRoutine()->getManagerParent()->getListeActionPossible()) ;

    for(register int i = 0; i < ListeActionPossible.length(); i++)
    {
        if(ListeActionPossible[i].bActive)
        {
            this->ui->comboBox->addItem(ListeActionPossible[i].sNomAction);
        }
    }
}

/**
 * Destructeur
 *
 * @brief f_ModInst_ES::~f_ModInst_ES
 */
f_ModInst_ES::~f_ModInst_ES()
{
    delete ui;
}

/**
 * Ascesseur de description
 * @brief f_ModInst_ES::getDescription
 * @return
 */
DescES f_ModInst_ES::getDescription()
{
    DescES Retour ;

    if(this->ui->Grp_Liste->isChecked())
    {
        QList<ActionArduino>  ListeActionPossible (pInstructionAModif->getRoutine()->getManagerParent()->getListeActionPossible()) ;

        for(register int i = 0; i < ListeActionPossible.length(); i++)
        {
            if(ListeActionPossible[i].sNomAction == this->ui->comboBox->currentText())
            {
                Retour.CommandeDemandee = ListeActionPossible[i] ;
            }
        }
    }
    else
    {
        Retour.CommandeDemandee.bActive = true;
        Retour.CommandeDemandee.sCommande = this->ui->lineEdit->text();
        Retour.CommandeDemandee.sNomAction = "Commande : " + this->ui->lineEdit->text();
    }


    return Retour ;
}


/**
 * Exclusion automatique des deux groupbox
 * @brief f_ModInst_ES::on_Grp_Liste_toggled
 * @param arg1
 */
void f_ModInst_ES::on_Grp_Liste_toggled(bool arg1)
{
    this->ui->Grp_Commande->setChecked(!arg1);
}

/**
 * Exclusion automatique des deux groupbox
 * @brief f_ModInst_ES::on_Grp_Commande_toggled
 * @param arg1
 */
void f_ModInst_ES::on_Grp_Commande_toggled(bool arg1)
{
    this->ui->Grp_Liste->setChecked(!arg1);
}
