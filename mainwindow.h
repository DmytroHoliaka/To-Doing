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
    void add_progress_mark();
    ~MainWindow();

    QIcon createIcon(Qt::CheckState state) {
            QPixmap pixmap(16, 16);
            pixmap.fill(Qt::transparent);

            QPainter painter(&pixmap);
            painter.setRenderHint(QPainter::Antialiasing, true);

            if (state == Qt::Unchecked) {
                painter.setPen(Qt::red);
                painter.drawLine(4, 4, 12, 12);
                painter.drawLine(4, 12, 12, 4);
            } else if (state == Qt::PartiallyChecked) {
                painter.setPen(Qt::gray);
                painter.drawLine(4, 8, 12, 8);
            } else {
                painter.setPen(Qt::green);
                painter.drawLine(4, 8, 12, 8);
                painter.drawLine(8, 4, 8, 12);
            }

            return QIcon(pixmap);
        }
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
