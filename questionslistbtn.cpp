#include "questionslistbtn.h"
#include "rowbutton.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QVariant>
#include <QPushButton>
#include <QSignalMapper>
#include <QDebug>
#include <QScrollBar>
#include <QGuiApplication>


QuestionsListBtn::QuestionsListBtn(QString table_name, int table_id, QLabel *lbl, QWidget *parent)
    : QScrollArea(parent), table_name(table_name), table_id(table_id), lbl(lbl), current_row(0)
{
    setWidgetResizable(true);
    vbx_layout = new QVBoxLayout;
    vbx_layout->setAlignment(Qt::AlignTop);
    vbx_layout->setSpacing(0);


    if (table_name == "user_")
        db_name = "ImagineTalkDB.db";
    else
        db_name = "GeneralQuestionsDB.db";

    Expand(0);
    setStyleSheet("RowButton:hover { background: lightgrey; }"
                  "RowButton { text-align: left; border: none; }");
    updateList();
}


void QuestionsListBtn::highlightCurrent(int sql_id)
{
    if (current_row > 0 && hash_visible_btns.value(current_row, 0)) {
        hash_visible_btns[current_row]->setStyleSheet("RowButton:hover { background: lightgrey; }"
                                         "RowButton { background: none; }");
    }
    hash_visible_btns[sql_id]->setStyleSheet("background: gray;");
}


void QuestionsListBtn::Expand(int sql_id)
{
    {
        int row_id = -1;
        RowButton *current_btn = nullptr;
        if (!hash_visible_btns.isEmpty()) {
            current_btn = hash_visible_btns[sql_id];
            row_id = vbx_layout->indexOf(current_btn);
        }

        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "connection_name");
        db.setDatabaseName(db_name);
        db.open();

        QSqlQuery query(db);
        query.prepare("SELECT * FROM " + table_name + QString::number(table_id) + " WHERE parentId=?");
        query.addBindValue(sql_id);
        query.exec();
        while (query.next()) {
            QString question = query.value("question").toString();
            int child_sql_id = query.value("id").toInt();
            RowButton *child_rb = createChildRowButton(child_sql_id, question, current_btn);
            if (child_rb->hasChildren()) {
                child_rb->createExpandBtn();
                QSignalMapper *signal_mapper_expand = connectMapBtnId(child_rb->getExpandBtn(), child_sql_id);
                connect(signal_mapper_expand, SIGNAL (mapped(int)), this, SLOT(slotClickExpand(int)));
            }
            vbx_layout->insertWidget(row_id+1, child_rb);
        }
        query.finish();
        db.close();
    }
    QSqlDatabase::removeDatabase("connection_name");
}

/*
 * При добавлении новой строки:
 *      1. Определить текущую строку
 *      2. В БД добавить новую запись
 *      3. Добавить на QVBoxLayout новую строку:
 *          - создать RowButton
 *          - добавить её в хэш видимых строк
 *          - определить положение предка и вставить под предка (parent_id+1)
 *      4. Если у предка не было expand_btn, то добавить
 *          чтобы она отобразилась, нужно:
 *          -
 *      5. Сделать предка expanded
 */
void QuestionsListBtn::addRow(const QString &question, const QByteArray *image)
{
    int sql_id = getCurrent();
    int row_id = -1;
    RowButton *current_btn = nullptr;
    if (sql_id) {
        current_btn = hash_visible_btns[sql_id];
        if (!current_btn) {
            return;
        }
        if(!current_btn->isExpanded()) {
            Expand(sql_id);
            current_btn->setExpanded(true);
        }

        if (!current_btn->getExpandBtn()) {
            current_btn = addExpandBtn(sql_id);
        }
        row_id = vbx_layout->indexOf(current_btn);
    }
    int child_sql_id = insertRowSql(sql_id, question, image);
    RowButton *child_rb = createChildRowButton(child_sql_id, question, current_btn);
    vbx_layout->insertWidget(row_id+1, child_rb);
}


