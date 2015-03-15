#include "ItemActionTest.h"

ItemActionTest::ItemActionTest(ActionArduino *Action) :
    QStandardItem(),
    Action(Action),
    Test(0)
{
    setText(Action->sNomAction + " " + Action->sCommande);
    setCheckable(true);
    if(Action->bActive)
        setCheckState(Qt::Checked);
    else
        setCheckState(Qt::Unchecked);
}

ItemActionTest::ItemActionTest(TestArduino* Test) :
    QStandardItem(),
    Action(0),
    Test(Test)
{
    setText(Test->sNomTest + " " + Test->sCommande);
    setCheckable(true);
    if(Test->bActive)
        setCheckState(Qt::Checked);
    else
        setCheckState(Qt::Unchecked);
}

ItemActionTest::~ItemActionTest()
{

}

void ItemActionTest::refresh()
{
    if(Test != 0)
    {
        setText(Test->sNomTest + " " + Test->sCommande);
        setCheckable(true);
        if(Test->bActive)
            setCheckState(Qt::Checked);
        else
            setCheckState(Qt::Unchecked);
    }
    else if(Action != 0)
    {
        setText(Action->sNomAction + " " + Action->sCommande);
        setCheckable(true);
        if(Action->bActive)
            setCheckState(Qt::Checked);
        else
            setCheckState(Qt::Unchecked);
    }
}

