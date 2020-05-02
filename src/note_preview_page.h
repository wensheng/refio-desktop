#ifndef NOTEPREVIEWPAGE_H
#define NOTEPREVIEWPAGE_H

#include <QWebEnginePage>

class NotePreviewPage : public QWebEnginePage
{
    Q_OBJECT

public:
    explicit NotePreviewPage(QObject *parent = nullptr) : QWebEnginePage(parent) {}

protected:
    bool acceptNavigationRequest(const QUrl &url, NavigationType type, bool isMainFrame);
};

#endif // NOTEPREVIEWPAGE_H
