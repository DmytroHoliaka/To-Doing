#ifndef APP_H
#define APP_H

#include <QApplication>

class Application : public QApplication
{
public:
    Application(int &argc, char **argv) : QApplication(argc, argv) {}
protected:
    bool notify(QObject* receiver, QEvent *event) override;
};

#endif // APP_H
