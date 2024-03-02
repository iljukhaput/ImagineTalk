#include "answerwidget.h"
#include <QVBoxLayout>
#include <QLabel>

AnswerWidget::AnswerWidget(const QString &text, int scale, const QByteArray *image, QWidget *parent)
    : QWidget(parent)
{
    int k = 10 + scale;
    QSize icon_size(30*k, 30*k);

    btn = new QPushButton;
    btn->setMinimumSize(icon_size);
    connect(btn, SIGNAL(clicked()), this, SIGNAL(clicked()));

    QLabel *lbl_text = nullptr;
    if(!image->isEmpty()) {
        QPixmap pix;
        pix.loadFromData(*image);
        QIcon icon(pix);
        btn->setIcon(icon);
        btn->setIconSize(icon_size);
        lbl_text = new QLabel(text);
        lbl_text->setAlignment(Qt::AlignCenter);
    } else {
        btn->setText(text);
        lbl_text = new QLabel("");
        lbl_text->setAlignment(Qt::AlignCenter);
    }

    QVBoxLayout *vbx_layout = new QVBoxLayout;
    vbx_layout->addWidget(btn);
    vbx_layout->addWidget(lbl_text);
    vbx_layout->setAlignment(Qt::AlignCenter);
    this->setLayout(vbx_layout);
}
