#include "restmdichild.h"
#include "mainwindow.h"
#include "trvscintillaedit.h"

#include <QSplitter>
#include <QTextEdit>
#include <QGridLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QToolButton>
#include <QLabel>
#include <QMessageBox>
#include <QtNetwork/QRestAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QRestReply>
#include <ScintillaEdit.h>

RestMdiChild::RestMdiChild(QWidget *parent)
    : QMdiSubWindow(parent)
{
    m_pSplit = new QSplitter(Qt::Vertical, this);
    m_pSplit->setFrameStyle(QFrame::StyledPanel);
    m_pSplit->setHandleWidth(2);
    setWidget(m_pSplit);
    m_pRequestHeader = new RequestHeader(m_pSplit);
    m_pSplit->addWidget(m_pRequestHeader);
    m_pSubSplit = new QSplitter(Qt::Horizontal, m_pSplit);
    m_pSplit->addWidget(m_pSubSplit);
    m_pRequestForm = new RequestForm(m_pSubSplit);
    m_pSubSplit->addWidget(m_pRequestForm);
    m_pResults = new TrvScintillaEdit(m_pSubSplit);
    m_pResults->set_doc(new ScintillaDocument);
    m_pSubSplit->addWidget(m_pResults);
    m_pNetMan = new QNetworkAccessManager(this);
    m_pRestMan = new QRestAccessManager(m_pNetMan);
}

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
        m_pRestMan->sendCustomRequest(req, method.toUtf8(),  data,  this, [this](QRestReply &reply) {
            MainWindow *pMain = MainWindow::getInstance();
            if (reply.isSuccess()) {
                QString results = reply.readText();
                ScintillaDocument *doc = m_pResults->get_doc();
                QByteArray sResponse = results.toUtf8();
                doc->insert_string(0, sResponse);
            }
            pMain->toggleProgressBar();
        });

    } else {
        QMessageBox msgBox;
        msgBox.setText("URL is missing");
        msgBox.exec();
    }
}

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
    RestMdiChild *pRestMain = dynamic_cast<RestMdiChild*>(parentWidget()->parentWidget());
    if (pRestMain) {
        connect(m_pSendBtn, &QToolButton::clicked, pRestMain, &RestMdiChild::sendRequest);
    }
    layout->addWidget(m_pMethodCB, 0,0);
    layout->addWidget(m_pUrlEdit, 0,1);
    layout->addWidget(m_pSendBtn, 0,2);
    setLayout(layout);
}

QString RequestHeader::getURL()
{
    return m_pUrlEdit->text();
}

QString RequestHeader::getMethod()
{
    return m_pMethodCB->currentText();
}

ParamsTab::ParamsTab(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(new QLabel(tr("Params")));
    setLayout(layout);
}

AuthTab::AuthTab(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(new QLabel(tr("Auth")));
    setLayout(layout);
}

HeadersTab::HeadersTab(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(new QLabel(tr("Headers")));
    setLayout(layout);
}

BodyTab::BodyTab(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(new QLabel(tr("Body")));
    setLayout(layout);
}

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
    setLayout(layout);
}