RowButton *QuestionsListBtn::createChildRowButton(int id_sql, const QString &text, RowButton *parent_btn)
{
    QString full_table_name = table_name + QString::number(table_id);
    RowButton *child_rb = new RowButton(id_sql, full_table_name, text, parent_btn);
    hash_visible_btns[id_sql] = child_rb;

    QSignalMapper *signal_mapper_row = connectMapBtnId(child_rb, id_sql);
    connect(signal_mapper_row, SIGNAL (mapped(int)), this, SLOT(slotClickRow(int)));

    if (parent_btn) {
        parent_btn->addChild(child_rb);
    }
    return child_rb;
}


QSignalMapper *QuestionsListBtn::connectMapBtnId(QPushButton *btn, int id)
{
    QSignalMapper *signal_mapper = new QSignalMapper(this);
    signal_mapper->setMapping(btn, id);
    connect(btn, SIGNAL (clicked()), signal_mapper, SLOT (map()));
    return signal_mapper;
}

void QuestionsListBtn::updateList()
{
    int scroll_val = verticalScrollBar()->value();

    scroll_widget = new QWidget;
    scroll_widget->setLayout(vbx_layout);
    setWidget(scroll_widget);

    verticalScrollBar()->setValue(scroll_val);
}


void QuestionsListBtn::Collapse(int sql_id)
{
    RowButton *current_btn = hash_visible_btns[sql_id];
    QList<RowButton*> children_list = current_btn->getChildren();
    for (RowButton *rb: children_list) {
        int child_sql_id = rb->getId();
        if(rb->isExpanded()) {
            Collapse(child_sql_id);
        }
        hash_visible_btns.remove(child_sql_id);
        vbx_layout->removeWidget(rb);
    }
    current_btn->removeChildren();
}


void QuestionsListBtn::slotClickExpand(int sql_id)
{
    RowButton *current_btn = hash_visible_btns[sql_id];
    if(current_btn->isExpanded()) {
        Collapse(sql_id);
        current_btn->setExpanded(false);
    } else {
        QGuiApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        Expand(sql_id);
        current_btn->setExpanded(true);
        QGuiApplication::restoreOverrideCursor();
    }
    qDebug() << hash_visible_btns.keys();
    updateList();
}


void QuestionsListBtn::slotClickRow(int sql_id)
{
    highlightCurrent(sql_id);
    current_row = sql_id;
    setImage(sql_id);
}


void QuestionsListBtn::setImage(int sql_id)
{
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "connection_name");
        db.setDatabaseName(db_name);
        db.open();

        QSqlQuery query(db);
        query.prepare("SELECT * FROM " + table_name + QString::number(table_id) + " WHERE id=?");
        query.addBindValue(sql_id);
        if(query.exec()) {
            query.first();
            QByteArray image = query.value("image").toByteArray();
            if (!image.isEmpty()) {
                QPixmap pix;
                pix.loadFromData(image);
                pix = pix.scaledToWidth(300, Qt::SmoothTransformation);
                lbl->setPixmap(pix);
            } else {
                QString text = query.value("question").toString();
                lbl->clear();
                lbl->setText(text);
            }
        }
        query.finish();
        db.close();
    }
    QSqlDatabase::removeDatabase("connection_name");
}


void QuestionsListBtn::slotDelete()
{
    int id = getCurrent();

    RowButton *parent_btn = hash_visible_btns[id]->getParentBtn();

    deleteWidgetRows(id);
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "connection_name");
        db.setDatabaseName(db_name);
        if (db.open()) {
            deleteDBRecords(id);
            db.close();
        }
    }
    QSqlDatabase::removeDatabase("connection_name");

    updateList();
    current_row = 0;
    lbl->clear();

    if (parent_btn) {
        if (parent_btn->getChildren().isEmpty())
            parent_btn->deleteExpandBtn();
    }

    hash_visible_btns.remove(id);
}


