#ifndef QUESTIONWINDOW_H
#define QUESTIONWINDOW_H

#include "secondlayoutwindow.h"
#include "sqltreemodel.h"
#include "questionslistbtn.h"

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
    // QPushButton *pb_expand;

    // QSqlTableModel *table_model;
    // SqlTreeModel *tree_model;

    // QTreeView *tree_view;
    QuestionsListBtn *listbtn;

    QLabel *selected_lbl;
    // void initView();
public:
    explicit QuestionWindow(QString &user, int table_id, QWidget *parent = nullptr);
    ~QuestionWindow();
private slots:
    void slotAddQuestionAnswer();
};
#endif // QUESTIONWINDOW_H
