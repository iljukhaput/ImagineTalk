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
#include <QByteArray>
#include <QBuffer>
#include <QProgressDialog>

void fillGeneralTable(QSqlDatabase &db, MainWindow *mw);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), wgt_questions_setup(nullptr)
{
    if(!CreateUsersTable()) {
        qDebug() << "Data base is not open";
    }
    stacked_widget_main = new QStackedWidget(this);

    CreateChooseUserWindow();

    setCentralWidget(stacked_widget_main);
    setGeometry(200, 100, 1150, 650);
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

    /* При клике на кнопку:
     *      - создать QuestionWindow
     *      - добавить его в stqcked_widget_main
     *      - сделать его currentWidget
     */
    pb_general_questions = new QPushButton("Общие вопросы");
    pb_general_questions->setFixedSize(size_pb);
    connect(pb_general_questions, SIGNAL (clicked()), this, SLOT(slotGeneralQuestions()));

    QPushButton *pb_quit = new QPushButton("Выйти");
    pb_quit->setFixedSize(size_pb);
    connect(pb_quit, SIGNAL (clicked()), QApplication::instance(), SLOT(quit()));

    QVBoxLayout *vbx_layout  = new QVBoxLayout();
    vbx_layout->addWidget(cbx_users);
    vbx_layout->addWidget(pb_ok);
    vbx_layout->addWidget(pb_add_user);
    vbx_layout->addWidget(pb_delete_user);
    vbx_layout->addWidget(pb_general_questions);
    vbx_layout->addWidget(pb_quit);
    vbx_layout->setSpacing(5);
    vbx_layout->setAlignment(Qt::AlignCenter);
    wgt_choose_user->setLayout(vbx_layout);
}


void MainWindow::FillComboBox(QComboBox &cbx)
{
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "connection_name");
        db.setDatabaseName("ImagineTalkDB.db");
        db.open();

        QSqlQuery q(db);
        q.prepare("SELECT user FROM Users");
        if(q.exec()) {
            while(q.next()) {
                cbx.addItem(q.value("user").toString());
            }
        }

        q.finish();
        db.close();
    }
    QSqlDatabase::removeDatabase("connection_name");
}


/* В зависимости от имени таблицы, определяем к какой БД будем подключаться,
 * проверяем, существует ли уже такая таблица,
 *  если нет, то создаем
 *      если эта таблица - пользовательская,
 *          то заполняем её данными из таблицы общих вопросов
 *      иначе
 *          заполняем таблицу общих вопросов из файла ресурсов
 */
bool MainWindow::CreateQuestionsTable(QString table_name, int id)
{
    bool ok = false;
    QString full_table_name = table_name + QString::number(id);
    QString db_name = "ImagineTalkDB.db";
    if (full_table_name == "general_0"){
        db_name = "GeneralQuestionsDB.db";
    }

    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "connection_name");
        db.setDatabaseName(db_name);
        db.open();

        QSqlRecord table = db.record(full_table_name);
        if(table.isEmpty()){
            QSqlQuery query(db);
            query.prepare("CREATE TABLE " + full_table_name + " ("
                                                              "id INTEGER PRIMARY KEY, "
                                                              "question TEXT, "
                                                              "image BLOB, "
                                                              "parentId INT);");
            if(!query.exec()) {
                qDebug() << "Create User's questions table failed: " << query.lastError().text();
            } else {
                if (full_table_name == "general_0") {
                    fillGeneralTable(db, this);
                } else {
                    FillUserTable(full_table_name);
                }
                ok = true;
            }
            query.finish();
        }
        db.close();
    }
    QSqlDatabase::removeDatabase("connection_name");

    return ok;
}


