#ifndef QUESTIONSSETUP_H
#define QUESTIONSSETUP_H

#include "questionslistbtn.h"
#include <QWidget>
#include <QPushButton>
#include <QStackedWidget>

class QuestionsSetup : public QWidget
{
    Q_OBJECT

    QPushButton *pb_add;
    QPushButton *pb_home;
    QPushButton *pb_delete;
    QuestionsListBtn *listbtn;
    QLabel *selected_lbl;
public:
    explicit QuestionsSetup(QString table_name, int table_id, QWidget *parent = nullptr);
    void ConnectPbHome(QStackedWidget *stacked_widget, QWidget *wgt_home);
private slots:
    void slotAddQuestionAnswer();
};

#endif // QUESTIONSSETUP_H
