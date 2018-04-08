#ifndef SATURDIALOG_H
#define SATURDIALOG_H

#include <QDialog>

namespace Ui {
class SaturDialog;
}

class SaturDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SaturDialog(QWidget *parent = 0);
    ~SaturDialog();

private slots:
    void on_saturInput_textEdited(const QString &arg1);

    void on_saturSlider_valueChanged(int value);

    void on_confSaturBtn_clicked();

signals:
    void sendSaturData(int);
private:
    Ui::SaturDialog *ui;
};

#endif // SATURDIALOG_H
