#include "requestoptions.h"
#include "authtab.h"
#include "../trvscintillaedit.h"
#include "../keyvaltableview.h"
#include "../dropdowntabwidget.h"

#include <QLabel>
#include <QTabWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QComboBox>
#include <QToolButton>
#include <QLineEdit>
#include <QtNetwork/QHttpHeaders>

RequestOptions::RequestOptions(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout *layout = new QVBoxLayout;
    m_pTabs =  new QTabWidget(this);
    m_pTabs->addTab(new AuthTab(m_pTabs),tr("Auth"));
    m_pTabs->addTab(new BodyTab(m_pTabs),tr("Body"));
    m_pTabs->addTab(new HeadersTab(m_pTabs),tr("Headers"));
    m_pTabs->addTab(new ParamsTab(m_pTabs),tr("Params"));

    layout->addWidget(m_pTabs);
    layout->setContentsMargins(0,0,0,0);
    setLayout(layout);
}

/**
 * @brief RequestOptions::getHeaders
 * @return
 */
QHttpHeaders RequestOptions::getHeaders()
{
    QHttpHeaders ret;

    HeadersTab *pTab = qobject_cast<HeadersTab*>(m_pTabs->widget(2));
    KeyValTableView *pTable = pTab->getKeyValTable();
    int rows = pTable->rowCount();
    QString sKey, sValue;
    for (int i=0; i < rows; i++) {
        sKey = pTable->getCellData(i,0);
        sValue = pTable->getCellData(i,1);
        ret.append(sKey, sValue);
    }

    //add auth headers
    AuthTab *pAuthTab = qobject_cast<AuthTab*>(m_pTabs->widget(AUTH_REQUEST_OPTION));
    KeyValList kvlist = pAuthTab->getHeaders();
    for (int i=0; i < kvlist.length(); i++) {
        KeyValItem item = kvlist[i];
        ret.append(item.key, item.value);
    }
    return ret;
}

/**
 * @brief RequestOptions::getBodyType
 * @return
 */
int RequestOptions::getBodyType()
{
    BodyTab *pTab = qobject_cast<BodyTab*>(m_pTabs->widget(BODY_REQUEST_OPTION));
    return pTab->getBodyType();
}

/**
 * @brief RequestOptions::getBodyString
 * @return
 */
QString RequestOptions::getBodyString()
{
    QString ret;
    BodyTab *pTab = qobject_cast<BodyTab*>(m_pTabs->widget(BODY_REQUEST_OPTION));
    TrvScintillaEdit *pEditor = pTab->getEditor();
    ret = pEditor->getText(pEditor->textLength());
    return ret;
}

QString RequestOptions::getRawBodyContentType()
{
    QString ret;
    BodyTab *pTab = qobject_cast<BodyTab*>(m_pTabs->widget(BODY_REQUEST_OPTION));

    switch (pTab->getRawBodyType()) {
        case JSON_RAW_TYPE:
            ret = "application/json";
            break;
        case XML_RAW_TYPE:
            ret = "application/xml";
            break;
    }

    return ret;
}

KeyValList RequestOptions::getBodyFormDataList()
{
    BodyTab *pTab = qobject_cast<BodyTab*>(m_pTabs->widget(BODY_REQUEST_OPTION));
    return pTab->getFormDataList();
}

/**
 * @brief RequestOptionTab::RequestOptionTab
 * @param parent
 */
RequestOptionTab::RequestOptionTab(QWidget *parent)
    : QWidget(parent)
{

}

/**
 * @brief RequestOptionTab::buildKeyValTable
 */
void RequestOptionTab::buildKeyValTable()
{
    QVBoxLayout *layout = new QVBoxLayout;
    m_pKeyValTable = new KeyValTableView(this);
    QToolButton *pAddBtn = new QToolButton(this);
    pAddBtn->setText(tr("Add+"));
    connect(pAddBtn, &QToolButton::clicked, this, &RequestOptionTab::addRow);
    layout->addWidget(pAddBtn,0,Qt::AlignRight);
    layout->addWidget(m_pKeyValTable);
    layout->setContentsMargins(0,0,0,0);
    setLayout(layout);
}

