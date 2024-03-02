#ifndef SECONDLAYOUTWINDOW_H
#define SECONDLAYOUTWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QSignalMapper>
#include <QStackedWidget>
#include <QSqlDatabase>

class SecondLayoutWindow : public QWidget
{
    Q_OBJECT
protected:
    QString user;
    QPushButton *pb_home;
public:
    static QSqlDatabase db;
    static bool initDB();
    explicit SecondLayoutWindow(QString &user, QWidget *parent = nullptr);
    virtual ~SecondLayoutWindow();
    void ConnectPbHome(QSignalMapper &sig_map, QStackedWidget &stack_wgt, QWidget &wgt_home) const;
};

#endif // SECONDLAYOUTWINDOW_H
