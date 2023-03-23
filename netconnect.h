#ifndef NETCONNECT_H
#define NETCONNECT_H
#include  <QObject>
#include <QNetworkAccessManager>

//从文件中读取后缀，完成组合，进行访问
class NetConnect : public QObject
{
    Q_OBJECT
public:
    explicit NetConnect(QNetworkAccessManager *pManger);
    NetConnect();
    ~NetConnect();

    //存储访问的IP+端口
    QString str = "";
    QString getUrl(QUrl url);
    bool retGetUrl(QUrl url);
    bool post(QUrl url, QByteArray& bytes);

private:
    QNetworkAccessManager *m_pManager;
    QByteArray strHead = "";
private slots:
    void setConnetHeader(QString);

signals:
private Q_SLOTS:
    void slot_replayFinished();
};

#endif // NETCONNECT_H
