#ifndef MULTHREAD_H_v
#define MULTHREAD_H_v
#include <QThread>
#include <QQueue>
#include <qfile.h>
#include <QElapsedTimer>
#include <QUrl>
#include <QList>
#include <netconnect.h>
#include "proxy.h"
#include <QMutex>

//#ifndef _QUEUE_Q
//#define _QUEUE_Q
//extern  QQueue <QString> Q;


//多线程管理
class Multhread : public QThread
{
    Q_OBJECT
public:
    explicit Multhread(QObject *parent = 0);
    static QQueue <QString> MyQueue;
    int Total;
    QString baseUrl = "http://testphp.vulnweb.com";
    void stop();
    bool isstop = 0;
    bool threadPauseStatus = 0;
    bool isproxy = 0;
    int time = 0;

    void GenerateTasks(QString Type);
    void threadPause();
    void threadResume();
signals:
    void sigSentCurrentUrl(QString url);
    void sigSentRetUrl(QString url);
    void sigSentRetUrlToBrowser(QString);
    //void sigSentFileBlock(int);

//    //发出开始和停止的信号

    //void start();

private slots:
    void setMessLegTime(QString strLegTime);
    void setMessUrl(const QString &);
    void setMessUrlDir(const QString &);

//public slots:


protected:
    void run();


private:
    int messLegTime = 0;
    QMutex threadPuseLock;
    QString urlDir = "/login.php";

};

#endif // MULTHREAD_H
