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
    QLabel *line_header = new QLabel("Текст вопроса или ответа:");
    line = new QLineEdit;
    line->setClearButtonEnabled(true);

    QPushButton *pb_ok = new QPushButton("Ок");
    pb_ok->setDefault(true);
    connect(pb_ok, SIGNAL(clicked()), this, SLOT(accept()));

    QPushButton *pb_cancel = new QPushButton("Отмена");
    connect(pb_cancel, SIGNAL(clicked()), this, SLOT(reject()));

    QPushButton *pb_add_image = new QPushButton("Добавить изображение");
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

    label = new QLabel;
    label->setMinimumSize(200, 200);
    label->setAlignment(Qt::AlignCenter);
    QHBoxLayout *hbx_layout = new QHBoxLayout(this);
    hbx_layout->addWidget(gbx_vbx);
    hbx_layout->addWidget(label);
}


void AddQADialog::slotAddImage()
{
    QStringList desktop_path = QStandardPaths::standardLocations(QStandardPaths::DesktopLocation);
    image_path = QFileDialog::getOpenFileName(this, "Открыть изображение", desktop_path.first(), "Image Files (*.jpg *.png *.bmp *.jpeg)");
    if (image_path.isEmpty())
        return;

    QPixmap image(image_path);
    image = image.scaledToWidth(200);
    if (image.height() > image.width()) {
        image = image.scaledToHeight(200, Qt::SmoothTransformation);
    } else {
        image = image.scaledToWidth(200, Qt::SmoothTransformation);
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

