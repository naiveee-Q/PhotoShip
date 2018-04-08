#include "saturdialog.h"
#include "ui_saturdialog.h"

SaturDialog::SaturDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaturDialog)
{
    ui->setupUi(this);

    ui->saturInput->setText("0");
    ui->saturSlider->setValue(0);
    ui->saturSlider->setRange(-100,100);
}

SaturDialog::~SaturDialog()
{
    delete ui;
}

void SaturDialog::on_saturInput_textEdited(const QString &arg1)
{
    if(arg1.toInt() < -100 || arg1.toInt() > 100){
        return;
    }
    ui->saturSlider->setValue(arg1.toInt());
}

void SaturDialog::on_saturSlider_valueChanged(int value)
{
    ui->saturInput->setText(QString("%1").arg(value));
}

void SaturDialog::on_confSaturBtn_clicked()
{
    emit sendSaturData(ui->saturInput->text().toInt());
}
