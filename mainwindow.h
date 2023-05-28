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
#include <QAbstractItemView>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow;
class WidgetManager;


class Tray : public QObject
{
    Q_OBJECT

private:
    QSystemTrayIcon* tray;
    QMenu* trayMenu;
    QAction* trayOpen;
    QAction* trayQuit;

private slots:
    void quitFromTray();
    void openFromTray(MainWindow*);

public:
    Tray();
    ~Tray();
    void makeConections(MainWindow*);
};



class TopPanel : public QObject
{
    Q_OBJECT

private:
    QAction* exit;
    QAction* top;
    QAction* reset;

private slots:
    void menuAlways_on_top(MainWindow*);
    void menuReset_to_default(MainWindow*);

public:
    TopPanel();
    TopPanel(QAction*, QAction*, QAction*);

    void setExit(QAction*);
    void setTop(QAction*);
    void setReset(QAction*);
    void makeConections(MainWindow*);
};



class Date
{
private:
    int daysCounter;
    QString currentDay;
    QString space;
    QLabel* dateLabel;

public:
    Date();

    void print();
    void recount();

    QString get_date();
    int getDaysCounter();
    QString getCurrentDay();
    QLabel* getDateLabel();

    void setDaysCounter(int);
    void setDateLabel(QLabel*);

    Date* operator++();
    Date* operator--();
};



class FileData
{
private:
    QString basePath;
    QMap<QString, QChar> task_state;
    QMap<QChar, QString> picture;
    QMap<QChar, QString> flag;

public:
    FileData();

    void getTasksFromFile(WidgetManager*, Date&);
    void putTasksIntoFile(WidgetManager*, Date&);
};



class WidgetManager : public QObject
{
private:
    QPushButton* add;
    QPushButton* remove;
    QPushButton* edit;

    QPushButton* done;
    QPushButton* expected;
    QPushButton* failed;

    QPushButton* next;
    QPushButton* previous;

    FileData* file;
    QListWidget* thingsList;


public:
    WidgetManager();
    WidgetManager(QPushButton*,
                  QPushButton*,
                  QPushButton*,
                  QPushButton*,
                  QPushButton*,
                  QPushButton*,
                  QPushButton*,
                  QPushButton*,
                  QListWidget*);
    ~WidgetManager();

    QListWidget* getThingsList();
    FileData* getFile();

    void setAdd(QPushButton*);
    void setRemove(QPushButton*);
    void setEdit(QPushButton*);
    void setThingsList(QListWidget*);
    void setDone(QPushButton*);
    void setExpected(QPushButton*);
    void setFailed(QPushButton*);
    void setNext(QPushButton*);
    void setPrevious(QPushButton*);

    void makeConections(Date&);
    void afterChanged(QListWidgetItem*);

    void on_Add_clicked(QListWidget*);
    void on_Edit_clicked(QListWidget*);
    void on_Remove_clicked(QListWidget*);
    void on_doneTask_clicked();
    void on_expectedTask_clicked();
    void on_failedTask_clicked();
    void on_previousDay_clicked(Date&);
    void on_nextDay_clicked(Date&);
};



class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Tray trayObj;
    TopPanel topPanelObj;
    Date date;
    WidgetManager manager;

    Ui::MainWindow *ui;

public:
    MainWindow(QWidget* = nullptr);
    ~MainWindow();

    void customize_list_font(QString, int, int);

protected:
    void closeEvent(QCloseEvent*) override;
};

#endif // MAINWINDOW_H
