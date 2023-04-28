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


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    void show_date();
    void customize_list_font(QString, int, int);
    ~MainWindow();


private slots:
    void on_Add_clicked();

    void on_Edit_clicked();

    void on_Remove_clicked();

    void menuExit();

    void menuAlways_on_top();

    void menuReset_to_default();

    void quitFromTray();

    void openFromTray();

    void on_things_list_itemChanged(QListWidgetItem*);

    void on_doneTask_clicked();

    void on_expectedTask_clicked();

    void on_failedTask_clicked();

protected:
    void closeEvent(QCloseEvent*) override;

private:
    Ui::MainWindow *ui;
    QSystemTrayIcon *mSystemTrayIcon;

    QSystemTrayIcon* tray;
    QMenu* trayMenu;
    QAction* trayOpen;
    QAction* trayQuit;
};

#endif // MAINWINDOW_H
