//-------------------------------------------------------------------------------
/**
 * @file        f_ModInst_cond.cpp
 * @brief       Fenetre de modification d'une insturction
 *
 * @author      N.Jarnoux
 * @author      STS IRIS, Lycée Nicolas APPERT, ORVAULT (FRANCE)
 * @since       19/03/14
 * @version     1.0
 * @date        19/03/14
 *
 * Classe affichant une interface de modification des propriété d'une instruction "Condition"
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
#include "f_ModInst_Cond.h"
#include "ui_f_ModInst_Cond.h"

/**
 * Constructeur
 *
 * @brief   f_ModInst_Cond::f_ModInst_Cond(Inst_Cond *InstructionAModif, QWidget *parent)
 * @param   InstructionAModif   Instruction à modifier
 * @param   parent              vers le widget parent
 */
f_ModInst_Cond::f_ModInst_Cond(Inst_Cond *InstructionAModif, QWidget *parent) :
    QDialog             (parent),
    InstructionAModif   (InstructionAModif),
    Manager             (InstructionAModif->getRoutine()->getManagerParent()),
    ui                  (new Ui::f_ModInst_Cond)
{
    this->ui->setupUi(this);

    //Si c'est une condition matérielle, on check
    if(InstructionAModif->getTCondition() == MAT)
    {
        this->ui->Grp_CondMat->setChecked(true) ;
    }
    //Si c'est un condition logicelle, on check
    if(InstructionAModif->getTCondition() == LOG)
    {
        this->ui->Grp_CondLog->setChecked(true) ;
    }

    //On initialise les différents champs
    unsigned int    IndiceVar   (InstructionAModif->getIndiceVar()) ;
    unsigned short  Valeur      (InstructionAModif->getValeur()) ;
    TypeComparaison TypeComp    (InstructionAModif->getTypeComp()) ;

    this->ui->CB_Variable->setCurrentIndex(IndiceVar) ;
    this->ui->CB_Condition->setCurrentIndex(TypeComp) ;
    this->ui->SB_Valeur->setValue(Valeur) ;

    //On rempli la liste des tests
    QList<TestArduino>  ListeTestPossible (InstructionAModif->getRoutine()->getManagerParent()->getListeTestPossible()) ;

    for(register int i = 0; i < ListeTestPossible.length(); i++)
    {
        if(ListeTestPossible[i].bActive)
        {
            this->ui->comboBox->addItem(ListeTestPossible[i].sNomTest);
        }
    }
}

/**
 * Constructeur à partir d'une description
 *
 * @brief f_ModInst_Cond::f_ModInst_Cond
 * @param Description   La description à modifier
 * @param parent        Widget Parent
 */
f_ModInst_Cond::f_ModInst_Cond(DescCondition Description, ModuleManager * Manager, QWidget * parent):
    QDialog (parent),
    Manager (Manager),
    ui      (new Ui::f_ModInst_Cond)
{
    this->ui->setupUi(this);

    //Init des checkbox
    if(Description.TCondition == MAT)
    {
        this->ui->Grp_CondMat->setChecked(true) ;
    }
    if(Description.TCondition == LOG)
    {
        this->ui->Grp_CondLog->setChecked(true) ;
    }

    //Init des champs
    unsigned int    IndiceVar   (Description.nIndiceVar) ;
    unsigned short  Valeur      (Description.nValeur) ;
    TypeComparaison TypeComp    (Description.TypeComp) ;

    this->ui->CB_Variable->setCurrentIndex(IndiceVar) ;
    this->ui->CB_Condition->setCurrentIndex(TypeComp) ;
    this->ui->SB_Valeur->setValue(Valeur) ;

    //Remplissage de la liste de test
    QList<TestArduino>  ListeTestPossible (Manager->getListeTestPossible()) ;

    for(register int i = 0; i < ListeTestPossible.length(); i++)
    {
        if(ListeTestPossible[i].bActive)
        {
            this->ui->comboBox->addItem(ListeTestPossible[i].sNomTest);
        }
    }
}

/**
 * Destructeur
 *
 * @brief f_ModInst_Cond::~f_ModInst_Cond
 */
f_ModInst_Cond::~f_ModInst_Cond()
{
    delete this->ui;
}

/**
 * Retourne la description de la condition modifiée
 *
 * @brief f_ModInst_Cond::getDescription
 * @return La description
 */
DescCondition f_ModInst_Cond::getDescription()
{
    DescCondition Retour ;

    if(this->ui->Grp_CondLog->isChecked())
    {
        Retour.TCondition = LOG ;
    }
    if(this->ui->Grp_CondMat->isChecked())
    {
        Retour.TCondition = MAT ;
    }

    Retour.nIndiceVar   = this->ui->CB_Variable->currentIndex() ;
    Retour.nValeur      = this->ui->SB_Valeur->value() ;
    Retour.TypeComp     = static_cast<TypeComparaison>(this->ui->CB_Condition->currentIndex()) ;

    QList<TestArduino>  ListeTestPossible   (Manager->getListeTestPossible()) ;

    for(register int i = 0; i < ListeTestPossible.length(); i++)
    {
        if(ListeTestPossible[i].sNomTest == this->ui->comboBox->currentText())
        {
            Retour.TestMat = ListeTestPossible[i] ;
        }
    }

    return Retour ;
}

/**
 * Exclusion des deux groupebox
 * @brief f_ModInst_Cond::on_Grp_CondLog_toggled
 * @param arg1
 */
void f_ModInst_Cond::on_Grp_CondLog_toggled(bool arg1)
{
    this->ui->Grp_CondMat->setChecked(!arg1);
}

/**
 * Exclusion des deux groupebox
 * @brief f_ModInst_Cond::on_Grp_CondLog_toggled
 * @param arg1
 */
void f_ModInst_Cond::on_Grp_CondMat_toggled(bool arg1)
{
    this->ui->Grp_CondLog->setChecked(!arg1);
}
