#ifndef PAINTEDWIDGET_H
#define PAINTEDWIDGET_H
#include <QGraphicsView>

class PaintedWidget : public QGraphicsView
{
    Q_OBJECT
public:
    PaintedWidget(QWidget *parent = 0);
protected:
    void paintEvent(QPaintEvent *);
};

#endif // PAINTEDWIDGET_H
