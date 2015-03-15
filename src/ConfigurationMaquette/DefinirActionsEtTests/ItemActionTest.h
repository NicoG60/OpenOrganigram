#ifndef ITEMACTIONTEST_H
#define ITEMACTIONTEST_H

#include <QStandardItem>
#include "../../TypeCommun.h"

class ItemActionTest : public QStandardItem
{
public:
    ItemActionTest(ActionArduino*  Action);
    ItemActionTest(TestArduino* Test);
    ~ItemActionTest();

    inline ActionArduino* getAction() { return Action; }
    inline TestArduino*   getTest()   { return Test; }

    void refresh();

private:
    ActionArduino*  Action;
    TestArduino*    Test;
};

#endif // ITEMACTIONTEST_H
