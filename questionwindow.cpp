#include "questionwindow.h"
#include "sqltablemodel.h"
#include "addqadialog.h"

#include <QInputDialog>
#include <QSqlQuery>
#include <QItemSelectionModel>
#include <QDebug>
#include <QStyledItemDelegate>
#include <QBuffer>
#include <QHBoxLayout>
#include <QVBoxLayout>


QuestionWindow::QuestionWindow(QString &user, int table_id, QWidget *parent)
    : SecondLayoutWindow(user, table_id, parent)
{
    qDebug() << "Constructor QuestionWindow - " << this;
    initView();

    QFontMetrics fm(font());
    QString max_string("На главный экран"); // maximum length string
    int width = fm.horizontalAdvance(max_string) + 20;
    int height = fm.height() + 20;

    QSize pb_size(width, height);
    pb_add = new QPushButton("Добавить");
    pb_add->setMinimumSize(pb_size);
    connect(pb_add, SIGNAL(clicked()), this, SLOT(slotAddQuestionAnswer()));

    pb_delete = new QPushButton("Удалить");
    pb_delete->setMinimumSize(pb_size);
    connect(pb_delete, SIGNAL(clicked()), this, SLOT(slotDeleteQuestionAnswer()));

    pb_home->setMinimumSize(pb_size);

    QHBoxLayout *hbx_btn_layout = new QHBoxLayout;
    hbx_btn_layout->addWidget(pb_add);
    hbx_btn_layout->addWidget(pb_delete);
    hbx_btn_layout->addWidget(pb_home);
    hbx_btn_layout->setAlignment(Qt::AlignCenter);

    selected_QA_image = new QLabel; // ----------
    selected_QA_image->setMinimumWidth(320);
    selected_QA_image->setAlignment(Qt::AlignCenter);

    QVBoxLayout *vbx_layout = new QVBoxLayout;
    vbx_layout->addWidget(tree_view);
    vbx_layout->addLayout(hbx_btn_layout);

    QHBoxLayout *hbx_wgt_layout = new QHBoxLayout(this);
    hbx_wgt_layout->addLayout(vbx_layout);
    // hbx_wgt_layout->addWidget(table_proxy_view);
    // hbx_wgt_layout->addWidget(table_view);
    hbx_wgt_layout->addWidget(selected_QA_image);
}

QuestionWindow::~QuestionWindow()
{
    qDebug() << "Destructor QuestionWindow - " << this;
}

void QuestionWindow::initView()
{
    table_model = new SqlTableModel;
    QString tablename = "user_" + QString::number(table_id);
    table_model->setTable(tablename);
    table_model->select();

    // filter_model = new FilterProxyModel(user);
    // filter_model->setSourceModel(table_model);

    tree_model = new SqlTreeModel(user, table_id);
    tree_model->setSourceModel(table_model);

    tree_view = new QTreeView;
    tree_view->setModel(tree_model);
    tree_view->setGeometry(0, 0, 500, 500);
    tree_view->setColumnWidth(1, 200);
    tree_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    tree_view->setSelectionMode(QAbstractItemView::SingleSelection);
    tree_view->expandAll();
    //tree_view->hideColumn(0);
    tree_view->hideColumn(2);
    tree_view->hideColumn(3);
    tree_view->setAlternatingRowColors(true);
    connect(tree_view, SIGNAL(clicked(QModelIndex)), this, SLOT(slotTreeViewClicked(QModelIndex)));

    // table_proxy_view = new QTableView;
    // table_proxy_view->setModel(filter_model);

    // table_view = new QTableView;
    // table_view->setModel(table_model);
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
        QByteArray *p_byte_array = nullptr;
        if (!dialog->getImagePath().isEmpty()) {
            QByteArray byte_array;
            QBuffer in_buffer(&byte_array);
            in_buffer.open(QIODevice::WriteOnly);
            QPixmap image(dialog->getImagePath());
            image.save(&in_buffer, "PNG");
            p_byte_array = new QByteArray(byte_array);

        }
        QModelIndex current_index = tree_view->currentIndex();
        //QModelIndex tree_index = tree_model->mapFromSource(current_index);
        tree_model->addRow(current_index, qa_text, p_byte_array);
        table_model->select();
        tree_view->expandAll();
        tree_view->setCurrentIndex(current_index);
        if(p_byte_array) {
            delete p_byte_array;
        }
    }
    delete dialog;
}


void QuestionWindow::slotDeleteQuestionAnswer()
{
    QModelIndex current_index = tree_view->currentIndex();
    //QModelIndex tree_index = tree_model->mapFromSource(current_index);
    tree_model->deleteQA(current_index);
    table_model->select();
    tree_view->expandAll();
}

void QuestionWindow::slotTreeViewClicked(QModelIndex selected_index)
{
    //QModelIndex tree_index = tree_model->mapFromSource(selected_index);
    int id = selected_index.internalId();
    QSqlQuery *query = new QSqlQuery(db);
    query->prepare("SELECT * FROM user_" + QString::number(table_id) + " WHERE id=?");
    query->addBindValue(id);
    if(query->exec()) {
        query->first();
        QByteArray image = query->value("image").toByteArray();
        if (!image.isEmpty()) {
            QPixmap pix;
            pix.loadFromData(image);
            pix = pix.scaledToWidth(300, Qt::SmoothTransformation);
            selected_QA_image->setPixmap(pix);
        } else {
            QString text = query->value("question").toString();
            selected_QA_image->clear();
            selected_QA_image->setText(text);
        }
    }
}
