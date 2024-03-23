#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QPushButton>
#include <QMainWindow>
#include <QStackedWidget>
#include <QWidget>
#include <QSignalMapper>
#include <QComboBox>

#include "secondlayoutwindow.h"
// #include "questionwindow.h"
#include "dialogwindow.h"
#include "questionssetup.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QStackedWidget *stacked_widget_main;

    QComboBox *cbx_users;
    QPushButton *pb_start_dialog;
    QPushButton *pb_add_question;
    QPushButton *pb_user_selection;
    QPushButton *pb_general_questions;
    QPushButton *pb_quit;

    QWidget *wgt_choose_user;
    QWidget *wgt_main;
    DialogWindow *wgt_dialog;
    // QuestionWindow *wgt_add_question;
    QuestionsSetup *wgt_add_question;
    QuestionsSetup *wgt_questions_setup;

    bool CreateQuestionsTable(QString table_name, int table_id);
    bool CreateUsersTable();
    void FillUserTable(QString full_table_name);
    // bool CreateGeneralTable(QString table_name, int table_id);
    void CreateDialogWindow(QString &user, int table_id);
    void CreateAddQuestionWindow(int table_id);
    void CreateMainWindow(QString &user);
    void CreateChooseUserWindow();
    void connectPbToWindow(SecondLayoutWindow *wgt, QPushButton *pb);
    void FillComboBox(QComboBox &cbx);
    // bool userTableIsExist(QString user);
    int createUserTableId();
    int getUserTableId(QString &user);
    bool userIsExist(QString &user);
public:
    MainWindow(QWidget *parent = nullptr);
    void goToMainWindow();
private slots:
    void slotAddUser();
    void slotDeleteUser();
    void slotGoToMainWindow();
    void slotGoUserSelection();
    void slotGeneralQuestions();
};
#endif // MAINWINDOW_H
