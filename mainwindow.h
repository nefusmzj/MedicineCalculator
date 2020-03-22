#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButtonAddChange_clicked();

    void on_pushButtonDelChange_clicked();

    void on_pushButtonCal_clicked();

    void on_pushButtonReset_clicked();

    void on_radioButtonMorning_clicked();

    void on_radioButtonNight_clicked();

private:
    Ui::MainWindow *ui;
    int MorningOrNightFlag;
    QDateTime CurrentDataTime;
};

#endif // MAINWINDOW_H
