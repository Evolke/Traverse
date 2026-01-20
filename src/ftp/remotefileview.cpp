#include "remotefileview.h"
#include "remotefilemodel.h"
#include "ftpmdichild.h"

#include <QTreeView>
#include <QDir>
#include <QVBoxLayout>
#include <QToolButton>
#include <QLineEdit>
#include <QFileDialog>
#include <QFileSystemModel>

RemoteFileView::RemoteFileView(QWidget *parent)
    : QWidget{parent}
{
    m_pRemoteInfo = new RemoteDirInfo("", this);
    m_pRemoteInfo->setMaximumHeight(40);
    m_pRemoteTree = new QTreeView(this);
    RemoteFileModel *pFileModel = new RemoteFileModel();
    m_pRemoteTree->setModel(pFileModel);
    m_pRemoteTree->setColumnWidth(0,NAME_COLUMN_WIDTH);
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(m_pRemoteInfo);
    layout->addWidget(m_pRemoteTree);
    setLayout(layout);
}

void RemoteFileView::browse()
{
    QFileDialog fd;
    fd.setOption(QFileDialog::ShowDirsOnly);
    QString sDir = fd.getExistingDirectory(this, tr("Select Directory"), m_pRemoteInfo->getPath());
    if (sDir.length()) {
        m_pRemoteInfo->setPath(sDir);
        QFileSystemModel *pModel = static_cast<QFileSystemModel*>(m_pRemoteTree->model());
        pModel->setRootPath(sDir);
        m_pRemoteTree->setRootIndex(pModel->index(sDir));
    }

}

RemoteDirInfo::RemoteDirInfo(QString sPath, QWidget *parent)
    : QWidget(parent)
{
    m_pPathEdit = new QLineEdit(this);
    m_pPathEdit->setText(sPath);
    m_pPathEdit->setReadOnly(true);
    m_pBrowseBtn = new QToolButton(this);
    m_pBrowseBtn->setText(tr("..."));
    connect(m_pBrowseBtn, &QToolButton::clicked, static_cast<RemoteFileView*>(parent), &RemoteFileView::browse);
    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(m_pPathEdit, 0, 0);
    layout->addWidget(m_pBrowseBtn, 0, 1);
    setLayout(layout);
}

void RemoteDirInfo::setPath(QString sPath)
{
    m_pPathEdit->setText(sPath);
}

QString RemoteDirInfo::getPath()
{
    return m_pPathEdit->text();
}

