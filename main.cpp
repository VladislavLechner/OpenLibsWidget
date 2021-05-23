#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    qDebug() << w.connectionToThelib();

//    w.showInputWidget();

//    w.closeLib();
    return a.exec();
}
