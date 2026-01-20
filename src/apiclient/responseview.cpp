#include "responseview.h"
#include "../trvcodeeditor.h"

#include <QTabWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QTableWidget>
#include <QJsonDocument>
#include <QtXml/QDomDocument>
#include <QLabel>
#include <QHeaderView>
#include <QtNetwork/QHttpHeaders>

/**
 * @brief ResponseView::ResponseView
 * @param parent
 */
ResponseView::ResponseView(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout *layout = new QVBoxLayout;
    m_pTabs =  new QTabWidget(this);
    m_pEditor = new TrvCodeEditor(m_pTabs);
    m_pHeadersTable = new QTableWidget(m_pTabs);
    m_pHeadersTable->verticalHeader()->setVisible(false);
    QStringList headings = {"Key", "Value"};
    m_pHeadersTable->setColumnCount(2);
    m_pHeadersTable->setHorizontalHeaderLabels(headings);
    m_pHeadersTable->setVerticalHeaderLabels({""});
    m_pTabs->addTab(m_pEditor, tr("Body"));
    m_pEditor->setReadOnly(true);
    m_pTabs->addTab(m_pHeadersTable, tr("Headers"));
    m_pStatus = new ResponseStatus(this);
    layout->addWidget(m_pTabs);
    layout->addWidget(m_pStatus);
    layout->setContentsMargins(0,0,0,0);
    setLayout(layout);
}


void ResponseView::setDataWithHeaders(QByteArray& data, QHttpHeaders &headers)
{
    QByteArrayView contentType = headers.value(QHttpHeaders::WellKnownHeader::ContentType);
    QString sContentType = contentType.toByteArray();
    if (sContentType.contains(QRegularExpression("(application|text)\\/json"))) {
        QJsonDocument jdoc = QJsonDocument::fromJson(data);
        m_pEditor->setJson(jdoc);
    } else if (sContentType.contains(QRegularExpression("application\\/(?:soap\\+)*xml"))) {
        QDomDocument xdoc;
        xdoc.setContent(data);
        m_pEditor->setXml(xdoc);
    }
    setHeaders(headers);
}

/**
 * @brief ResponseView::setStatus
 * @param status
 */
void ResponseView::setStatus(int status)
{
    m_pStatus->setStatus(status);
}

/**
 * @brief ResponseView::setExecTime
 * @param time
 */
void ResponseView::setExecTime(qint64 time)
{
    m_pStatus->setExecTime(time);
}

/**
 * @brief ResponseView::setSize
 * @param size
 */
void ResponseView::setSize(qsizetype size)
{
    m_pStatus->setSize(size);
}

/**
 * @brief ResponseView::setHeaders
 * @param headers
 */
void ResponseView::setHeaders(QHttpHeaders &headers)
{
    m_pHeadersTable->clearContents();
    m_pHeadersTable->setRowCount(headers.size());
    QList<std::pair<QByteArray, QByteArray>> headerList = headers.toListOfPairs();
    QString sKey, sValue;
    QTableWidgetItem *pKeyItem, *pValueItem;
    for (int i = 0; i < headerList.length(); i++) {
        sKey = headerList[i].first;
        sValue = headerList[i].second;
        pKeyItem = new QTableWidgetItem(sKey);
        pKeyItem->setFlags(pKeyItem->flags() & ~Qt::ItemIsEditable);
        pValueItem = new QTableWidgetItem(sValue);
        pValueItem->setFlags(pValueItem->flags() & ~Qt::ItemIsEditable);
        m_pHeadersTable->setItem(i,0,pKeyItem);
        m_pHeadersTable->setItem(i,1,pValueItem);
    }
    m_pHeadersTable->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);

}

/**
 * @brief ResponseStatus::ResponseStatus
 * @param parent
 */
ResponseStatus::ResponseStatus(QWidget *parent)
    : QWidget{parent}
{
    QGridLayout *layout = new QGridLayout(this);
    m_pStatusLabel = new QLabel(tr(""));
    m_pStatusLabel->setAlignment(Qt::AlignRight);
    m_pStatusLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    m_pStatusLabel->setMaximumWidth(60);
    m_pStatusLabel->setMinimumWidth(60);
    m_pExecTimeLabel = new QLabel(tr(""));
    m_pExecTimeLabel->setAlignment(Qt::AlignRight);
    m_pExecTimeLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    m_pExecTimeLabel->setMaximumWidth(100);
    m_pExecTimeLabel->setMinimumWidth(100);
    m_pSizeLabel = new QLabel(tr(""));
    m_pSizeLabel->setAlignment(Qt::AlignRight);
    m_pSizeLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    m_pSizeLabel->setMaximumWidth(100);
    m_pSizeLabel->setMinimumWidth(100);
    layout->addWidget(m_pStatusLabel,0,0);
    layout->addWidget(m_pExecTimeLabel,0,1);
    layout->addWidget(m_pSizeLabel,0,2);
    layout->addItem(new QSpacerItem(300,10,QSizePolicy::MinimumExpanding), 0, 3);
    layout->setContentsMargins(0,0,0,0);
    setMaximumHeight(30);
    layout->setVerticalSpacing(0);
    setLayout(layout);
}

/**
 * @brief ResponseStatus::setStatus
 * @param status
 */
void ResponseStatus::setStatus(int status)
{
    QString sStatus = QString::number(status);
    m_pStatusLabel->setText(sStatus);
}

/**
 * @brief ResponseStatus::setExecTime
 * @param time
 */
void ResponseStatus::setExecTime(qint64 time)
{
    QString sUnits = "ms";
    qreal output = time;
    if (time > 1000) {
        output = static_cast<long double>(time) / 1000;
        sUnits = "s";
    }
    QString sExecTime = QString::number(output);
    sExecTime += sUnits;
    m_pExecTimeLabel->setText(sExecTime);
}

/**
 * @brief ResponseStatus::setSize
 * @param size
 */
void ResponseStatus::setSize(qsizetype size)
{
    QString sUnits = "B";
    qreal output = size;
    if (size > 1024) {
        output = static_cast<long double>(size) / 1024;
        sUnits = "KB";
    }
    QString sSize = QString::number(output);
    sSize += sUnits;
    m_pSizeLabel->setText(sSize);
}
