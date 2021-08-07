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
#include <QToolTip>


#include "scandirectory.h"
#include "treeview.h"
#include "/home/semen/qtProjects/QtProjects/AbstractClasses/abstractclasses.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

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
    TreeView         * m_treeView;

    std::pair< std::list<std::string>, std::list<std::string>> m_libsNamesAndDescription;


    void* m_lib;
    void* m_libInfo;

    QWidget * m_widget = nullptr;

private:
    bool getWidgetInstance();
    void releaseWidgetInstance(QWidget * instance);
    void memoryAllocation();
    void setUpWidgets();
    void handle_eptr(std::exception_ptr eptr);
    void setUpFileSystemModel(std::pair< std::list<std::string>, std::list<std::string>>, QString path);

};
#endif // WIDGET_H
