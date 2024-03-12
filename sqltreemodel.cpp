#include "sqltreemodel.h"
#include <QDebug>
#include <QSqlError>

enum {id_coumn = 0};

SqlTreeModel::SqlTreeModel(QString user, int table_id, QObject *parent)
    : QAbstractProxyModel(parent)
    , user(user)
    , table_id(table_id) {}


QModelIndex SqlTreeModel::mapFromSource(const QModelIndex &sourceIndex) const
{
    if(!sourceIndex.isValid())
        return QModelIndex();

    int id = (sourceIndex.column() == id_coumn) ? sourceIndex.data().toInt() : sourceIndex.sibling(sourceIndex.row(), id_coumn).data().toInt();

    int row = -1;
    QSqlQuery* q = getChildren(getParentId(id));
    while(q->next()) {
        row++;
        if(q->value(id_coumn).toInt() == id)
            break;
    }
    delete q;
    return createIndex(row, sourceIndex.column(), id);
}


QModelIndex SqlTreeModel::mapToSource(const QModelIndex &proxyIndex) const
{
    if(!proxyIndex.isValid())
        return QModelIndex();

    int id = proxyIndex.internalId();

    QSqlQuery q;
    q.prepare("SELECT id FROM user_" + QString::number(table_id));
    if(!q.exec()) {
        qDebug() << "Error in SqlTreeModel::mapToSource:" << q.lastError().text();
    }

    int row = -1;
    while(q.next()) {
        row++;
        if(q.value(id_coumn).toInt() == id)
            break;
    }
    return sourceModel()->index(row, proxyIndex.column());
}

bool SqlTreeModel::hasChildren(const QModelIndex &parent) const {

    QSqlQuery q;
    q.prepare("SELECT COUNT(*) FROM user_" + QString::number(table_id) + " WHERE parentId=?");
    q.addBindValue(parent.internalId());
    if(!q.exec()) {
        qDebug() << "Error in SqlTreeModel::hasChildren:" << q.lastError().text();
    }
    q.first();
    return q.value(id_coumn).toInt() > 0;
}

QModelIndex SqlTreeModel::parent(const QModelIndex &childIndex) const {

    int childId  = childIndex.internalId();
    int parentId = getParentId(childId);
    if(parentId == 0)
        return QModelIndex();

    int parentRow = -1;
    QSqlQuery* q = getChildren(getParentId(parentId));
    while(q->next()) {
        parentRow++;
        if(q->value(id_coumn).toInt() == parentId)
            break;
    }
    delete q;
    return createIndex(parentRow, childIndex.row(), parentId);
}

QModelIndex SqlTreeModel::index(int row, int column, const QModelIndex &parent) const {

    if(row < 0 || column < 0)
        return QModelIndex();

    QSqlQuery* q = getChildren(parent.internalId());
    q->seek(row);
    int id = q->value(id_coumn).toInt();
    delete q;
    return createIndex(row, column, id);
}

int SqlTreeModel::rowCount(const QModelIndex &parent) const {

    QSqlQuery* q = getChildren(parent.internalId());

    //use last() and at() since SQLite does not support query size calls
    q->last();
    int size = q->at() + 1;
    delete q;
    return size;
}

int SqlTreeModel::getParentId(int childId) const {
    QSqlQuery q;
    q.prepare("SELECT parentId FROM user_" + QString::number(table_id) + " WHERE id=?");
    q.addBindValue(childId);
    if(!q.exec()) {
        qDebug() << "Error in SqlTreeModel::getParentId:" << q.lastError().text();
    }
    q.first();
    return q.value(id_coumn).toInt();
}

QSqlQuery* SqlTreeModel::getChildren(int parentId) const {
    QSqlQuery* q = new QSqlQuery;
    q->prepare("SELECT id FROM user_" + QString::number(table_id) + " WHERE parentId=?");
    q->addBindValue(parentId);
    if(!q->exec()) {
        qDebug() << "Error in SqlTreeModel::getChildren:" << q->lastError().text();
    }
    return q;
}

void SqlTreeModel::deleteQA(QModelIndex &id)
{
    beginResetModel();
    deleteRecursively(id);
    endResetModel();
}

void SqlTreeModel::deleteRecursively(QModelIndex &id)
{
    if(hasChildren(id)){
        int rows = rowCount(id);
        qDebug() << "has children - " << rows << " | id = " << id;
        for(int i = 0; i < rows; ++i) {
            QModelIndex child = index(0, id_coumn, id);
            deleteRecursively(child);
        }
    } else {
        qDebug() << "has not children | id = " << id;
    }
    deleteOneRow(id);
}


void SqlTreeModel::deleteOneRow(QModelIndex &index)
{
    int id = index.internalId();
    QSqlQuery* q = new QSqlQuery;
    q->prepare("DELETE FROM user_" + QString::number(table_id) + " WHERE id=?");
    q->addBindValue(id);
    if(!q->exec()) {
        qDebug() << "Error in SqlTreeModel::deleteOneRow:" << q->lastError().text();
    }
    delete q;
}


void SqlTreeModel::addRow(const QModelIndex &parent, const QString &question, const QByteArray *image)
{
    beginResetModel();
    QModelIndex source_index = mapFromSource(parent);
    int id = source_index.internalId();
    queryInsertRow(table_id, id, question, image);
    endResetModel();
}


void SqlTreeModel::queryInsertRow(int table_id, int parent_id, const QString &question, const QByteArray *image)
{
    QSqlQuery query;
    if(image) {
        query.prepare("INSERT INTO user_" + QString::number(table_id) + " (question, image, parentId)"
                                                                        "VALUES (?, ?, ?)");
        query.addBindValue(question);
        query.addBindValue(*image);
        query.addBindValue(parent_id);
    } else {
        query.prepare("INSERT INTO user_" + QString::number(table_id) + " (question, parentId)"
                                                                        "VALUES (?, ?)");
        query.addBindValue(question);
        query.addBindValue(parent_id);
    }
    if(!query.exec())
        qDebug() << "Error in SqlTreeModel::addRow:" << query.lastError().text();
}


Qt::ItemFlags SqlTreeModel::flags(const QModelIndex &index) const {
    //QSqlTableModel doesn't allow children so use these flags
    if(index.isValid()) {
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
    }
    else
        return Qt::NoItemFlags;
}

