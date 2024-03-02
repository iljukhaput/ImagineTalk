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

DialogWindow::DialogWindow(QString &user, QWidget *parent)
    : SecondLayoutWindow(user, parent)
    , scale(1)
{
    qDebug() << "Constructor DialogWindow"; // --------------------------- DEBUG
    stacked_widget_dialog = new QStackedWidget();

    QSize size_pb(100, 30);

    pb_home->setFixedSize(size_pb);
    connect(pb_home, SIGNAL(clicked()), this, SLOT(slotClearStackedWidget()));

    QPushButton *pb_back = new QPushButton("Back");
    pb_back->setFixedSize(size_pb);
    connect(pb_back, SIGNAL (clicked()), this, SLOT (slotGoToPreviousAnswer()));

    QPushButton *pb_increase = new QPushButton("+");
    pb_increase->setFixedSize(size_pb);
    connect(pb_increase, SIGNAL (clicked()), this, SLOT (slotIncreaseImages()));

    QPushButton *pb_decrease = new QPushButton("-");
    pb_decrease->setFixedSize(size_pb);
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

void DialogWindow::slotShowAnswers(int id)
{
    qDebug() << "\n-- slotShowAnswers | id =" << id; // --------------------------- DEBUG

    backpath.append(id);
    QScrollArea *scroll_area = createQAWidget(id);
    stacked_widget_dialog->addWidget(scroll_area);
    stacked_widget_dialog->setCurrentWidget(scroll_area);

    qDebug() << "-- slotShowAnswers - current index =" << stacked_widget_dialog->currentIndex(); // --------------------------- DEBUG
}


QScrollArea *DialogWindow::createQAWidget(int id)
{
    QSqlQuery *query = new QSqlQuery(db);
    query->prepare("SELECT * FROM Questions WHERE parentId=?");
    query->addBindValue(id);
    query->exec();

    QVBoxLayout *vboxlayout = new QVBoxLayout;
    QHBoxLayout *hboxlayout = new QHBoxLayout;
    int row_count = rowCount(query);
    int counter = 0;
    while(query->next())
    {
        QString question = query->value("question").toString();
        QByteArray image = query->value("image").toByteArray();
        AnswerWidget *pb = new AnswerWidget(question, scale, &image);
        connectPbQuestion(pb, query->value("id").toInt());
        if (row_count == 4) {
            if (counter == 2) {
                qDebug() << "NEW LAYOUT"; // --------------------------- DEBUG
                vboxlayout->addLayout(hboxlayout);
                hboxlayout = new QHBoxLayout;
            }
        } else {
            if (counter % 3 == 0 && counter > 0) {
                qDebug() << "NEW LAYOUT"; // --------------------------- DEBUG
                vboxlayout->addLayout(hboxlayout);
                hboxlayout = new QHBoxLayout;
            }
        }
        hboxlayout->addWidget(pb);
        counter++;
        qDebug() << question; // --------------------------- DEBUG
    }
    delete query;

    vboxlayout->addLayout(hboxlayout);

    QWidget *wgt = new QWidget;
    wgt->setLayout(vboxlayout);

    QScrollArea *scroll_area = new QScrollArea(this);
    scroll_area->setWidget(wgt);
    scroll_area->setAlignment(Qt::AlignCenter);
    scroll_area->setWidgetResizable(true);

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
    clearStackedWidget();
    scale++;
    QScrollArea *scroll_area = nullptr;
    for (int i = 0; i < backpath.size(); ++i) {
        scroll_area = createQAWidget(backpath.at(i));
        stacked_widget_dialog->addWidget(scroll_area);
    }
    stacked_widget_dialog->setCurrentWidget(scroll_area);
}


void DialogWindow::slotDecreaseImages()
{
    clearStackedWidget();
    scale--;
    QScrollArea *scroll_area = nullptr;
    for (int i = 0; i < backpath.size(); ++i) {
        scroll_area = createQAWidget(backpath.at(i));
        stacked_widget_dialog->addWidget(scroll_area);
    }
    stacked_widget_dialog->setCurrentWidget(scroll_area);
}


void DialogWindow::slotGoToPreviousAnswer()
{
    int index = stacked_widget_dialog->currentIndex();
    if (index == 0)
        return;
    stacked_widget_dialog->removeWidget(stacked_widget_dialog->currentWidget());
    backpath.pop_back();
}


void DialogWindow::slotClearStackedWidget()
{
    while(stacked_widget_dialog->count() != 0) {
        stacked_widget_dialog->removeWidget(stacked_widget_dialog->currentWidget());
        backpath.pop_back();
    }
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
        stacked_widget_dialog->removeWidget(stacked_widget_dialog->currentWidget());
    }
}
