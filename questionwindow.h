#ifndef QUESTIONWINDOW_H
#define QUESTIONWINDOW_H

#include "secondlayoutwindow.h"
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
    QuestionsListBtn *listbtn;
    QLabel *selected_lbl;
public:
    explicit QuestionWindow(QString username, int table_id, QWidget *parent = nullptr);
    ~QuestionWindow();
private slots:
    void slotAddQuestionAnswer();
};
#endif // QUESTIONWINDOW_H
