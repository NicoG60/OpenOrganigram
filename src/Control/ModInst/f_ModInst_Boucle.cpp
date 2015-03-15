//-------------------------------------------------------------------------------
/**
 * @file        f_ModInst_Boucle.cpp
 * @brief       Fenetre de modification d'une insturction boucle
 *
 * @author      N.Jarnoux
 * @author      STS IRIS, Lycée Nicolas APPERT, ORVAULT (FRANCE)
 * @since       20/03/14
 * @version     1.0
 * @date        20/03/14
 *
 * Classe affichant une interface de modification des propriété d'une instruction "Boucle"
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
#include "f_ModInst_Boucle.h"
#include "ui_f_ModInst_Boucle.h"

/**
 * Constructeur
 *
 * @brief f_ModInst_Boucle::f_ModInst_Boucle
 * @param pInstructionAModifier Intruction à modifier
 * @param parent                Widget parent
 */
f_ModInst_Boucle::f_ModInst_Boucle(Inst_Boucle *pInstructionAModifier, QWidget *parent) :
    QDialog                 (parent),
    pInstructionAModifier   (pInstructionAModifier),
    pf_ModInst_Cond         (new f_ModInst_Cond(pInstructionAModifier->getDescriptionCondition(), pInstructionAModifier->getRoutine()->getManagerParent())),
    ui                      (new Ui::f_ModInst_Boucle)
{
    ui->setupUi(this);

    //Ajout du widget de configuration de condition
    this->ui->verticalLayout->addWidget(this->pf_ModInst_Cond);

    //Si c'est un tant que, on check
    if(pInstructionAModifier->getTypeDeBoucle() == TANT_QUE)
    {
        this->ui->RadioTQ->setChecked(true);
    }

    //Si c'est un jusqu'à on check
    if(pInstructionAModifier->getTypeDeBoucle() == JUSQU_A)
    {
        this->ui->RadioJQ->setChecked(true);
    }

    //On connect les signaux qui vont bien
    this->connect(this->pf_ModInst_Cond, SIGNAL(accepted()), this, SLOT(accept()));
    this->connect(this->pf_ModInst_Cond, SIGNAL(rejected()), this, SLOT(reject()));
}

/**
 * Destructeur
 *
 * @brief f_ModInst_Boucle::~f_ModInst_Boucle
 */
f_ModInst_Boucle::~f_ModInst_Boucle()
{
    delete ui;
}

/**
 * Renvois la description de la boucle modifiée
 *
 * @brief f_ModInst_Boucle::getDescription
 * @return La description
 */
DescBoucle f_ModInst_Boucle::getDescription()
{
    DescBoucle Retour ;

    if(this->ui->RadioTQ->isChecked())
    {
        Retour.TypeDeBoucle = TANT_QUE ;
    }

    if(this->ui->RadioJQ->isChecked())
    {
        Retour.TypeDeBoucle = JUSQU_A ;
    }

    Retour.DescriptionCondition = this->pf_ModInst_Cond->getDescription() ;

    return Retour ;
}
