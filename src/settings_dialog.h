/****************************************************************************
**
**
****************************************************************************/

#ifndef SETTINGS_DIALOG_H
#define SETTINGS_DIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
namespace Ui {
    class SettingsDialog;
}
QT_END_NAMESPACE

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    SettingsDialog(QWidget *parent = nullptr);

private slots:
    void createNewLibrary();
    void acceptChange();

private:
    Ui::SettingsDialog *ui;
    QList<int> libIds;
    bool libChanged;
};

#endif // SETTINGS_DIALOG_H
