#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QPushButton>
#include <QMainWindow>
#include <QStackedWidget>
#include <QWidget>
#include <QSignalMapper>
#include "secondlayoutwindow.h"
#include "questionwindow.h"
#include "dialogwindow.h"
#include <QComboBox>

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QStackedWidget *stacked_widget_main;

    QComboBox *cbx_users;
    QPushButton *pb_start_dialog;
    QPushButton *pb_add_question;
    QPushButton *pb_user_selection;
    QPushButton *pb_quit;

    QWidget *wgt_choose_user;
    QWidget *wgt_main;
    DialogWindow *wgt_dialog;
    QuestionWindow *wgt_add_question;

    bool CreateUserTable(int id);
    void CreateDialogWindow(QString &user, int table_id);
    void CreateAddQuestionWindow(QString &user, int table_id);
    void CreateMainWindow(QString &user);
    void CreateChooseUserWindow();
    void connectPbToWindow(SecondLayoutWindow *wgt, QPushButton *pb);
    void FillComboBox(QComboBox &cbx);
    bool userTableIsExist(QString user);
    int createUserTableId();
    int getUserTableId(QString &user);
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void goToMainWindow();
private slots:
    void slotAddUser();
    void slotDeleteUser();
    void slotGoToMainWindow();
    void slotGoUserSelection();
};
#endif // MAINWINDOW_H
