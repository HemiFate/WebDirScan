#include "webdirscan.h"

#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    WebDirScan w;
    w.show();
    return a.exec();
}
