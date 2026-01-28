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
    m_pTabs->addTab(new ParamsTab(m_pTabs),tr("Params"));
    m_pTabs->addTab(new AuthTab(m_pTabs),tr("Auth"));
    m_pTabs->addTab(new HeadersTab(m_pTabs),tr("Headers"));
    m_pTabs->addTab(new BodyTab(m_pTabs),tr("Body"));
    layout->addWidget(m_pTabs);
    layout->setContentsMargins(0,0,0,0);
    setLayout(layout);
}

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
    return ret;
}

int RequestOptions::getBodyType()
{
    BodyTab *pTab = qobject_cast<BodyTab*>(m_pTabs->widget(3));
    return pTab->getBodyType();
}

QString RequestOptions::getBodyString()
{
    QString ret;
    BodyTab *pTab = qobject_cast<BodyTab*>(m_pTabs->widget(3));
    TrvScintillaEdit *pEditor = pTab->getEditor();
    ret = pEditor->getText(pEditor->textLength());
    return ret;
}

RequestOptionTab::RequestOptionTab(QWidget *parent)
    : QWidget(parent)
{

}

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
    m_pBodyEditor = new TrvScintillaEdit(this);
    m_pBodyEditor->setContentType("application/json",false);
    m_pDTabs->addTab(new QWidget(m_pDTabs), "None");
    m_pDTabs->addTab(m_pBodyEditor, "JSON");
    RequestOptionTab *pFormDataTab = new RequestOptionTab(m_pDTabs);
    pFormDataTab->buildKeyValTable();
    m_pDTabs->addTab(pFormDataTab,"Form-Data");
    setLayout(layout);
}

int BodyTab::getBodyType()
{
    return m_pDTabs->currentIndex();
}
