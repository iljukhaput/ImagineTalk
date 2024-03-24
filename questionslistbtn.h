#ifndef QUESTIONSLISTBTN_H
#define QUESTIONSLISTBTN_H

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QHash>
#include <QLabel>
#include <QSqlQuery>


class RowButton;
class QPushButton;
class QSignalMapper;


/*
 * При создании списка отображаются элементы с parentId == 0 - реализовано как развертывание индекса 0
 *
 * Хэш видимых строк - нужен для определения объекта класса при клике на строку
 * Список видимых строк-потомков - нужен для сворачивания строки
 *
 */


class QuestionsListBtn : public QScrollArea
{
    Q_OBJECT

    QString table_name;
    int table_id;
    QLabel *lbl;

    QString db_name;
    int current_row;

    QVBoxLayout *vbx_layout;
    QWidget *scroll_widget;
    QHash<int, RowButton*> hash_visible_btns;

    void Expand(int sql_id);
    void Collapse(int sql_id);
    void highlightCurrent(int sql_id);
    void setImage(int sql_id);
    void deleteWidgetRows(int sql_id);
    void deleteDBRecords(int sql_id);
    void deleteOneRow(int sql_id);
    void updateList();
    bool hasChildrenSql(int sql_id) const;
    // int rowCountSql(int sql_id) const;
    int insertRowSql(int parent_id, const QString &question, const QByteArray *image);
    RowButton *createChildRowButton(int id_sql, const QString &text, RowButton *parent_btn);
    QSignalMapper *connectMapBtnId(QPushButton *btn, int id);
    RowButton *addExpandBtn(int sql_id);
public:
    QuestionsListBtn(QString table_name, int table_id, QLabel *lbl, QWidget *parent = nullptr);
    int getCurrent() const { return current_row; }
    void addRow(const QString &question, const QByteArray *image);
private slots:
    void slotClickExpand(int id);
    void slotClickRow(int id);
public slots:
    void slotDelete();
};

#endif // QUESTIONSLISTBTN_H
