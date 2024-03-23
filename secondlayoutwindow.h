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
    int table_id;
    QPushButton *pb_home;
public:
    // static bool initDB();
    explicit SecondLayoutWindow(int table_id, QWidget *parent = nullptr);
    virtual ~SecondLayoutWindow() {}
    void ConnectPbHome(QSignalMapper &sig_map, QStackedWidget &stack_wgt, QWidget &wgt_home) const;
};

#endif // SECONDLAYOUTWINDOW_H
