#ifndef SWITCH_H
#define SWITCH_H

class Switch;

#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>

class Switch : public QWidget
{
    Q_OBJECT

public:
    Switch(QWidget* parent=0);
    ~Switch();

    inline  bool                isChecked() { return checked; }
            void                setChecked(bool checked);

signals:
    void toggled(bool);

protected:
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent* e);

private:
    bool checked;
    bool precCheck;
};

#endif // SWITCH_H
