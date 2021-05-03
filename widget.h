#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <stdio.h>
#include <dlfcn.h>
#include <QDebug>

#include "inputwidget.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    bool connectionToTheSumWidget();
    bool closeLib();
    void showInputWidget();

private:
    void* m_lib;
    InputWidget* m_inputWidget = nullptr;

private:
    bool getSumWidgetInstance();
    bool releaseSumWidgetInstance();

};
#endif // WIDGET_H
