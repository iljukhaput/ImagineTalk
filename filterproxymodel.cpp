#include "filterproxymodel.h"

#include <QSqlQuery>

enum {id_coumn = 0};


FilterProxyModel::FilterProxyModel(QString user, QObject *parent)
    : QSortFilterProxyModel (parent), user(user)
{}


bool FilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    // if( user == sourceModel()->index(sourceRow, 4, sourceParent).data().toString()) {
    //     return true;
    // } else {
    //     return false;
    // }
    return true;
}


bool FilterProxyModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const
{
    if(source_column == 2) {
        return false;
    } else {
        return true;
    }
}


QModelIndex FilterProxyModel::mapFromSource(const QModelIndex &sourceIndex) const
{
    if(!sourceIndex.isValid())
        return QModelIndex();

    int id = (sourceIndex.column() == id_coumn) ? sourceIndex.data().toInt() : sourceIndex.sibling(sourceIndex.row(), id_coumn).data().toInt();

    QSqlQuery q;
    q.prepare("SELECT id FROM Questions WHERE user=?");
    q.addBindValue(user);
    q.exec();
    int row = -1;
    while(q.next()) {
        row++;
        if(q.value(id_coumn).toInt() == id)
            break;
    }
    QModelIndex proxy_index = createIndex(row, sourceIndex.column(), id);
    return proxy_index;
}

QModelIndex FilterProxyModel::mapToSource(const QModelIndex &proxyIndex) const
{
    if(!proxyIndex.isValid())
        return QModelIndex();

    int id = proxyIndex.internalId();
    // int id = (proxyIndex.column() == id_coumn) ? proxyIndex.data().toInt() : proxyIndex.sibling(proxyIndex.row(), id_coumn).data().toInt();

    QSqlQuery q;
    q.prepare("SELECT id FROM Questions");
    q.exec();
    int row = -1;
    while(q.next()) {
        row++;
        if(q.value(id_coumn).toInt() == id)
            break;
    }
    QModelIndex source_index = sourceModel()->index(row, proxyIndex.column());
    return source_index;
}


QModelIndex FilterProxyModel::index(int row, int column, const QModelIndex &parent) const
{
    if(row < 0 || column < 0)
        return QModelIndex();

    QSqlQuery q;
    q.prepare("SELECT id FROM Questions WHERE user=?");
    q.addBindValue(user);
    q.exec();
    q.seek(row);
    int id = q.value(id_coumn).toInt();
    return createIndex(row, column, id);
}

int FilterProxyModel::rowCount(const QModelIndex &parent) const
{
    QSqlQuery q;
    q.prepare("SELECT id FROM Questions WHERE user=?");
    q.addBindValue(user);
    q.exec();
    return q.size();
}
