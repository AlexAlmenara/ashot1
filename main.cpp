#include <QtGui/QApplication>
#include "ashot.h"


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    aShot w;
    w.show();

    return a.exec();
}
