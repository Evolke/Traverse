#ifndef PREFSDIALOG_H
#define PREFSDIALOG_H

#include "trvdialog.h"

QT_BEGIN_NAMESPACE
    class QTreeWidget;
    class QStackedWidget;
    class QLineEdit;
    class QTreeWidgetItem;
    class QComboBox;
    class QCheckBox;
    class QSpinBox;
QT_END_NAMESPACE

struct PrefsInfo
{
    PrefsInfo() {
        sTheme = "none";
        nToolBarStyle = 0;
    }
    QString sTheme;
    int nToolBarStyle;
};

class UIPrefsPage : public QWidget
{
    Q_OBJECT

public:
    explicit UIPrefsPage(QWidget *parent = Q_NULLPTR);

    QComboBox* getToolbarCombo();
    QComboBox* getThemeCombo();

public slots:
    void themeChanged(int index);

private:
    QComboBox *m_pThemeCombo, *m_pToolbarCombo;
};

/**
 * @brief The PrefsDialog class
 */
class PrefsDialog : public TRVDialog
{
    Q_OBJECT
public:
    explicit PrefsDialog(QWidget *parent = 0);

    int getUIToolbarButtonType();
    PrefsInfo getPreferences();
    void setPreferences(PrefsInfo prefs);

signals:

public slots:
    void changePage(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void changeTheme(const QString text);

private:
    void createTreeItems();

    QTreeWidget *m_pTabs;
    QStackedWidget *m_pPages;
};

#endif // PREFSDIALOG_H
