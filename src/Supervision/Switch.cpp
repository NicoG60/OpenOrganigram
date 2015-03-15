#include <QPainter>

#include "Switch.h"

Switch::Switch(QWidget *parent) :
    QWidget(parent),
    checked(false),
    precCheck(false)
{
    setMinimumSize(67, 26);
    setMaximumSize(67, 26);
}

Switch::~Switch()
{

}

void Switch::setChecked(bool checked)
{
    this->checked = checked;
    repaint();
    if(checked != precCheck)
        emit toggled(checked);

    precCheck = checked;
}

void Switch::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setWindow(-33, -13, 67, 26);

    if(checked)
    {
        painter.drawPixmap(-33, -13, 67, 26, QPixmap(":/Images/Sup/ON.png"));
    }
    else
    {
        painter.drawPixmap(-33, -13, 67, 26, QPixmap(":/Images/Sup/OFF.png"));
    }
}

void Switch::mousePressEvent(QMouseEvent* e)
{
    e->accept();
    if(e->button() == Qt::LeftButton)
    {
        checked = !checked;
    }

    repaint();
    emit toggled(checked);
}
