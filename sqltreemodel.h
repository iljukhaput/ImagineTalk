#ifndef SQLTREEMODEL_H
#define SQLTREEMODEL_H

#include <QAbstractProxyModel>
#include <QSqlQuery>

class SqlTreeModel : public QAbstractProxyModel
{
    Q_OBJECT

    QString user;
    int table_id;
public:
    SqlTreeModel(QString user, int table_id, QObject *parent = nullptr);
    virtual QModelIndex mapFromSource(const QModelIndex &sourceIndex) const override;
    virtual QModelIndex mapToSource(const QModelIndex &proxyIndex) const override;
    virtual QModelIndex parent(const QModelIndex &child) const override;

    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual int columnCount(const QModelIndex &parent) const override
        { return sourceModel()->columnCount(parent); }

    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override
        { return sourceModel()->headerData(section,orientation,role); }
    virtual bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role) override
        { return sourceModel()->setHeaderData(section,orientation,value,role); }

    virtual bool hasChildren(const QModelIndex &parent) const override;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    void deleteQA(QModelIndex &index);
    void addRow(const QModelIndex &parent, const QString &question, const QByteArray *image);
private:
    int getParentId(int childId) const;
    QSqlQuery* getChildren(int parentId) const;
    void deleteRecursively(QModelIndex &index);
    void deleteOneRow(QModelIndex &index);
};

#endif // SQLTREEMODEL_H
