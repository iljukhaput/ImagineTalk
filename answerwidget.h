#ifndef ANSWER_H
#define ANSWER_H

#include <QPushButton>

class AnswerWidget : public QWidget
{
    Q_OBJECT

    QPushButton *btn;
    void emitClicked();
public:
    AnswerWidget(const QString &text, int scale = 1, const QByteArray *image = nullptr, QWidget *parent = nullptr);
signals:
    void clicked();
};

#endif // ANSWER_H
