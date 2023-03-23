#ifndef WEBDIRSCAN_H
#define WEBDIRSCAN_H

#include <QMainWindow>
#include <QDebug>
#include <QQueue>
#include "scanner.h"
#include <QStandardItem>
#include <QMimeData>
#include <QProcess>
#include <QTextCodec>
#include "multhread.h"
//#define ERROR_COLOR #d54623

QT_BEGIN_NAMESPACE
namespace Ui { class WebDirScan; }
QT_END_NAMESPACE

class WebDirScan : public QMainWindow
{
    Q_OBJECT

public:
    WebDirScan(QWidget *parent = nullptr);
    ~WebDirScan();
public:
        QString urlHead;
        QStringList stringList;

protected:
        void dragEnterEvent(QDragEnterEvent *event);
        void dropEvent(QDropEvent *event);

private slots:
        void on_pbtPortStart_clicked();
        void on_proBarPort_valueChanged(int value);
        void on_pbtPortStop_clicked();
        void setProBarMess(QString url);
        void setCurrentUrl(QString curUrl);
        void setBrowserText(QString retUrl);

signals:
        int sentPortLegTime(int tmpTime);

private slots:
    void setProBarPort(int value);

    void setPortResult(QString url);

    void on_pbtWhoisStart_clicked();

    void on_pbtMessStart_clicked();

    void setHeader(int);

    void on_pbtMessStop_clicked();

private:
    Ui::WebDirScan *ui;

private:
        void initWindows();
        void showEvent(QShowEvent *);
        void mousePressEvent(QMouseEvent *event);
        void onJsonToRead(QString jsonStr);

public:
        bool checkIP(QString ip);

        void startScanner(QString ip,int startPort, int stopPort);
        QString setQstringToEllipsis(const QString& text, int width);

        QQueue <QString>    QurlMess;
        QQueue <QString>    QurlPort;
        int portLegTime = 1;

        bool isFile = 0;
        bool twice = 0;  //设置当前进行的状态-->初始化为未开始0

        scanner * scan = new scanner();

private:
    int nowPort;
    int startPort;
    int endPort;
    int num;

    int probarPercent = 0;

    QString ip;
    Multhread *threadAdd;
    NetConnect *netConnectHead;
};
#endif // WEBDIRSCAN_H
