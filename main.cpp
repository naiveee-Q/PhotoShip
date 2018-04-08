#include "photoship.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PhotoShip w;
    w.show();

    return a.exec();
}
