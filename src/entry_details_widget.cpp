/****************************************************************************
**
**
****************************************************************************/

#include "constants.h"
#include "entry_details_widget.h"
#include "adddialog.h"

#include <QtWidgets>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

EntryDetailsWidget::EntryDetailsWidget(QWidget *parent)
    : QTabWidget(parent),
      entryInfoTab(new EntryInfoTab(this)),
      entryNoteTab(new EntryNoteTab(this))
{
    //auto tb = new QToolBar();
    //tb->addAction("hello");
    //tb->addAction("world");
    //connect(entryInfoTab, &EntryInfoTab::sendDetails, this, &EntryDetailsWidget::addEntry);

    addTab(entryInfoTab, tr("Info"));

    setupTabs();
    setObjectName(REF_ENTRY_DETAILS_WIDGET_NAME);
    qDebug() << metaObject()->className();
}

void EntryDetailsWidget::setupTabs()
{
    addTab(entryNoteTab, tr("Note"));

}

void EntryDetailsWidget::updateDetail(const MTreeItem *entry)
{
    //"Title", "id", "cid", "icode", "parent", "Note"
    int entry_id = entry->itemId();

    QString info("iCode: ");
    info = info.append(entry->data(3).toString()).append("\n");
    info = info.append("Title: ").append(entry->data(0).toString()).append("\n");
    info = info.append("Created: ").append(entry->data(6).toString());
    entryInfoTab->updateLabel(info);

    QSqlDatabase db = QSqlDatabase::database(DATABASE_NAME);
    QList<QVector<QVariant>> notes;
    QSqlQuery query(db);
    query.prepare("select id, entry_id, title, body, created from ref_notes where entry_id=:eid");
    query.bindValue(":eid", entry_id);
    if(!query.exec()){
        qDebug() << query.lastError().text();
    }else{
        while (query.next()) {
            QVector<QVariant> result;
            result.push_back(query.value(0).toInt()); // id
            result.push_back(query.value(1).toInt()); // eid
            result.push_back(query.value(2).toString()); // title
            result.push_back(query.value(3).toString()); // body
            result.push_back(query.value(4).toString()); // created
            notes.push_back(result);
        }
    }
    entryNoteTab->updateTab(notes);
}
