#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    show_date();

    ui->things_list->addItems({"Виконати КДМ", "Написати есе", "Зробити КР",  "Зустріч",  "Математичний аналіз"});
    customize_list_font("Constantia", 17, 60);
    add_progress_mark();

}

MainWindow::~MainWindow() {
    delete ui;
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
