#include "mainwindow.h"

#include <QApplication>
#include <QHBoxLayout>
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <QPushButton>
#include <QFont>
#include <QIcon>
#include <QPicture>
#include <QString>

#include <QFileSystemModel>
#include <QTreeView>
#include <QSplitter>
#include <QTableView>
#include <QMdiSubWindow>


int test(int argc, char *argv[]){
    QApplication a(argc, argv);

    QPushButton *pb = new QPushButton("X");
    QMdiSubWindow subwindow;
    subwindow.setWidget(pb);
    subwindow.show();

    return a.exec();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
