//-------------------------------------------------------------------------------
/**
 * @file        f_ModInst_Com.cpp
 * @brief       Fenetre de modification d'un commentaire
 *
 * @author      N.Jarnoux
 * @author      STS IRIS, Lycée Nicolas APPERT, ORVAULT (FRANCE)
 * @since       19/03/14
 * @version     1.0
 * @date        19/03/14
 *
 * Classe affichant une interface de modification d'un commentaire
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
#include "f_ModInst_Com.h"
#include "ui_f_ModInst_Com.h"

/**
 * Constructeur
 *
 * @brief f_ModInst_Com::f_ModInst_Com
 * @param pInstructionAModif    Instruction à modifier
 * @param parent                Widget parent
 */
f_ModInst_Com::f_ModInst_Com(Inst_Com *pInstructionAModif, QWidget *parent) :
    QDialog             (parent),
    pInstructionAModif  (pInstructionAModif),
    ui                  (new Ui::f_ModInst_Com)
{
    ui->setupUi(this);

    this->ui->plainTextEdit->setPlainText(this->pInstructionAModif->getCommentaire());
}

/**
 * Destructeur
 *
 * @brief f_ModInst_Com::~f_ModInst_Com
 */
f_ModInst_Com::~f_ModInst_Com()
{
    delete ui;
}

/**
 * Renvois la description modifiée du commentaire
 *
 * @brief   f_ModInst_Com::getDescription
 * @return  Description modifiée
 */
DescCommentaire f_ModInst_Com::getDescription()
{
    DescCommentaire Retour;

    Retour.sCommentaire = this->ui->plainTextEdit->toPlainText() ;

    return Retour ;
}
