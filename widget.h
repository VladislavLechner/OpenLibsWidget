#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <stdio.h>
#include <iostream>
#include <exception>
#include <QMessageBox>
#include <dlfcn.h>
#include <QDebug>

#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QFileSystemModel>
#include <QTreeView>

#include "/home/semen/qtProjects/QtProjects/SumWidget/inputwidget.h"
#include "/home/semen/qtProjects/QtProjects/FileWidget/inputwidget.h"
#include "scandirectory.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
//    void showInputWidget();

public slots:
    void connectionToThelib();
    void startScanPressed();
    void closeLib();

private:
    QPushButton * m_connectToTheLib;
    QPushButton * m_disconnectFromTheLib;
    QPushButton * m_startScan;
    QLabel      * m_pathForScan;
    QLineEdit   * m_inputPathForScan;
    QGridLayout * m_layOut;

    QFileSystemModel * m_fileModel;
    QTreeView        * m_treeView;


    void* m_lib;
    void* m_libInfo;
    InputSumWidget* m_sumWidget = nullptr;
    InputFileWidget * m_fileWidget = nullptr;



private:
    bool getWidgetInstance();
    bool releaseSumWidgetInstance();
    void memoryAllocation();
    void setUpWidgets();
    void handle_eptr(std::exception_ptr eptr);
    void setUpFileSystemModel(std::pair< std::list<std::string>, std::list<std::string>>, QString path);

};
#endif // WIDGET_H
