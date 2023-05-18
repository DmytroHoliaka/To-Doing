#include "mainwindow.h"


int main(int argc, char *argv[]) {

    QApplication app(argc, argv);
    Tray tray;
    MainWindow w(tray);
    w.show();

    return app.exec();
}




