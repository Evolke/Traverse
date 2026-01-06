#include "trv_storage.h"
#include <QDir>
#include <QStringList>
#include <QTextStream>
#include <QCryptographicHash>
#include <QDebug>
#include <QCoreApplication>

TRV_Storage::TRV_Storage()
{

}

QString TRV_Storage::getStoragePath()
{
    QString sStoragePath;
    QTextStream(&sStoragePath) << QDir::homePath() << QDir::separator() << TRV_STORAGE_DIR;

    return sStoragePath;
}

QString TRV_Storage::getCachePath()
{
    QString sCachePath;
    QTextStream(&sCachePath) << getStoragePath() << QDir::separator() << TRV_STORAGE_CACHE_DIR;

    return sCachePath;
}

QString TRV_Storage::getThemesPath()
{
    QString sThemesPath;
    QTextStream(&sThemesPath) << getStoragePath() << QDir::separator() << TRV_STORAGE_THEMES_DIR;

    return sThemesPath;
}

QString TRV_Storage::getGravatarUrl(QString sEmail)
{
    QByteArray baEmail = sEmail.toUtf8();
    QByteArray ba = QCryptographicHash::hash(baEmail, QCryptographicHash::Md5);
    QString sUrl;
    QTextStream(&sUrl) << "https://www.gravatar.com/avatar/" << ba.toHex() << "?d=identicon&s=40";
    //qDebug() << "getGravatarUrl:" << sUrl;
    return sUrl;
}

QStringList TRV_Storage::getThemes()
{
    QString sThemesPath = getThemesPath();
    QDir themesDir(sThemesPath);
    if (!themesDir.exists())
    {
        themesDir.mkpath(sThemesPath);
    }

    QStringList themes;
    themes << QString(QCoreApplication::tr("none"));
    themes << QString(QCoreApplication::tr("dark"));
    themes << QString(QCoreApplication::tr("light"));

    //find themes in the themes dir
    QStringList filters;
    filters << "*.qss";
    themesDir.setNameFilters(filters);
    QStringList exThemes = themesDir.entryList();
    for (int i = 0; i < exThemes.size(); i++)
    {
        QString filename = exThemes.at(i);
        QString lcFName = filename.toLower();
        lcFName = lcFName.replace(".qss", "");
        themes << lcFName;
    }
    themes.sort();
    return themes;
}

QByteArray TRV_Storage::readRestGroups()
{
    QByteArray restGroupsData;

    QString sPath = getStoragePath();
    sPath += QDir::separator();
    sPath += TRV_STORAGE_REST_GROUPS_FILE;
    QFile file(sPath);
    if (file.open(QFile::ReadOnly))
    {
        restGroupsData = file.readAll();
    }

    return restGroupsData;
}

bool TRV_Storage::saveRestGroups(QByteArray restGroupsData)
{
    bool bRet = false;

    QString sPath = TRV_Storage::getStoragePath();
    sPath += QDir::separator();
    sPath += TRV_STORAGE_REST_GROUPS_FILE;

    QFile file(sPath);
    bool bWriteRestGroups = true;

    if (file.exists())
    {
        file.open(QFile::ReadOnly);
        QByteArray data = file.readAll();
        if (data == restGroupsData)
        {
            bWriteRestGroups = false;
        }
        file.close();
    }

    if (bWriteRestGroups)
    {
        file.open(QFile::WriteOnly);
        QTextStream jStream(&file);
        jStream << restGroupsData;
        file.close();
        bRet = true;
    }

    return bRet;
}

