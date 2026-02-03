#ifndef TRV_DEFINES_H
#define TRV_DEFINES_H

#include <QString>

#define STR_EXPAND(tok) #tok
#define STR(tok) STR_EXPAND(tok)
#define TRV_APP_VERSION STR(VERSION_MAJOR) "." STR(VERSION_MINOR) "." STR(VERSION_BUILD)
#define TRV_APP_COMPANY_NAME "Evolke"
#define TRV_APP_NAME "Traverse"


struct KeyValItem
{
    KeyValItem(QString ikey, QString ival) { key = ikey; value = ival; }
    QString key;
    QString value;
};


typedef QList<KeyValItem> KeyValList;

#endif // TRV_DEFINES_H
