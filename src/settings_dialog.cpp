/****************************************************************************
**
**
****************************************************************************/

#include "constants.h"
#include "settings_dialog.h"
#include "ui_settings_dialog.h"
#include "common.h"
#include "mainwindow.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include <QtWidgets>

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::SettingsDialog)
{
    libChanged = false;
    Qt::WindowFlags flags(Qt::Dialog | Qt::WindowStaysOnTopHint | Qt::WindowTitleHint);
    setWindowFlags(flags);
    ui->setupUi(this);

    QSqlDatabase db = QSqlDatabase::database(DATABASE_NAME);
    QSqlQuery query(db);

    if(!query.exec("SELECT id, name FROM ref_libraries")){
        qDebug() << query.lastError().text();
    }else{
        while (query.next()) {
            ui->libraryComboBox->addItem(query.value(1).toString());
            libIds.append(query.value(0).toInt());
        }
    }
    connect(ui->createNewLibPushButton, &QAbstractButton::clicked, this, &SettingsDialog::createNewLibrary);
    connect(ui->libraryComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){ libChanged = true; });
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &SettingsDialog::acceptChange);

    setWindowTitle(tr("Refio Preferences"));
}

void SettingsDialog::createNewLibrary()
{
    QString name = QInputDialog::getText(this, "Create New Library", "New Library Name").trimmed();
    if(!name.isEmpty()){
        QByteArray iCodeSeq = genICodeSeq();
        QSqlDatabase db = QSqlDatabase::database(DATABASE_NAME);
        QSqlQuery query(db);
        query.prepare( "INSERT INTO ref_libraries (name, icodeseq, created) VALUES (:name, :icodeseq, :created)");
        query.bindValue(":name", name);
        query.bindValue(":icodeseq", iCodeSeq);
        query.bindValue(":created", QDateTime::currentDateTime().toString());
        if( !query.exec() ){
            qDebug() << "Error inserting first library into table:\n" << query.lastError();
            return;
        }
        int new_lib_id = query.lastInsertId().toInt();
        ui->libraryComboBox->addItem(name);
        libIds.append(new_lib_id);
        ui->libraryComboBox->setCurrentIndex(ui->libraryComboBox->count()-1);

        query.prepare( "INSERT INTO ref_collections (lib_id, name, created)"
                       " VALUES (:lib_id, \"Default Collection\", :created)");
        query.bindValue( ":lib_id", new_lib_id);
        query.bindValue( ":created", QDateTime::currentDateTime().toString());
        if( !query.exec() ){
            qDebug() << "Error inserting first collection into table:\n" << query.lastError();
        }
    }
}

void SettingsDialog::acceptChange()
{
    if(libChanged){
        QMessageBox::StandardButton reply;
        QString("%1/%2-%3.txt").arg("~", "Tom", "Jane");
        QString areYouSure = QString("Are you sure changing library to '%1'?").arg(ui->libraryComboBox->currentText());

        reply = QMessageBox::question(this, "Save Change", areYouSure, QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            //MainWindow *mainWin = qobject_cast<MainWindow*>(getMainWindow());
            QMainWindow *mainWin = getMainWindow();
            CollectionsWidget *w = mainWin->findChild<CollectionsWidget *>(REF_COLLECTIONS_WIDGET_NAME);
            w->setupModel(libIds.at(ui->libraryComboBox->currentIndex()));
            qDebug() << "Yes was clicked";
        } else {
            qDebug() << "Yes was *not* clicked";
        }
    }
}
