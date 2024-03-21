#include "rowbutton.h"
#include <QStyle>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

RowButton::RowButton(int sql_id, int table_id, const QString &text, RowButton* parent_btn, QWidget *parent)
    : QPushButton(parent)
    , sql_id(sql_id)
    , question(text)
    , expanded(false)
    , table_id(table_id)
    , parent_btn(parent_btn)
    , expand_btn(nullptr)
{
    level = parent_btn ? parent_btn->getLevel() + 1 : 0;

    QFontMetrics fm(font());
    int height = fm.height();
    setFixedHeight(height);

    QString level_prefix = createPrefix(level);
    setText(level_prefix + "|    " + text);
}

QString RowButton::createPrefix(int level)
{
    QString prefix;
    for (int i = 0; i < level; ++i) {
        prefix.append("    ");
    }
    return prefix;
}


ExpandButton::ExpandButton(QWidget *parent)
    : QPushButton(parent)
{
    setStyleSheet("ExpandButton:hover { background: darkgrey; }"
                  "ExpandButton { border: none; }");
}


void RowButton::createExpandBtn()
{
    QFontMetrics fm(font());
    QString level_prefix = createPrefix(level);
    int ax_expand_btn = fm.horizontalAdvance(level_prefix + " ");
    int height = fm.height();
    expand_btn = new ExpandButton(this);
    expand_btn->setText(isExpanded() ? " - " : " + ");
    int aw_expand_btn = fm.horizontalAdvance(" + ");
    expand_btn->setGeometry(ax_expand_btn, 0, aw_expand_btn, height);
}


void RowButton::setExpanded(bool exp)
{
    expanded = exp;
    if (expand_btn) {
        expand_btn->setText(exp ? " - " : " + ");
    }
}


bool RowButton::hasChildren() const
{
    bool ans = false;
    QSqlDatabase db = QSqlDatabase::database();
    db.setDatabaseName("ImagineTalkDB.db");
    db.open();

    QSqlQuery q(db);
    q.prepare("SELECT COUNT(*) FROM user_" + QString::number(table_id) + " WHERE parentId=?");
    q.addBindValue(sql_id);
    if(!q.exec()) {
        qDebug() << "Error in SqlTreeModel::hasChildren:" << q.lastError().text();
    }
    q.first();
    ans = q.value(0).toInt() > 0;
    q.finish();
    db.close();
    return ans;
}


void RowButton::deleteExpandBtn()
{
    delete expand_btn;
    expand_btn = nullptr;
}