/**
 * @brief RequestOptionTab::addRow
 */
void RequestOptionTab::addRow()
{
    //m_pHeadersTable->setRowCount(m_pHeadersTable->rowCount()+1);
    m_pKeyValTable->addRow();
}

/**
 * @brief ParamsTab::ParamsTab
 * @param parent
 */
ParamsTab::ParamsTab(QWidget *parent)
    : RequestOptionTab(parent)
{
    buildKeyValTable();
}


/**
 * @brief HeadersTab::HeadersTab
 * @param parent
 */
HeadersTab::HeadersTab(QWidget *parent)
    : RequestOptionTab(parent)
{
    buildKeyValTable();
}


/**
 * @brief BodyTab::BodyTab
 * @param parent
 */
BodyTab::BodyTab(QWidget *parent)
    : RequestOptionTab(parent)
{
    QVBoxLayout *layout = new QVBoxLayout;
    m_pDTabs = new DropDownTabWidget(this);
    layout->addWidget(m_pDTabs);
    layout->setContentsMargins(0, 0, 0, 0);
    m_pDTabs->addTab(new QWidget(m_pDTabs), "None");
    m_pDTabs->addTab(new RawBodyTab(this), "Raw");
    RequestOptionTab *pFormDataTab = new RequestOptionTab(m_pDTabs);
    pFormDataTab->buildKeyValTable();
    m_pDTabs->addTab(pFormDataTab,"Form Data");
    setLayout(layout);
}

/**
 * @brief BodyTab::getBodyType
 * @return
 */
int BodyTab::getBodyType()
{
    return m_pDTabs->currentIndex();
}

/**
 * @brief BodyTab::getRawBodyType
 * @return
 */
int BodyTab::getRawBodyType()
{
    int index = -1;
    if (getBodyType() == RAW_BODY_TYPE) {
        RawBodyTab *pTab = qobject_cast<RawBodyTab*>(m_pDTabs->getTab(RAW_BODY_TYPE));
        index = pTab->getRawBodyType();
    }
    return index;
}


/**
 * @brief BodyTab::getEditor
 * @return
 */
TrvScintillaEdit *BodyTab::getEditor()
{
    RawBodyTab *pTab = qobject_cast<RawBodyTab*>(m_pDTabs->getTab(RAW_BODY_TYPE));
    return pTab->getEditor();
}

KeyValList BodyTab::getFormDataList()
{
    RequestOptionTab *pFormDataTab = qobject_cast<RequestOptionTab*>(m_pDTabs->getTab(FORM_DATA_BODY_TYPE));
    KeyValTableModel *pModel = qobject_cast<KeyValTableModel*>(pFormDataTab->getKeyValTable()->model());

    return pModel->getKeyValList();
}


/**
 * @brief RawBodyTab::RawBodyTab
 * @param parent
 */
RawBodyTab::RawBodyTab(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(0,0,0,0);
    m_pRawTypeCombo = new QComboBox(this);
    m_pRawTypeCombo->addItems({"JSON", "XML"});
    m_pBodyEditor = new TrvScintillaEdit(this);
    m_pBodyEditor->setContentType("application/json",false);
    layout->addWidget(m_pRawTypeCombo);
    layout->addWidget(m_pBodyEditor);
    setLayout(layout);

    connect(m_pRawTypeCombo, &QComboBox::currentIndexChanged, this, &RawBodyTab::onRawTypeChanged);
}

/**
 * @brief RawBodyTab::getRawBodyType
 * @return
 */
int RawBodyTab::getRawBodyType()
{
    return m_pRawTypeCombo->currentIndex();
}

/**
 * @brief RawBodyTab::onRawTypeChanged
 * @param index
 */
void RawBodyTab::onRawTypeChanged(int index)
{
    m_pBodyEditor->setupGlobalStyles();
    switch ((RawTypes)index)
    {
        case JSON_RAW_TYPE:
            m_pBodyEditor->setContentType("application/json",false);
            break;
        case XML_RAW_TYPE:
            m_pBodyEditor->setContentType("application/xml",false);
            break;
    }
}
