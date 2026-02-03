#include "restmdichild.h"
#include "../mainwindow.h"
#include "responseview.h"
#include "requestoptions.h"
#include "../trvthread.h"

#include <QSplitter>
#include <QTextEdit>
#include <QGridLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QToolButton>
#include <QLabel>
#include <QMessageBox>
#include <QJsonDocument>
#include <QtNetwork/QRestAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QRestReply>
#include <QDateTime>

/**
 * @brief RestMdiChild::RestMdiChild
 * @param parent
 */
RestMdiChild::RestMdiChild(QWidget *parent)
    : QFrame(parent)
{
    m_pSplit = new QSplitter(Qt::Vertical, this);
    m_pSplit->setFrameStyle(QFrame::StyledPanel);
    m_pSplit->setHandleWidth(2);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_pSplit);
    m_pRequestHeader = new RequestHeader(m_pSplit);
    m_pSplit->addWidget(m_pRequestHeader);
    m_pSubSplit = new QSplitter(Qt::Horizontal, m_pSplit);
    m_pSplit->addWidget(m_pSubSplit);
    m_pRequestOptions = new RequestOptions(m_pSubSplit);
    m_pSubSplit->addWidget(m_pRequestOptions);
    m_pResponse = new ResponseView(m_pSubSplit);
    m_pSubSplit->addWidget(m_pResponse);
    m_pNetMan = new QNetworkAccessManager(this);
    m_pRestMan = new QRestAccessManager(m_pNetMan);
    layout->setContentsMargins(0,0,0,0);
    setLayout(layout);
    m_pFormatThread = new TrvFormatThread();
    connect(m_pFormatThread, SIGNAL(formatFinished(QString*, QString*)), this, SLOT(formatFinished(QString*, QString*)));
}

RestMdiChild::~RestMdiChild()
{
    delete m_pFormatThread;
}

/**
 * @brief RestMdiChild::buildMultiPartFormData
 * @param mpData
 */
void RestMdiChild::buildMultiPartFormData(QHttpMultiPart &mpData)
{
    KeyValList list = m_pRequestOptions->getBodyFormDataList();
    QHttpPart textPart;

    for (int i=0; i < list.length(); i++) {
        textPart.setHeader(QNetworkRequest::ContentDispositionHeader, QString("form-data; name=\"%1\"").arg(list[i].key));
        textPart.setBody(list[i].value.toUtf8());
        mpData.append(textPart);
    }

}

/**
 * @brief RestMdiChild::prepareBody
 * @param headers
 */
void RestMdiChild::prepareBody(QHttpHeaders &headers)
{
    switch (m_pRequestOptions->getBodyType()) {
        case RAW_BODY_TYPE:
            m_data = m_pRequestOptions->getBodyString().toUtf8();
            if (m_data.length() > 0) {
                headers.append(QHttpHeaders::WellKnownHeader::ContentType, m_pRequestOptions->getRawBodyContentType());
                headers.append(QHttpHeaders::WellKnownHeader::ContentLength, QString::number(m_data.length()));
            }
            break;

        case FORM_DATA_BODY_TYPE:
            buildMultiPartFormData(m_mpData);
            headers.append(QHttpHeaders::WellKnownHeader::ContentType, QString("multipart/form-data; boundary=%1").arg(m_mpData.boundary()));
            break;
    }

}

/**
 * @brief RestMdiChild::HandleReply
 * @param pReply
 */
void RestMdiChild::HandleReply(QRestReply *pReply)
{
    MainWindow *pMain = MainWindow::getInstance();
    qint64 execTime = QDateTime::currentMSecsSinceEpoch() - m_startTime;
    QByteArray data = pReply->readBody();
    QHttpHeaders headers = pReply->networkReply()->headers();
    QString contentType = headers.value(QHttpHeaders::WellKnownHeader::ContentType).toByteArray();
    m_pResponse->setHeaders(headers);

    if (data.length() > 0 || pReply->hasError()) {
        if (data.length() == 0) { data = pReply->errorString().toUtf8(); }
        m_pResponse->setStatus(pReply->httpStatus());
        m_pResponse->setExecTime(execTime);
        m_pResponse->setSize(data.length());
        QString sText = data;
        //m_pResponse->setDataWithHeaders(sText, contentType);
        m_pFormatThread->format(data,contentType);
    } else {
        m_pRequestHeader->toggleEnableSendButton();
    }

    pMain->toggleProgressBar();
}

