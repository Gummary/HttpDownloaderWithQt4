#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "download.h"
#include <QUrl>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_pushButton_clicked();

    void Update(QUrl, QString, QString,  qint64, qint64);

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_Start2_clicked();

    void on_Stop2_clicked();

    void on_Continue2_clicked();

private:
    Ui::MainWindow *ui;
    DownLoad *downLoad;
    QUrl url;

};

#endif // MAINWINDOW_H
