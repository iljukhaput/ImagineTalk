#include "secondlayoutwindow.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

// QSqlDatabase SecondLayoutWindow::db = QSqlDatabase::addDatabase("QSQLITE");

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


void SecondLayoutWindow::closeDB()
{
    // if(db.isOpen()) {
    //     db.close();
    //     qDebug() << "DB is closed";
    // }
}


int SecondLayoutWindow::getUserTableId()
{
    int table_number = -1;
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "connection_name");
        db.setDatabaseName("ImagineTalkDB.db");
        db.open();

        QSqlQuery q(db);
        q.prepare("SELECT table_number FROM Users WHERE user=?");
        q.addBindValue(user);
        q.exec();
        if (q.first()) {
            table_number = q.value("table_number").toInt();
        }
        q.finish();

        db.close();
    }
    QSqlDatabase::removeDatabase("connection_name");

    return table_number;
}

