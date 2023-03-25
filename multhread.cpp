#include "multhread.h"
#include <QMutex>
#include <QDebug>
#include <QCoreApplication>

QQueue<QString> Multhread::MyQueue;

Multhread::Multhread(QObject *parent ) : QThread(parent)
{

}
//文件内的地址入队
void Multhread::GenerateTasks(QString filePath)
{
    QString runPath = "";

    if(filePath == "./config/asp" || filePath == "./config/ctf" || filePath == "./config/dir" ||
       filePath == "./config/php" || filePath == "./config/aspx" || filePath == "./config/java"||
       filePath == "./config/backup")
         runPath = QCoreApplication::applicationDirPath() + filePath;
    else
        runPath = filePath;

    runPath.replace(QString("/"),QString("\\"));
    QFile file(runPath);
    qDebug() << runPath << endl;

    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        while(!file.atEnd()){
            QByteArray ba = file.readLine();
            QString str(ba);
            //qDebug() << this->baseUrl << endl;
            str = this->baseUrl + str;
            qDebug() << str << endl;
            MyQueue.enqueue(str.remove(QChar('\n'), Qt::CaseInsensitive));
        }
        this->Total = MyQueue.size();
        qDebug() << "Q size: " << MyQueue.size() << MyQueue.length() << endl;
        file.close();
    }
    else
    {
        qDebug() << "文件路径打开有问题" << endl;

    }

}

void Multhread::threadPause()
{
    qDebug() << "Thread Pause: " << endl;
    threadPauseStatus = true;
    threadPuseLock.lock();
}

void Multhread::threadResume()
{
    qDebug() << "Thread Resume: " << endl;
    threadPauseStatus = false;
    threadPuseLock.unlock();
}

void Multhread::setMessLegTime(QString strLegTime)
{
    this->time = 1;
    qDebug() << strLegTime << endl;
    messLegTime = strLegTime.toUInt()*1000;
}

void Multhread::setMessUrl(const QString & tempUrl)
{
    baseUrl = tempUrl;
    qDebug() << baseUrl << endl;
}

void Multhread::setMessUrlDir(const QString & tmpUrlDir)
{
    urlDir = tmpUrlDir;
}

void Multhread::stop()
{
    qDebug() << "QUEUE Size Now: " << MyQueue.size() << endl;
   //this->quit();
    this->isstop = 1;
    wait();
}

void Multhread::run()
{
    qDebug() << "DealTimeoutObject1：" << QThread::currentThreadId() << QThread::currentThread();

    QMutex mutex;
    mutex.lock();
    int status = 0;
    QString url = "";
    QNetworkAccessManager *netManager =new QNetworkAccessManager();
    NetConnect manager(netManager);

    qDebug() <<  " Q CONTENT: " << MyQueue.size() << MyQueue.empty() << endl;
    if(MyQueue.empty())
    {
        url = baseUrl + urlDir;
        qDebug() << "----->>> " << QString(url);

        status = manager.retGetUrl(QUrl(url));
        qDebug() << "reGetUrl status: " << status << endl;
        if(this->time)
            msleep(messLegTime);

        emit sigSentCurrentUrl(url);
        emit sigSentRetUrl(url);

        if(status)
        {
            QString strMess = "<font color =\"#6EFFFF\">"  + url
                               + "    --->    This page is using!" + "</font>"; //color: #FFFFFF;font-weight:bold
            emit sigSentRetUrlToBrowser(strMess);
        }
        //manager.deleteLater();
        mutex.unlock();
    }
    else
    {
        while(!MyQueue.empty())
        {
            if(!this->isstop)
            {
                threadPuseLock.lock();
                url = MyQueue.dequeue();
                qDebug() << QString(url);

                status = manager.retGetUrl(QUrl(url));
                qDebug() << status << endl;
                if(this->time)
                    msleep(messLegTime);

                emit sigSentCurrentUrl(url);
                emit sigSentRetUrl(url);


                if(status)
                {
                    QString strMess = "<font color =\"#6EFFFF\">"  + url
                                       + "    --->    This page is using!" + "</font>"; //color: #FFFFFF;font-weight:bold
                    emit sigSentRetUrlToBrowser(strMess);
                }
               threadPuseLock.unlock();
            }
            else
            {
                this->quit();
                break;
            }
        }
       mutex.unlock();
    }
    msleep(10);
}
