//  TypePerso.h     1.0     21/02/14        N.Jarnoux
#ifndef TYPECOMMUN_H
#define TYPECOMMUN_H

#include <QString>
#include <QMetaType>

#define CONNECTE    (0)
#define DECONNECTE  (1)
#define PORT_FERME  (2)

typedef struct
{
    unsigned int    nIdRoutine ;
    unsigned int    nIdInstruction ;
}   IdentifiantInst ;

typedef struct
{
    QString sNomAction ;
    QString sCommande ;
    bool    bActive ;
}   ActionArduino ;

typedef struct
{
    QString sNomTest ;
    QString sCommande ;
    bool    bActive ;
}   TestArduino ;


typedef enum
{
    DEBFIN  = 0,
    OPE     = 1,
    COND    = 2,
    ES      = 3,
    ROUT    = 4,
    TEMPO   = 5,
    COM     = 6,
    BOUCLE  = 7
} TypeCommande; //Type de commande possible dans l'organigramme

typedef enum {
    EGUAL   = 0,
    SUP     = 1,
    INF     = 2,
    SUP_EG  = 3,
    INF_EG  = 4,
    DIFF    = 5
} TypeComparaison ; //Définit les type de comparaison entre deux valeurs possibles

typedef enum {
    LOG = 0,
    MAT = 1
} TypeCondition ;   //Définit les type de condition : logicielle (sur une variable/valeur) ou materielle (appui sur un bouton..)

typedef struct {
    QString sCommentaire ;
} DescCommentaire ; //Description du commentaire, structure qui représente le contenu métier du commentaire, sans la partie conception

typedef struct {
    TypeCondition   TCondition ;    //Indique s'il s'agit d'une comparaison logicielle ou matérielle

    unsigned int    nIndiceVar ;    //Indice de la variable Ã  comparer
    TypeComparaison TypeComp ;      //Type de comparaison
    unsigned short  nValeur ;       //Valeur Ã  laquelle comparer

    TestArduino     TestMat ;
} DescCondition ;   //description de la condition sans la partie conception de l'objet

typedef struct
{
    ActionArduino CommandeDemandee ;
}   DescES ;

typedef enum
{
    TANT_QUE    = 0,
    JUSQU_A     = 1
} TypeBoucle ;    //Définit les type de boucle possible

typedef struct
{
    TypeBoucle        TypeDeBoucle ;
    DescCondition   DescriptionCondition ;
} DescBoucle ;    //Description d'une boucle, sans la partie conception

typedef enum
{
    AFFECTER    = 0,
    AJOUTER     = 1,
    SOUSTRAIRE  = 2,
    INCREMENTER = 3,
    DECREMENTER = 4
} TypeOperation ;   //Définit les types d'opérations possibles

typedef struct {
    unsigned int    nIndiceVariable ;       //La variable
    TypeOperation   Operation ;             //Type d'opération à effectuer
    unsigned short  nValeur ;               //La valeur avec laquelle faire l'opération
} DescOperation ;   //Description de l'opération sans la partie conception

typedef struct {
    int     nIdRoutine ;    //Identifiant de la routine
    QString sNom ;          //Nom de la routine

} DescRoutine ; //Description de la routine dans la partie conception

typedef struct {
    unsigned short nMilliseconde ;
} DescTempo ;   //Description de la temporisation sans la partie conception

typedef struct {
    int             Position;   //La position relative du premier octet à modifier dans la liste d'opcode d'une instruction
    IdentifiantInst IdInst;     //L'identifiant de l'instruction à viser
    bool            Absolue;
} LienCompil;   //Identifie un lien à faire avec une autre instruction dans la compilation



















typedef enum {
    NI,
    NO,
    AI,
    I2C,
    TC,
    SRV,
    PWM,
    RTC,
    MOT1
} TypeFenetreSup;   //Type de fenetre de supervision

typedef enum {
    ORG,
    SUPV,
    SUPW,
    GEN
} EmetteurCommande ;    //Type d'emmeteur de commande à l'arduino

#endif // TYPECOMMUN_H
