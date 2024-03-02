#ifndef QUESTIONWINDOW_H
#define QUESTIONWINDOW_H

#include "secondlayoutwindow.h"
#include "sqltreemodel.h"
#include "filterproxymodel.h"

#include <QWidget>
#include <QPushButton>
#include <QSignalMapper>
#include <QStackedWidget>
#include <QStandardItemModel>
#include <QTreeView>
#include <QTableView>
#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QTableView>
#include <QSplitter>
#include <QLabel>

class QuestionWindow : public SecondLayoutWindow
{
    Q_OBJECT

    QPushButton *pb_add;
    QPushButton *pb_delete;
    QPushButton *pb_expand;

    QSqlTableModel *table_model;
    SqlTreeModel *tree_model;
    FilterProxyModel *filter_model;

    QTableView *table_view;
    QTableView *table_proxy_view;
    QTreeView *tree_view;

    QLabel *selected_QA_image;
    void initView();
public:
    explicit QuestionWindow(QString &user, QWidget *parent = nullptr);
private slots:
    void slotAddQuestionAnswer();
    void slotDeleteQuestionAnswer();
    void slotTreeViewClicked(QModelIndex id);
};
#endif // QUESTIONWINDOW_H
