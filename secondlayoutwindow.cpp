#include "secondlayoutwindow.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

QSqlDatabase SecondLayoutWindow::db = QSqlDatabase::addDatabase("QSQLITE");

SecondLayoutWindow::SecondLayoutWindow(QString &user, int table_id, QWidget *parent)
    : QWidget(parent), user(user), table_id(table_id)
{
    pb_home = new QPushButton("На главный экран", this);
}


void SecondLayoutWindow::ConnectPbHome(QSignalMapper &signal_mapper, QStackedWidget &stacked_widget, QWidget &wgt_home) const
{
    signal_mapper.setMapping(pb_home, &wgt_home);
    connect(pb_home, SIGNAL (clicked()), &signal_mapper, SLOT (map()));
    connect(&signal_mapper, SIGNAL (mapped(QWidget *)), &stacked_widget, SLOT (setCurrentWidget(QWidget *)));
}

bool SecondLayoutWindow::initDB()
{
    if(db.isOpen()) {
        return true;
    }
    db.setDatabaseName("testDB.db");

    if(!db.open()) {
        qDebug() << "open DB failed: " << db.lastError().text();
        return false;
    }

    qDebug() << "Data base opened";

    QSqlRecord table_users = db.record("Users");
    if(!table_users.isEmpty()) {
        return true;
    }

    QSqlQuery query(db);
    query.prepare("CREATE TABLE Users (user TEXT, table_number INT);");
    if(!query.exec()) {
        qDebug() << "Create table Users failed: " << query.lastError().text();
        return false;
    }
    return true;
}


void SecondLayoutWindow::closeDB()
{
    if(db.isOpen()) {
        db.close();
        qDebug() << "DB is closed";
    }
}


int SecondLayoutWindow::getUserTableId()
{
    QSqlQuery q(db);
    q.prepare("SELECT table_number FROM Users WHERE user=?");
    q.addBindValue(user);
    q.exec();
    if (q.first()) {
        return q.value("table_number").toInt();
    } else {
        return -1;
    }
}

