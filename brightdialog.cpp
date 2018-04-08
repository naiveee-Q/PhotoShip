#include "brightdialog.h"
#include "ui_brightdialog.h"

BrightDialog::BrightDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BrightDialog)
{
    ui->setupUi(this);
    ui->brightInput->setText("0");
    ui->brightSlider->setValue(0);
    ui->brightSlider->setRange(-100,100);
}

BrightDialog::~BrightDialog()
{
    delete ui;
}

void BrightDialog::on_brightInput_textEdited(const QString &arg1)
{
    if(arg1.toInt() < -100 || arg1.toInt() > 100){
        return;
    }
    ui->brightSlider->setValue(arg1.toInt());
}

void BrightDialog::on_brightSlider_valueChanged(int value)
{
    ui->brightInput->setText(QString("%1").arg(value));
}

void BrightDialog::on_confBriBtn_clicked()
{
    emit sendBriData(ui->brightInput->text().toInt());
}
