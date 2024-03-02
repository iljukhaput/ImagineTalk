#include "addqadialog.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QStandardPaths>
#include <QBuffer>
#include <QRadioButton>
#include <QGroupBox>
#include <QSplitter>
#include <QDebug>


AddQADialog::AddQADialog(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
    /*
    QRadioButton *rb_question = new QRadioButton("Question");
    QRadioButton *rb_answer = new QRadioButton("Answer");
    QVBoxLayout *vbx_rb = new QVBoxLayout;
    vbx_rb->addWidget(rb_question);
    vbx_rb->addWidget(rb_answer);
    QGroupBox *gbx_rb = new QGroupBox;
    gbx_rb->setLayout(vbx_rb);
    rb_answer->setChecked(true);
    */

    QLabel *line_header = new QLabel("Question or answer:");
    line = new QLineEdit;
    line->setClearButtonEnabled(true);

    QPushButton *pb_ok = new QPushButton("Ok");
    pb_ok->setDefault(true);
    connect(pb_ok, SIGNAL(clicked()), this, SLOT(accept()));

    QPushButton *pb_cancel = new QPushButton("Cancel");
    connect(pb_cancel, SIGNAL(clicked()), this, SLOT(reject()));

    QPushButton *pb_add_image = new QPushButton("Add image");
    connect(pb_add_image, SIGNAL(clicked()), this, SLOT(slotAddImage()));

    QHBoxLayout *hbx_layuot = new QHBoxLayout;
    hbx_layuot->addWidget(pb_ok);
    hbx_layuot->addWidget(pb_cancel);

    QVBoxLayout *vbx_layuot = new QVBoxLayout;
    vbx_layuot->addStretch(1);
    // vbx_layuot->addWidget(gbx_rb);
    vbx_layuot->addWidget(line_header);
    vbx_layuot->addWidget(line);
    vbx_layuot->addWidget(pb_add_image);
    vbx_layuot->addLayout(hbx_layuot);
    QGroupBox *gbx_vbx = new QGroupBox;
    gbx_vbx->setLayout(vbx_layuot);

    label = new QLabel("no image");
    label->setMinimumSize(200, 200);
    label->setAlignment(Qt::AlignCenter);
    QHBoxLayout *splitter = new QHBoxLayout(this);
    splitter->addWidget(gbx_vbx);
    splitter->addWidget(label);
}


void AddQADialog::slotAddImage()
{
    QStringList desktop_path = QStandardPaths::standardLocations(QStandardPaths::DesktopLocation);
    image_path = QFileDialog::getOpenFileName(this, "Open Image", desktop_path.first(), "Image Files (*.jpg *.png *.bmp *.jpeg)");
    if (image_path.isEmpty())
        return;

    QPixmap image(image_path);
    image = image.scaledToWidth(200);
    if (image.height() > image.width()) {
        image = image.scaledToHeight(200);
    } else {
        image = image.scaledToWidth(200);
    }
    label->setPixmap(image);
    //label->setScaledContents(true);
}

QString AddQADialog::getImagePath()
{
    return image_path;
}


QString AddQADialog::getQA()
{
    return line->text();
}

