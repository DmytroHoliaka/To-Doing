#include "mainwindow.h"

// Todo: QAbstractItemView::closeEditor called with an editor that does not belong to this view

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);

    MainWindow w;
    w.show();

    return app.exec();
}




