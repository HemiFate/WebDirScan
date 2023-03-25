#include "threadmanger.h"
#include <QString>
#include <QMutex>

threadManger::threadManger(QObject *parent) : QObject(parent)
{
//    //void sigSentCurrentUrl(QString url);
//    void sigSentRetUrl(QString url);
//    void sigSentRetUrlToBrowser(QString);
//    tmpThread = new Multhread(this);
//    connect(tmpThread, SIGNAL(sigSentCurrentUrl(QString)), this, SLOT(setTmpUrl(QString)));
//    connect(tmpThread, SIGNAL(sigSentRetUrl(QString)), this, SLOT(setTmpReUrl(QString)));
//    connect(tmpThread, SIGNAL(sigSentRetUrlToBrowser(QString)), this, SLOT(setTmpReUrlToBrowser(QString)));

}

threadManger::~threadManger()
{
    qDeleteAll(threadList);
    threadList.clear();
}


void threadManger::threadStart()
{
    threadList.reserve(threadNum);

    qDebug() << "threadNum: " << threadNum;
    for(int i = 0; i < threadNum; ++i)
    {
        Multhread *tmpThread = new Multhread();
        tmpThread->start();
        //返回值在这里，需要从这里返回变量
//        QMutex mutex;
//        mutex.lock();sigSentRetUrl sigSentRetUrlToBrowser
        connect(tmpThread, SIGNAL(sigSentCurrentUrl(QString)), this, SLOT(setTmpUrl(QString)));
        connect(tmpThread, SIGNAL(sigSentRetUrl(QString)), this, SLOT(setTmpReUrl(QString)));
        connect(tmpThread, SIGNAL(sigSentRetUrlToBrowser(QString)), this, SLOT(setTmpReUrlToBrowser(QString)));
        //mutex.unlock();
        threadList.append(tmpThread);
        qDebug() << "threadList.size: " << threadList.size() << endl;
    }
}

void threadManger::threadStop()
{
    qDebug() << "Thread ongoing stop!!!" << endl;
    for(int i = 0; i < threadList.size(); ++i)
        threadList.at(i)->stop();
}

void threadManger::mulThreadNum(const QString & number)
{
    //qDebug() << "number: " << number;
    threadNum = number.toInt();
}

void threadManger::setTmpReUrlToBrowser(QString str)
{
    emit sentTmpReUrlToBrowser(str);
}

void threadManger::setTmpUrl(QString str)
{
    //qDebug() << "tmpurl: " << str;
    emit sentTmpUrl(str);
}

void threadManger::setTmpReUrl(QString str)
{
    emit sentTmpReUrl(str);
}
