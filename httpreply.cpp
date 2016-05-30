#include "httpreply.h"

HttpReply::HttpReply(QUrl url)
{
    reply_ = NULL;
    receiveSize_ = 0;
    file_ = NULL;
    status_ = NODOWNLOAD;
    url_ = url;
    manger_ = new QNetworkAccessManager();
    total_ = 0;
}

HttpReply::~HttpReply()
{
    if(manger_)
        delete manger_;
}

//QString HttpReply::PATH("/home/download");


int HttpReply::DownLoad()
{
    int rc;
    QString tempName = QString("%1.tmp")
            .arg(QDateTime::currentDateTime()
                 .toString("yyyy-MM-dd-hh-mm-ss-zzz"));
    if(status_ == ABORT)
    {
        qDebug() << "Abort";
        if(!file_->open(QIODevice::Append))
        {
            return APPENDFILEFAILED;
        }
        downLoadTime_.restart();
    }
    else if(status_ == NODOWNLOAD)
    {
        qDebug() << "New";
        file_ = new QFile(QString("/home/SKZH/download/") + tempName);
        qDebug() << tempName;
        if(!file_->open(QIODevice::WriteOnly))
        {
            qDebug() << "Open Failed";
            delete file_;
            file_ = NULL;
            return OPENFILEFAILED;
        }
    }
    else
    {
        return FILEALREADYDOWNLOAD;
    }

    if((rc = startDownLoad()))
    {
        file_->close();
        return rc;
    }

    return (0);
}

int HttpReply::startDownLoad()
{
    status_ = DOWNLOADING;

    QNetworkRequest qheader;


    qheader.setRawHeader("Range",
                         tr("bytes=%1-").arg(receiveSize_).toUtf8());
    qDebug() << "URL : " << url_ << "  \nSize : " << receiveSize_;
    qheader.setUrl(url_);
    reply_ = manger_->get(qheader);
    connect(reply_, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(reply_, SIGNAL(downloadProgress(qint64, qint64)),
            this, SLOT(Update(qint64,qint64)));

    connect(reply_,  SIGNAL(finished()), this, SLOT(Finish()));

    downLoadTime_.start();

    return 0;

}

void HttpReply::readyRead()
{
    if(file_)
    {
        file_->write(reply_->readAll());
    }
}

void HttpReply::Update(qint64 receive, qint64 total)
{
    if(total_ == 0) total_ = total;
    int time = downLoadTime_.elapsed();
    emit downProcess(url_, receiveSize_ + receive, total_, time);

}

void HttpReply::Finish()
{
    if(status_ == ABORT)
    {
        file_->flush();
        file_->close();
        reply_->deleteLater();
    }
    else
    {
        file_->flush();
        file_->close();
        delete file_;
        reply_->deleteLater();
        emit Finish(url_);
    }
}

int HttpReply::StopDownLoad()
{
    if(status_==ABORT)
    {
        return DOWNLOADABORT;
    }
    if(status_ == NODOWNLOAD)
    {
        return DOWNNOTSTART;
    }

    status_ = ABORT;
    receiveSize_ = file_->size();
    reply_->abort();
}
