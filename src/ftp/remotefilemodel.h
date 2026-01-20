#ifndef REMOTEFILEMODEL_H
#define REMOTEFILEMODEL_H


#include <QStandardItemModel>

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE


class RemoteFileTreeItem
{
public:
    explicit RemoteFileTreeItem(QString sPath, RemoteFileTreeItem *parent=Q_NULLPTR);

    QVariant data(int column, int role) const;
    RemoteFileTreeItem *child(int number);
    int childCount();
    void populateChildren();
    RemoteFileTreeItem *parent() { return m_pParent; }
    int row() const;
    bool canFetchMore();
    bool hasChildren();
    void reset();

private:
    RemoteFileTreeItem *m_pParent;
    QString m_sPath;
    QList<RemoteFileTreeItem*> *m_pChildren;
    bool m_bPopulated;
};

class RemoteFileModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit RemoteFileModel(QObject *parent = Q_NULLPTR);
    ~RemoteFileModel();

    void setPath(QString sPath);

    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    RemoteFileTreeItem *getItem(const QModelIndex &index) const;

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = {}) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = {}) const override;
    int columnCount(const QModelIndex &parent = {}) const override;
    bool canFetchMore(const QModelIndex &parent) const override;
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;
    void fetchMore(const QModelIndex &parent) override;

signals:

public slots:

private:
    QStringList m_headings;
    QString m_sPath;
    std::unique_ptr<RemoteFileTreeItem> rootItem;

};

#endif // REMOTEFILEMODEL_H
