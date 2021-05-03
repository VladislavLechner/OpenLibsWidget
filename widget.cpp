#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
}

Widget::~Widget()
{
}

bool Widget::connectionToTheSumWidget()
{
    m_lib = dlopen("/home/semen/qtProjects/QtProjects/build-SumWidget-Desktop_Qt_5_12_10_GCC_64bit-Debug/libSumWidget.so", RTLD_NOW);
    if (m_lib == nullptr)
    {
        qDebug() << "Cannot load library: " << dlerror() << '\n';
        return false;
    }
    dlerror();

    return true;
}

bool Widget::closeLib()
{
    return dlclose(m_lib);
}

void Widget::showInputWidget()
{
    if (getSumWidgetInstance())
        m_inputWidget->show();
}

bool Widget::getSumWidgetInstance()
{
    typedef void *(*GetInputWidget)();
    GetInputWidget getInputWidget = nullptr;

    getInputWidget = reinterpret_cast<GetInputWidget>(dlsym(m_lib, "getInstance")); // приводим к указателю на фукцнию
    if (getInputWidget == nullptr)
    {
        qDebug() << "Cannot load create function: " << dlerror() << '\n';
        return false;
    }
    dlerror();

    m_inputWidget = reinterpret_cast<InputWidget *>(getInputWidget()); // получаем экземпляр класса
    if (m_inputWidget == nullptr)
    {
        qDebug() << "Cannot create data base" << '\n';
        return false;
    }
    return true;
}

bool Widget::releaseSumWidgetInstance()
{
    typedef void *(*ReleaseInputWidget)(InputWidget* );
    ReleaseInputWidget releaseInputWidget = nullptr;

    releaseInputWidget = reinterpret_cast<ReleaseInputWidget>(dlsym(m_lib, "releaseInstance")); // приводим к указателю на фукцнию
    if (releaseInputWidget == nullptr)
    {
        qDebug() << "Cannot load create function: " << dlerror() << '\n';
        return false;
    }
    dlerror();

    qDebug() << m_inputWidget;

    releaseInputWidget(m_inputWidget);

    qDebug() << m_inputWidget;

    return true;
}

