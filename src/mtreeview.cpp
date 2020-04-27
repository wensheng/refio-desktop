/****************************************************************************
**
**
****************************************************************************/

/*
    treeitem.cpp

    A container for items of data supplied by the simple tree model.
*/

#include "mtreeview.h"
#include <QtWidgets>
#include <QPoint>
#include <QDebug>

MTreeView::MTreeView(QWidget *parent)
    :QTreeView(parent)
{
    //setContextMenuPolicy(Qt::CustomContextMenu);
    //connect(this,SIGNAL(customContextMenuRequested(QPoint &point)), this, SLOT(handleCustomContextMenu(QPoint &point)));
}

/*
void MTreeView::handleCustomContextMenu(QPoint &point)
{
    QModelIndex index = this->currentIndex();
    QMenu menu;
    menu.addAction("Hello");
    menu.addAction("world");
    menu.exec();
    qDebug() << "right clicked";
}*/

void MTreeView::mousePressEvent(QMouseEvent *event)
{
    QTreeView::mousePressEvent(event);
    QModelIndex index = indexAt( event->pos() );
    if ( index.isValid() ){
        if ( event->button() == Qt::RightButton){
            QMenu menu;
            QAction *addNoteAction = menu.addAction("Add Note");
            connect(addNoteAction, &QAction::triggered, this, &MTreeView::handleAddNote);
            QAction *addSubItemAction = menu.addAction("Add Sub Item");
            connect(addSubItemAction, &QAction::triggered, this, &MTreeView::handleAddSubItem);
            menu.exec(QCursor::pos());

        }
    }
}

void MTreeView::handleAddNote()
{
    qDebug() << "will add note";
}

void MTreeView::handleAddSubItem()
{
    qDebug() << "will add sub item";
}
