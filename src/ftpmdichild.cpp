#include "ftpmdichild.h"

#include <QSplitter>
#include <QTreeView>

FtpMdiChild::FtpMdiChild(QWidget *parent)
    : QMdiSubWindow(parent)
{
    m_pSplit = new QSplitter(Qt::Vertical, this);
    m_pConnectHeader = new ConnectHeader(m_pSplit);
    m_pSubSplit = new QSplitter(Qt::Horizontal, m_pSplit);
    m_pLocalTree = new QTreeView(m_pSubSplit);
    m_pRemoteTree = new QTreeView(m_pSubSplit);

    m_pSplit->setFrameStyle(QFrame::StyledPanel);
    m_pSplit->setHandleWidth(2);
    setWidget(m_pSplit);
    m_pSplit->addWidget(m_pSubSplit);
    m_pSubSplit->addWidget(m_pLocalTree);
    m_pSubSplit->addWidget(m_pRemoteTree);
    m_pSubSplit->setFrameStyle(QFrame::StyledPanel);
    m_pSubSplit->setHandleWidth(2);

}


ConnectHeader::ConnectHeader(QWidget *parent)
    : QWidget(parent)
{

}
