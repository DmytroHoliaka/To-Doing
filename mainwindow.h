#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QDate>
#include <QMainWindow>
#include <QListWidget>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QPainter>
#include <QLineEdit>
#include <QMessageBox>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QThread>
#include <QCloseEvent>
#include <QFile>
#include <QMessageBox>
#include <QFileInfo>
#include <QDir>
#include <QCoreApplication>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Tray;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    QString get_date();
    void customize_list_font(QString, int, int);
    void getTasksFromFile();
    void putTasksIntoFile();
    ~MainWindow();


private slots:
    void on_Add_clicked();

    void on_Edit_clicked();

    void on_Remove_clicked();

    void menuExit();

    void menuAlways_on_top();

    void menuReset_to_default();

    void on_things_list_itemChanged(QListWidgetItem*);

    void on_doneTask_clicked();

    void on_expectedTask_clicked();

    void on_failedTask_clicked();

    void on_previousDay_clicked();

    void on_nextDay_clicked();

protected:
    void closeEvent(QCloseEvent*) override;

private:
    int daysCounter;
    QString currentDay;
    QString space;
    QString basePath;
    Ui::MainWindow *ui;

    QMap<QString, QChar> task_state;
    QMap<QChar, QString> picture;
    QMap<QChar, QString> flag;
};

class Tray : public QObject
{
    Q_OBJECT

private:
    QSystemTrayIcon *mSystemTrayIcon;

    QSystemTrayIcon* tray;
    QMenu* trayMenu;
    QAction* trayOpen;
    QAction* trayQuit;

//public slots:
//    void quitFromTray();
//    void openFromTray();
//    void set_tray_settings();

public:
    Tray()
    {
        tray = new QSystemTrayIcon();
        tray->setIcon(QIcon("://icon.png"));
        tray->setVisible(true);

        trayMenu = new QMenu();
        trayOpen = trayMenu->addAction("Open");
        trayQuit = trayMenu->addAction("Quit");

//        QObject::connect(trayOpen, &QAction::triggered, this, &Tray::openFromTray);
//        QObject::connect(trayQuit, &QAction::triggered, this, &Tray::quitFromTray);

        tray->setContextMenu(trayMenu);
    }
   ~Tray()
   {
       qDebug() << "From Dtor";
       delete mSystemTrayIcon;
       delete tray;
       delete trayMenu;
       delete trayOpen;
       delete trayQuit;
   }
};

#endif // MAINWINDOW_H
