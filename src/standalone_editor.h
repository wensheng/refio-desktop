#ifndef STANDALONEEDITOR_H
#define STANDALONEEDITOR_H

#include <QWidget>

class StandaloneEditor : public QWidget
{
    Q_OBJECT
public:
    explicit StandaloneEditor(QWidget *parent = nullptr);

signals:
    void beClosed();

private:
    void closeEvent(QCloseEvent *event);
};

#endif // STANDALONEEDITOR_H
