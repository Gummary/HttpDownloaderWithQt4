#ifndef HTTPREPLY_H
#define HTTPREPLY_H

#include <QObject>
#include <QNetworkReply>
#include <QFile>
#include <QUrl>
#include <QDateTime>
#include <QFileInfo>
#include <QNetworkAccessManager>
#include <QDebug>

enum Status
{
    NODOWNLOAD,
    ABORT,
    DOWNLOADING
};

class HttpReply : public QObject
{
    Q_OBJECT
public:
    explicit HttpReply(QUrl reply);

    ~HttpReply();

    int DownLoad();

    int StopDownLoad();

private slots:
    void readyRead();

signals:
    void downProcess(QString, qint64, qint64, int);

    void Finish(QUrl);

public slots:
    void Update(qint64, qint64);

    void Finish();

    int startDownLoad();

private:

    //const static QString PATH;
    qint64 receiveSize_;
    Status status_;
    QFile *file_;
    QUrl url_;
    QNetworkReply *reply_;
    QString fileName_;
    QTime downLoadTime_;
    QNetworkAccessManager *manger_;
    qint64 total_;

};

#define APPENDFILEFAILED            100
#define OPENFILEFAILED              101
#define FILEALREADYDOWNLOAD         102
#define DOWNLOADABORT               103
#define DOWNNOTSTART                104

#endif // HTTPREPLY_H
