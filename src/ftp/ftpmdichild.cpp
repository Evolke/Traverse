#include "ftpmdichild.h"
#include "../flowlayout.h"
#include "localfileview.h"
#include "remotefileview.h"

#include <QSplitter>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QToolButton>
#include <QSpacerItem>
#include <QDir>
#include <QFileIconProvider>

#define SPACER_WIDTH 10
#define SPACER_HEIGHT 10
#define LABEL_HEIGHT 26

/**
 * @brief FtpMdiChild::FtpMdiChild
 * @param parent
 */
FtpMdiChild::FtpMdiChild(QWidget *parent)
    : QFrame(parent)
{
    m_pSplit = new QSplitter(Qt::Vertical, this);
    m_pConnectHeader = new ConnectHeader(m_pSplit);
    m_pSubSplit = new QSplitter(Qt::Horizontal, m_pSplit);
    QString sLabel;
    m_pLocalView = new LocalFileView(m_pSubSplit);
    m_pRemoteView = new RemoteFileView(m_pSubSplit);
    m_pSplit->setFrameStyle(QFrame::StyledPanel);
    m_pSplit->setHandleWidth(2);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_pSplit);
    m_pSplit->addWidget(m_pSubSplit);
    m_pSubSplit->addWidget(m_pLocalView);
    m_pSubSplit->addWidget(m_pRemoteView);
    m_pSubSplit->setFrameStyle(QFrame::StyledPanel);
    m_pSubSplit->setHandleWidth(2);
    layout->setContentsMargins(0,0,0,0);
    setLayout(layout);
    //populateLocalTree(sPath);
}

/**
 * @brief ConnectHeader::ConnectHeader
 * @param parent
 */
ConnectHeader::ConnectHeader(QWidget *parent)
    : QWidget(parent)
{

    FlowLayout *layout = new FlowLayout(this, 10, 10, 10);
    layout->setAlignment(Qt::AlignVCenter);

    //Host
    QLabel *hostLabel = new QLabel(tr("Host:"));
    hostLabel->setMinimumHeight(LABEL_HEIGHT);
    layout->addWidget(hostLabel);
    layout->setAlignment(hostLabel, Qt::AlignVCenter);
    m_pHostEdit = new QLineEdit(this);
    m_pHostEdit->setMinimumWidth(200);
    layout->addWidget(m_pHostEdit);

    //Username
    layout->addItem(new QSpacerItem(SPACER_WIDTH,SPACER_HEIGHT));
    QLabel *unLabel = new QLabel(tr("Username:"));
    unLabel->setMinimumHeight(LABEL_HEIGHT);

    layout->addWidget(unLabel);
    m_pUsernameEdit = new QLineEdit(this);
    layout->addWidget(m_pUsernameEdit);

    //Password
    QLabel *pwdLabel = new QLabel(tr("Password:"));
    pwdLabel->setMinimumHeight(LABEL_HEIGHT);
    layout->addItem(new QSpacerItem(SPACER_WIDTH,SPACER_HEIGHT));
    layout->addWidget(pwdLabel);
    m_pPasswordEdit = new QLineEdit(this);
    layout->addWidget(m_pPasswordEdit);

    //Port
    QLabel *prtLabel = new QLabel(tr("Port:"));
    prtLabel->setMinimumHeight(LABEL_HEIGHT);
    layout->addItem(new QSpacerItem(SPACER_WIDTH,SPACER_HEIGHT));
    layout->addWidget(prtLabel);
    m_pPortSB = new QSpinBox(this);
    m_pPortSB->setValue(22);
    layout->addWidget(m_pPortSB);

    //Connect
    layout->addItem(new QSpacerItem(SPACER_WIDTH,SPACER_HEIGHT));
    m_pConnectBtn = new QToolButton(this);
    m_pConnectBtn->setMinimumHeight(LABEL_HEIGHT);
    m_pConnectBtn->setText(tr("Connect"));
    m_pConnectBtn->setArrowType(Qt::RightArrow);
    m_pConnectBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    layout->addWidget(m_pConnectBtn);

    setLayout(layout);
    setMaximumHeight(100);
}
