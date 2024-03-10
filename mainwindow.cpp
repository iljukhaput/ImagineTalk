#include "mainwindow.h"
#include "secondlayoutwindow.h"
#include "dialogwindow.h"
#include "acceptdialog.h"

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
#include <QSqlError>
#include <QSqlRecord>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    if(!SecondLayoutWindow::initDB()) {
        qDebug() << "Data base is not open";
    }
    stacked_widget_main = new QStackedWidget(this);

    CreateChooseUserWindow();

    setCentralWidget(stacked_widget_main);
    setGeometry(200, 200, 1150, 550);
}

MainWindow::~MainWindow()
{
    SecondLayoutWindow::closeDB();
}


void MainWindow::CreateMainWindow(QString &user)
{
    wgt_main = new QWidget;
    stacked_widget_main->addWidget(wgt_main);


    QFontMetrics fm(font());
    QString max_string("Вернуться к выбору пользователя"); // maximum length string
    int width = fm.horizontalAdvance(max_string) + 40;
    QSize size_pb(width, 100);

    QLabel *lbl_username = new QLabel(user);
    lbl_username->setFixedSize(size_pb.width(), 50);
    lbl_username->setAlignment(Qt::AlignCenter);
    lbl_username->setWordWrap(true);

    pb_start_dialog = new QPushButton("Начать диалог");
    pb_start_dialog->setFixedSize(size_pb);

    pb_add_question = new QPushButton("Список вопросов/ответов");
    pb_add_question->setFixedSize(size_pb);

    pb_user_selection = new QPushButton("Вернуться к выбору пользователя");
    pb_user_selection->setFixedSize(size_pb);
    connect(pb_user_selection, SIGNAL(clicked()), this, SLOT(slotGoUserSelection()));

    pb_quit = new QPushButton("Выйти");
    pb_quit->setFixedSize(size_pb);
    connect(pb_quit, SIGNAL (clicked()), QApplication::instance(), SLOT (quit()));

    QVBoxLayout *vbx_layout  = new QVBoxLayout();
    vbx_layout->addWidget(lbl_username);
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

    QFontMetrics fm(font());
    QString max_string("Удалить выбранного пользователя"); // maximum length string
    int width = fm.horizontalAdvance(max_string) + 40;
    QSize size_pb(width, 100);

    cbx_users = new QComboBox;
    cbx_users->setFixedSize(size_pb.width(), 50);
    FillComboBox(*cbx_users);

    QPushButton *pb_ok = new QPushButton("Ок");
    pb_ok->setFixedSize(size_pb);
    connect(pb_ok, SIGNAL(clicked()), this, SLOT(slotGoToMainWindow()));

    QPushButton *pb_add_user = new QPushButton("Добавить пользователя");
    pb_add_user->setFixedSize(size_pb);
    connect(pb_add_user, SIGNAL (clicked()), this, SLOT(slotAddUser()));

    QPushButton *pb_delete_user = new QPushButton("Удалить выбранного пользователя");
    pb_delete_user->setFixedSize(size_pb);
    connect(pb_delete_user, SIGNAL (clicked()), this, SLOT(slotDeleteUser()));

    QPushButton *pb_quit = new QPushButton("Выйти");
    pb_quit->setFixedSize(size_pb);
    connect(pb_quit, SIGNAL (clicked()), QApplication::instance(), SLOT(quit()));

    QVBoxLayout *vbx_layout  = new QVBoxLayout();
    vbx_layout->addWidget(cbx_users);
    vbx_layout->addWidget(pb_ok);
    vbx_layout->addWidget(pb_add_user);
    vbx_layout->addWidget(pb_delete_user);
    vbx_layout->addWidget(pb_quit);
    vbx_layout->setSpacing(5);
    vbx_layout->setAlignment(Qt::AlignCenter);
    wgt_choose_user->setLayout(vbx_layout);
}


void MainWindow::FillComboBox(QComboBox &cbx)
{
    QSqlQuery q(SecondLayoutWindow::db);
    q.prepare("SELECT user FROM Users");
    if(q.exec()) {
        while(q.next()) {
            cbx.addItem(q.value("user").toString());
        }
    }
}


bool MainWindow::CreateUserTable(int id)
{
    QString tablename = "user_" + QString::number(id);
    if(userTableIsExist(tablename)) {
        return true;
    }

    QSqlQuery query(SecondLayoutWindow::db);
    query.prepare("CREATE TABLE " + tablename + " ("
                  "id INTEGER PRIMARY KEY, "
                  "question TEXT, "
                  "image BLOB, "
                  "parentId  INT);");
    if(!query.exec()) {
        qDebug() << "Create User's questions table failed: " << query.lastError().text();
        return false;
    }
    return true;
}


