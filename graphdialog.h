#ifndef GRAPHDIALOG_H
#define GRAPHDIALOG_H

#include <QDialog>
#include "paintedwidget.h"

namespace Ui {
class GraphDialog;
}

class GraphDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GraphDialog(QWidget *parent = 0,QImage *i = new QImage);
    ~GraphDialog();    
private:
    Ui::GraphDialog *ui;
    QImage *image;
protected:
    void paintEvent(QPaintEvent *e);
};

#endif // GRAPHDIALOG_H
