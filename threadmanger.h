#ifndef THREADMANGER_H
#define THREADMANGER_H

#include <QObject>
#include "multhread.h"
#include <QList>


class threadManger : public QObject
{
    Q_OBJECT
public:
    explicit threadManger(QObject *parent = nullptr);
    ~threadManger();

signals:
        void sentTmpReUrlToBrowser(QString);
        void sentTmpUrl(QString);
        void sentTmpReUrl(QString);

public slots:
    void threadStart();
    void threadStop();
    void mulThreadNum(const QString &);

    void setTmpReUrlToBrowser(QString);
    void setTmpUrl(QString);
    void setTmpReUrl(QString);
    //setTmpReUrlToBrowser(QString) setTmpUrl(QString)  setTmpReUrl(QString)

private:
    QList<Multhread* > threadList;
    int threadNum = 1;
    //Multhread *tmpThread;

};

#endif // THREADMANGER_H
