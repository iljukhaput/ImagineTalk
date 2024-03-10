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
#include <QFont>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFont main_font = a.font();
    // main_font.setCapitalization(QFont::AllUppercase);
    main_font.setFamily("Arial");
    main_font.setWeight(QFont::Medium);
    main_font.setStretch(QFont::SemiCondensed);
    main_font.setLetterSpacing(QFont::AbsoluteSpacing, 1);
    main_font.setPointSize(12);
    a.setFont(main_font);

    MainWindow w;
    w.show();
    return a.exec();
}
