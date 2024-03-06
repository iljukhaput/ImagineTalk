#ifndef ACCEPTDIALOG_H
#define ACCEPTDIALOG_H

#include <QDialog>

class AcceptDialog : public QDialog
{
    Q_OBJECT
public:
    AcceptDialog(QString &text, QWidget *parent = nullptr);
};

#endif // ACCEPTDIALOG_H
