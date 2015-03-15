#ifndef F_MOTEUR_H
#define F_MOTEUR_H

#include <QWidget>

namespace Ui {
class f_Moteur;
}

class f_Moteur : public QWidget
{
    Q_OBJECT

public:
    explicit f_Moteur(QWidget *parent = 0);
    ~f_Moteur();

private:
    Ui::f_Moteur *ui;
};

#endif // F_MOTEUR_H
