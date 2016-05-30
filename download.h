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
    void Update(QString url, QString, QString,  qint64, qint64);

    void UpdateSpeed(QString, QString);
    void UpdateTime(QString,QString);
    void UpdatSize(QString,qint64, qint64);

    void taskFinish(QString url);

public slots:
    void UpdateProcess(QString url, qint64 recesize, qint64 totalsize, int usedTime);

    void Finish(QString url);
public:
    explicit DownLoad(QObject *parent = 0);
    QHash<QString,HttpReply*>  downloadSet;
    QHash<QString, int> usedTime_;
};

#define TASKNOTEXIST         100
#define TASKEXIST           101

#endif // DOWNLOAD_H
