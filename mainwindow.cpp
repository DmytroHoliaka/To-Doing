#include "mainwindow.h"
#include "ui_mainwindow.h"

// ------------------- Main Window -------------------
MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    trayObj.makeConections(this);


    ui->setupUi(this);
    date.print(ui->date_label);

    topPanelObj.setExit(ui->actionExit);
    topPanelObj.setTop(ui->actionAlways_on_top);
    topPanelObj.setReset(ui->actionReset_to_default);
    topPanelObj.makeConections(this);


    manager.setAdd(ui->Add);
    manager.setRemove(ui->Remove);
    manager.setEdit(ui->Edit);
    manager.setThingsList(ui->things_list);
    manager.setDone(ui->doneTask);
    manager.setExpected(ui->expectedTask);
    manager.setFailed(ui->failedTask);

    manager.makeConections();

    this->setWindowTitle("ToDoing");
    this->setWindowIcon(QIcon(":/icon.png"));



    basePath = QCoreApplication::applicationDirPath() + "\\tasks_base\\";



    customize_list_font("Constantia", 17, 60);

    task_state.insert("flag_done", '2');
    task_state.insert("", '1');
    task_state.insert("flag_failed", '0');

    picture.insert('2', "://done.png");
    picture.insert('1', "://expected.png");
    picture.insert('0', "://failed.png");

    flag.insert('2', "flag_done");
    flag.insert('1', "");
    flag.insert('0', "flag_failed");

    getTasksFromFile();
}

MainWindow::~MainWindow() {

    putTasksIntoFile();

    delete ui;
}

// Дії при закритті вікна
void MainWindow::closeEvent(QCloseEvent* event)
{
    event->ignore();
    this->hide();
}

void MainWindow::customize_list_font(QString font_name, int size, int block_height) {
    QFont font(font_name, size);
    ui->things_list->setFont(font);
    ui->things_list->setStyleSheet(QString("QListWidget::item { height:%1px; }").arg(block_height));
}

void MainWindow::on_previousDay_clicked()
{
    putTasksIntoFile();
    ui->things_list->clear();

    --date;
    date.recount();
    date.print(ui->date_label);
    getTasksFromFile();
}

void MainWindow::on_nextDay_clicked()
{
    putTasksIntoFile();
    ui->things_list->clear();

    ++date;
    date.recount();
    date.print(ui->date_label);
    getTasksFromFile();
}

void MainWindow::getTasksFromFile()
{
    QFile file(basePath + date.getCurrentDay() + ".txt");
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);

        while(!in.atEnd()) {
            QString line = in.readLine();
            const QChar state = line.at(0);

            QListWidgetItem* item = new QListWidgetItem(line.mid(1), ui->things_list);
            ui->things_list->addItem(item);
            item->setFlags(item->flags() | Qt::ItemIsEditable);

            QIcon icon(picture[state]);
            item->setIcon(icon);

            item->setData(Qt::UserRole, flag[state]);
        }
    }

    file.close();
}

void MainWindow::putTasksIntoFile()
{
    QDir directory(QCoreApplication::applicationDirPath());
    QString folderName = "tasks_base";
    if (!directory.exists(folderName)) {
        directory.mkpath(folderName);
    }

    QFile file(basePath + date.getCurrentDay() + ".txt");
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::information(0, "Writing error", file.errorString());
    }

    QTextStream out(&file);
    for (int i = 0; i < ui->things_list->count(); ++i) {
        QString key = ui->things_list->item(i)->data(Qt::UserRole).toString();

        out << task_state[key] << ui->things_list->item(i)->text() << "\n";
    }

    file.close();
}




// ------------------- Tray -------------------
void Tray::openFromTray(MainWindow* mw) {
    mw->show();
}

void Tray::quitFromTray() {
    QApplication::quit();
}




// ------------------- Top Panel -------------------

TopPanel::~TopPanel()
{
    delete exit;
    delete top;
    delete reset;
}

void TopPanel::menuAlways_on_top(MainWindow* mw) {
    mw->setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint); // закріплення вікна
    mw->show();
}

void TopPanel::menuReset_to_default(MainWindow* mw) {
    mw->setWindowFlags(Qt::Window);
    mw->show();
}



// ------------------- Date -------------------
void Date::print(QLabel* dateLabel)
{
    dateLabel->setText(this->space + "  " + this->currentDay + "  " + this->space);
}



// ------------------- Main Button Manager -------------------
void MainButtonManager::on_doneTask_clicked() {      // Done
    QListWidgetItem* item = this->thingsList->currentItem();

    if(item){
        QIcon icon("://done.png");
        item->setIcon(icon);

       item->setData(Qt::UserRole, "flag_done");
    }
}

void MainButtonManager::on_expectedTask_clicked() {  // Expected
    QListWidgetItem* item = this->thingsList->currentItem();

    if(item) {
        QIcon icon("://expected.png");
        item->setIcon(icon);

        item->setData(Qt::UserRole, "");
    }
}

void MainButtonManager::on_failedTask_clicked() {    // Failed
    QListWidgetItem* item = this->thingsList->currentItem();

    if(item){
        QIcon icon("://failed.png");
        item->setIcon(icon);

        item->setData(Qt::UserRole, "flag_failed");
    }
}

void MainButtonManager::on_Add_clicked(QListWidget* list)  // Add
{
    QListWidgetItem* item = new QListWidgetItem(list);
    item->setFlags(item->flags() | Qt::ItemIsEditable);

    QIcon icon("://expected.png");
    item->setIcon(QIcon(icon));

    list->setCurrentItem(item);                  // встановлюємо активність на елемент
    list->edit(list->currentIndex()); // викликаємо метод edit для редагування елемента
    item->setSelected(true);                                // виділяємо елемент
}

void MainButtonManager::on_Remove_clicked(QListWidget* list)  // Remove
{
    QListWidgetItem* item = list->currentItem();
    if (item) {
        delete item;
    }
}

void MainButtonManager::on_Edit_clicked(QListWidget* list)  // Edit
{
    QListWidgetItem* item = list->currentItem();
    if (item) {
        list->editItem(item);
    }
}

// Видалення пустих рядків(коли вони обрані)
void MainButtonManager::afterChanged(QListWidgetItem* tempItem)
{
    if(tempItem != nullptr && tempItem->isSelected() && tempItem->text().isEmpty()){
        delete tempItem;
    }
}

void MainButtonManager::makeConections()
{
    QObject::connect(this->add, &QPushButton::clicked, this, [this](){
        this->on_Add_clicked(this->thingsList);
    });

    QObject::connect(this->remove, &QPushButton::clicked, this, [this](){
        this->on_Remove_clicked(this->thingsList);
    });

    QObject::connect(this->edit, &QPushButton::clicked, this, [this](){
        this->on_Edit_clicked(this->thingsList);
    });

    QObject::connect(this->done, &QPushButton::clicked, this, [this](){
        this->on_doneTask_clicked();
    });

    QObject::connect(this->expected, &QPushButton::clicked, this, [this](){
        this->on_expectedTask_clicked();
    });

    QObject::connect(this->failed, &QPushButton::clicked, this, [this](){
        this->on_failedTask_clicked();
    });

    QObject::connect(this->thingsList, &QListWidget::itemChanged, this, [this](){
        this->afterChanged(this->thingsList->currentItem());
    });


}

MainButtonManager::~MainButtonManager()
{
    delete add;
    delete remove;
    delete edit;
    delete thingsList;
}

