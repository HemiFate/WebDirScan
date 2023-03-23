#include "scanner.h"
#include<QMutex>
#include <QNetworkProxy>


scanner::scanner()
{
    this->stopped = 0;
}

void scanner::run()
{
    QMutex mutex;
    QMutex text;
    QString ip = this->ip;

    while(!this->Q.empty())
    {
        QTcpSocket socket(0);
        socket.abort();
        mutex.lock();
        int tmpport = this->Q.dequeue();
        if(this->stopped)
        {
            mutex.unlock();
            this->nowport = tmpport;
            this->quit();
            break;
        }
        socket.connectToHost(ip, tmpport);
        if(socket.waitForConnected(portLegTime))  //延迟访问的时间
        {
            text.lock();
            QString message = ip + ": ";
            message += QString::number(tmpport, 10) + "--->Opened! ";
            QString strMess = "<font color =\"#6EFFFF\">"  + message + "</font>"; //color: #FFFFFF;font-weight:bold
            emit sentPortRet(strMess);
            text.unlock();
        }
        else
        {
            text.lock();
            QString message = ip + ": ";
            message += QString::number(tmpport, 10) + "--->Not Open! ";
            QString strMess = "<font color =\"#d54623\">"  + message + "</font>"; //color: #FFFFFF;font-weight:bold
            emit sentPortRet(strMess);
            text.unlock();
        }
        emit sentNowPort(tmpport);
        mutex.unlock();
    }
}

void scanner::stop()
{
    this->stopped = true;

}

void scanner::generate(int startport, int endport)
{
    for(int i = startport; i <= endport; ++i)
    {
        this->Q.enqueue(i);
    }
}

void scanner::setPortLegTime(const QString &str)
{
    qDebug()<<str;
    portLegTime = str.toInt()*1000;
}
