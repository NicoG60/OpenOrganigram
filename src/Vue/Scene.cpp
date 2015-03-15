//-------------------------------------------------------------------------------
/**
 * @file        Scene.cpp
 * @brief       Scene d'un affichage d'organigramme
 *
 * @author      N.Jarnoux
 * @author      STS IRIS, Lycée Nicolas APPERT, ORVAULT (FRANCE)
 * @since       21/02/14
 * @version     1.0
 * @date        21/02/14
 *
 * Cette classe contiendra toute les informations necessaire à la vue pour l'affichage, notament les coords
 * des éléments graphiques.
 *
 * Fabrication  OpenOrganigram.pro
 *
 * @todo        Tout est à faire, la classe est codée au minimum syndicale pour tester d'autres fonctionnalités
 *
 * @bug         /
 */
//-------------------------------------------------------------------------------

//===== Headers standards =====
#include <QMimeData>
#include <QGraphicsItem>
#include <QApplication>

//=====   Headers Peros   =====
#include "Scene.h"

/**
 * Constructeur.
 *
 * @brief   Scene::Scene(QObject *  parent = 0)
 * @param   pParent     Pointeur vers l'objet QObject (ou dérivé) parent.
 */
Scene::Scene(QObject *  pParent) :
    QGraphicsScene(pParent),
    pMarqueur(new MarqueurDrop),
    pFlecheMarqueur(0)
{
}

/**
 * Destructeur
 *
 * @brief Scene::~Scene
 */
Scene::~Scene()
{
    this->RAZ();
    if(this->pMarqueur != NULL)
    {
        delete this->pMarqueur ;
    }

    this->pMarqueur = NULL ;
}

/**
 * Gestion de l'évenement d'entrée de drag dans la scène. cela permet d'accepter ou non le dépot en fonction
 * du type de donnée draggée
 *
 * @brief   Scene::dragEnterEvent(QGraphicsSceneDragDropEvent *    pEvent)
 * @param   pEvent  Pointeur vers l'évement visé
 */
void Scene::dragEnterEvent(QGraphicsSceneDragDropEvent *    pEvent)
{
    //Si c'est un drag que l'on a déclenché
    if(pEvent->mimeData()->text().left(10) == "NouvelItem" || pEvent->mimeData()->text().left(8) == "Deplacer")
    {
        //On accept l'evenement on ajoute le marqueur à la scene et on calcul sa position
        pEvent->accept();

        this->addItem(this->pMarqueur);

        this->CalculerEmplacementMarqueur(pEvent->scenePos());
    }
    else
    {
        QApplication::setOverrideCursor(QCursor(Qt::ForbiddenCursor));
        QGraphicsScene::dragEnterEvent(pEvent) ;
    }
}

/**
 * Gestion de l'évenement de sortie d'un drag de la scène. cela permet de gérer les affichage particulier
 * par exemple la surbrillance d'un emplacement précis où l'on pouvait déposer le drag.
 *
 * @brief   Scene::dragLeaveEvent(QGraphicsSceneDragDropEvent *    pEvent)
 * @param   pEvent  Pointeur vers l'évement visé
 */
void Scene::dragLeaveEvent(QGraphicsSceneDragDropEvent *    pEvent)
{
    //Quand un drag quitte la scene, on retire le marqueur
    this->removeItem(this->pMarqueur);
    pEvent->accept();
    QApplication::restoreOverrideCursor();
}

/**
 * Gestion de l'évenement de déplacement du drag. Permet de mettre la jour les surbrillance de zone de dépot
 * particulière.
 *
 * @brief   Scene::dragMoveEvent(QGraphicsSceneDragDropEvent * pEvent)
 * @param   pEvent  Pointeur vers l'évement visé
 */
