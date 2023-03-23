#ifndef SCANNER_H
#define SCANNER_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QQueue>
//#include "webdirscan.h"

class scanner : public QThread
{
    Q_OBJECT
public:
    scanner();
    void run();
    void stop();
    void generate(int startport,int endport);
    QQueue <int> Q;  //文件中读取需要扫描的队列
    QString ip;
    bool stopped = 0; //停止扫描
    int nowport = 1;
private:
   // WebDirScan *webInit = new WebDirScan();
    int portLegTime = 1;

private slots:
    void setPortLegTime(const QString &/*,int tmpTime*/);
signals:
    void sentNowPort(int value);
    void sentPortRet(QString url);

};

#endif // SCANNER_H