void MainWindow::FillUserTable(QString full_table_name)
{
    QGuiApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    {
        QSqlDatabase general_db = QSqlDatabase::addDatabase("QSQLITE", "connection_general");
        general_db.setDatabaseName("GeneralQuestionsDB.db");
        if (!general_db.open()) {
            qDebug() << "Error in MainWindow::FillUserTable: cannot open GeneralQuestionsDB.db";
            return;
        }

        QSqlDatabase user_db = QSqlDatabase::database("connection_name");

        QSqlQuery q_to(user_db);
        QSqlQuery q_from(general_db);
        q_from.prepare("SELECT * FROM general_0;");
        q_from.exec();
        while (q_from.next()) {
            q_to.prepare("INSERT INTO " + full_table_name + " (id, question, image, parentId) "
                                                            "VALUES (?, ?, ?, ?)");
            q_to.addBindValue(q_from.value("id").toInt());
            q_to.addBindValue(q_from.value("question").toString());
            q_to.addBindValue(q_from.value("image").toByteArray());
            q_to.addBindValue(q_from.value("parentId").toInt());

            qDebug() << q_from.value("id").toInt() << q_from.value("question").toString() << q_from.value("parentId").toInt();
            if (!q_to.exec()) {
                qDebug() << "Error in MainWindow::FillUserTable:" << q_to.lastError();
                break;
            }
        }
        user_db.close();
        general_db.close();
    }
    QSqlDatabase::removeDatabase("connection_general");
    QGuiApplication::restoreOverrideCursor();
}


void MainWindow::CreateDialogWindow(QString &user, int table_id)
{
    wgt_dialog = new DialogWindow(table_id, this);
    stacked_widget_main->addWidget(wgt_dialog);
    connectPbToWindow(wgt_dialog, pb_start_dialog);
    connect(pb_start_dialog, SIGNAL(clicked()), wgt_dialog, SLOT(slotShowInitialQuestion()));
}


void MainWindow::CreateAddQuestionWindow(int table_id)
{
    // wgt_add_question = new QuestionWindow("user_", table_id, this); QuestionsSetup
    wgt_add_question = new QuestionsSetup("user_", table_id, this);
    stacked_widget_main->addWidget(wgt_add_question);
    // connectPbToWindow(wgt_add_question, pb_add_question);
    QSignalMapper *signal_mapper = new QSignalMapper(this);
    signal_mapper->setMapping(pb_add_question, wgt_add_question);
    connect(pb_add_question, SIGNAL (clicked()), signal_mapper, SLOT (map()));
    connect(signal_mapper, SIGNAL (mapped(QWidget *)), stacked_widget_main, SLOT (setCurrentWidget(QWidget *)));
    wgt_add_question->ConnectPbHome(stacked_widget_main, wgt_main);
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
    stacked_widget_main->setCurrentWidget(wgt_main);
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
    CreateQuestionsTable("user_", table_id);
    CreateDialogWindow(user, table_id);
    CreateAddQuestionWindow(table_id);
    goToMainWindow();
}


int MainWindow::getUserTableId(QString &user)
{
    int id = -1;
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "connection_name");
        db.setDatabaseName("ImagineTalkDB.db");
        db.open();

        QSqlQuery q(db);
        q.prepare("SELECT table_number FROM Users WHERE user=?");
        q.addBindValue(user);
        q.exec();
        if (q.first()) {
            id = q.value("table_number").toInt();
        }
        q.finish();
        db.close();
    }
    QSqlDatabase::removeDatabase("connection_name");
    return id;
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
        if (userIsExist(username)) {
            QMessageBox msg_box;
            msg_box.setText("Пользователь с таким именем уже существует");
            msg_box.exec();
            return;
        }
        int table_id = createUserTableId();
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "connection_name");
        db.setDatabaseName("ImagineTalkDB.db");
        if (db.open()) {
            cbx_users->addItem(username);
            QSqlQuery q(db);
            q.prepare("INSERT INTO Users (user, table_number)"
                      "VALUES (?, ?)");
            q.addBindValue(username);
            q.addBindValue(table_id);
            if(!q.exec()) {
                qDebug() << "Insert into Users failed:" << q.lastError().text();
            }
            q.finish();
            db.close();
        }
    }
    QSqlDatabase::removeDatabase("connection_name");
    cbx_users->setCurrentText(username);
}


int MainWindow::createUserTableId()
{
    QSet<int> set;
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "connection_name");
        db.setDatabaseName("ImagineTalkDB.db");
        db.open();

        QSqlQuery q(db);
        q.prepare("SELECT table_number FROM Users");
        if(!q.exec()) {
            qDebug() << "Error in createUserTableId:" << q.lastError().text();
        }
        while (q.next()) {
            set.insert(q.value("table_number").toInt());
        }

        q.finish();
        db.close();
    }
    QSqlDatabase::removeDatabase("connection_name");

    for(int i = 0; ; ++i) {
        if(!set.contains(i)) {
            return i;
        }
    }
}


