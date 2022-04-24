#include "ash.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Ash ash;
    ash.show();

    return a.exec();
}
