#include "treeview.h"

TreeView::TreeView(QWidget *parent)
    : QTreeView( parent)
{
    setMouseTracking( true);
    setToolTipDuration( 15000);
}

void TreeView::setLibsNamesAndDescription(std::pair<std::list<std::string>, std::list<std::string> > libsNamesAndDescription)
{
    m_libsNamesAndDescription = libsNamesAndDescription;
}

void TreeView::mouseMoveEvent(QMouseEvent *event)
{
    QToolTip::hideText();
    QPoint pt = event->pos();
    QModelIndex index = indexAt(pt);


    for (auto x1 = m_libsNamesAndDescription.first.begin(), x2 = m_libsNamesAndDescription.second.begin();
         x1 != m_libsNamesAndDescription.first.end() && x2 != m_libsNamesAndDescription.second.end(); ++x1,++x2)
    {
        if (x1->find("/") < x1->size())
        {
            int pos = x1->rfind("/");
            *x1 = x1->substr(pos + 1,x1->size());
        }

        if (index.data().toString() == QString::fromStdString(*x1))
        {
            setToolTip(QString::fromStdString(*x2));
            break;
        }
        else if (index.data().toString() != QString::fromStdString(*x1))
            setToolTip("");
    }

}
