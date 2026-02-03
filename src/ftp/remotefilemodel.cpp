#include "remotefilemodel.h"

#include <QDebug>
#include <QPixmap>
#include <QFileInfo>
#include <QFileIconProvider>
#include <QDir>

RemoteFileModel::RemoteFileModel(QObject *parent) : QStandardItemModel(parent)
{
    m_headings = {"Name", "Size", "Type", "Date Modified"};
    //m_sPath = sPath;
    rootItem = std::make_unique<RemoteFileTreeItem>(RemoteFileTreeItem(tr("")));
    //rootItem.get()->populateChildren();
}

RemoteFileModel::~RemoteFileModel()
{
}

void RemoteFileModel::setPath(QString sPath)
{
    m_sPath = sPath;
    beginResetModel();
    removeRows(0,rowCount());
    rootItem.get()->reset();
    rootItem.get()->populateChildren();
    endResetModel();
}

int RemoteFileModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return m_headings.count();
}

int RemoteFileModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() > 0)
        return 0;

    RemoteFileTreeItem *parentItem = getItem(parent);

    return parentItem ? parentItem->childCount() : 0;
}

QVariant RemoteFileModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return {};

    RemoteFileTreeItem *item = getItem(index);

    return item->data(index.column(), role);
}

QVariant RemoteFileModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(orientation);

    if (role == Qt::DisplayRole) {
        return m_headings.at(section);
    }

    return QVariant{};
}

QModelIndex RemoteFileModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return {};

    RemoteFileTreeItem *childItem = getItem(index);
    RemoteFileTreeItem *parentItem = childItem ? childItem->parent() : nullptr;

    return (parentItem != rootItem.get() && parentItem != nullptr)
                ? createIndex(parentItem->row(), 0, parentItem) : QModelIndex{};
}

RemoteFileTreeItem *RemoteFileModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        if (auto *item = static_cast<RemoteFileTreeItem*>(index.internalPointer()))
            return item;
    }
    return rootItem.get();
}


QModelIndex RemoteFileModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(row);
    Q_UNUSED(column);

    if (parent.isValid() && parent.column() != 0)
        return {};

    RemoteFileTreeItem *parentItem = getItem(parent);
    if (!parentItem)
        return {};

    if (auto *childItem = parentItem->child(row))
        return createIndex(row, column, childItem);
    return {};
}

bool RemoteFileModel::canFetchMore(const QModelIndex &parent) const
{
    bool ret = false;

    RemoteFileTreeItem *parentItem = getItem(parent);
    if (parentItem) {
        ret = parentItem->canFetchMore();
    }
    return ret;
}

void RemoteFileModel::fetchMore(const QModelIndex &parent)
{
    RemoteFileTreeItem *parentItem = getItem(parent);
    if (parentItem->canFetchMore()) {
        parentItem->populateChildren();
    }
}

bool RemoteFileModel::hasChildren(const QModelIndex &parent) const
{
    bool ret = false;

    RemoteFileTreeItem *parentItem = getItem(parent);
    if (parentItem) {
        ret = parentItem->hasChildren();
    }
    return ret;

}


/********************************************
 * RemoteFileTreeItem
 ********************************************/
RemoteFileTreeItem::RemoteFileTreeItem(QString sPath, RemoteFileTreeItem *parent)
{
    m_sPath = sPath;
    m_pParent = parent;
    m_pChildren = Q_NULLPTR;
    m_bPopulated = false;
}

void RemoteFileTreeItem::reset()
{
    if (m_pChildren) {
        for (int i=0; i < m_pChildren->count(); i++) {
            m_pChildren->at(i)->reset();
            delete m_pChildren->at(i);
        }
        m_pChildren->clear();

    }

    m_bPopulated = false;
}

//! [5]
QVariant RemoteFileTreeItem::data(int column, int role) const
{
    QVariant ret = {};
    QFileInfo fi(m_sPath);
    QFileIconProvider iconFileProvider;

    switch (role) {
        case Qt::DisplayRole:
            if (column == 0) { ret = fi.fileName(); }
            break;

        case Qt::DecorationRole:
            if (column == 0) {
                ret = QVariant::fromValue(iconFileProvider.icon(fi));
            }
            break;
    }

    return ret;
}

int RemoteFileTreeItem::childCount()
{
    if (m_pChildren) {
        if (!m_bPopulated) { populateChildren(); }
        return m_pChildren->count();
    }
    return 0;
}

RemoteFileTreeItem *RemoteFileTreeItem::child(int number)
{
    return (number >= 0 && number < childCount())
    ? m_pChildren->at(number) : nullptr;
}

int RemoteFileTreeItem::row() const
{
    if (!m_pParent || !m_pParent->m_pChildren)
        return 0;

    return m_pParent->m_pChildren->indexOf(this);
}

bool RemoteFileTreeItem::canFetchMore()
{
    return hasChildren() && !m_bPopulated;
}

bool RemoteFileTreeItem::hasChildren()
{
    bool ret = false;
    QFileInfo fi(m_sPath);
    if (fi.isDir()) {
        QDir dir(m_sPath);
        ret = !dir.isEmpty();
    }

    return ret;
}

void RemoteFileTreeItem::populateChildren()
{
    QFileInfo fi(m_sPath);
    if (fi.isDir()) {
        m_pChildren = new QList<RemoteFileTreeItem*>();
        QDir dir(m_sPath);
        QFileInfoList list = dir.entryInfoList(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);
        if (list.count()) {
            m_pChildren = new QList<RemoteFileTreeItem*>();
            for (int i = 0; i < list.size(); i++) {
                QFileInfo fileInfo = list.at(i);
                m_pChildren->append(new RemoteFileTreeItem(fileInfo.absoluteFilePath(), this));
            }
        }
    }

    m_bPopulated = true;
}

