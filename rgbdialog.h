#ifndef RGBDIALOG_H
#define RGBDIALOG_H

#include <QDialog>
//#include <photoship.h>

namespace Ui {
class RgbDialog;
}

class RgbDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RgbDialog(QWidget *parent = 0);
    ~RgbDialog();

private slots:
    void on_confRgbBtn_clicked();

    void on_redInput_textEdited(const QString &arg1);

    void on_redSlider_valueChanged(int value);

    void on_greenInput_textEdited(const QString &arg1);

    void on_greenSlider_valueChanged(int value);

    void on_blueInput_textEdited(const QString &arg1);

    void on_blueSlider_valueChanged(int value);

signals:
    void sendData(QList<int>);

private:
    Ui::RgbDialog *ui;
    QImage org_img;
    void initialRgb();
    //QImage changed_img;
};

#endif // RGBDIALOG_H
