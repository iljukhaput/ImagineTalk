#include "questionssetup.h"
#include "addqadialog.h"
#include <QDebug>
#include <QSignalMapper>

#include <QSqlTableModel>
#include <QTableView>


QuestionsSetup::QuestionsSetup(QString table_name, int table_id, QWidget *parent)
    : QWidget{parent}
{
    qDebug() << "Constructor QuestionsSetup - " << this;

    QFontMetrics fm(font());
    QString max_string("На главный экран"); // maximum length string
    int width = fm.horizontalAdvance(max_string) + 20;
    int height = fm.height() + 20;
    QSize pb_size(width, height);

    selected_lbl = new QLabel;
    selected_lbl->setMinimumWidth(320);
    selected_lbl->setAlignment(Qt::AlignCenter);

    listbtn = new QuestionsListBtn(table_name, table_id, selected_lbl, this);

    pb_add = new QPushButton("Добавить");
    pb_add->setMinimumSize(pb_size);
    connect(pb_add, SIGNAL(clicked()), this, SLOT(slotAddQuestionAnswer()));

    pb_delete = new QPushButton("Удалить");
    pb_delete->setMinimumSize(pb_size);
    connect(pb_delete, SIGNAL(clicked()), listbtn, SLOT(slotDelete()));

    pb_home = new QPushButton("На главный экран", this);
    pb_home->setMinimumSize(pb_size);

    QHBoxLayout *hbx_btn_layout = new QHBoxLayout;
    hbx_btn_layout->addWidget(pb_add);
    hbx_btn_layout->addWidget(pb_delete);
    hbx_btn_layout->addWidget(pb_home);
    hbx_btn_layout->setAlignment(Qt::AlignCenter);

    // --
    // QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "connection_name");
    // db.setDatabaseName("GeneralQuestionsDB.db");
    // db.open();
    // QSqlTableModel *table_model = new QSqlTableModel(this, db);
    // QString tablename = table_name + QString::number(table_id);
    // table_model->setTable(tablename);
    // table_model->select();
    // QTableView *table_view = new QTableView;
    // table_view->setModel(table_model);
    // db.close();
    // QSqlDatabase::removeDatabase("connection_name");
    // --
    QVBoxLayout *vbx_layout = new QVBoxLayout;
    vbx_layout->addWidget(listbtn);
    vbx_layout->addLayout(hbx_btn_layout);

    QHBoxLayout *hbx_wgt_layout = new QHBoxLayout(this);
    hbx_wgt_layout->addLayout(vbx_layout);
    // hbx_wgt_layout->addWidget(table_view);
    hbx_wgt_layout->addWidget(selected_lbl);
}


void QuestionsSetup::slotAddQuestionAnswer()
{
    AddQADialog *dialog = new AddQADialog;
    if(dialog->exec()){
        QString qa_text = dialog->getQA();
        if(qa_text.isEmpty()) {
            qDebug() << "QA is empty";
            return;
        }
        listbtn->addRow(qa_text, dialog->getImageByteArray());
    }
    delete dialog;
}


void QuestionsSetup::ConnectPbHome(QStackedWidget *stacked_widget, QWidget *wgt_home)
{
    QSignalMapper *signal_mapper = new QSignalMapper(this);
    signal_mapper->setMapping(pb_home, wgt_home);
    connect(pb_home, SIGNAL (clicked()), signal_mapper, SLOT (map()));
    connect(signal_mapper, SIGNAL (mapped(QWidget *)), stacked_widget, SLOT (setCurrentWidget(QWidget *)));
}
