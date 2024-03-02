#include "sqltablemodel.h"

SqlTableModel::SqlTableModel(QObject *parent) : QSqlTableModel(parent) {}

Qt::ItemFlags SqlTableModel::flags(const QModelIndex &index) const {
    //QSqlTableModel doesn't allow children so use these flags
    if(index.isValid()) {
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
    }
    else
        return Qt::NoItemFlags;
}
