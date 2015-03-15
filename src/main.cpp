//-------------------------------------------------------------------------------
/**
 * @file        main.cpp
 * @brief       Programme principal
 *
 * @author      N.Jarnoux, J. Aubry, F. De Dutto, K. Batard
 * @author      STS IRIS, Lycée Nicolas APPERT, ORVAULT (FRANCE)
 * @since       18/02/14
 * @version     1.0
 * @date        18/02/14
 *
 * Programme principal : initialise l'application et ses différents composants
 *
 * Fabrication  OpenOrganigram.pro
 *
 * @todo        A completer lors de la réunification des parties
 *
 * @bug         /
 */
//-------------------------------------------------------------------------------

//=== Headers standards ===
#include <QApplication>

//===   Headers persos  ===
#include "f_MainWindow.h"
#include "Interface/Arduino.h"


int main(int argc, char *argv[])
{
    QApplication    a  (argc, argv) ;

    f_MainWindow    w  (0) ;
    w.showMaximized() ;

    return a.exec();
}
