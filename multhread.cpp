#include "multhread.h"
#include <QMutex>
#include <QDebug>
#include <QCoreApplication>

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
            this->Q.enqueue(str.remove(QChar('\n'), Qt::CaseInsensitive));
        }
        this->Total = Q.size();
        file.close();
    }
    else
    {
        qDebug() << "文件路径打开有问题" << endl;
    }
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

void Multhread::run()
{
    QMutex mutex;
    int status = 0;
    QString url = "";
    QNetworkAccessManager *netManager =new QNetworkAccessManager();
    NetConnect manager(netManager);

    qDebug() <<  "Q CONTENT:" << Q.empty() << endl;
    if(Q.empty())
    {
//        if(this->isstop)
//        {
//            this->quit();
//            return;
//        }
        mutex.lock();
        url = baseUrl + urlDir;
        qDebug() << QString(url);
        status = manager.retGetUrl(QUrl(url));
        qDebug() << status << endl;
        if(this->time)
            msleep(messLegTime);

        emit sigSentCurrentUrl(url);
        emit sigSentRetUrl(url);
        mutex.unlock();

        if(status)
        {
            QString strMess = "<font color =\"#6EFFFF\">"  + url + "</font>"; //color: #FFFFFF;font-weight:bold
            emit sigSentRetUrlToBrowser(strMess);
        }
    }
    else
    {
        while(!this->Q.empty())
        {
            if(this->isstop)
            {
                this->quit();
                break;
            }
            mutex.lock();
            url = this->Q.dequeue();
            qDebug() << QString(url);

    //    if(this->isproxy){
    //        Proxy * test= new Proxy();
    //        test->generateList();
    //        test->setProxy();
    //        netManager->setProxy(test->m_proxy);
    //    }
            status = manager.retGetUrl(QUrl(url));
            qDebug() << status << endl;
            if(this->time)
                msleep(messLegTime);

            emit sigSentCurrentUrl(url);
            emit sigSentRetUrl(url);
            mutex.unlock();

            if(status)
            {
                QString strMess = "<font color =\"#6EFFFF\">"  + url
                                   + "    --->    This page is using!" + "</font>"; //color: #FFFFFF;font-weight:bold
                emit sigSentRetUrlToBrowser(strMess);
            }
        }
    }

    //msleep(10);
}
