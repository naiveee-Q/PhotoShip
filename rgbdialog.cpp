#include "rgbdialog.h"
#include "ui_rgbdialog.h"
#include "photoship.h"
#include <QPixmap>

//QImage changed_img;

RgbDialog::RgbDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RgbDialog)
{
    ui->setupUi(this);
    initialRgb();
    //org_img = *img;
    //ui->blueLabel->setPixmap(QPixmap::fromImage(*img));
    //changed_img = new QImage;
}

RgbDialog::~RgbDialog()
{
    delete ui;
}

void RgbDialog::initialRgb(){
    ui->redInput->setText("49");
    ui->redSlider->setValue(49);
    ui->redSlider->setRange(0,99);

    ui->greenInput->setText("49");
    ui->greenSlider->setValue(49);
    ui->greenSlider->setRange(0,99);

    ui->blueInput->setText("49");
    ui->blueSlider->setValue(49);
    ui->blueSlider->setRange(0,99);
}

void RgbDialog::on_confRgbBtn_clicked()
{
    QList<int> list;
    list.append(ui->redInput->text().toInt());
    list.append(ui->greenInput->text().toInt());
    list.append(ui->blueInput->text().toInt());
    emit sendData(list);
}

void RgbDialog::on_redInput_textEdited(const QString &arg1)
{
    int position = ui->redInput->text().toInt();
        if(position > 99){
            return;
        }

        ui->redSlider->setValue(position);
}

void RgbDialog::on_redSlider_valueChanged(int value)
{
    ui->redInput->setText(QString("%1").arg(value));

}

void RgbDialog::on_greenInput_textEdited(const QString &arg1)
{
    int position = ui->greenInput->text().toInt();
    if(position > 99){
        return;
    }

    ui->greenSlider->setValue(position);


}

void RgbDialog::on_greenSlider_valueChanged(int value)
{
    ui->greenInput->setText(QString("%1").arg(value));

}

void RgbDialog::on_blueInput_textEdited(const QString &arg1)
{
    int position = ui->blueInput->text().toInt();
    if(position > 99){
        return;
    }

    ui->blueSlider->setValue(position);

}

void RgbDialog::on_blueSlider_valueChanged(int value)
{
    ui->blueInput->setText(QString("%1").arg(value));
}
