#include "photoshop.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Photoshop w;

    w.show();
    return a.exec();
}
