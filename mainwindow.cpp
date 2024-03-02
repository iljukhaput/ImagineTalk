#include "mainwindow.h"
#include "secondlayoutwindow.h"
#include "dialogwindow.h"

#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QAbstractButton>
#include <QVBoxLayout>
#include <QDebug>
#include <QComboBox>
#include <QInputDialog>
#include <QDir>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    if(!SecondLayoutWindow::initDB()) {
        qDebug() << "Data base is not open";
    }
    stacked_widget_main = new QStackedWidget(this);

    CreateChooseUserWindow();
    CreateMainWindow();

    setCentralWidget(stacked_widget_main);
    setGeometry(200, 200, 1150, 550);
}

MainWindow::~MainWindow() {}


void MainWindow::CreateMainWindow()
{
    wgt_main = new QWidget;
    stacked_widget_main->addWidget(wgt_main);

    QSize size_pb(200, 100);

    pb_start_dialog = new QPushButton("Start dialog");
    pb_start_dialog->setFixedSize(size_pb);

    pb_add_question = new QPushButton("Add question/answer");
    pb_add_question->setFixedSize(size_pb);

    pb_user_selection = new QPushButton("Return to user selection");
    pb_user_selection->setFixedSize(size_pb);
    connect(pb_user_selection, SIGNAL(clicked()), this, SLOT(slotGoUserSelection()));

    pb_quit = new QPushButton("QUIT");
    pb_quit->setFixedSize(size_pb);
    connect(pb_quit, SIGNAL (clicked()), QApplication::instance(), SLOT (quit()));

    QVBoxLayout *vbx_layout  = new QVBoxLayout();
    vbx_layout->addWidget(pb_start_dialog);
    vbx_layout->addWidget(pb_add_question);
    vbx_layout->addWidget(pb_user_selection);
    vbx_layout->addWidget(pb_quit);
    vbx_layout->setSpacing(5);
    vbx_layout->setAlignment(Qt::AlignCenter);
    wgt_main->setLayout(vbx_layout);
}


void MainWindow::CreateChooseUserWindow()
{
    wgt_choose_user = new QWidget;
    stacked_widget_main->addWidget(wgt_choose_user);

    QSize size_pb(200, 50);

    cbx_users = new QComboBox;
    cbx_users->setFixedSize(size_pb);
    FillComboBox(*cbx_users);

    QPushButton *pb_ok = new QPushButton("Ok");
    pb_ok->setFixedSize(size_pb);
    connect(pb_ok, SIGNAL(clicked()), this, SLOT(slotGoToMainWindow()));

    QPushButton *pb_add_user = new QPushButton("Add new user");
    pb_add_user->setFixedSize(size_pb);
    connect(pb_add_user, SIGNAL (clicked()), this, SLOT(slotAddUser()));

    QPushButton *pb_quit = new QPushButton("QUIT");
    pb_quit->setFixedSize(size_pb);
    connect(pb_quit, SIGNAL (clicked()), QApplication::instance(), SLOT(quit()));


    QVBoxLayout *vbx_layout  = new QVBoxLayout();
    vbx_layout->addWidget(cbx_users);
    vbx_layout->addWidget(pb_ok);
    vbx_layout->addWidget(pb_add_user);
    vbx_layout->addWidget(pb_quit);
    vbx_layout->setSpacing(5);
    vbx_layout->setAlignment(Qt::AlignCenter);
    wgt_choose_user->setLayout(vbx_layout);
}


void MainWindow::FillComboBox(QComboBox &cbx)
{
    QSqlQuery q(SecondLayoutWindow::db);
    q.prepare("SELECT user FROM Questions GROUP BY user");
    if(q.exec()) {
        while(q.next()) {
            cbx.addItem(q.value("user").toString());
        }
    }
}


void MainWindow::CreateDialogWindow(QString &user)
{
    wgt_dialog = new DialogWindow(user, this);
    stacked_widget_main->addWidget(wgt_dialog);
    connectPbToWindow(wgt_dialog, pb_start_dialog);
    connect(pb_start_dialog, SIGNAL(clicked()), wgt_dialog, SLOT(slotShowInitialQuestion()));
}


void MainWindow::CreateAddQuestionWindow(QString &user)
{
    wgt_add_question = new QuestionWindow(user, this);
    stacked_widget_main->addWidget(wgt_add_question);
    connectPbToWindow(wgt_add_question, pb_add_question);
}


void MainWindow::connectPbToWindow(SecondLayoutWindow *wgt, QPushButton *pb)
{
    QSignalMapper *signal_mapper = new QSignalMapper(this);
    signal_mapper->setMapping(pb, wgt);
    connect(pb, SIGNAL (clicked()), signal_mapper, SLOT (map()));
    connect(signal_mapper, SIGNAL (mapped(QWidget *)), stacked_widget_main, SLOT (setCurrentWidget(QWidget *)));
    wgt->ConnectPbHome(*signal_mapper, *stacked_widget_main, *wgt_main);
}


void MainWindow::goToMainWindow()
{
    stacked_widget_main->setCurrentIndex(1);
}


void MainWindow::slotGoToMainWindow()
{
    QString user = cbx_users->currentText();
    if(user.isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Please select a user or create a new one"));
        msgBox.exec();
        return;
    }
    CreateDialogWindow(user);
    CreateAddQuestionWindow(user);
    goToMainWindow();
}


void MainWindow::slotAddUser()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Add new user"),
                                         tr("User name:"), QLineEdit::Normal,
                                         QString(), &ok);
    if (ok && !text.isEmpty())
    {
        cbx_users->addItem(text);
        cbx_users->setCurrentText(text);
    }
}


void MainWindow::slotGoUserSelection()
{
    int count = stacked_widget_main->count();
    qDebug() << count;
    for(int i = count-1; i > 2; --i) {
        stacked_widget_main->removeWidget(stacked_widget_main->widget(i));
    }
    stacked_widget_main->setCurrentIndex(0);
}
