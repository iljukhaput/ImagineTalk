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
    QByteArray *image_byte_array;

    bool saveToBuffer(QPixmap &pix);
    void setLabel(QPixmap &pix);
public:
    AddQADialog(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~AddQADialog();
    QString getImagePath();
    QByteArray *getImageByteArray();
    QString getQA();
private slots:
    void slotAddImage();
    void slotCtrlV();
};

#endif // ADDQADIALOG_H
