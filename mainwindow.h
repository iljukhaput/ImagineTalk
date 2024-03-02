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
    QPushButton *pb_start_dialog;
    QPushButton *pb_add_question;
    QPushButton *pb_user_selection;
    QPushButton *pb_quit;
    QWidget *wgt_main;
    QWidget *wgt_choose_user;
    DialogWindow *wgt_dialog;
    QuestionWindow *wgt_add_question;
    QComboBox *cbx_users;

    void CreateDialogWindow(QString &user);
    void CreateAddQuestionWindow(QString &user);
    void CreateMainWindow();
    void CreateChooseUserWindow();
    void connectPbToWindow(SecondLayoutWindow *wgt, QPushButton *pb);
    void FillComboBox(QComboBox &cbx);
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void goToMainWindow();
private slots:
    void slotAddUser();
    void slotGoToMainWindow();
    void slotGoUserSelection();
};
#endif // MAINWINDOW_H
