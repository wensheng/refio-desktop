#include "constants.h"
#include "entry_info_tab.h"
#include "ui_entry_info_tab.h"
#include "entries_widget.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

EntryInfoTab::EntryInfoTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EntryInfoTab)
{
    entryId = 0;
    ui->setupUi(this);
    ui->titlePushButton->setToolTip(tr("Click to edit"));
    connect(ui->titlePushButton, &QAbstractButton::clicked, this, &EntryInfoTab::titlePushButtonClicked);
    connect(ui->savePushButton, &QAbstractButton::clicked, this, &EntryInfoTab::saveEntryInfo);
    ui->titleLineEdit->installEventFilter(this);
    ui->titleLineEdit->hide();
}

EntryInfoTab::~EntryInfoTab()
{
    delete ui;
}

void EntryInfoTab::updateLabel(const MTreeItem *entry)
{
    entryId = entry->itemId();
    ui->titlePushButton->setText(entry->data(0).toString());
    ui->titleLineEdit->setText(entry->data(0).toString());
    QString icode("iCode: ");
    icode = icode.append(entry->data(3).toString());
    ui->icodeLabel->setText(icode);
    QString created("Created: ");
    created = created.append(entry->data(6).toString());
    ui->createdLabel->setText(created);
    ui->infoPlainTextEdit->setPlainText(entry->data(5).toString());
}

void EntryInfoTab::titlePushButtonClicked()
{
    ui->titlePushButton->hide();
    ui->titleLineEdit->show();
    ui->titleLineEdit->setFocus();
}

bool EntryInfoTab::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->titleLineEdit){
        if(event->type() == QEvent::KeyPress){
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if(Qt::Key_Enter == keyEvent->key() || Qt::Key_Return == keyEvent->key()){
                titleChanged();
            }
        }
        else if( event->type() == QEvent::FocusOut ) {
            titleChanged();
        }
    }
    return false;
}

void EntryInfoTab::titleChanged()
{
    ui->titlePushButton->setText(ui->titleLineEdit->text());
    ui->titleLineEdit->hide();
    ui->titlePushButton->show();

    QSqlDatabase db = QSqlDatabase::database(DATABASE_NAME);
    QList<QVector<QVariant>> notes;
    QSqlQuery query(db);
    query.prepare("update ref_entries set title=:newTitle where id=:iid");
    query.bindValue(":iid", entryId);
    query.bindValue(":newTitle", ui->titleLineEdit->text());
    if(!query.exec()){
        qDebug() << query.lastError().text();
    }
}

void EntryInfoTab::saveEntryInfo()
{
    QSqlDatabase db = QSqlDatabase::database(DATABASE_NAME);
    QList<QVector<QVariant>> notes;
    QSqlQuery query(db);
    query.prepare("update ref_entries set info=:newInfo where id=:iid");
    query.bindValue(":iid", entryId);
    query.bindValue(":newInfo", ui->infoPlainTextEdit->toPlainText());
    if(!query.exec()){
        qDebug() << query.lastError().text();
    }
}
