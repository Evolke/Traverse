#ifndef KEYVALTABLEVIEW_H
#define KEYVALTABLEVIEW_H

#include <QAbstractTableModel>
#include <QTableView>

struct KeyValItem
{
    KeyValItem(QString ikey, QString ival) { key = ikey; value = ival; }
    QString key;
    QString value;
};

class KeyValTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit KeyValTableModel(QObject *parent = nullptr);

    QVariant data(const QModelIndex &idx, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    void addRow();

private:
    QList<KeyValItem> m_kvList;
    QStringList m_headings;
};

class KeyValTableView : public QTableView
{
    Q_OBJECT
public:
    explicit KeyValTableView(QWidget *parent = nullptr);

    QString getCellData(int row, int col);
    int rowCount();
    void addRow();

signals:
};

#endif // KEYVALTABLEVIEW_H
