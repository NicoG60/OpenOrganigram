//-------------------------------------------------------------------------------
/**
 * @file        f_ModInst_Tempo.cpp
 * @brief       Fenetre de modification d'une insturction
 *
 * @author      N.Jarnoux
 * @author      STS IRIS, Lycée Nicolas APPERT, ORVAULT (FRANCE)
 * @since       20/03/14
 * @version     1.0
 * @date        20/03/14
 *
 * Classe affichant une interface de modification des propriété d'une instruction "Temporisation"
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
#include "f_ModInst_Tempo.h"
#include "ui_f_ModInst_Tempo.h"

/**
 * Constructeur
 *
 * @brief f_ModInst_Tempo::f_ModInst_Tempo
 * @param pInstructionAModifier L'instruction à modifier
 * @param parent                Widget parent
 */
f_ModInst_Tempo::f_ModInst_Tempo(Inst_Tempo *pInstructionAModifier, QWidget *parent) :
    QDialog                 (parent),
    pInstructionAModifier   (pInstructionAModifier),
    ui                      (new Ui::f_ModInst_Tempo)
{
    ui->setupUi(this);

    unsigned short Temps    (this->pInstructionAModifier->getMillisecondes()) ;
    this->ui->spinBox->setValue(Temps) ;
}

/**
 * Destructeur
 *
 * @brief f_ModInst_Tempo::~f_ModInst_Tempo
 */
f_ModInst_Tempo::~f_ModInst_Tempo()
{
    delete ui;
}

/**
 * Renvois la description de la temporisation modifiée
 *
 * @brief f_ModInst_Tempo::getDescription
 * @return La description
 */
DescTempo f_ModInst_Tempo::getDescription()
{
    DescTempo Retour ;

    Retour.nMilliseconde = this->ui->spinBox->value() ;

    return Retour ;
}
