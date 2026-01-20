#include "mainwindow.h"
#include "trv_version.h"
#include "statusprogressbar.h"
#include "aboutdialog.h"
#include "trv_storage.h"
#include "prefsdialog.h"
#include "urlpixmap.h"
#include "consoledock.h"
#include "apiclient/restmdichild.h"
#include "ftp/ftpmdichild.h"

#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QSettings>
#include <QApplication>
#include <QScreen>
#include <QDir>
#include <QStyleOptionToolBar>

#define TOOLBAR_ICON_SIZE 24

MainWindow* MainWindow::m_pSingleInst = Q_NULLPTR;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_pMdiArea(new QMdiArea)
{
    m_pStorage = new TRV_Storage();
    m_sTheme = "none";
    setCentralWidget(m_pMdiArea);
    m_pMdiArea->setViewMode(QMdiArea::TabbedView);
    m_pMdiArea->setTabsClosable(true);
    m_pMdiArea->setTabsMovable(true);
    m_pMdiArea->setTabPosition(QTabWidget::North);
    m_pCurrentChild = Q_NULLPTR;

    setInstance(this);

    init();
}

MainWindow::~MainWindow()
{
}

void MainWindow::init()
{
    m_pToolBar = addToolBar(tr(TRV_APP_NAME));
    m_pToolBar->setIconSize(QSize(TOOLBAR_ICON_SIZE, TOOLBAR_ICON_SIZE));
    connect(m_pMdiArea,&QMdiArea::subWindowActivated,this,&MainWindow::activateChild);

    setWindowTitle(tr(TRV_APP_NAME));
    QStatusBar *pStat = statusBar();
    pStat->showMessage(tr("Ready"));
    m_pStatProg = new StatusProgressBar(pStat);
    pStat->addPermanentWidget(m_pStatProg);
    createActions();
    //createDocks must follow createActions
    createDocks();

    readSettings();
    m_pStatProg->hide();

    m_pMdiArea->setBackground(Qt::NoBrush);

}

void MainWindow::createActions()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
    m_pViewMenu = menuBar()->addMenu(tr("&View"));
    //QMenu *wndMenu = menuBar()->addMenu(tr("&Window"));
    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));

    QMenu *newMenu = fileMenu->addMenu(tr("&New"));
    QAction *reqAct = new QAction(tr("&New Request"));
    reqAct->setStatusTip(tr("Create New Request"));

    newMenu->addAction(reqAct);
    connect(reqAct, &QAction::triggered, this, &MainWindow::newRequestWindow);
    QAction *ftpAct = new QAction(tr("&New Connection"));
    ftpAct->setStatusTip(tr("Create New Connection"));
    newMenu->addAction(ftpAct);
    connect(ftpAct, &QAction::triggered, this, &MainWindow::newFtpWindow);


#ifdef Q_OS_WIN
    QString sQuit = tr("&Exit");
#else
    QString sQuit = tr("&Quit");
#endif
    QAction *exitAct = new QAction(sQuit);
    //exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Quit the application"));
    connect(exitAct, &QAction::triggered, this, &QWidget::close);
    fileMenu->addAction(exitAct);

    QAction *prfAct = new QAction(tr("&Preferences..."));
    editMenu->addAction(prfAct);
    m_actionMap["prefs"] = prfAct;
    connect(prfAct, &QAction::triggered, this, &MainWindow::preferences);

    QAction *tbAct = new QAction(tr("&Toolbar"));
    m_pViewMenu->addAction(tbAct);
    QAction *sbAct = new QAction(tr("&Statusbar"));
    m_pViewMenu->addAction(sbAct);
    tbAct->setCheckable(true);
    tbAct->setChecked(true);
    sbAct->setCheckable(true);
    sbAct->setChecked(true);
    connect(tbAct, &QAction::toggled, this, &MainWindow::toggleToolBar);
    connect(sbAct, &QAction::toggled, this, &MainWindow::toggleStatusBar);

    QAction *abtAct = new QAction(tr("About"));
    helpMenu->addAction(abtAct);
    connect(abtAct, &QAction::triggered, this, &MainWindow::about);

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);

    writeSettings();
}

void MainWindow::about()
{
    /*QMessageBox::about(this, tr("About GitBusyLivin"),
            tr("Hope is a good thing, maybe the best of things, and no good thing ever dies."));*/
    AboutDialog about(this);
    about.exec();
}

void MainWindow::preferences()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    QString currentTheme = settings.value("UI/Theme", "none").toString();

    PrefsDialog prefsDlg(this);
    PrefsInfo prefs;
    prefs.sTheme = currentTheme;
    prefs.nToolBarStyle = settings.value("UI/ToolbarStyle",0).toInt();
    prefsDlg.setPreferences(prefs);
    if (prefsDlg.exec() == QDialog::Accepted)
    {
        QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
        PrefsInfo prefs = prefsDlg.getPreferences();
        settings.setValue("UI/Theme", prefs.sTheme);

        int nTBType = prefsDlg.getUIToolbarButtonType();
        settings.setValue("UI/ToolbarStyle", nTBType);
        Qt::ToolButtonStyle nTBStyle;
        switch (nTBType)
        {
            case 1:
                nTBStyle = Qt::ToolButtonTextBesideIcon;
                break;
            default:
                nTBStyle = Qt::ToolButtonIconOnly;
                break;
        }

        m_pToolBar->setToolButtonStyle(nTBStyle);
    }
    else
    {
        setTheme(currentTheme);
    }
}

