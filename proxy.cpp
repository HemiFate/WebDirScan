#include "proxy.h"
Proxy::Proxy()
{
    //设置代理的请求方式
    this->m_proxy.setType(QNetworkProxy::Socks5Proxy);
}

//设置代理 返回代理的IP和Port
QNetworkProxy Proxy::setProxy()
{
    int size = this->container.size();
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    int index =qrand()%(size-1);
    qDebug() << index << endl;

    QString proxy=this->container[index];
    QString url=proxy.split(":")[0];
    int port=proxy.split(":")[1].toInt();

    qDebug()<<url;
    qDebug()<<port;

    this->m_proxy.setType(QNetworkProxy::HttpProxy);
    this->m_proxy.setHostName(url);
    this->m_proxy.setPort(port);
    /*this->proxy1.setHostName("127.0.0.1");
    this->proxy1.setPort(8080);*/
    return this->m_proxy;
}

//将代理添加到代理池中
void Proxy::generateList()
{
    QFile file("./config/proxy/proxy");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        while(!file.atEnd()){
        QByteArray ba = file.readLine();
        QString str(ba);
        this->container.append(str.remove(QChar('\n')));
     }
        file.close();
    }
}
