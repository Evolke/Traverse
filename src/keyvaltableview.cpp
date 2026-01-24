#include "keyvaltableview.h"

#include <QHeaderView>

KeyValTableView::KeyValTableView(QWidget *parent)
    : QTableView{parent}
{
    setModel(new KeyValTableModel);
    horizontalHeader()->setStretchLastSection(true);
}

QString KeyValTableView::getCellData(int row, int col)
{
    QString ret;

    KeyValTableModel *pModel = qobject_cast<KeyValTableModel*>(model());
    if (pModel) {
        ret = pModel->data(pModel->index(row,col)).toString();
    }
    return ret;
}

int KeyValTableView::rowCount()
{
    int ret = 0;

    KeyValTableModel *pModel = qobject_cast<KeyValTableModel*>(model());
    if (pModel) {
        ret = pModel->rowCount();
    }
    return ret;
}

void KeyValTableView::addRow()
{
    KeyValTableModel *pModel = qobject_cast<KeyValTableModel*>(model());
    if (pModel) {
        pModel->addRow();
    }

}

KeyValTableModel::KeyValTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    m_headings = {"Key", "Value"};
    KeyValItem row = KeyValItem("","");
    m_kvList = {row};
}


int KeyValTableModel::rowCount(const QModelIndex &index) const
{
    return m_kvList.count();
}

void KeyValTableModel::addRow()
{
    m_kvList.append(KeyValItem("",""));
    insertRow(m_kvList.count()-1);
    emit layoutChanged();
}

int KeyValTableModel::columnCount(const QModelIndex &index) const
{
    return 2;
}

QVariant KeyValTableModel::data(const QModelIndex &item, int role) const
{
    if (!item.isValid())
        return QVariant();

    if (role & ~(Qt::DisplayRole | Qt::EditRole))
        return QVariant();

    if (item.row() >= m_kvList.count())
        return QVariant{};

    if (item.column() == 0) {
        QString sKey = m_kvList[item.row()].key;
        return QVariant(sKey);
    }

    if (item.column() == 1) {
        QString sValue = m_kvList[item.row()].value;
        return QVariant(sValue);
    }

    return QVariant{};
}

bool KeyValTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{

    if (role != Qt::EditRole)
        return QAbstractTableModel::setData(index, value, role);

    if (!index.isValid() || index.column() >= columnCount(index) || index.row() >= rowCount(index))
        return false;

    if (index.column() == 0) {
        m_kvList[index.row()].key = value.toString();
    } else if (index.column() == 1) {
        m_kvList[index.row()].value = value.toString();
    }
    //emit dataChanged(index, index);
    qDebug() << m_kvList[index.row()].key << m_kvList[index.row()].value;

    return true;
}

Qt::ItemFlags KeyValTableModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

/*!
    Returns the header data for the given \a role in the \a section
    of the header with the specified \a orientation.
*/
QVariant KeyValTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        return m_headings.at(section);
    }

    return QVariant{};
}
