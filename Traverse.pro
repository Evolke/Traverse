QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT += network
QT += svg

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/flowlayout.cpp \
    src/ftpmdichild.cpp \
    src/aboutdialog.cpp \
    src/consoledock.cpp \
    src/localfileview.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/prefsdialog.cpp \
    src/remotefilemodel.cpp \
    src/remotefileview.cpp \
    src/restmdichild.cpp \
    src/statusprogressbar.cpp \
    src/trv_storage.cpp \
    src/trvdialog.cpp \
    src/urlpixmap.cpp

HEADERS += \
    src/flowlayout.h \
    src/ftpmdichild.h \
    src/aboutdialog.h \
    src/consoledock.h \
    src/localfileview.h \
    src/mainwindow.h \
    src/prefsdialog.h \
    src/remotefilemodel.h \
    src/remotefileview.h \
    src/restmdichild.h \
    src/statusprogressbar.h \
    src/trv_storage.h \
    src/trv_version.h \
    src/trvdialog.h \
    src/urlpixmap.h

TRANSLATIONS += \
    Traverse_en_US.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources/traverse.qrc

RC_ICONS = resources/images/traverse.ico
ICON = resources/images/traverse.icns

VERSION_MAJOR = 0
VERSION_MINOR = 0
VERSION_BUILD = 1

DEFINES += "VERSION_MAJOR=$$VERSION_MAJOR"\
       "VERSION_MINOR=$$VERSION_MINOR"\
       "VERSION_BUILD=$$VERSION_BUILD"

#Target version
VERSION = $${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_BUILD}
