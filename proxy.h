#ifndef PROXY_H
#define PROXY_H
#include<QNetworkProxy>
#include <QTime>
#include <QFile>


class Proxy
{
public:
    QNetworkProxy  m_proxy;
    Proxy();
    QNetworkProxy setProxy();
    void generateList();
    QStringList container;
};

#endif // PROXY_H
