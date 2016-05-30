#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //ui->progressBar->hide();
    downLoad = new DownLoad();
    ui->progressBar->setValue(0);

    connect(downLoad, SIGNAL(Update(QUrl,QString,QString,qint64,qint64))
            , this, SLOT(Update(QUrl,QString,QString,qint64,qint64)));

    url = "http://115.159.127.79/ihome/download/iot.avi";
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    int rc = downLoad->NewDownLoad(url);
    qDebug() << rc;
    ui->pushButton_2->setEnabled(true);

}

void MainWindow::on_pushButton_2_clicked()
{
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(true);

    downLoad->StopDownLoad(url);
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_3->setEnabled(false);

    downLoad->ReStartDownLoad(url);
}

void MainWindow::Update(QUrl url, QString time,
                        QString speed, qint64 recesize, qint64 totalsize)
{
    ui->URL->setText(url.toString());
    ui->progressBar->setMaximum(totalsize);
    ui->progressBar->setValue(recesize);
    ui->Speed->setText(QString("Speed : ") + speed);
    ui->Receive->setText(QString("Time : ") + time);
}

void MainWindow::on_Start2_clicked()
{
    QUrl u ("http://115.159.127.79/ihome/download/PAM.tar.bz2");
    int rc = downLoad->NewDownLoad(u);
    qDebug() << rc;
}

void MainWindow::on_Stop2_clicked()
{
    QUrl u ("http://115.159.127.79/ihome/download/PAM.tar.bz2");
    downLoad->StopDownLoad(u);
}

void MainWindow::on_Continue2_clicked()
{
    QUrl u ("http://115.159.127.79/ihome/download/PAM.tar.bz2");
    downLoad->ReStartDownLoad(u);
}