void Scene::dragMoveEvent(QGraphicsSceneDragDropEvent * pEvent)
{
    //Si c'est un drag que l'on a déclenché
    if(pEvent->mimeData()->text().left(10) == "NouvelItem" || pEvent->mimeData()->text().left(8) == "Deplacer")
    {
        //On calcul la position du marqueur
        pEvent->accept();
        this->CalculerEmplacementMarqueur(pEvent->scenePos());
    }
    else
    {
        QGraphicsScene::dragMoveEvent(pEvent) ;
    }
}

/**
 * Gestion de l'évenement de dépot d'un drag.
 *
 * @brief   dropEvent(QGraphicsSceneDragDropEvent * pEvent)
 * @param   pEvent  Pointeur vers l'évement visé
 */
void Scene::dropEvent(QGraphicsSceneDragDropEvent * pEvent)
{
    bool Dep    (false);

    QApplication::restoreOverrideCursor();

    //Si c'est un nouvel item à ajouter
    if(pEvent->mimeData()->text().left(10) == "NouvelItem")
    {
        pEvent->accept();
    }
    else //Sinon si c'est un déplacement, on l'indique avec la variable dep
    {
        if(pEvent->mimeData()->text().left(8) == "Deplacer")
        {
            Dep = true;
            pEvent->accept();
        }
        else
        {
            QGraphicsScene::dropEvent(pEvent) ;
        }
    }

    //Si l'evenement est accepté (donc que l'on valide le drop)
    if(pEvent->isAccepted())
    {
        //On retire le marqueur et on initialise les variables
        this->removeItem(this->pMarqueur);

        bool            Ecartement  (false);
        bool            RetourCond  (false);
        char            cType ;

        //On récupere le type en fonction de si c'est un déplacement ou un nouvel item
        if(Dep)
        {
            cType = pEvent->mimeData()->text().at(9).toLatin1();
        }
        else
        {
            cType = pEvent->mimeData()->text().at(11).toLatin1();
        }

        //Transtypage du type d'item
        TypeCommande    Type        (static_cast<TypeCommande>((int)cType));

        // On calcul si c'est un ecarte et/ou un retour de condition
        Ecartement = this->pFlecheMarqueur->EstDecaleeDroite();
        RetourCond = this->pFlecheMarqueur->EstVerticale() && !this->pFlecheMarqueur->EstPresDuDepart(this->pMarqueur->scenePos());

        //On ajoute l'instruction correspondante dans la routine affichée
        Vue * VueScene  (qobject_cast<Vue *>(this->views().at(0)));
        VueScene->getRoutineAffichee()->AjouterInstruction(Type, this->pFlecheMarqueur->getItemDepart()->getIdInst(), Ecartement, RetourCond, Dep);

        //Si c'est un déplacement
        if(Dep)
        {
            DescOperation   DescOpe;
            DescCondition   DescCond;
            DescES          DescEntreeSortie;
            DescRoutine     DescRout;
            DescTempo       DescTemporisation;
            DescBoucle      DescDeBoucle;
            DescCommentaire DescCom;

            //En fonction du type, on rappel la config qu'on avait avant le déplacement
            switch(Type)
            {
            case OPE:
                Instruction::ConvertirVersDesc(pEvent->mimeData()->text().mid(11), &DescOpe);
                VueScene->getRoutineAffichee()->getListe().last()->setDesc(DescOpe);
                break;

            case COND:
                Instruction::ConvertirVersDesc(pEvent->mimeData()->text().mid(11), &DescCond);
                VueScene->getRoutineAffichee()->getListe().last()->setDesc(DescCond);
                break;

            case ES:
                Instruction::ConvertirVersDesc(pEvent->mimeData()->text().mid(11), &DescEntreeSortie);
                VueScene->getRoutineAffichee()->getListe().last()->setDesc(DescEntreeSortie);
                break;

            case ROUT:
                Instruction::ConvertirVersDesc(pEvent->mimeData()->text().mid(11), &DescRout);
                VueScene->getRoutineAffichee()->getListe().last()->setDesc(DescRout);
                break;

            case TEMPO:
                Instruction::ConvertirVersDesc(pEvent->mimeData()->text().mid(11), &DescTemporisation);
                VueScene->getRoutineAffichee()->getListe().last()->setDesc(DescTemporisation);
                break;

            case COM:
                Instruction::ConvertirVersDesc(pEvent->mimeData()->text().mid(11), &DescCom);
                VueScene->getRoutineAffichee()->getListe().last()->setDesc(DescCom);
                break;

            case BOUCLE:
                Instruction::ConvertirVersDesc(pEvent->mimeData()->text().mid(11), &DescDeBoucle);
                VueScene->getRoutineAffichee()->getListe().last()->setDesc(DescDeBoucle);
                break;

            default:
                break;
            }
        }

        //On rafrachi la vue
        VueScene->Rafraichir();
    }
}

