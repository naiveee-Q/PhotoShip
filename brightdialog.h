#ifndef BRIGHTDIALOG_H
#define BRIGHTDIALOG_H

#include <QDialog>

namespace Ui {
class BrightDialog;
}

class BrightDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BrightDialog(QWidget *parent = 0);
    ~BrightDialog();

private slots:
    void on_brightInput_textEdited(const QString &arg1);

    void on_brightSlider_valueChanged(int value);

    void on_confBriBtn_clicked();

signals:
    void sendBriData(int);
private:
    Ui::BrightDialog *ui;
};

#endif // BRIGHTDIALOG_H
