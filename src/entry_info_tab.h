#ifndef ENTRY_INFO_TAB_H
#define ENTRY_INFO_TAB_H

#include "mtree_item.h"
#include <QWidget>

namespace Ui {
class EntryInfoTab;
}

class EntryInfoTab : public QWidget
{
    Q_OBJECT

public:
    explicit EntryInfoTab(QWidget *parent = nullptr);
    ~EntryInfoTab();
     void updateLabel(const MTreeItem *entry);

private:
    Ui::EntryInfoTab *ui;
    bool eventFilter(QObject *watched, QEvent *event);
    void titleChanged();
    int entryId;

private slots:
    void titlePushButtonClicked();
    void saveEntryInfo();
};

#endif // ENTRY_INFO_TAB_H
