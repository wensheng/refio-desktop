QT += core gui sql widgets network gui-private  webenginewidgets
TEMPLATE = app
CONFIG += c++17

#requires(qtConfig(listview))

include ($$PWD/third-parties/qmarkdowntextedit/qmarkdowntextedit.pri)

SOURCES   += adddialog.cpp \
            about_refio_dialog.cpp \
            collections_widget.cpp \
            markdown_document.cpp \
            note_preview_page.cpp \
            reference_widget.cpp \
            entries_widget.cpp \
            entry_details_widget.cpp \
            entry_info_tab.cpp \
            entry_note_tab.cpp \
            slipbox_widget.cpp \
            addresswidget.cpp \
            main.cpp \
            mainwindow.cpp \
            newaddresstab.cpp \
            settings_dialog.cpp \
            standalone_editor.cpp \
            treemodel.cpp \
            treeitem.cpp \
            mtreeview.cpp \
            mtree_item.cpp \
            mtree_model.cpp \
            tablemodel.cpp

HEADERS   += adddialog.h \
            about_refio_dialog.h \
            collections_widget.h \
            constants.h \
            markdown_document.h \
            note_preview_page.h \
            reference_widget.h \
            entries_widget.h \
            entry_details_widget.h \
            entry_info_tab.h \
            entry_note_tab.h \
            slipbox_widget.h \
            addresswidget.h \
            mainwindow.h \
            newaddresstab.h \
            settings_dialog.h \
            standalone_editor.h \
            treemodel.h \
            treeitem.h \
            mtreeview.h \
            mtree_item.h \
            mtree_model.h \
            tablemodel.h

RESOURCES += application.qrc

# install
target.path = refio_desktop
INSTALLS += target
