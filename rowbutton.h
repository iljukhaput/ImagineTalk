#ifndef ROWBUTTON_H
#define ROWBUTTON_H

#include <QPushButton>
#include <QWidget>


class ExpandButton : public QPushButton
{
    Q_OBJECT
public:
    ExpandButton(QWidget *parent = nullptr);
};


class RowButton : public QPushButton
{
    Q_OBJECT
    int sql_id;
    QString question;
    bool expanded;
    int level;
    int table_id;
    RowButton* parent_btn;
    ExpandButton *expand_btn;
    QList<RowButton*> children_list;
    QString createPrefix(int level);
public:
    RowButton(int id, int table_id, const QString &text, RowButton* parent_btn, QWidget *parent = nullptr);
    bool isExpanded() const { return expanded; }
    int getId() const { return sql_id; }
    QString getQuestion() const { return question; }
    int getLevel() const { return level; }
    ExpandButton *getExpandBtn() const { return expand_btn; }
    QList<RowButton*> getChildren() const { return children_list; }
    RowButton *getParentBtn() const { return parent_btn; }
    void setExpanded(bool exp);
    void addChild(RowButton *child_btn) { children_list.append(child_btn); }
    void removeChildren() { children_list.clear(); }
    bool removeChild(RowButton* rb) { return children_list.removeOne(rb); }
    bool hasChildren() const;
    void createExpandBtn();
    void deleteExpandBtn();
    // void addExpandBtn();
};

#endif // ROWBUTTON_H
