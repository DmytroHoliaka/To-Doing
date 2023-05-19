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
    Tray()
    {
        tray = new QSystemTrayIcon();
        tray->setIcon(QIcon("://icon.png"));
        tray->setVisible(true);

        trayMenu = new QMenu();
        trayOpen = trayMenu->addAction("Open");
        trayQuit = trayMenu->addAction("Quit");

        tray->setContextMenu(trayMenu);
    }

    void makeConections(MainWindow* mw)
    {
        QObject::connect(trayOpen, &QAction::triggered, this, [this, mw]() {
            this->openFromTray(mw);
        });
        QObject::connect(trayQuit, &QAction::triggered, this, &Tray::quitFromTray);
    }

   ~Tray()
   {
       delete tray;
       delete trayOpen;
       delete trayQuit;
       delete trayMenu;
   }
};

class TopPanel : public QObject
{
    Q_OBJECT

private:
    QAction* exit;
    QAction* top;
    QAction* reset;

private slots:
    void menuAlways_on_top(MainWindow* mw);

    void menuReset_to_default(MainWindow* mw);

public:
    TopPanel()
    {
        this->exit = nullptr;
        this->top = nullptr;
        this->reset = nullptr;
    }

    TopPanel(QAction* exitMenu, QAction* topMenu, QAction* resetMenu)
    {
        this->exit = exitMenu;
        this->top = topMenu;
        this->reset = resetMenu;
    }

    void setExit(QAction* exitMenu)
    {
        this->exit = exitMenu;
    }

    void setTop(QAction* topMenu)
    {
        this->top = topMenu;
    }

    void setReset(QAction* resetMenu)
    {
        this->reset = resetMenu;
    }

    void makeConections(MainWindow* mw)
    {
        QObject::connect(this->exit, &QAction::triggered, this, [](){
             QApplication::quit();
        });

        QObject::connect(this->top, &QAction::triggered, this, [this, mw](){
            this->menuAlways_on_top(mw);
        });
        QObject::connect(this->reset, &QAction::triggered, this, [this, mw](){
            this->menuReset_to_default(mw);
        });
    }

    ~TopPanel();
};

class Date
{
    friend class MainWindow;
private:
    int daysCounter;
    QString currentDay;
    QString space;

public:
    Date()
    {
        this->daysCounter = 0;
        this->space = "----------------------";
        this->currentDay = this->get_date();
    }

    void print(QLabel* dataLabel);

    QString get_date() {
        return QDate::currentDate().addDays(this->daysCounter).toString("dd.MM.yyyy");
    }

    int getDaysCounter()
    {
        return this->daysCounter;
    }

    QString getCurrentDay()
    {
        return this->currentDay;
    }

    void recount()
    {
        this->currentDay = this->get_date();
    }

    void setDaysCounter(int count)
    {
        this->daysCounter = count;
    }

    Date operator++()
    {
        ++this->daysCounter;
        return *this;
    }

    Date operator--()
    {
        --this->daysCounter;
         return *this;
    }

    Date operator++(int)
    {
        Date temp = *this;
        ++this->daysCounter;
        return *this;
    }

    Date operator--(int)
    {
        Date temp = *this;
        --this->daysCounter;
        return *this;
    }
};

class MainButtonManager : public QObject
{

private:
    QPushButton* add;
    QPushButton* remove;
    QPushButton* edit;
    QPushButton* done;
    QPushButton* expected;
    QPushButton* failed;

    QListWidget* thingsList;



public:
    MainButtonManager()
    {
        this->add = nullptr;
        this->remove = nullptr;
        this->edit = nullptr;
        this->thingsList = nullptr;
    }

    MainButtonManager(QPushButton* addButton, QPushButton* removeButton, QPushButton* editButton, QListWidget* thingsList)
    {
        this->add = addButton;
        this->remove = removeButton;
        this->edit = editButton;
        this->thingsList = thingsList;
    }

    ~MainButtonManager();


    void setAdd(QPushButton* addButton)
    {
        this->add = addButton;
    }

    void setRemove(QPushButton* removeButton)
    {
        this->remove = removeButton;
    }

    void setEdit(QPushButton* editButton)
    {
        this->edit = editButton;
    }

    void setThingsList(QListWidget* thingsList)
    {
        this->thingsList = thingsList;
    }

    void setDone(QPushButton* doneButton)
    {
        this->done = doneButton;
    }

    void setExpected(QPushButton* expectedButton)
    {
        this->expected = expectedButton;
    }

    void setFailed(QPushButton* failedButton)
    {
        this->failed = failedButton;
    }


    void makeConections();
    void afterChanged(QListWidgetItem*);


    void on_Add_clicked(QListWidget* list);
    void on_Edit_clicked(QListWidget* list);
    void on_Remove_clicked(QListWidget* list);
    void on_doneTask_clicked();
    void on_expectedTask_clicked();
    void on_failedTask_clicked();
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Tray trayObj;
    TopPanel topPanelObj;
    Date date;
    MainButtonManager manager;


    QString basePath;
    Ui::MainWindow *ui;

    QMap<QString, QChar> task_state;
    QMap<QChar, QString> picture;
    QMap<QChar, QString> flag;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void customize_list_font(QString, int, int);
    void getTasksFromFile();
    void putTasksIntoFile();

private slots:
    void on_previousDay_clicked();
    void on_nextDay_clicked();

protected:
    void closeEvent(QCloseEvent*) override;

};

#endif // MAINWINDOW_H
