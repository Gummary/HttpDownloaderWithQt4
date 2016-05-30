#include "download.h"

DownLoad::DownLoad(QObject *parent) :
    QObject(parent)
{
    downloadSet.clear();
    usedTime_.clear();
}

int DownLoad::NewDownLoad(QUrl url)
{
    int rc;
    if(downloadSet.contains(url.toString()))
    {
        return TASKEXIST;
    }

    HttpReply *reply = new HttpReply(url);

    downloadSet.insert(url.toString(), reply);
    usedTime_.insert(url.toString(), 0);

    if((rc= reply->DownLoad()))
    {
        downloadSet.remove(url.toString());
        usedTime_.remove(url.toString());
        delete reply;
        return rc;
    }

    connect(reply, SIGNAL(downProcess(QString,qint64,qint64,int)),
            this, SLOT(UpdateProcess(QString,qint64,qint64,int)));

    connect(reply, SIGNAL(Finish(QString)), this, SLOT(Finish(QString)));
    return (0);
}

int DownLoad::ReStartDownLoad(QUrl url)
{
    int rc;
    if(!downloadSet.contains(url.toString()))
    {
        return TASKNOTEXIST;
    }

    HttpReply *reply = downloadSet.value(url.toString());

    if(rc = reply->DownLoad())
    {
        return rc;
    }
}

void DownLoad::Finish(QString url)
{
    HttpReply *reply = downloadSet.value(url);
    downloadSet.remove(url);
    usedTime_.remove(url);
    qDebug() << "remove";
    delete reply;
    qDebug() << "delete";
}

void DownLoad::UpdateProcess(QString url, qint64 recesize, qint64 totalsize, int usedTime)
{
    qDebug() << "2233";
    // 总时间
    int nTime = usedTime;

    // 本次下载所用时间
    nTime -= usedTime_.value(url, 0);

    // 下载速度
    double dBytesSpeed = (recesize * 1000.0) / nTime;
    double dSpeed = dBytesSpeed;

    //剩余时间
    qint64 leftBytes = (totalsize - recesize);
    double dLeftTime = (leftBytes * 1.0) / dBytesSpeed;

    // 获取上一次的时间
    usedTime_.insert(url, nTime);

    emit Update(url, speed(dSpeed),timeFormat(qCeil(dLeftTime)), recesize, totalsize);
}

// 速度转KB/S、MB/S、GB/S
QString DownLoad::speed(double speed)
{
    QString strUnit;
    if (speed <= 0)
    {
        speed = 0;
        strUnit = "Bytes/S";
    }
    else if (speed < 1024)
    {
        strUnit = "Bytes/S";
    }
    else if (speed < 1024 * 1024)
    {
        speed /= 1024;
        strUnit = "KB/S";
    }
    else if (speed < 1024 * 1024 * 1024)
    {
        speed /= (1024 * 1024);
        strUnit = "MB/S";
    }
    else
    {
        speed /= (1024 * 1024 * 1024);
        strUnit = "GB/S";
    }

    QString strSpeed = QString::number(speed, 'f', 2);
    return QString("%1 %2").arg(strSpeed).arg(strUnit);
}

// 秒转*d *h *m *s
QString DownLoad::timeFormat(int seconds)
{
    QString strValue;
    QString strSpacing(" ");
    if (seconds <= 0)
    {
        strValue = QString("%1s").arg(0);
    }
    else if (seconds < 60)
    {
        strValue = QString("%1s").arg(seconds);
    }
    else if (seconds < 60 * 60)
    {
        int nMinute = seconds / 60;
        int nSecond = seconds - nMinute * 60;

        strValue = QString("%1m").arg(nMinute);

        if (nSecond > 0)
            strValue += strSpacing + QString("%1s").arg(nSecond);
    }
    else if (seconds < 60 * 60 * 24)
    {
        int nHour = seconds / (60 * 60);
        int nMinute = (seconds - nHour * 60 * 60) / 60;
        int nSecond = seconds - nHour * 60 * 60 - nMinute * 60;

        strValue = QString("%1h").arg(nHour);

        if (nMinute > 0)
            strValue += strSpacing + QString("%1m").arg(nMinute);

        if (nSecond > 0)
            strValue += strSpacing + QString("%1s").arg(nSecond);
    }
    else
    {
        int nDay = seconds / (60 * 60 * 24);
        int nHour = (seconds - nDay * 60 * 60 * 24) / (60 * 60);
        int nMinute = (seconds - nDay * 60 * 60 * 24 - nHour * 60 * 60) / 60;
        int nSecond = seconds - nDay * 60 * 60 * 24 - nHour * 60 * 60 - nMinute * 60;

        strValue = QString("%1d").arg(nDay);

        if (nHour > 0)
            strValue += strSpacing + QString("%1h").arg(nHour);

        if (nMinute > 0)
            strValue += strSpacing + QString("%1m").arg(nMinute);

        if (nSecond > 0)
            strValue += strSpacing + QString("%1s").arg(nSecond);
    }

    return strValue;
}



int DownLoad::StopDownLoad(QUrl url)
{
    HttpReply *reply = downloadSet.value(url.toString(), NULL);
    if(reply!=NULL)
    {
        reply->StopDownLoad();
    }
}
