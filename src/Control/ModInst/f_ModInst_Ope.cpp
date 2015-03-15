//-------------------------------------------------------------------------------
/**
 * @file        f_ModInst_Ope.cpp
 * @brief       Fenetre de modification d'une insturction
 *
 * @author      N.Jarnoux
 * @author      STS IRIS, Lycée Nicolas APPERT, ORVAULT (FRANCE)
 * @since       04/03/14
 * @version     1.0
 * @date        04/03/14
 *
 * Classe affichant une interface de modification des propriété d'une instruction "Opération sur une variable"
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
#include "f_ModInst_Ope.h"
#include "ui_f_ModInst_Ope.h"

/**
 * Constructeur
 *
 * @brief   f_ModInst_Ope::f_ModInst_Ope(Inst_Ope * InstructionAModif, QWidget *    parent)
 * @param   InstructionAModif   Instruction à modifier
 * @param   parent              vers le widget parent
 */
f_ModInst_Ope::f_ModInst_Ope(Inst_Ope * InstructionAModif, QWidget *    parent) :
    QDialog             (parent),
    InstructionAModif   (InstructionAModif),
    ui                  (new Ui::f_ModInst_Ope)
{
    this->ui->setupUi(this) ;

    int nIndexCBVar  (InstructionAModif->getIndiceVariable()) ;
    this->ui->CB_Variable->setCurrentIndex(nIndexCBVar) ;

    switch(InstructionAModif->getOperation())
    {
    case AFFECTER :
        this->ui->Radio_Affecter->setChecked(true) ;
        break ;

    case INCREMENTER :
        this->ui->Radio_Inc->setChecked(true) ;
        break ;

    case DECREMENTER :
        this->ui->Radio_Dec->setChecked(true) ;
        break ;

    case AJOUTER :
        this->ui->Radio_Ajouter->setChecked(true) ;
        break ;

    case SOUSTRAIRE :
        this->ui->Radio_Soustraire->setChecked(true) ;
        break ;
    }
}

/**
 * Destructeur de la classe, il met à jour l'instruction avant la destruction
 *
 * @brief   f_ModInst_Ope::~f_ModInst_Ope()
 */
f_ModInst_Ope::~f_ModInst_Ope()
{
    delete ui;
}

/**
 * Retourne la description de l'operation. C'est à dire la valeur de tous les paramètres necessaires.
 *
 * @brief   f_ModInst_Ope::~f_ModInst_Ope()
 */
DescOperation f_ModInst_Ope::getDescription()
{
    DescOperation   Retour;
    TypeOperation   Operation ;

    if(this->ui->Radio_Affecter->isChecked())
    {
        Operation = AFFECTER ;
    }
    if(this->ui->Radio_Ajouter->isChecked())
    {
        Operation = AJOUTER ;
    }
    if(this->ui->Radio_Dec->isChecked())
    {
        Operation = DECREMENTER ;
    }
    if(this->ui->Radio_Inc->isChecked())
    {
        Operation = INCREMENTER ;
    }
    if(this->ui->Radio_Soustraire->isChecked())
    {
        Operation = SOUSTRAIRE ;
    }

    Retour.nIndiceVariable  = this->ui->CB_Variable->currentIndex() ;
    Retour.Operation        = Operation ;
    Retour.nValeur          = this->ui->SB_Valeur->value() ;

    return Retour ;
}
