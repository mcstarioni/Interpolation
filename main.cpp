#include <QApplication>
#include "interpolation.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Interpolation i;
    i.resize(300,400);
    i.show();
    return a.exec();
}
