#ifndef TREEVIEW_H
#define TREEVIEW_H

#include <QObject>
#include <QTreeView>
#include <QToolTip>
#include <QMouseEvent>
#include <QAbstractItemModel>
#include <QEvent>

#include <iostream>
#include <QDebug>

class TreeView: public QTreeView
{
public:
    TreeView( QWidget* parent = nullptr);
    void setLibsNamesAndDescription(std::pair< std::list<std::string>, std::list<std::string>> libsNamesAndDescription);

protected:
    void mouseMoveEvent(QMouseEvent *event);

private:
    std::pair< std::list<std::string>, std::list<std::string>> m_libsNamesAndDescription;
};

#endif // TREEVIEW_H
