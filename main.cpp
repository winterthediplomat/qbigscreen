#include "mainwindow.h"

#include <QApplication>

#define BIGSCREEN_FULLSCREEN

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
#ifdef BIGSCREEN_FULLSCREEN
    w.showMaximized();
#else
    w.show();
#endif
    return a.exec();
}