RowButton *QuestionsListBtn::addExpandBtn(int sql_id)
{
    if (!sql_id)
        return nullptr;

    RowButton *current_btn = hash_visible_btns[sql_id];
    int id = vbx_layout->indexOf(current_btn);
    vbx_layout->removeWidget(current_btn);

    QString text = current_btn->getQuestion();
    RowButton *parent_btn = current_btn->getParentBtn();

    // удалить объект RowButton из списка потомков у предка
    // из хэша видимых строк
    // и из кучи
    if(parent_btn)
        parent_btn->removeChild(current_btn);
    delete current_btn;
    hash_visible_btns.remove(sql_id);

    current_btn = createChildRowButton(sql_id, text, parent_btn);
    current_btn->setExpanded(true);
    current_btn->createExpandBtn();

    QSignalMapper *signal_mapper_expand = connectMapBtnId(current_btn->getExpandBtn(), sql_id);
    connect(signal_mapper_expand, SIGNAL (mapped(int)), this, SLOT(slotClickExpand(int)));

    vbx_layout->insertWidget(id, current_btn);

    return current_btn;
}


void QuestionsListBtn::deleteWidgetRows(int sql_id)
{
    Collapse(sql_id);
    RowButton *current_btn = hash_visible_btns[sql_id];
    RowButton *parent = current_btn->getParentBtn();
    if(parent) {
        parent->removeChild(current_btn);
    }
    vbx_layout->removeWidget(current_btn);
}


/*
 * Так как функция рекурсивная, подключение к базе происходит перед вызовом этой функции
 */
void QuestionsListBtn::deleteDBRecords(int sql_id)
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery q(db);
    q.prepare("SELECT id FROM " + table_name + QString::number(table_id) + " WHERE parentId=?");
    q.addBindValue(sql_id);
    if(!q.exec()) {
        qDebug() << "Error in QuestionsListBtn::deleteDBRecords:" << q.lastError().text();
    }
    while (q.next()) {
        int child_id = q.value("id").toInt();
        deleteDBRecords(child_id);
    }
    deleteOneRow(sql_id);
    q.finish();
}


void QuestionsListBtn::deleteOneRow(int sql_id)
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery q(db);
    q.prepare("DELETE FROM " + table_name + QString::number(table_id) + " WHERE id=?");
    q.addBindValue(sql_id);
    if(!q.exec()) {
        qDebug() << "Error in QuestionsListBtn::deleteOneRow:" << q.lastError().text();
    }
    q.finish();
}


int QuestionsListBtn::insertRowSql(int parent_id, const QString &question, const QByteArray *image)
{
    int row_id = -1;
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "connection_name");
        db.setDatabaseName(db_name);
        db.open();

        QSqlQuery query(db);
        if(image) {
            query.prepare("INSERT INTO " + table_name + QString::number(table_id) + " (question, image, parentId)"
                                                                            "VALUES (?, ?, ?)");
            query.addBindValue(question);
            query.addBindValue(*image);
            query.addBindValue(parent_id);
        } else {
            query.prepare("INSERT INTO " + table_name + QString::number(table_id) + " (question, parentId)"
                                                                            "VALUES (?, ?)");
            query.addBindValue(question);
            query.addBindValue(parent_id);
        }
        if(!query.exec())
            qDebug() << "Error in QuestionsListBtn::insertRowSql:" << query.lastError().text();


        query.prepare("SELECT id FROM " + table_name + QString::number(table_id) + " WHERE question=? AND parentId=?");
        query.addBindValue(question);
        query.addBindValue(parent_id);
        if(!query.exec()) {
            qDebug() << "Error in QuestionsListBtn::insertRowSql:" << query.lastError().text();
        } else {
            query.last();
            row_id = query.value("id").toInt();
        }
        query.finish();
        db.close();
    }
    QSqlDatabase::removeDatabase("connection_name");
    return row_id;
}
