#include "mainwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    std::ios_base::sync_with_stdio(false);
    QApplication a(argc, argv);

    MainWindow w;
    w.showFullScreen();

    return a.exec();
}
