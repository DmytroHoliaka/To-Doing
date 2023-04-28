#include "mainwindow.h"
// TODO: Розбирись із видаленням динамічних об'єктів з методів і класу
// TODO: Файл-база-даних з усією інформацією(список справ, їх стан і т.д.)
// TODO: Подумай проектуванням коду(довлення класів і т.д.)
// (+)TODO: Перевір, щоб справа не могла бути пустою

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow w;
    w.show();

    return app.exec();
}




