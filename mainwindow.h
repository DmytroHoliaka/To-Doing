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
class MainButtonManager;


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

    ~Tray()
    {
        delete tray;
        delete trayOpen;
        delete trayQuit;
        delete trayMenu;
    }

    void makeConections(MainWindow* mw)
    {
        QObject::connect(trayOpen, &QAction::triggered, this, [this, mw]() {
            this->openFromTray(mw);
        });
        QObject::connect(trayQuit, &QAction::triggered, this, &Tray::quitFromTray);
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

    ~TopPanel();

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
};

class Date
{
    // Todo: Ctor Copy
    friend class MainWindow;
public: // Todo: private
    int daysCounter;
    QString currentDay;
    QString space;
    QLabel* dateLabel;

public:
    Date()
    {
        this->daysCounter = 0;
        this->space = "----------------------";
        this->currentDay = this->get_date();
        this->dateLabel = nullptr;
    }

    Date(const Date& other)
    {
        this->daysCounter = 0;
        this->space = "----------------------";
        this->currentDay = this->get_date();
        this->dateLabel = nullptr;
        this->setDateLabel(other.dateLabel);
    }

    ~Date();

    void print();

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

    void setDateLabel(QLabel* dateLabel)
    {
        this->dateLabel = dateLabel;
    }

    QLabel* getDateLabel()
    {
        return this->dateLabel;
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

class FileData
{
private:
    QString basePath;
    QMap<QString, QChar> task_state;
    QMap<QChar, QString> picture;
    QMap<QChar, QString> flag;

public:
    FileData()
    {
        basePath = QCoreApplication::applicationDirPath() + "\\tasks_base\\";
        task_state.insert("flag_done", '2');
        task_state.insert("", '1');
        task_state.insert("flag_failed", '0');

        picture.insert('2', "://done.png");
        picture.insert('1', "://expected.png");
        picture.insert('0', "://failed.png");

        flag.insert('2', "flag_done");
        flag.insert('1', "");
        flag.insert('0', "flag_failed");
    }

    void getTasksFromFile(MainButtonManager* manager, Date& date);
    void putTasksIntoFile(MainButtonManager* manager, Date& date);
};

class MainButtonManager : public QObject    // Todo: Перейменуй, можливо WingetManager
{
public: // Todo: private
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
    MainButtonManager()
    {
        this->add = nullptr;
        this->remove = nullptr;
        this->edit = nullptr;
        this->next = nullptr;
        this->previous = nullptr;

        this->file = new FileData;
        this->thingsList = nullptr;
    }

    MainButtonManager(QPushButton* addButton,
                      QPushButton* removeButton,
                      QPushButton* editButton,
                      QPushButton* nextButton,
                      QPushButton* prevButton,
                      FileData* file,
                      QListWidget* thingsList)
    {
        this->add = addButton;
        this->remove = removeButton;
        this->edit = editButton;
        this->next = nextButton;
        this->previous = prevButton;

        this->file = file;
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

    void setNext(QPushButton* nextButton)
    {
        this->next = nextButton;
    }

    void setPrevious(QPushButton* previousButton)
    {
        this->previous = previousButton;
    }

    void makeConections(Date& date);
    void afterChanged(QListWidgetItem*);


    void on_Add_clicked(QListWidget* list);
    void on_Edit_clicked(QListWidget* list);
    void on_Remove_clicked(QListWidget* list);
    void on_doneTask_clicked();
    void on_expectedTask_clicked();
    void on_failedTask_clicked();
    void on_previousDay_clicked(Date& date);
    void on_nextDay_clicked(Date& date);
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Tray trayObj;
    TopPanel topPanelObj;
    Date date;
    MainButtonManager manager;


    Ui::MainWindow *ui;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void customize_list_font(QString, int, int);

protected:
    void closeEvent(QCloseEvent*) override;
};

#endif // MAINWINDOW_H
