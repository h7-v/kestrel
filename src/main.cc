#include "src/kestrel.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Kestrel w;
    w.show();
    return a.exec();
}
