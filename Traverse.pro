QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT += network
QT += svg
QT += xml

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/apiclient/authtab.cpp \
    src/apiclient/responseview.cpp \
    src/apiclient/requestoptions.cpp \
    src/dropdowntabwidget.cpp \
    src/flowlayout.cpp \
    src/ftp/ftpmdichild.cpp \
    src/aboutdialog.cpp \
    src/consoledock.cpp \
    src/ftp/localfileview.cpp \
    src/keyvaltableview.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/prefsdialog.cpp \
    src/ftp/remotefilemodel.cpp \
    src/ftp/remotefileview.cpp \
    src/apiclient/restmdichild.cpp \
    src/statusprogressbar.cpp \
    src/trv_storage.cpp \
    src/trvcodeeditor.cpp \
    src/trvdialog.cpp \
    src/trvscintillaedit.cpp \
    src/trvthread.cpp \
    src/urlpixmap.cpp

HEADERS += \
    src/apiclient/authtab.h \
    src/apiclient/responseview.h \
    src/apiclient/requestoptions.h \
    src/dropdowntabwidget.h \
    src/flowlayout.h \
    src/ftp/ftpmdichild.h \
    src/aboutdialog.h \
    src/consoledock.h \
    src/ftp/localfileview.h \
    src/keyvaltableview.h \
    src/mainwindow.h \
    src/prefsdialog.h \
    src/ftp/remotefilemodel.h \
    src/ftp/remotefileview.h \
    src/apiclient/restmdichild.h \
    src/statusprogressbar.h \
    src/trv_storage.h \
    src/trv_version.h \
    src/trvcodeeditor.h \
    src/trvdialog.h \
    src/trvscintillaedit.h \
    src/trvthread.h \
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

unix|win32: LIBS += -L$$PWD/scintilla/qt/ScintillaEdit/bin/ -lScintillaEdit5
unix|win32: LIBS += -L$$PWD/lexilla/src/build/bin/ -lLexilla5
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/K-Editor/lib/build/Desktop_Qt_6_10_1_MSVC2022_64bit-Release/bin -lK-Editor-lib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/K-Editor/lib/build/Desktop_Qt_6_10_1_MSVC2022_64bit-Debug/bin -lK-Editor-lib
else:unix: LIBS += -L$$PWD/K-Editor/lib/build/Desktop_Qt_6_10_1_MSVC2022_64bit-Debug/ -lK-Editor-lib

INCLUDEPATH += $$PWD/scintilla/include
INCLUDEPATH += $$PWD/scintilla/src
INCLUDEPATH += $$PWD/scintilla/qt/ScintillaEdit
INCLUDEPATH += $$PWD/scintilla/qt/ScintillaEditBase
DEPENDPATH += $$PWD/scintilla/include



INCLUDEPATH += $$PWD/K-Editor/lib/
INCLUDEPATH += $$PWD/K-Editor/lib/public/
DEPENDPATH += $$PWD/K-Editor/lib/
