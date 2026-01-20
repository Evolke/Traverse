#ifndef REMOTEFILEVIEW_H
#define REMOTEFILEVIEW_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QTreeView;
class QToolButton;
class QLineEdit;
QT_END_NAMESPACE

class RemoteDirInfo : public QWidget
{
    Q_OBJECT
public:
    explicit RemoteDirInfo(QString sPath, QWidget *parent = nullptr);

    void setPath(QString sPath);
    QString getPath();

private:
    QLineEdit *m_pPathEdit;
    QToolButton *m_pBrowseBtn;
};

class RemoteFileView : public QWidget
{
    Q_OBJECT
public:
    explicit RemoteFileView(QWidget *parent = nullptr);

public slots:
    void browse();

signals:

private:
    RemoteDirInfo *m_pRemoteInfo;
    QTreeView *m_pRemoteTree;
};

#endif // REMOTEFILEVIEW_H
