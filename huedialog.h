#ifndef HUEDIALOG_H
#define HUEDIALOG_H

#include <QDialog>

namespace Ui {
class HueDialog;
}

class HueDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HueDialog(QWidget *parent = 0);
    ~HueDialog();

private slots:
    void on_hueInput_textEdited(const QString &arg1);

    void on_hueSlider_valueChanged(int value);

    void on_confHueBtn_clicked();

private:
    Ui::HueDialog *ui;
signals:
    void sendHueData(int);
};

#endif // HUEDIALOG_H
