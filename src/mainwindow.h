#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QMdiArea;
class StatusProgressBar;
class QMdiSubWindow;
class TRV_Storage;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    static MainWindow* getInstance() { return m_pSingleInst; }
    static void setInstance(MainWindow* pInst) { m_pSingleInst = pInst; }

    void setTheme(const QString &theme);
    QString getTheme() { return m_sTheme; }
    TRV_Storage* getStorage() { return m_pStorage; }
    QToolBar* getToolBar() { return m_pToolBar; }

    void logToConsole(QString &log);
    void toggleProgressBar();
    QMdiSubWindow* currentMdiChild();

public slots:

private slots:
    void newRequestWindow();
    void newFtpWindow();
    void about();
    void preferences();
    void toggleToolBar();
    void toggleStatusBar();
    void activateChild(QMdiSubWindow *window);
    void closeEvent(QCloseEvent *event);

private:
    void init();
    void createActions();
    void createDocks();
    void readSettings();
    void writeSettings();
    void openTab(QMdiSubWindow *pChild);

    QToolBar *m_pToolBar;
    StatusProgressBar *m_pStatProg;
    QMenu *m_pViewMenu;
    QMdiArea *m_pMdiArea;
    TRV_Storage *m_pStorage;
    QString m_sTheme;
    QMap<QString, QAction*> m_actionMap;
    QMap<QString, QDockWidget*> m_docks;
    QMdiSubWindow* m_pCurrentChild;

    static MainWindow *m_pSingleInst;


};
#endif // MAINWINDOW_H
