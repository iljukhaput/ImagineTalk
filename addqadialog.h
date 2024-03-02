#ifndef ADDQADIALOG_H
#define ADDQADIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>

class AddQADialog : public QDialog
{
    Q_OBJECT
    QLabel *label;
    QLineEdit *line;
    QString image_path;
public:
    AddQADialog(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    QString getImagePath();
    QString getQA();
private slots:
    void slotAddImage();
};

#endif // ADDQADIALOG_H
