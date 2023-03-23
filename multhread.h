#ifndef MULTHREAD_H
#define MULTHREAD_H
#include <QThread>
#include <QQueue>
#include <qfile.h>
#include <QElapsedTimer>
#include <QUrl>
#include <netconnect.h>
#include "proxy.h"

//多线程管理
class Multhread : public QThread
{
    Q_OBJECT
public:
    explicit Multhread(QObject *parent = 0);
    QQueue <QString> Q;
    int Total;
    QString baseUrl;
    void GenerateTasks(QString Type);
    bool isstop = 0;
    bool isproxy = 0;
    int time = 0;
signals:
    void sigSentCurrentUrl(QString url);
    void sigSentRetUrl(QString url);
    void sigSentRetUrlToBrowser(QString);

private slots:
    void setMessLegTime(QString strLegTime);
    void setMessUrl(const QString &);
    void setMessUrlDir(const QString &);

protected:
    void run();
private:
     int messLegTime = 0;
     QString urlDir = "";

};

#endif // MULTHREAD_H
