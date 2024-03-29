#include "dialogwindow.h"

#include <QSignalMapper>
#include <QStackedWidget>
#include <QObject>
#include <QMetaObject>
#include <QSqlQuery>
#include <QDebug>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QStackedWidget>
#include <QScrollBar>

DialogWindow::DialogWindow(int table_id, QWidget *parent)
    : SecondLayoutWindow(table_id, parent)
    , scale(1)
{
    qDebug() << "Constructor DialogWindow - " << this; // --------------------------- DEBUG
    stacked_widget_dialog = new QStackedWidget(this);

    QFontMetrics fm(font());
    QString max_string("На главный экран"); // maximum length string
    int width = fm.horizontalAdvance(max_string) + 20;
    int height = fm.height() + 20;

    QSize size_pb(width, height);

    pb_home->setFixedSize(size_pb);
    connect(pb_home, SIGNAL(clicked()), this, SLOT(slotBackpath()));

    QPushButton *pb_back = new QPushButton("Назад");
    pb_back->setFixedSize(size_pb);
    connect(pb_back, SIGNAL (clicked()), this, SLOT (slotGoToPreviousAnswer()));

    QPushButton *pb_increase = new QPushButton("+");
    pb_increase->setFixedSize(size_pb.width() / 2, size_pb.height());
    connect(pb_increase, SIGNAL (clicked()), this, SLOT (slotIncreaseImages()));

    QPushButton *pb_decrease = new QPushButton("-");
    pb_decrease->setFixedSize(size_pb.width() / 2, size_pb.height());
    connect(pb_decrease, SIGNAL (clicked()), this, SLOT (slotDecreaseImages()));

    QHBoxLayout *hbx_layout = new QHBoxLayout();
    hbx_layout->addWidget(pb_increase);
    hbx_layout->addWidget(pb_decrease);
    hbx_layout->addWidget(pb_back);
    hbx_layout->addWidget(pb_home);
    hbx_layout->setAlignment(Qt::AlignCenter);

    QVBoxLayout *vbx_layout  = new QVBoxLayout();
    vbx_layout->addWidget(stacked_widget_dialog);
    vbx_layout->addLayout(hbx_layout);
    vbx_layout->setAlignment(Qt::AlignCenter);
    this->setLayout(vbx_layout);
}

DialogWindow::~DialogWindow()
{
    qDebug() << "Destructor DialogWindow - " << this;
}

void DialogWindow::slotShowAnswers(int id)
{
    backpath.append(id);
    QScrollArea *scroll_area = createQAWidget(id);
    stacked_widget_dialog->addWidget(scroll_area);
    stacked_widget_dialog->setCurrentWidget(scroll_area);
}


QScrollArea *DialogWindow::createQAWidget(int id)
{
    QScrollArea *scroll_area = new QScrollArea(this);
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "connection_name");
        db.setDatabaseName("ImagineTalkDB.db");
        db.open();

        QSqlQuery query(db);
        query.prepare("SELECT * FROM user_" + QString::number(table_id) + " WHERE parentId=?");
        query.addBindValue(id);
        query.exec();

        QVBoxLayout *vboxlayout = new QVBoxLayout;
        QHBoxLayout *hboxlayout = new QHBoxLayout;
        int row_count = rowCount(&query);
        int counter = 0;
        while(query.next())
        {
            QString question = query.value("question").toString();
            QByteArray image = query.value("image").toByteArray();
            AnswerWidget *pb = new AnswerWidget(question, scale, &image, this);
            connectPbQuestion(pb, query.value("id").toInt());
            if (row_count == 4) {
                if (counter == 2) {
                    vboxlayout->addLayout(hboxlayout);
                    hboxlayout = new QHBoxLayout;
                }
            } else {
                if (counter % 3 == 0 && counter > 0) {
                    vboxlayout->addLayout(hboxlayout);
                    hboxlayout = new QHBoxLayout;
                }
            }
            hboxlayout->addWidget(pb);
            counter++;
        }
        query.finish();
        db.close();

        vboxlayout->addLayout(hboxlayout);

        QWidget *wgt = new QWidget;
        wgt->setLayout(vboxlayout);

        scroll_area->setWidget(wgt);
        scroll_area->setAlignment(Qt::AlignCenter);
        scroll_area->setWidgetResizable(true);
    }
    QSqlDatabase::removeDatabase("connection_name");

    return scroll_area;
}


int DialogWindow::rowCount(QSqlQuery *query)
{
    int row_count = 0;
    if(query->last()) {
        row_count = query->at() + 1;
        query->first();
        query->previous();
    }
    return row_count;
}


void DialogWindow::slotIncreaseImages()
{
    QScrollArea *scroll_area = static_cast<QScrollArea *>(stacked_widget_dialog->currentWidget());
    int scroll_val = scroll_area->verticalScrollBar()->value();
    clearStackedWidget();
    scale++;
    for (int i = 0; i < backpath.size(); ++i) {
        scroll_area = createQAWidget(backpath.at(i));
        stacked_widget_dialog->addWidget(scroll_area);
    }
    scroll_area->verticalScrollBar()->setValue(scroll_val);
    stacked_widget_dialog->setCurrentWidget(scroll_area);
}


void DialogWindow::slotDecreaseImages()
{
    QScrollArea *scroll_area = static_cast<QScrollArea *>(stacked_widget_dialog->currentWidget());
    int scroll_val = scroll_area->verticalScrollBar()->value();
    clearStackedWidget();
    scale--;
    for (int i = 0; i < backpath.size(); ++i) {
        scroll_area = createQAWidget(backpath.at(i));
        stacked_widget_dialog->addWidget(scroll_area);
    }
    scroll_area->verticalScrollBar()->setValue(scroll_val);
    stacked_widget_dialog->setCurrentWidget(scroll_area);
}


void DialogWindow::slotGoToPreviousAnswer()
{
    int index = stacked_widget_dialog->currentIndex();
    if (index == 0)
        return;
    QWidget *tmp = stacked_widget_dialog->currentWidget();
    stacked_widget_dialog->removeWidget(stacked_widget_dialog->currentWidget());
    delete tmp;
    backpath.pop_back();
}


void DialogWindow::slotBackpath()
{
    clearStackedWidget();
    backpath.clear();
}


void DialogWindow::connectPbQuestion(AnswerWidget *pb, int id)
{
    QSignalMapper *mapper = new QSignalMapper(this);
    mapper->setMapping(pb, id);
    connect(pb, SIGNAL (clicked()), mapper, SLOT (map()));
    connect(mapper, SIGNAL (mapped(int)), this, SLOT (slotShowAnswers(int)));
}


void DialogWindow::slotShowInitialQuestion()
{
    slotShowAnswers(0);
}


void DialogWindow::clearStackedWidget()
{
    while(stacked_widget_dialog->count() != 0) {
        QWidget *tmp = stacked_widget_dialog->currentWidget();
        stacked_widget_dialog->removeWidget(stacked_widget_dialog->currentWidget());
        delete tmp;
    }
}
