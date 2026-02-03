#ifndef LOCALFILEVIEW_H
#define LOCALFILEVIEW_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QTreeView;
class QToolButton;
class QLineEdit;
QT_END_NAMESPACE

class LocalDirInfo : public QWidget
{
    Q_OBJECT
public:
    explicit LocalDirInfo(QString sPath, QWidget *parent = nullptr);

    void setPath(QString sPath);
    QString getPath();

private:
    QLineEdit *m_pPathEdit;
    QToolButton *m_pBrowseBtn;
};

class LocalFileView : public QWidget
{
    Q_OBJECT
public:
    explicit LocalFileView(QWidget *parent = nullptr);

public slots:
    void browse();

signals:

private:
    LocalDirInfo *m_pLocalInfo;
    QTreeView *m_pLocalTree;
};

#endif // LOCALFILEVIEW_H
