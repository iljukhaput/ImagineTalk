#include "secondlayoutwindow.h"
#include <QDebug>
#include <QSqlQuery>

QSqlDatabase SecondLayoutWindow::db = QSqlDatabase::addDatabase("QSQLITE");

SecondLayoutWindow::SecondLayoutWindow(QString &user, QWidget *parent)
    : QWidget(parent), user(user)
{
    pb_home = new QPushButton("Home", this);
}


void SecondLayoutWindow::ConnectPbHome(QSignalMapper &signal_mapper, QStackedWidget &stacked_widget, QWidget &wgt_home) const
{
    signal_mapper.setMapping(pb_home, &wgt_home);
    connect(pb_home, SIGNAL (clicked()), &signal_mapper, SLOT (map()));
    connect(&signal_mapper, SIGNAL (mapped(QWidget *)), &stacked_widget, SLOT (setCurrentWidget(QWidget *)));
}

bool SecondLayoutWindow::initDB()
{
    if(!db.isOpen()) {
        db.setDatabaseName("testDB.db");
        if(db.open()) {
            qDebug() << "Data base opened";
            QSqlQuery *query = new QSqlQuery(db);
            query->prepare("CREATE TABLE Questions ("
                        "id INTEGER PRIMARY KEY, "
                        "question TEXT, "
                        "image BLOB, "
                        "parentId  INT, "
                        "user TEXT);");
            // query->addBindValue(user);

            // query->exec("INSERT INTO Questions (question, type, parentId)"
            //             "VALUES ('What do you wnat?', 'Q', 0),"
            //             "('Eat', 'A', 1);");
            return true;
        }
        else {
            qDebug() << "Data base didn't open\n";
        }
    }
    return false;
}

SecondLayoutWindow::~SecondLayoutWindow()
{
    if(db.isOpen()) {
        db.close();
        qDebug() << "DB is closed";
    }
}
