#include "huedialog.h"
#include "ui_huedialog.h"

HueDialog::HueDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HueDialog)
{
    ui->setupUi(this);
    ui->hueInput->setText("0");
    ui->hueSlider->setValue(0);
    ui->hueSlider->setRange(-100,100);
}

HueDialog::~HueDialog()
{
    delete ui;
}

void HueDialog::on_hueInput_textEdited(const QString &arg1)
{
    if(arg1.toInt() < -100 || arg1.toInt()>100){
        return;
    }
    ui->hueSlider->setValue(arg1.toInt());
}

void HueDialog::on_hueSlider_valueChanged(int value)
{
    ui->hueInput->setText(QString("%1").arg(value));
}

void HueDialog::on_confHueBtn_clicked()
{
    emit sendHueData(ui->hueInput->text().toInt());
}
