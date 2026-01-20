#include "restmdichild.h"
#include "../mainwindow.h"
#include "responseview.h"

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
    m_pRequestForm = new RequestForm(m_pSubSplit);
    m_pSubSplit->addWidget(m_pRequestForm);
    m_pResponse = new ResponseView(m_pSubSplit);
    m_pSubSplit->addWidget(m_pResponse);
    m_pNetMan = new QNetworkAccessManager(this);
    m_pRestMan = new QRestAccessManager(m_pNetMan);
    layout->setContentsMargins(0,0,0,0);
    setLayout(layout);
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
        QByteArray data;
        m_startTime = QDateTime::currentMSecsSinceEpoch();

        m_pRestMan->sendCustomRequest(req, method.toUtf8(),  data,  this, [this](QRestReply &reply) {
            MainWindow *pMain = MainWindow::getInstance();
            qint64 execTime = QDateTime::currentMSecsSinceEpoch() - m_startTime;
            QByteArray data = reply.readBody();
            QHttpHeaders headers = reply.networkReply()->headers();
            if (data.length() > 0 || reply.hasError()) {
                if (data.length() == 0) { data = reply.errorString().toUtf8(); }
                m_pResponse->setDataWithHeaders(data, headers);
                m_pResponse->setStatus(reply.httpStatus());
                m_pResponse->setExecTime(execTime);
                m_pResponse->setSize(data.length());
            }

            pMain->toggleProgressBar();
        });

    } else {
        QMessageBox msgBox;
        msgBox.setText("URL is missing");
        msgBox.exec();
    }
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
    QString regEx = "^https?:\\/\\/(?:www\\.)?[-a-zA-Z0-9@:%._\\+~#=]{1,256}\\.[a-zA-Z0-9()]{1,6}\\b(?:[-a-zA-Z0-9()@:%_\\+.\\,~#?&\\/=]*)$";
    QRegularExpressionValidator *urlValidator = new QRegularExpressionValidator(QRegularExpression(regEx),this);
    m_pUrlEdit->setValidator(urlValidator);
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
    return m_pUrlEdit->text();
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
 * @brief ParamsTab::ParamsTab
 * @param parent
 */
ParamsTab::ParamsTab(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(new QLabel(tr("Params")));
    setLayout(layout);
}

/**
 * @brief AuthTab::AuthTab
 * @param parent
 */
AuthTab::AuthTab(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(new QLabel(tr("Auth")));
    setLayout(layout);
}

/**
 * @brief HeadersTab::HeadersTab
 * @param parent
 */
HeadersTab::HeadersTab(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(new QLabel(tr("Headers")));
    setLayout(layout);
}

/**
 * @brief BodyTab::BodyTab
 * @param parent
 */
BodyTab::BodyTab(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(new QLabel(tr("Body")));
    setLayout(layout);
}

/**
 * @brief RequestForm::RequestForm
 * @param parent
 */
RequestForm::RequestForm(QWidget *parent)
    : QWidget(parent)
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
