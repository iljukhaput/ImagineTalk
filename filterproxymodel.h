#ifndef FILTERPROXYMODEL_H
#define FILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

class FilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    QString user;
public:
    FilterProxyModel(QString user, QObject *parent = nullptr);
    virtual QModelIndex mapFromSource(const QModelIndex &sourceIndex) const override;
    virtual QModelIndex mapToSource(const QModelIndex &proxyIndex) const override;

    virtual QModelIndex parent(const QModelIndex &index) const override
        { return index.parent(); }
    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual int columnCount(const QModelIndex &parent) const override
        { return sourceModel()->columnCount(parent); }
protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    bool filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const override;
};

#endif // FILTERPROXYMODEL_H