/**
 * Renvois un pointeur vers l'item représentant l'instruction d'identifiant nIdInst
 *
 * @brief   getItem(unsigned int nIdInst)
 * @param   nIdItem  Id de l'item recherché
 */
Item * Scene::getItem(unsigned int nIdInst)
{
    Item *                  Retour          (0) ;
    Item *                  ItemCourant     (0) ;
    bool                    SortieBoucle    (false) ;

    for(register int i = 0 ; (i < this->ListeItems.length()) && !SortieBoucle ; i++)
    {
        ItemCourant = this->ListeItems[i] ;
        if(ItemCourant->getIdInst() == nIdInst)
        {
            Retour          = ItemCourant ;
            SortieBoucle    = true ;
        }
    }

    return Retour ;
}

/**
 * Ajoute un item à la scene
 * @brief Scene::AjouterItem
 * @param ItemAAjouter
 */
void Scene::AjouterItem(Item * ItemAAjouter)
{
    if(ItemAAjouter != NULL)
    {
        this->ListeItems.append(ItemAAjouter) ;
        this->addItem(ItemAAjouter) ;
    }
}

/**
 * Ajoute un fleche à la scene
 * @brief Scene::AjouterFleche
 * @param FlecheAAjouter
 */
void Scene::AjouterFleche(Fleche * FlecheAAjouter)
{
    if(FlecheAAjouter != NULL)
    {
        this->ListeFleches.append(FlecheAAjouter) ;
        this->addItem(FlecheAAjouter) ;
    }
}

/**
 * Supprime tout ce qui est sur la scene
 * @brief Scene::RAZ
 */
void Scene::RAZ()
{
    this->ListeFleches.clear();
    this->ListeItems.clear();
    this->clear();
}

/**
 * Calcul la position du marqueur de drop
 * @brief Scene::CalculerEmplacementMarqueur
 * @param Position
 */
void Scene::CalculerEmplacementMarqueur(QPointF Position)
{
    Fleche *    FlecheVisee (0);
    QPointF     PlusProche ;
    QPointF     PointCourant ;
    qreal       DistanceMini(0);
    qreal       Distance    (0);
    QLineF      Ligne ;


    //Initialisation de la boucle de calcul
    Ligne.setP1(Position);
    Ligne.setP2(this->ListeFleches[0]->PointLePlusPres(Position)) ;

    DistanceMini = Ligne.length() ;

    //Pour chaque fleche on va calculer le point le plus proche
    for(register int i = 0; i < this->ListeFleches.length(); i++)
    {
        PointCourant = this->ListeFleches[i]->PointLePlusPres(Position) ;
        if(PointCourant.x() != 0 || PointCourant.y() != 0)
        {
            Ligne.setP2(PointCourant) ;

            Distance = Ligne.length() ;

            //Celui qui a la plus petite distance est le bon, c'est lui que l'on retourne
            if(Distance <= DistanceMini)
            {
                FlecheVisee     = this->ListeFleches[i] ;
                PlusProche      = PointCourant;
                DistanceMini    = Distance ;
            }
        }
    }

    //On remet la position du marqueur à jour
    this->pFlecheMarqueur = FlecheVisee ;
    this->pMarqueur->setPos(PlusProche);
}
