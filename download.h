#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include <QHash>
#include <QUrl>
#include <qmath.h>
#include "httpreply.h"

class DownLoad : public QObject
{
    Q_OBJECT

public:
    int NewDownLoad(QUrl url);

    int ReStartDownLoad(QUrl url);

    int StopDownLoad(QUrl url);

private:
    QString speed(double);
    QString timeFormat(int);

signals:
    void Update(QUrl, QString, QString,  qint64, qint64);

    void UpdateSpeed(QUrl, QString);
    void UpdateTime(QUrl,QString);
    void UpdatSize(QUrl,qint64, qint64);

    void taskFinish(QUrl);

public slots:
    void UpdateProcess(QUrl url, qint64 recesize, qint64 totalsize, int usedTime);

    void Finish(QUrl url);
public:
    explicit DownLoad(QObject *parent = 0);
    QHash<QUrl,HttpReply*>  downloadSet;
    QHash<QUrl, int> usedTime_;
};

#define TASKNOTEXIST         100
#define TASKEXIST           101

#endif // DOWNLOAD_H
