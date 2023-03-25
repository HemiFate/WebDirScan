#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <QObject>

class threadManager : public QObject
{
    Q_OBJECT
public:
    explicit threadManager(QObject *parent = nullptr);

signals:

};

#endif // THREADMANAGER_H
