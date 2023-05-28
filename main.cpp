#include "mainwindow.h"

// Todo: QAbstractItemView::closeEditor called with an editor that does not belong to this view
// Todo: Перевірити, щоб з бази не можна було дістати пусту справу - інакше краш

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow w;
    w.show();

    return app.exec();
}