/**
 * @brief RestMdiChild::sendRequest
 */
void RestMdiChild::sendRequest()
{
    QString sUrl = m_pRequestHeader->getURL();
    if (sUrl.length()) {
        MainWindow *pMain = MainWindow::getInstance();
        pMain->toggleProgressBar();
        QString method = m_pRequestHeader->getMethod();
        QUrl url(sUrl);
        QNetworkRequest req(url);
        QHttpHeaders headers = m_pRequestOptions->getHeaders();
        QHttpMultiPart mpData(QHttpMultiPart::FormDataType);
        prepareBody(headers);

        req.setHeaders(headers);

        m_startTime = QDateTime::currentMSecsSinceEpoch();
        m_pRequestHeader->toggleEnableSendButton();
        QString log = QString("method:%1 url:%2").arg(method,sUrl);
        pMain->logToConsole(log);
        log = "request headers:";
        QList<std::pair<QByteArray, QByteArray>> headerList = headers.toListOfPairs();
        for (int i=0; i < headerList.length(); i++) {
            log += headerList[i].first + ": " + headerList[i].second + "\n";
        }
        pMain->logToConsole(log);
        if (method.indexOf(QRegularExpression("POST|PUT|PATCH")) == 0 && m_pRequestOptions->getBodyType() == FORM_DATA_BODY_TYPE) {
            m_pRestMan->sendCustomRequest(req, method.toUtf8(),  &m_mpData,  this, [this](QRestReply &reply) {
                m_mpData.setParent((QObject*)(&reply));
                HandleReply(&reply);
            });
        } else {
            m_pRestMan->sendCustomRequest(req, method.toUtf8(),  m_data,  this, [this](QRestReply &reply) {
                HandleReply(&reply);
            });
        }

    } else {
        QMessageBox msgBox;
        msgBox.setText("URL is missing");
        msgBox.exec();
    }
}

/**
 * @brief RestMdiChild::formatFinished
 * @param formattedText
 * @param contentType
 */
void RestMdiChild::formatFinished(QString *formattedText, QString *contentType)
{
    m_pResponse->setDataWithHeaders(*formattedText, *contentType);
    m_pRequestHeader->toggleEnableSendButton();
}

/**
 * @brief RequestHeader::RequestHeader
 * @param parent
 */
RequestHeader::RequestHeader(QWidget *parent)
    : QWidget(parent)
{
    QGridLayout *layout = new QGridLayout;
    m_pMethodCB = new QComboBox(this);
    QStringList methods = {"GET", "POST", "PUT", "PATCH"};
    m_pMethodCB->addItems(methods);
    m_pUrlEdit = new QLineEdit(this);
    m_pSendBtn = new QToolButton(this);
    m_pSendBtn->setText(tr("Send"));
    m_pSendBtn->setArrowType(Qt::RightArrow);
    m_pSendBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_pSendBtn->setMinimumHeight(26);
    RestMdiChild *pRestMain = dynamic_cast<RestMdiChild*>(parentWidget()->parentWidget());
    if (pRestMain) {
        connect(m_pSendBtn, &QToolButton::clicked, pRestMain, &RestMdiChild::sendRequest);
    }
    layout->addWidget(m_pMethodCB, 0,0);
    layout->addWidget(m_pUrlEdit, 0,1);
    layout->addWidget(m_pSendBtn, 0,2);
    setLayout(layout);
}

/**
 * @brief RequestHeader::getURL
 * @return
 */
QString RequestHeader::getURL()
{
    return m_pUrlEdit->text().trimmed();
}

/**
 * @brief RequestHeader::getMethod
 * @return
 */
QString RequestHeader::getMethod()
{
    return m_pMethodCB->currentText();
}

/**
 * @brief RequestHeader::toggleEnableSendButton
 */
void RequestHeader::toggleEnableSendButton()
{
    m_pSendBtn->setDisabled(m_pSendBtn->isEnabled());
}
