#include "questionwindow.h"
// #include "sqltablemodel.h"
#include "addqadialog.h"

#include <QInputDialog>
#include <QSqlQuery>
#include <QItemSelectionModel>
#include <QDebug>
#include <QStyledItemDelegate>
#include <QBuffer>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>


QuestionWindow::QuestionWindow(QString &user, int table_id, QWidget *parent)
    : SecondLayoutWindow(user, table_id, parent)
{
    qDebug() << "Constructor QuestionWindow - " << this;

    QFontMetrics fm(font());
    QString max_string("На главный экран"); // maximum length string
    int width = fm.horizontalAdvance(max_string) + 20;
    int height = fm.height() + 20;

    selected_lbl = new QLabel;
    selected_lbl->setMinimumWidth(320);
    selected_lbl->setAlignment(Qt::AlignCenter);
    listbtn = new QuestionsListBtn(table_id, selected_lbl, this);

    QSize pb_size(width, height);
    pb_add = new QPushButton("Добавить");
    pb_add->setMinimumSize(pb_size);
    connect(pb_add, SIGNAL(clicked()), this, SLOT(slotAddQuestionAnswer()));

    pb_delete = new QPushButton("Удалить");
    pb_delete->setMinimumSize(pb_size);
    connect(pb_delete, SIGNAL(clicked()), listbtn, SLOT(slotDelete()));

    pb_home->setMinimumSize(pb_size);

    QHBoxLayout *hbx_btn_layout = new QHBoxLayout;
    hbx_btn_layout->addWidget(pb_add);
    hbx_btn_layout->addWidget(pb_delete);
    hbx_btn_layout->addWidget(pb_home);
    hbx_btn_layout->setAlignment(Qt::AlignCenter);

    QVBoxLayout *vbx_layout = new QVBoxLayout;
    vbx_layout->addWidget(listbtn);
    vbx_layout->addLayout(hbx_btn_layout);

    QHBoxLayout *hbx_wgt_layout = new QHBoxLayout(this);
    hbx_wgt_layout->addLayout(vbx_layout);
    hbx_wgt_layout->addWidget(selected_lbl);
}

QuestionWindow::~QuestionWindow()
{
    qDebug() << "Destructor QuestionWindow - " << this;
}



void QuestionWindow::slotAddQuestionAnswer()
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


