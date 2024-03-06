#include "acceptdialog.h"
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

AcceptDialog::AcceptDialog(QString &text, QWidget *parent)
    : QDialog(parent)
{
    QLabel *lbl = new QLabel(text);

    QPushButton *pb_ok = new QPushButton("Ок");
    connect(pb_ok, SIGNAL(clicked()), this, SLOT(accept()));

    QPushButton *pb_cancel = new QPushButton("Отмена");
    connect(pb_cancel, SIGNAL(clicked()), this, SLOT(reject()));

    QHBoxLayout *hbx_layout = new QHBoxLayout;
    hbx_layout->addWidget(pb_ok);
    hbx_layout->addWidget(pb_cancel);

    QVBoxLayout *vbx_layout = new QVBoxLayout(this);
    vbx_layout->addWidget(lbl);
    vbx_layout->addLayout(hbx_layout);
}
