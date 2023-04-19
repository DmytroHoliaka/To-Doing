#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    show_date();

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::show_date() {
    QDate currentDate = QDate::currentDate();
    ui->date_label->setText("----------------------  "+ currentDate.toString("dd.MM.yyyy") + "  ----------------------");

}