void MainWindow::slotDeleteUser()
{
    {
        QString username = cbx_users->currentText();
        if (username.isEmpty())
            return;

        QString text = "Вы действительно хотите удалить пользователя " + username + "\nи все данные, связанные с ним?";
                           AcceptDialog dialog(text, this);
        if (!dialog.exec())
            return;

        int table_id = getUserTableId(username);

        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "connection_name");
        db.setDatabaseName("ImagineTalkDB.db");
        db.open();

        QSqlQuery q(db);
        q.prepare("DROP TABLE IF EXISTS user_" + QString::number(table_id));
        if(!q.exec()) {
            qDebug() << "Error in MainWindow::slotDeleteUser - Deleting table user_" + QString::number(table_id) + " from DB failed:" << q.lastError().text();
            db.close();
            QSqlDatabase::removeDatabase("connection_name");
            return;
        }

        q.prepare("DELETE FROM Users WHERE user=?");
        q.addBindValue(username);
        if(!q.exec()) {
            qDebug() << "Error in MainWindow::slotDeleteUser in query 'DELETE FROM Users WHERE user=':" << q.lastError().text();
            db.close();
            QSqlDatabase::removeDatabase("connection_name");
            return;
        }

        q.finish();
        db.close();

        cbx_users->removeItem(cbx_users->findText(username));
    }
    QSqlDatabase::removeDatabase("connection_name");
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

bool MainWindow::userIsExist(QString &username)
{
    bool ok = false;
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "connection_name");
        db.setDatabaseName("ImagineTalkDB.db");
        db.open();
        QSqlQuery q(db);
        q.prepare("SELECT user FROM Users where user=?");
        q.addBindValue(username);
        if(q.exec()) {
            q.first();
            ok = q.isValid();
        }
        q.finish();
        db.close();
    }
    QSqlDatabase::removeDatabase("connection_name");
    return ok;
}



void MainWindow::slotGeneralQuestions()
{
    QString table_name = "general_";
    int table_id = 0;
    CreateQuestionsTable(table_name, table_id);
    if (stacked_widget_main->indexOf(wgt_questions_setup) < 0) {
        wgt_questions_setup = new QuestionsSetup(table_name, table_id, this);
        stacked_widget_main->addWidget(wgt_questions_setup);
    }
    stacked_widget_main->setCurrentWidget(wgt_questions_setup);
    wgt_questions_setup->ConnectPbHome(stacked_widget_main, wgt_choose_user);
}


bool MainWindow::CreateUsersTable()
{
    bool ok = false;
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("ImagineTalkDB.db");
        if(db.open()) {
            qDebug() << "Data base opened";

            QSqlRecord table_users = db.record("Users");
            if(!table_users.isEmpty()) {
                ok = true;
            } else {
                QSqlQuery query(db);
                query.prepare("CREATE TABLE Users (user TEXT, table_number INT);");
                if(!query.exec()) {
                    qDebug() << "Create table Users failed: " << query.lastError().text();
                } else {
                    ok = true;
                }
                query.finish();
            }
        } else {
            qDebug() << "open DB failed: " << db.lastError().text();
        }
        db.close();
    }
    QSqlDatabase::removeDatabase("connection_name");
    return ok;
}

// bool MainWindow::CreateGeneralTable(QString table_name, int table_id)
// {

//     {
//         QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "connection_name");
//         db.setDatabaseName("GeneralQuestionsDB.db");
//         db.open();
//         QSqlRecord table = db.record(table_name + QString::number(table_id));
//         if(table.isEmpty()) {
//             QSqlQuery query(db);
//             query.prepare("CREATE TABLE " + table_name + QString::number(table_id) + " ("
//                                                         "id INTEGER PRIMARY KEY, "
//                                                         "question TEXT, "
//                                                         "image BLOB, "
//                                                         "parentId INT);");
//             if(!query.exec()) {
//                 qDebug() << "Error in MainWindow::CreateGeneralTable: Create GeneralQuestions table failed: " << query.lastError().text();
//             } else {
//                 // fillGeneralQuestionsTable(id, this);
//             }
//             query.finish();
//         }
//         db.close();
//     }
//     QSqlDatabase::removeDatabase("connection_name");
//     return true;
// }
