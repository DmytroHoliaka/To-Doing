#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    show_date();

//    ui->things_list->addItems({"Виконати КДМ", "Написати есе", "Зробити КР",  "Зустріч",  "Математичний аналіз"});
    customize_list_font("Constantia", 17, 60);
    add_progress_mark();


    // ------------------- Tray -------------------
    tray = new QSystemTrayIcon();
    tray->setIcon(QIcon("://icon.png"));
    tray->setVisible(true);

    trayMenu = new QMenu();
    trayOpen = trayMenu->addAction("Open");
    trayQuit = trayMenu->addAction("Quit");

    QObject::connect(trayOpen, &QAction::triggered, this, &MainWindow::openFromTray);
    QObject::connect(trayQuit, &QAction::triggered, this, &MainWindow::quitFromTray);

    tray->setContextMenu(trayMenu);

    // ------------------- Menu actions -------------------
    QObject::connect(ui->actionExit, &QAction::triggered, this, &MainWindow::menuExit);
    QObject::connect(ui->actionAlways_on_top, &QAction::triggered, this, &MainWindow::menuAlways_on_top);
    QObject::connect(ui->actionReset_to_default, &QAction::triggered, this, &MainWindow::menuReset_to_default);
}

void MainWindow::openFromTray() {
        this->show();
}

void MainWindow::quitFromTray() {
    QApplication::quit();
}

void MainWindow::menuExit() {
    QApplication::quit();
}

void MainWindow::menuAlways_on_top() {
    this->setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint); // закріплення вікна
    this->show();
}

void MainWindow::menuReset_to_default() {
    this->setWindowFlags(Qt::Window);
    this->show();
}

MainWindow::~MainWindow() {
    delete tray;
    delete trayMenu;
    delete trayOpen;
    delete trayQuit;
    delete ui;
}

// Дії при закритті вікна
void MainWindow::closeEvent(QCloseEvent* event)
{
    event->ignore();
    this->hide();
}

// Видалення пустих рядків(коли вони обрані)
void MainWindow::on_things_list_itemChanged(QListWidgetItem* item)
{
    if(item->isSelected() && item->text().isEmpty()){
        delete item;
    }
}

void MainWindow::show_date() {
    QDate currentDate = QDate::currentDate();
    QString space = "----------------------";
    ui->date_label->setText(space + "  " + currentDate.toString("dd.MM.yyyy") + "  " + space);
//    ui->date_label->setStyleSheet("background-color: rgb(211, 157, 124);");
}

void MainWindow::add_progress_mark() {
    for (int i = 0; i < ui->things_list->count(); i++) {
        QListWidgetItem *item = ui->things_list->item(i);

        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
    }
}

void MainWindow::customize_list_font(QString font_name, int size, int block_height) {
    QFont font(font_name, size);
    ui->things_list->setFont(font);
    ui->things_list->setStyleSheet(QString("QListWidget::item { height:%1px; }").arg(block_height));
}

void MainWindow::on_Add_clicked()  // Add
{
    QListWidgetItem* item = new QListWidgetItem(ui->things_list);

    item->setFlags(item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEditable);
    item->setCheckState(Qt::Unchecked);

    ui->things_list->setCurrentItem(item);                  // встановлюємо активність на елемент
    ui->things_list->edit(ui->things_list->currentIndex()); // викликаємо метод edit для редагування елемента
    item->setSelected(true);                                // виділяємо елемент
}

void MainWindow::on_Remove_clicked()  // Remove
{
    QListWidgetItem* item = ui->things_list->currentItem();
    if (item) {
        delete item;
    }
}

void MainWindow::on_Edit_clicked()  // Edit
{
    QListWidgetItem* item = ui->things_list->currentItem();
    if (item) {
        ui->things_list->editItem(item);
    }
}


