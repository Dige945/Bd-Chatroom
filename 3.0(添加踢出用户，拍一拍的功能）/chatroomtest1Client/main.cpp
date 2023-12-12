#include "widget.h"
#include "login.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //Widget w;
    //w.show();
    login l;
    l.show();
    //login l;
    //l.show();
    return a.exec();
}
