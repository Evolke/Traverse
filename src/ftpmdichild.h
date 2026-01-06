#ifndef FTPMDICHILD_H
#define FTPMDICHILD_H

#include <QMdiSubWindow>

QT_BEGIN_NAMESPACE
class QSplitter;
class QTreeView;
QT_END_NAMESPACE

class ConnectHeader : public QWidget
{
    Q_OBJECT
public:
    explicit ConnectHeader(QWidget *parent = nullptr);

public slots:

};

class FtpMdiChild : public QMdiSubWindow
{
    Q_OBJECT
public:
    explicit FtpMdiChild(QWidget *parent = nullptr);

signals:

private:
    QSplitter *m_pSplit;
    QSplitter *m_pSubSplit;
    ConnectHeader *m_pConnectHeader;
    QTreeView *m_pLocalTree;
    QTreeView *m_pRemoteTree;
};

#endif // FTPMDICHILD_H
