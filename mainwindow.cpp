#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    trayObj.makeConections(this);


    ui->setupUi(this);
    date.print(ui->date_label);

    topPanelObj.setExit(ui->actionExit);
    topPanelObj.setTop(ui->actionAlways_on_top);
    topPanelObj.setReset(ui->actionReset_to_default);
    topPanelObj.makeConections(this);


    this->setWindowTitle("ToDoing");
    this->setWindowIcon(QIcon(":/icon.png"));

    basePath = QCoreApplication::applicationDirPath() + "\\tasks_base\\";

//    daysCounter = 0;
//    space = "----------------------";
//    currentDay = get_date();
//    ui->date_label->setText(space + "  " + currentDay + "  " + space);


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

    // ------------------- Menu actions -------------------

    // ---------------------------------------------------

    getTasksFromFile();
}

void Date::print(QLabel* dateLabel)
{
    dateLabel->setText(this->space + "  " + this->currentDay + "  " + this->space);
}

MainWindow::~MainWindow() {

    putTasksIntoFile();

    delete ui;
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

// ------------------- Menu -------------------

void TopPanel::menuAlways_on_top(MainWindow* mw) {
    mw->setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint); // закріплення вікна
    mw->show();
}

void TopPanel::menuReset_to_default(MainWindow* mw) {
    mw->setWindowFlags(Qt::Window);
    mw->show();
}

// ------------------- Menu -------------------

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

void MainWindow::customize_list_font(QString font_name, int size, int block_height) {
    QFont font(font_name, size);
    ui->things_list->setFont(font);
    ui->things_list->setStyleSheet(QString("QListWidget::item { height:%1px; }").arg(block_height));
}

// ------------------- Main Buttons -------------------
void MainWindow::on_Add_clicked()  // Add
{

    QListWidgetItem* item = new QListWidgetItem(ui->things_list);
    item->setFlags(item->flags() | Qt::ItemIsEditable);

    QIcon icon("://expected.png");
    item->setIcon(QIcon(icon));

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


// ------------------- Marking buttons -------------------
void MainWindow::on_doneTask_clicked() {      // Done
    QListWidgetItem* item = ui->things_list->currentItem();

    if(item){
        QIcon icon("://done.png");
        item->setIcon(icon);

       item->setData(Qt::UserRole, "flag_done");
    }
}

void MainWindow::on_expectedTask_clicked() {  // Expected
    QListWidgetItem* item = ui->things_list->currentItem();

    if(item) {
        QIcon icon("://expected.png");
        item->setIcon(icon);

        item->setData(Qt::UserRole, "");
    }
}


void MainWindow::on_failedTask_clicked() {    // Failed
    QListWidgetItem* item = ui->things_list->currentItem();

    if(item){
        QIcon icon("://failed.png");
        item->setIcon(icon);

        item->setData(Qt::UserRole, "flag_failed");
    }
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