void MainWindow::toggleStatusBar()
{
    if (statusBar()->isVisible())
    {
        statusBar()->hide();
    }
    else
    {
        statusBar()->show();
    }
}

void MainWindow::toggleToolBar()
{
    if (m_pToolBar->isVisible())
    {
        m_pToolBar->hide();
    }
    else
    {
        m_pToolBar->show();
    }
}

void MainWindow::readSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());

    QByteArray state = settings.value("MainWindow/WindowState", QByteArray()).toByteArray();
    if (!state.isEmpty())
    {
        restoreState(state);
    }

    const QByteArray geometry = settings.value("MainWindow/Geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty()) {
        const QRect availableGeometry = QGuiApplication::primaryScreen()->availableGeometry();
        resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2,
             (availableGeometry.height() - height()) / 2);
    } else {
        restoreGeometry(geometry);
    }

    setTheme(settings.value("UI/Theme", "none").toString());

    int nTBType = settings.value("UI/ToolbarStyle",0).toInt();
    Qt::ToolButtonStyle nTBStyle;
    switch (nTBType)
    {
        case 1:
            nTBStyle = Qt::ToolButtonTextBesideIcon;
            break;
        default:
            nTBStyle = Qt::ToolButtonIconOnly;
            break;
    }

    m_pToolBar->setToolButtonStyle(nTBStyle);

    UrlPixmap svgpix(Q_NULLPTR);

    QStyleOptionToolBar option;
    option.initFrom(m_pToolBar);
    QPalette pal = option.palette;
    QColor txtClr = pal.color(QPalette::Text);
    QString sBorderClr = txtClr.name(QColor::HexRgb);

    m_pToolBar->setIconSize(QSize(TOOLBAR_ICON_SIZE,TOOLBAR_ICON_SIZE));

    svgpix.loadSVGResource(":/images/pref_toolbar_icon.svg", sBorderClr, QSize(TOOLBAR_ICON_SIZE,TOOLBAR_ICON_SIZE));
    QAction *prefAct = m_actionMap["prefs"];
    prefAct->setIcon(QIcon(*svgpix.getSmallPixmap(TOOLBAR_ICON_SIZE)));
    m_pToolBar->addAction(prefAct);

}

void MainWindow::writeSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("MainWindow/Geometry", saveGeometry());
    settings.setValue("MainWindow/WindowState", saveState());
}

void MainWindow::activateChild(QMdiSubWindow *window)
{
    QFrame *oldChild = m_pCurrentChild;
    m_pCurrentChild = currentMdiChild();
    if (oldChild != m_pCurrentChild) {

    }

}

QFrame* MainWindow::currentMdiChild()
{
    QMdiSubWindow *currentSubWindow = m_pMdiArea->currentSubWindow();
    if (currentSubWindow)
    {
        QWidget *pChild = currentSubWindow->widget();
        QString sChildClass = pChild->metaObject()->className();
        if (sChildClass == "RestMdiChild" || sChildClass == "FtpMdiChild")
        {
            return qobject_cast<QFrame*>(pChild);
        }
    }

    return Q_NULLPTR;
}

void MainWindow::newRequestWindow()
{
    openTab(new RestMdiChild(m_pMdiArea));
}

void MainWindow::newFtpWindow()
{
    openTab(new FtpMdiChild(m_pMdiArea));
}

void MainWindow::openTab(QFrame *pChild)
{
    QMdiSubWindow *pSubWnd = m_pMdiArea->addSubWindow(pChild);
    m_pMdiArea->setActiveSubWindow(pSubWnd);
    pChild->show();
    m_pCurrentChild = pChild;
}

void MainWindow::createDocks()
{
    ConsoleDock* pConsole = new ConsoleDock(tr("Console"));
    m_docks["console"] = pConsole;
    addDockWidget(Qt::BottomDockWidgetArea, pConsole);
    pConsole->hide();

    QAction *conAct = pConsole->toggleViewAction();//new QAction(tr("&Console"),pConsole);
    m_actionMap["console"] = conAct;
    conAct->setCheckable(true);
    conAct->setChecked(false);
    m_pViewMenu->addAction(conAct);
}

void MainWindow::setTheme(const QString &theme)
{
    QString styleSheet;
    QString sPath;

    //m_sTheme.assign(QAnyStringView(theme.toUtf8()));

    if (theme != "none")
    {
        if (theme == "dark")
        {
            sPath = ":/styles/dark.qss";
        }
        else if (theme == "light")
        {
            sPath = ":/styles/light.qss";
        }
        else
        {
            sPath = TRV_Storage::getThemesPath();
            sPath += QDir::separator();
            sPath += theme;
            sPath += ".qss";
        }
    }

    if (!sPath.isEmpty())
    {
        QFile file(sPath);
        if (file.open(QFile::ReadOnly)) {
            styleSheet = QString::fromUtf8(file.readAll());
        }
    }

    QApplication *app = (QApplication*)QCoreApplication::instance();
    app->setStyleSheet(styleSheet);
}

void MainWindow::toggleProgressBar()
{
    if (m_pStatProg->isVisible()) {
        m_pStatProg->hide();
    } else {
        m_pStatProg->show();
    }
}

void MainWindow::logToConsole(QString &log)
{
    ConsoleDock *pConsole = (ConsoleDock*)m_docks["console"];
    if (pConsole) {
        pConsole->addText(log);
    }
}

