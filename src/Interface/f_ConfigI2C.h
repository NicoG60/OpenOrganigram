#ifndef F_CONFIGI2C_H
#define F_CONFIGI2C_H

class f_ConfigI2C;

#include <QDialog>

#include "Arduino.h"

namespace Ui {
class f_ConfigI2C;
}

class f_ConfigI2C : public QDialog
{
    Q_OBJECT

public:
    explicit f_ConfigI2C(Arduino* pArduino, QWidget *parent = 0);
    ~f_ConfigI2C();

private slots:
    void on_Btn_Reprogrammer_clicked();

private:
    Arduino* pArduino;
    Ui::f_ConfigI2C *ui;
};

#endif // F_CONFIGI2C_H
