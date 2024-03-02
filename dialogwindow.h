#ifndef DIALOGWINDOW_H
#define DIALOGWINDOW_H

#include "secondlayoutwindow.h"
#include "answerwidget.h"
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QSignalMapper>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QVector>


class DialogWindow : public SecondLayoutWindow
{
    Q_OBJECT
    QStackedWidget *stacked_widget_dialog;
    int scale;
    QVector<int> backpath;

    void connectPbQuestion(AnswerWidget *pb, int id);
    int rowCount(QSqlQuery *query);
    QScrollArea *createQAWidget(int index);
    void clearStackedWidget();
public:
    explicit DialogWindow(QString &user, QWidget *parent = nullptr);

private slots:
    void slotShowAnswers(int id);
    void slotGoToPreviousAnswer();
    void slotClearStackedWidget();
    void slotIncreaseImages();
    void slotDecreaseImages();
public slots:
    void slotShowInitialQuestion();
};

#endif // DIALOGWINDOW_H