bool MainWindow::userTableIsExist(QString tablename)
{
    QSqlRecord table_of_user = SecondLayoutWindow::db.record(tablename);
    if(table_of_user.isEmpty()) {
        return false;
    } else {
        return true;
    }
}


void MainWindow::CreateDialogWindow(QString &user, int table_id)
{
    wgt_dialog = new DialogWindow(user, table_id, this);
    stacked_widget_main->addWidget(wgt_dialog);
    connectPbToWindow(wgt_dialog, pb_start_dialog);
    connect(pb_start_dialog, SIGNAL(clicked()), wgt_dialog, SLOT(slotShowInitialQuestion()));
}


void MainWindow::CreateAddQuestionWindow(QString &user, int table_id)
{
    wgt_add_question = new QuestionWindow(user, table_id, this);
    stacked_widget_main->addWidget(wgt_add_question);
    connectPbToWindow(wgt_add_question, pb_add_question);
}


void MainWindow::connectPbToWindow(SecondLayoutWindow *wgt, QPushButton *pb)
{
    qDebug() << "connectPbToWindow: " << wgt << pb;
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
    int table_id = getUserTableId(user);
    if(user.isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Выберите пользователя или создайте нового"));
        msgBox.exec();
        return;
    }
    CreateMainWindow(user);
    CreateUserTable(table_id);
    CreateDialogWindow(user, table_id);
    CreateAddQuestionWindow(user, table_id);
    goToMainWindow();
}


int MainWindow::getUserTableId(QString &user)
{
    QSqlQuery q(SecondLayoutWindow::db);
    q.prepare("SELECT table_number FROM Users WHERE user=?");
    q.addBindValue(user);
    q.exec();
    if (q.first()) {
        return q.value("table_number").toInt();
    } else {
        return -1;
    }
}


void MainWindow::slotAddUser()
{

    QInputDialog dialog(this);
    dialog.setLabelText("Имя пользователя:");
    dialog.setInputMode(QInputDialog::TextInput);
    dialog.setOkButtonText("Ок");
    dialog.setCancelButtonText("Отмена");
    if (!dialog.exec()) {
        return;
    }

    QString username = dialog.textValue();
    if (!username.isEmpty())
    {
        int table_id = createUserTableId();
        cbx_users->addItem(username);
        QSqlQuery q(SecondLayoutWindow::db);
        q.prepare("INSERT INTO Users (user, table_number)"
                  "VALUES (?, ?)");
        q.addBindValue(username);
        q.addBindValue(table_id);
        if(!q.exec()) {
            qDebug() << "Insert into Users failed:" << q.lastError().text();
        }
    }
    cbx_users->setCurrentText(username);
}


int MainWindow::createUserTableId()
{
    QSqlQuery q(SecondLayoutWindow::db);
    q.prepare("SELECT table_number FROM Users");
    if(!q.exec()) {
        qDebug() << "Error in createUserTableId:" << q.lastError().text();
    }
    QSet<int> set;
    while (q.next()) {
        set.insert(q.value("table_number").toInt());
    }
    for(int i = 0; ; ++i) {
        if(!set.contains(i))
            return i;
    }
    return -1;
}


void MainWindow::slotDeleteUser()
{
    QString username = cbx_users->currentText();
    QString text = "Вы действительно хотите удалить пользователя " + username + " и все данные, связанные с ним?";
    AcceptDialog dialog(text, this);
    if (!dialog.exec())
        return;

    qDebug() << "deleting" << text;

    QSqlQuery q(SecondLayoutWindow::db);

    int table_id = getUserTableId(username);
    q.prepare("DROP TABLE IF EXISTS user_" + QString::number(table_id));
    if(!q.exec()) {
        qDebug() << "Error in MainWindow::slotDeleteUser - Deleting table user_" + QString::number(table_id) + " from DB failed:" << q.lastError().text();
    }

    q.prepare("DELETE FROM Users WHERE user=?");
    q.addBindValue(username);
    if(!q.exec()) {
        qDebug() << "Error in MainWindow::slotDeleteUser in query 'DELETE FROM Users WHERE user=':" << q.lastError().text();
    }

    cbx_users->removeItem(cbx_users->findText(username));
}

void MainWindow::slotGoUserSelection()
{
    int count = stacked_widget_main->count();
    qDebug() << count;
    for(int i = count-1; i > 0; --i) {
        qDebug() << "stacked_widget_main->widget(" << i << ") -" << stacked_widget_main->widget(i);
        QWidget *tmp = stacked_widget_main->widget(i);
        stacked_widget_main->removeWidget(tmp);
        delete tmp;
    }
    stacked_widget_main->setCurrentIndex(0);
    count = stacked_widget_main->count();
    qDebug() << count;
}
