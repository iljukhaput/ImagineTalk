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
#include <QKeyEvent>
#include <QShortcut>
#include <QApplication>
#include <QMimeData>
#include <QClipboard>


AddQADialog::AddQADialog(QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f)
    , image_byte_array(nullptr)
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

    label = new QLabel("Чтобы добавить картинку\nнажмите на этот текст,\nа затем Ctrl+V\n\nили на кнопку\n\"Добавить изображение\"");
    label->setMinimumSize(200, 200);
    label->setAlignment(Qt::AlignCenter);
    QHBoxLayout *hbx_layout = new QHBoxLayout(this);
    hbx_layout->addWidget(gbx_vbx);
    hbx_layout->addWidget(label);

    QShortcut *sc = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_V), this);
    connect(sc, SIGNAL(activated()), this, SLOT(slotCtrlV()));

    setFocusPolicy(Qt::ClickFocus);
}


void AddQADialog::slotAddImage()
{
    QStringList desktop_path = QStandardPaths::standardLocations(QStandardPaths::DesktopLocation);
    image_path = QFileDialog::getOpenFileName(this, "Открыть изображение", desktop_path.first(), "Image Files (*.jpg *.png *.bmp *.jpeg)");
    if (image_path.isEmpty())
        return;

    QPixmap pix(image_path);
    if (saveToBuffer(pix))
        setLabel(pix);
}


void AddQADialog::slotCtrlV()
{
    qDebug() << "Ctrl+V pressed!";
    const QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();
    if (mimeData->hasImage()) {
        QPixmap pix = qvariant_cast<QPixmap>(mimeData->imageData());
        if (saveToBuffer(pix))
            setLabel(pix);
    }
}


bool AddQADialog::saveToBuffer(QPixmap &pix)
{
    QByteArray in_byte_array;
    QBuffer in_buffer(&in_byte_array);
    in_buffer.open(QIODevice::WriteOnly);
    if (pix.save(&in_buffer, "PNG")) {
        image_byte_array = new QByteArray(in_byte_array);
        return true;
    }
    return false;
}


void AddQADialog::setLabel(QPixmap &pix)
{
    if (pix.height() > pix.width()) {
        pix = pix.scaledToHeight(200, Qt::SmoothTransformation);
    } else {
        pix = pix.scaledToWidth(200, Qt::SmoothTransformation);
    }
    label->setPixmap(pix);
}


QString AddQADialog::getImagePath()
{
    return image_path;
}


QString AddQADialog::getQA()
{
    return line->text();
}


QByteArray *AddQADialog::getImageByteArray()
{
    return image_byte_array;
}

AddQADialog::~AddQADialog()
{
    if (image_byte_array)
        delete image_byte_array;
}
