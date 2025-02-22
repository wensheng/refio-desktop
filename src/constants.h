#ifndef CONSTANTS_H
#define CONSTANTS_H

#define APPLICATION_VERSION "0.1"
#define ORGANIZATION_NAME "Refio"
#define ORGANIZATION_DOMAIN "refio.org"
#define APPLICATION_NAME "RefioDesktop"
#define MAIN_WINDOW_NAME "main_window"
#define REF_COLLECTIONS_WIDGET_NAME "ref_collections_widget"
#define REF_ENTRIES_WIDGET_NAME "ref_entries_widget"
#define REF_ENTRY_DETAILS_WIDGET_NAME "ref_entry_details_widget"
#define REF_ENTRY_DETAILS_INFO_TAB_NAME "ref_entry_details_info_tab"
#define REF_ENTRY_DETAILS_NOTE_TAB_NAME "ref_entry_details_note_tab"
#define DATABASE_NAME "refio"
#include <QVector>
const QVector<char> iCodeChar = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                                 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'J', 'K', 'M',
                                 'N', 'P', 'Q', 'R', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
static const QString UPDATE_DEFS_URL = "https://refio.org/"
                                       "refio_desktop/updates.json";
#endif // CONSTANTS_H
