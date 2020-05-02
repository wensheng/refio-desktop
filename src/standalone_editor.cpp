#include "standalone_editor.h"

StandaloneEditor::StandaloneEditor(QWidget *parent) : QWidget(parent)
{

}

void StandaloneEditor::closeEvent(QCloseEvent *event)
{
    // no native signal when close so I have to emit one
    emit beClosed();
}
