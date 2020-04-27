/****************************************************************************
**
****************************************************************************/

#ifndef MTREEVIEW_H
#define MTREEVIEW_H

#include <QTreeView>
#include <QMouseEvent>

class MTreeView : public QTreeView
{
    Q_OBJECT

public:
    explicit MTreeView(QWidget *parent = nullptr);

public slots:
    //void handleCustomContextMenu(QPoint &point);

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    void handleAddNote();
    void handleAddSubItem();

};

#endif 
