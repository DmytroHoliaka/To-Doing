#include "header.h"
#include "app.h"

int main(int argc, char *argv[])
{
    Application app(argc, argv);
    MainWindow w;
    w.show();

    return app.exec();
}

