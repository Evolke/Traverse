#include "localfileview.h"
#include "ftpmdichild.h"

#include <QTreeView>
#include <QDir>
#include <QVBoxLayout>
#include <QToolButton>
#include <QLineEdit>
#include <QFileDialog>
#include <QFileSystemModel>

LocalFileView::LocalFileView(QWidget *parent)
    : QWidget{parent}
{
    QString sPath = QDir::homePath();
    m_pLocalInfo = new LocalDirInfo(sPath, this);
    m_pLocalInfo->setMaximumHeight(40);
    m_pLocalTree = new QTreeView(this);
    QFileSystemModel *pFileModel = new QFileSystemModel;
    pFileModel->setRootPath(sPath);
    m_pLocalTree->setModel(pFileModel);
    m_pLocalTree->setRootIndex(pFileModel->index(sPath));
    m_pLocalTree->setColumnWidth(0, NAME_COLUMN_WIDTH);
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(m_pLocalInfo);
    layout->addWidget(m_pLocalTree);
    setLayout(layout);
}

void LocalFileView::browse()
{
    QFileDialog fd;
    fd.setOption(QFileDialog::ShowDirsOnly);
    QString sDir = fd.getExistingDirectory(this, tr("Select Directory"), m_pLocalInfo->getPath());
    if (sDir.length()) {
        m_pLocalInfo->setPath(sDir);
        QFileSystemModel *pModel = static_cast<QFileSystemModel*>(m_pLocalTree->model());
        pModel->setRootPath(sDir);
        m_pLocalTree->setRootIndex(pModel->index(sDir));
    }

}

LocalDirInfo::LocalDirInfo(QString sPath, QWidget *parent)
    : QWidget(parent)
{
    m_pPathEdit = new QLineEdit(this);
    m_pPathEdit->setText(sPath);
    m_pPathEdit->setReadOnly(true);
    m_pBrowseBtn = new QToolButton(this);
    m_pBrowseBtn->setText(tr("..."));
    connect(m_pBrowseBtn, &QToolButton::clicked, static_cast<LocalFileView*>(parent), &LocalFileView::browse);
    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(m_pPathEdit, 0, 0);
    layout->addWidget(m_pBrowseBtn, 0, 1);
    setLayout(layout);
}

void LocalDirInfo::setPath(QString sPath)
{
    m_pPathEdit->setText(sPath);
}

QString LocalDirInfo::getPath()
{
    return m_pPathEdit->text();
}

