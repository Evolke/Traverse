#include "prefsdialog.h"
#include "urlpixmap.h"
#include "mainwindow.h"
#include "trv_storage.h"

#include <QDialogButtonBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTreeWidget>
#include <QStackedWidget>
#include <QDebug>
#include <QComboBox>
#include <QGroupBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QToolBar>



/**
 * @brief PrefsDialog::PrefsDialog
 * @param parent
 */
PrefsDialog::PrefsDialog(QWidget *parent) : TRVDialog(parent)
{
    init(600,300,parent);

    QGridLayout *mainLayout = new QGridLayout(this);
    m_pTabs = new QTreeWidget(this);
    m_pTabs->setIconSize(QSize(16, 16));
    m_pTabs->setHeaderHidden(true);
    m_pTabs->setIndentation(0);

    //m_pTabs->setMovement(QListView::Static);
    m_pTabs->setMaximumWidth(150);
    //m_pTabs->setSpacing(5);
    //m_pTabs->setWordWrap(true);
    //m_pTabs->setWrapping(true);
    //m_pTabs->setGridSize(QSize(80,64));

    m_pPages = new QStackedWidget(this);
    m_pPages->addWidget(new UIPrefsPage(this));
    mainLayout->addWidget(m_pTabs,0,0);
    mainLayout->addWidget(m_pPages,0,1);
    mainLayout->addWidget(m_pBtnBox,1,1,1,1, Qt::AlignBottom);
    //mainLayout->setMargin(5);
    createTreeItems();
    //m_pTabs->setCurrentRow(0);

    setWindowTitle(tr("Preferences"));
}

void PrefsDialog::changePage(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    if (!current)
        current = previous;

    m_pPages->setCurrentIndex(m_pTabs->indexOfTopLevelItem(current));
}


void PrefsDialog::changeTheme(QString text)
{
    MainWindow *pMain = (MainWindow*)parentWidget();
    pMain->setTheme(text);
}


/**
 * @brief PrefsDialog::createListItems
 */
void PrefsDialog::createTreeItems()
{
    QTreeWidgetItem *configButton = new QTreeWidgetItem(m_pTabs);
    UrlPixmap svgPm(Q_NULLPTR);
    QColor txtClr = parentWidget()->palette().color(QPalette::Text);
    QString sBorderClr = txtClr.name(QColor::HexRgb);
    //qDebug() << sBorderClr;

    // svgPm.loadSVGResource(":/images/general_pref_icon.svg", sBorderClr, QSize(16,16));
    // configButton->setIcon(0,QIcon(*svgPm.getPixmap()));
    // configButton->setText(0,tr("General"));
    //m_pTabs->setCurrentItem(configButton);

    //configButton->setTextAlignment(Qt::AlignHCenter);
    //configButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    configButton = new QTreeWidgetItem(m_pTabs);
    svgPm.loadSVGResource(":/images/ui_pref_icon.svg", sBorderClr, QSize(16,16));
    configButton->setIcon(0,QIcon(*svgPm.getPixmap()));
    configButton->setText(0,tr("User Interface"));
    //configButton->setTextAlignment(Qt::AlignHCenter);
    //configButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    connect(m_pTabs, &QTreeWidget::currentItemChanged, this, &PrefsDialog::changePage);
}


int PrefsDialog::getUIToolbarButtonType()
{
    UIPrefsPage *pPage = dynamic_cast<UIPrefsPage*>(m_pPages->widget(0));

    QComboBox *pTBBox = pPage->getToolbarCombo();

    return pTBBox->currentIndex();
}

PrefsInfo PrefsDialog::getPreferences()
{
    PrefsInfo ret;
    UIPrefsPage *pPage = dynamic_cast<UIPrefsPage*>(m_pPages->widget(0));

    QComboBox *pTBBox = pPage->getToolbarCombo();
    QComboBox *pThemeCB = pPage->getThemeCombo();
    ret.nToolBarStyle = pTBBox->currentIndex();
    ret.sTheme = pThemeCB->currentText();

    return ret;
}

void PrefsDialog::setPreferences(PrefsInfo prefs)
{
    UIPrefsPage *pPage = dynamic_cast<UIPrefsPage*>(m_pPages->widget(0));

    QComboBox *pTBBox = pPage->getToolbarCombo();
    QComboBox *pThemeCB = pPage->getThemeCombo();
    pTBBox->setCurrentIndex(prefs.nToolBarStyle);
    pThemeCB->setCurrentText(prefs.sTheme);
}

/******************* UIPrefsPage ***************/
UIPrefsPage::UIPrefsPage(QWidget *parent) : QWidget(parent)
{
    QGroupBox *pUIBox = new QGroupBox();
    QGridLayout *pUILayout = new QGridLayout(this);
    QLabel *pThemeLabel = new QLabel(tr("Theme:"));
    pThemeLabel->setMaximumWidth(60);
    m_pThemeCombo = new QComboBox(this);
    MainWindow *pMain = (MainWindow*)parent->parentWidget();
    QStringList themes = pMain->getStorage()->getThemes();
    m_pThemeCombo->addItems(themes);
    QString sTheme = pMain->getTheme();
    int index = themes.indexOf(QRegularExpression(sTheme));
    if (index > -1)
    {
        m_pThemeCombo->setCurrentIndex(index);
    }
    pUILayout->addWidget(pThemeLabel, 0,0);
    pUILayout->addWidget(m_pThemeCombo, 0,1);

    // QGridLayout *pTBLayout = new QGridLayout(this);
    QLabel *pTBLabel = new QLabel(tr("Toolbar Buttons:"));
    pTBLabel->setMaximumWidth(100);
    m_pToolbarCombo = new QComboBox(this);
    m_pToolbarCombo->addItem(tr("Icon Only"));
    m_pToolbarCombo->addItem(tr("Icon with Text"));
    QToolBar *pToolbar = pMain->getToolBar();
    switch (pToolbar->toolButtonStyle())
    {
        case Qt::ToolButtonTextBesideIcon:
            m_pToolbarCombo->setCurrentIndex(1);
            break;

        default:
            m_pToolbarCombo->setCurrentIndex(0);
            break;
    }

    pUILayout->addWidget(pTBLabel,1,0);
    pUILayout->addWidget(m_pToolbarCombo,1,1);

    pUIBox->setLayout(pUILayout);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(pUIBox);
    //mainLayout->addWidget(pTBBox);
    mainLayout->addSpacing(60);
    setLayout(mainLayout);

    connect(m_pThemeCombo, &QComboBox::currentIndexChanged, this, &UIPrefsPage::themeChanged);
}

void UIPrefsPage::themeChanged(int index)
{
    QString sTheme = m_pThemeCombo->itemText(index);
    ((PrefsDialog*)parentWidget())->changeTheme(sTheme);
}

QComboBox* UIPrefsPage::getToolbarCombo()
{
    return m_pToolbarCombo;
}

QComboBox* UIPrefsPage::getThemeCombo()
{
    return m_pThemeCombo;
}
