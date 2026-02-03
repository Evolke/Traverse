#ifndef FTPMDICHILD_H
#define FTPMDICHILD_H

#include <QFrame>

#define NAME_COLUMN_WIDTH 200

QT_BEGIN_NAMESPACE
class QSplitter;
class QTreeView;
class QLineEdit;
class QSpinBox;
class QToolButton;
class LocalFileView;
class RemoteFileView;
QT_END_NAMESPACE

class ConnectHeader : public QWidget
{
    Q_OBJECT
public:
    explicit ConnectHeader(QWidget *parent = nullptr);

public slots:

private:
    QLineEdit *m_pHostEdit;
    QLineEdit *m_pUsernameEdit;
    QLineEdit *m_pPasswordEdit;
    QSpinBox *m_pPortSB;
    QToolButton *m_pConnectBtn;
};

class FtpMdiChild : public QFrame
{
    Q_OBJECT
public:
    explicit FtpMdiChild(QWidget *parent = nullptr);

signals:

private:
    // void populateLocalTree(QString &sPath);
    // void populateLocalTreeItem(QTreeWidgetItem &treeItem, QString &path);

    QSplitter *m_pSplit;
    QSplitter *m_pSubSplit;
    ConnectHeader *m_pConnectHeader;
    LocalFileView *m_pLocalView;
    RemoteFileView *m_pRemoteView;
    qint64 m_startTime;
};

#endif // FTPMDICHILD_H
