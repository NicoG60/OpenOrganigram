//--------------------------------------------------------------------------------------------------
/** @file       f_RealiserPlanCablage
 *  @brief      IHM Réaliser le plan de câblage
 *
 *  @author     N. Jarnoux
 *  @author     BTS IRIS, Lycée Nicolas APPPERT, ORVAULT (FRANCE)
 *  @since      2014-12-14
 *  @version    1.0
 *  @date       2014-12-14
 *
 *  Classe de gestion de l'IHM pour réaliser le plan de câblage qui consiste a déposer
 *  les capteurs sur tel ou tel prise de la carte easycon ou arduino.
 *
 */
//--------------------------------------------------------------------------------------------------
//=== Headers standards ===
#include <QDir>
#include <QDebug>

//===   Headers persos  ===#
#include "f_RealiserPlanCablage.h"
#include "ui_f_RealiserPlanCablage.h"


/**
 * Constructeur de la fenetre, Liste les capteurs en fonction des interfaces
 * @brief f_RealiserPlanCablage::f_RealiserPlanCablage
 * @param Config : Pointeur vers la config
 * @param ListeInterfaceInstall : liste des interfaces installée
 * @param parent : widget parent
 */
f_RealiserPlanCablage::f_RealiserPlanCablage(QSettings* Config, QStringList ListeInterfaceInstall, QWidget *parent) :
    QDialog (parent),
    ui      (new Ui::f_RealiserPlanCablage),
    ListeCap(new ListeCapteurVue),
    Config  (Config)
{
    this->ui->setupUi(this) ;
    this->ui->GrpBx->layout()->addWidget(this->ListeCap);


    this->ui->CB_Interface->addItems(ListeInterfaceInstall);

    for(register int i = 0; i < ListeInterfaceInstall.length(); i++)
    {
        //Scene
        this->Scenes.append(new ScenePlanDeCablage(Config));
        this->Scenes.last()->setInterface(ListeInterfaceInstall[i]);

        //Model Liste Capteur
        this->Models.append(new QStandardItemModel);

        QDir DirControl("./IOCard");

        QFileInfoList ListeDir (DirControl.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot));

        for(register int j = 0; j < ListeDir.length(); j++)
        {
            if(ListeDir[j].isDir())
            {
                QDir RepertoireCourant("./IOCard/" + ListeDir[j].completeBaseName());

                QFileInfoList ListeFichier (RepertoireCourant.entryInfoList());

                bool SortieBoucle(false);

                for(register int k = 0; k < ListeFichier.length() && !SortieBoucle; k++)
                {
                    if(ListeFichier[k].isFile())
                    {
                        if(ListeFichier[k].suffix() == "ini")
                        {
                            QSettings ConfigCarte(ListeFichier[k].filePath(), QSettings::IniFormat);
                            QString Interface(ConfigCarte.value("PORT/Interface").toString());
                            if(Interface == "ALL" || Interface == ListeInterfaceInstall[i].toUpper())
                            {

                                SortieBoucle = true;

                                QStandardItem* NouvelItem(new QStandardItem(ConfigCarte.value("TITRE/NOM").toString()));

                                NouvelItem->setData(ListeDir[j].completeBaseName(), Qt::UserRole);

                                NouvelItem->setIcon(QIcon(RepertoireCourant.absoluteFilePath("photo.png")));

                                this->Models.last()->appendRow(NouvelItem);
                            }
                        }
                    }
                }
            }
        }
    }

    this->ListeCap->setModel(this->Models.at(this->ui->CB_Interface->currentIndex()));
    this->ui->Vue->setScene(this->Scenes.at(this->ui->CB_Interface->currentIndex()));
}

/** Description détaillée de la méthode
 *  @pre    f_RealiserPlanCablage(QWidget *parent) doit être lancée
 *  @post
 *  @param
 *  @retval
 *  @return
 *  @test
 *  @see    ui.
 */
f_RealiserPlanCablage::~f_RealiserPlanCablage()
{
    delete this->ui ;
}

/**
 * Slot déclenché quand on change d'interfce dans le menu déroulant. cela charge la vue du plan de câblage
 * @brief f_RealiserPlanCablage::on_CB_Interface_currentIndexChanged
 * @param index
 */
void f_RealiserPlanCablage::on_CB_Interface_currentIndexChanged(int index)
{
    if(index < this->Models.length())
    {
        this->ListeCap->setModel(this->Models[index]);
        this->ui->Vue->setScene(this->Scenes[index]);
    }
}

/**
 * Quand on click sur précédent
 * @brief f_RealiserPlanCablage::on_Btn_Prec_clicked
 */
void f_RealiserPlanCablage::on_Btn_Prec_clicked()
{
    this->done(2);
}

/**
 * Quand on click sur suivant, enregistre tout le plan de câblage
 * @brief f_RealiserPlanCablage::on_Btn_Suiv_clicked
 */
void f_RealiserPlanCablage::on_Btn_Suiv_clicked()
{
    for(register int i = 0; i < Scenes.length(); i++)
    {
        Scenes[i]->Enregistrer();
    }

    this->accept();
}
