#ifndef TRV_STORAGE_H
#define TRV_STORAGE_H

#define TRV_STORAGE_DIR ".gitbusylivin"
#define TRV_STORAGE_CACHE_DIR "cache"
#define TRV_STORAGE_THEMES_DIR "themes"
#define TRV_STORAGE_REST_GROUPS_FILE "rest_groups.json"


#include <QString>

class TRV_Storage
{
public:
    TRV_Storage();

    static QString getStoragePath();
    static QString getCachePath();
    static QString getGravatarUrl(QString sEmail);
    static QString getThemesPath();
    QStringList getThemes();
    QByteArray readRestGroups();
    bool saveRestGroups(QByteArray restGroupsData);

};

#endif // TRV_STORAGE_H
