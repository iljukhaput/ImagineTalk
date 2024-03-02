#ifndef SQLTABLEMODEL_H
#define SQLTABLEMODEL_H

#include <QObject>
#include <QSqlTableModel>

class SqlTableModel : public QSqlTableModel
{
    Q_OBJECT
public:
    SqlTableModel(QObject *parent = 0);
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
};

#endif // SQLTABLEMODEL_H
