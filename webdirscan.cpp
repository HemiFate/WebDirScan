#include "webdirscan.h"
#include "ui_webdirscan.h"
#include <QPixmap >
#include <QMessageBox>
#include <QDragEnterEvent>
#include <netconnect.h>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QLabel>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QComboBox>


WebDirScan::WebDirScan(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::WebDirScan)
{
    ui->setupUi(this);
    initWindows();
    connect(scan, SIGNAL(sentPortRet(QString)), this, SLOT(setPortResult(QString)));
    connect(scan,SIGNAL(sentNowPort(int)),this,SLOT(setProBarPort(int)));
    //检查当前页
    threadAdd = new Multhread();
    netConnectHead = new NetConnect();
    //设置状态栏 设置显示的内容
    connect(threadAdd, SIGNAL(sigSentCurrentUrl(QString)), this, SLOT(setCurrentUrl(QString)));
    connect(threadAdd, SIGNAL(sigSentRetUrl(QString)), this, SLOT(setProBarMess(QString)));
    connect(threadAdd, SIGNAL(sigSentRetUrlToBrowser(QString)), this, SLOT(setBrowserText(QString)));

    connect(ui->lePortLegTime, SIGNAL(textChanged(const QString &)), scan, SLOT(setPortLegTime(const QString &)));
    connect(ui->leMessLegTime, SIGNAL(textChanged(const QString &)), threadAdd, SLOT(setMessLegTime(const QString &)));
    connect(ui->leMessUrl, SIGNAL(textChanged(const QString &)), threadAdd, SLOT(setMessUrl(const QString &)));
    connect(ui->leMessRoute, SIGNAL(textChanged(const QString &)), threadAdd, SLOT(setMessUrlDir(const QString &)));
    connect(ui->cboboxMessHead, SIGNAL(currentIndexChanged(int)), this, SLOT(setHeader(int)));
    connect(ui->cboboxMessHead, SIGNAL(currentIndexChanged(QString)), netConnectHead, SLOT(setConnetHeader(QString)));
}

WebDirScan::~WebDirScan()
{
    delete ui;
}

void WebDirScan::initWindows()
{
   setFixedSize(this->width(), this->height());
   setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
   setAcceptDrops(true);
   //ui->cboboxMessHead->setView(new  QListView());
   //ui->cboboxPortHead->setView(new  QListView());

   QString  str = "User-Agent:Mozilla/5.0 (compatible; Baiduspider/2.0;+http://www.baidu.com/search/spider.html）";
   //QString str1 = setQstringToEllipsis(str, 15);
   ui->lbMessHeadText->adjustSize();
   ui->lbMessHeadText->setWordWrap(true);
   ui->lbMessHeadText->setText(str);

}

void WebDirScan::on_pbtMessStart_clicked()
{
    ui->textBrowser_2->clear();
    ui->lbMessScanContent->clear();
    threadAdd->isstop = 0;

    ui->textBrowser_2->setStyleSheet("color:#FFFFFF");
    ui->textBrowser_2->setText("正在扫描：...");

    qDebug() << "this sent text type is file?：" << isFile << endl;
        if(this->isFile)
        {
            QString baseUrlText = ui->leMessUrl->text();
            threadAdd->baseUrl = baseUrlText;
            qDebug() << "url:" << threadAdd->baseUrl << endl;
            if(ui->CboxMineRoute->checkState())
            {
                QString fileRoute = ui->leMessRoute->text();
                threadAdd->GenerateTasks(fileRoute);
            }
            //threadAdd->isproxy = 1;
            int threadNum = ui->leMessThread->text().toInt();
            threadAdd->time = ui->leMessLegTime->text().toUInt();
            for (int i = 0; i < threadNum; ++i)
                   threadAdd->start();
        }
        else    //当字典路径不是文件时，使用当前的后缀和名称+定义好的后缀进行访问
        {
            QString baseUrlText = ui->leMessUrl->text();
            QString dirUrl = ui->leMessRoute->text();

            threadAdd->baseUrl = baseUrlText + dirUrl;
            QRegExp rx("http[s]*://\\S+/");
            QRegExpValidator v(rx, 0);
            qDebug() << threadAdd->baseUrl << endl;
            //QurlMess.enqueue(threadAdd->baseUrl);
            int pos = 0;
            int threadNum = ui->leMessThread->text().toInt();

            if(v.validate(baseUrlText, pos)==0 || baseUrlText == "")
            {
                QMessageBox::warning(NULL, "输入错误", "您输入的url有误，请重新输入.", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
                return;
             }
            //QString str = this->QurlMess.dequeue();
            if(ui->CboxASP->checkState())
                threadAdd->GenerateTasks("./config/asp");
            if(ui->CboxCTF->checkState())
                threadAdd->GenerateTasks("./config/ctf");
            if(ui->CboxDIR->checkState())
                threadAdd->GenerateTasks("./config/dir");
            if(ui->CboxPHP->checkState())
                threadAdd->GenerateTasks("./config/php");
            if(ui->CboxAspx->checkState())
                threadAdd->GenerateTasks("./config/aspx");
            if(ui->CboxJAVA->checkState())
                threadAdd->GenerateTasks("./config/java");
            if(ui->CboxBACKUP->checkState())
                threadAdd->GenerateTasks("./config/backup");

            //threadAdd->isproxy = 1;
            threadAdd->time = ui->leMessLegTime->text().toUInt();
            for (int i = 0; i < threadNum; ++i)
                    threadAdd->start();
        }
}


void WebDirScan::on_pbtMessStop_clicked()
{
     threadAdd->isstop = 1;
}

void WebDirScan::on_proBarPort_valueChanged(int value)
{
    Q_UNUSED(value);
}

void WebDirScan::setCurrentUrl(QString url)
{
    ui->lbMessScanContent->setText(url);
}

void WebDirScan::setBrowserText(QString retUrl)
{
    ui->textBrowser_2->append(retUrl);
}

void WebDirScan::setProBarMess(QString url)
{
    probarPercent = (1.0-(float(threadAdd->Q.size())/float(threadAdd->Total)))*100;
    ui->proBarMess->setValue(probarPercent);
    ui->lbMessScanContent->setText(QString("%1").arg(url));

    //qDebug() << "probarPercent:" << probarPercent << endl;
//    if(probarPercent == 100)
//    {
//        ui->pbtMessStart->setText("开始");
//        ui->textBrowser_2->append(">>>>>>  扫描结束！ <<<<<<");
//    }
}

void WebDirScan::on_pbtPortStart_clicked()
{
    ui->textBrowser->clear();
    ui->lbPortScanContent->clear();
    ui->textBrowser->setStyleSheet("color:#FFFFFF");
    ui->textBrowser->setText("正在扫描：...");
    QString tmpStr = ui->lePortUrl->text();
    ui->proBarPort->setValue(0);
    if(isFile)
    {
        //ui->sbFirst->setValue(scan->nowport);
        startPort = ui->sbFirst->value();
        endPort = ui->sbSecond->value();
        ui->proBarPort->setRange(startPort, endPort);
        scan->stopped = 0;

        //根据线程数开始扫描
        int num = ui->lePortThread->text().toInt();
        qDebug() << num << endl;
        while(!this->QurlPort.empty())
        {
            scan->generate(startPort, endPort);
            scan->ip = this->QurlPort.dequeue();
        }
        for(int i = 0; i < num; ++i)
        {
            scan->start();
        }
    }
    else
    {
        if(!checkIP(tmpStr))
        {
            ui->textBrowser->setStyleSheet("color:#6EFFFF");
            ui->textBrowser->setText("IP 地址/格式错误，请输入正确的IP!");
        }
        else
        {
            ui->sbFirst->setValue(scan->nowport);
            //ip = tmpStr;
            startPort = ui->sbFirst->value();
            endPort = ui->sbSecond->value();

            ui->proBarPort->setRange(startPort, endPort);
            scan->generate(startPort,endPort);
            //ip获取出现问题
            scan->ip = tmpStr;
            scan->stopped = 0;

            int num = ui->lePortThread->text().toInt();
            for(int i = 0; i < num; i++)
            {
                scan->start();
            }
        }
    }
}

void WebDirScan::setPortResult(QString message)
{
    ui->textBrowser->append(message);
}

void WebDirScan::setProBarPort(int value)
{
    //qDebug() << value;
    ui->proBarPort->setValue(value);
    QString str = QString("  %1").arg(scan->ip);
    ui->lbPortScanContent->setText(QString(str + " :%1").arg(value));
}

bool WebDirScan::checkIP(QString ip)
{
    QRegExp rx2("\\d+\\.\\d+\\.\\d+\\.\\d+");
     if( !rx2.exactMatch(ip) )
     {
           return false;
     }
     //qDebug()<<"check true";
     return true;
}


void WebDirScan::on_pbtPortStop_clicked()
{
    scan->stopped = 1;
}

void WebDirScan::on_pbtWhoisStart_clicked()
{
    //输出当前HTTPS的协议支持
//    qDebug() << QSslSocket::sslLibraryBuildVersionNumber() << endl;
//    qDebug() << QSslSocket::sslLibraryBuildVersionString() << endl;
//    qDebug() << QSslSocket::sslLibraryVersionNumber() << endl;
//    qDebug() << QSslSocket::sslLibraryVersionString() << endl;
//    qDebug() << QSslSocket::supportsSsl() << endl;

    //构造需要请求的地址
    QString strAPIKey =  ui->leWhoisApiKey->text();
    QString strURL = ui->leWhoisUrl->text();
    QString Qurl = "https://www.whoisxmlapi.com/whoisserver/WhoisService?apiKey="
                    +  strAPIKey + "&domainName=" + strURL + "&outputFormat=JSON";

    qDebug() << Qurl << endl;

    QNetworkAccessManager *m_pManager = new QNetworkAccessManager();
    m_pManager->setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy); //设置安全策略

    QNetworkRequest request;
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true); //允许重定向

    request.setUrl(Qurl);
    QNetworkReply *reply = m_pManager->get(request);


    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    request.setRawHeader("User-Agent", "Mozilla/5.0 (compatible; Baiduspider/2.0; +http://www.baidu.com/search/spider.html)");
    int code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if(reply->error() == QNetworkReply::NoError && code == 200 )
     {
        QString  baData = reply->readAll();
        //qDebug() << baData;
        //QString retStr = codec->toUnicode(reply->readAll());
        //Json中查找关键信息并返回
        //QByteArray byteArray = reply->readAll();
        onJsonToRead(baData);
     }
    else
    {
        ui->lbWhoisRecode->setStyleSheet("color: #d54623");
        ui->lbWhoisRecode->setText("查询异常：");
        ui->lbCreateDate->setStyleSheet("color:#6EFFFF");
        ui->lbUpdateDate->setStyleSheet("color:#6EFFFF");
        ui->lbExpiresDate->setStyleSheet("color:#6EFFFF");
        ui->lbCreateDate->setText("1）请重新检查输入");
        ui->lbUpdateDate->setText("2）服务器错误");
        ui->lbExpiresDate->setText("3）该地址不可查");
        ui->label_8->clear();
        ui->label_21->clear();
        ui->label_25->clear();
        ui->label_27->clear();
        ui->lbrReDate->clear();
        ui->label_28->clear();
    }
    reply->deleteLater();
}

void WebDirScan::onJsonToRead(QString jsonStr)
{
    //qDebug() << "jsonStr:" << jsonStr << endl;
    QByteArray byteArray = jsonStr.toUtf8();
    QJsonParseError jsonError;
    QJsonDocument  rootDoc = QJsonDocument::fromJson(byteArray, &jsonError);
    //whoisInfo = "";
    if(!rootDoc.isNull() && (jsonError.error == QJsonParseError::NoError))
    {
        if(rootDoc.isObject())
        {
            QJsonObject jsonObj = rootDoc.object();
            QJsonValue jsonValue = jsonObj.value("WhoisRecord");

            if(jsonValue.isObject())
            {
                QJsonObject objRecord = jsonValue.toObject();
                QJsonValue jsonValueCreatedDate = objRecord.value("createdDate").toString();
                QJsonValue jsonValueUpdatedDate = objRecord.value("updatedDate").toString();
                QJsonValue jsonValueExpiresDate = objRecord.value("expiresDate").toString();
                //qDebug() << jsonValueCreatedDate << jsonValueUpdatedDate << jsonValueExpiresDate << endl;
                ui->lbCreateDate->setStyleSheet("color: #FFFFFF; font-weight:bold");
                ui->lbUpdateDate->setStyleSheet("color: #FFFFFF; font-weight:bold");
                ui->lbExpiresDate->setStyleSheet("color: #FFFFFF; font-weight:bold");
                ui->lbCreateDate->adjustSize();
                ui->lbUpdateDate->adjustSize();
                ui->lbExpiresDate->adjustSize();
                ui->lbCreateDate->setText("CreatedDate:  " + objRecord.value("createdDate").toString());
                ui->lbUpdateDate->setText("UpdatedDate:  " + objRecord.value("updatedDate").toString());
                ui->lbExpiresDate->setText("ExpiresDate:  " + objRecord.value("expiresDate").toString());
                QJsonValue jsonValueReg = objRecord.value("registrant");
                qDebug() << !jsonValueReg.isNull() << endl;
                if(!jsonValueReg.isNull())
                {
                    if(jsonValueReg.isObject())
                    {
                        QJsonObject objReg = jsonValueReg.toObject();
                        QJsonValue jsonValueOrganization = objReg.value("organization");
                        QJsonValue jsonValueState = objReg.value("state");
                        QJsonValue jsonValueCountry = objReg.value("country");
                        qDebug() << jsonValueOrganization << jsonValueState << jsonValueCountry << endl;
                        ui->lbReOrganization->setStyleSheet("color: #FFFFFF; font-weight:bold");
                        ui->lbReStatus->setStyleSheet("color: #FFFFFF; font-weight:bold");
                        ui->lbReCountry->setStyleSheet("color: #FFFFFF; font-weight:bold");
                        ui->lbReOrganization->setText("Organization:  " + objReg.value("organization").toString());
                        ui->lbReStatus->setText("State:  " + objReg.value("state").toString());
                        ui->lbReCountry->setText("Country:  " + objReg.value("country").toString());
                    }
                }
                else
                {
                    ui->lbReOrganization->setStyleSheet("color: #d54623");
                    ui->lbReOrganization->setText("未能查询到：Registrant");
                }

                QJsonValue jsonValueAdContact = objRecord.value("administrativeContact");
                if(!jsonValueAdContact.isNull())
                {
                    if(jsonValueAdContact.isObject())
                    {
                        QJsonObject objAdContact = jsonValueAdContact.toObject();
                        QJsonValue jsonValueOrganization = objAdContact.value("organization");
                        QJsonValue jsonValueState = objAdContact.value("state");
                        QJsonValue jsonValueCountry = objAdContact.value("country");
                        //qDebug() << jsonValueOrganization << jsonValueState << jsonValueCountry << endl;

                        ui->lbAdOrganization->setStyleSheet("color: #FFFFFF; font-weight:bold");
                        ui->lbAdState->setStyleSheet("color: #FFFFFF; font-weight:bold");
                        ui->lbAdCountry->setStyleSheet("color: #FFFFFF; font-weight:bold");
                        ui->lbAdOrganization->setText("Organization:  " + objAdContact.value("organization").toString());
                        ui->lbAdState->setText("State:  " + objAdContact.value("state").toString());
                        ui->lbAdCountry->setText("Country:  " + objAdContact.value("country").toString());
                    }
                }
                else
                {
                   ui->lbAdOrganization->setStyleSheet("color: #d54623");
                   ui->lbAdOrganization->setText("未能查询到：AdministrativeContact");
                }

                QJsonValue jsonValueNameServers = objRecord.value("nameServers");
                if(jsonValueNameServers.isObject())
                {
                    QJsonObject objNameServers = jsonValueNameServers.toObject();
                    QJsonValue jsonValueHostName = objNameServers.value("hostNames");

                    if(!jsonValueHostName.isNull())
                    {
                        if(jsonValueHostName.isArray())
                        {
                            ui->lbNameServer->setStyleSheet("color: #FFFFFF; font-weight:bold");
                            //QJsonObject objHostName = jsonValueHostName.toObject();
                            QJsonArray jsonArrayHostName = jsonValueHostName.toArray();
                            QString strName = "";
                            for (int i = 0; i < jsonArrayHostName.size(); ++i)
                            {
                                QJsonValue jsonValueHosti = jsonValueHostName[i];
                                strName += jsonValueHosti.toString() + ";  ";
                                qDebug() << jsonValueHosti << endl;
                            }
                            if(jsonArrayHostName.size())
                                ui->lbNameServer->setText(strName);
                            else
                                ui->lbNameServer->setText("HostName查询结果为空！");
                             //qDebug() << jsonValueHost0 << jsonValueHost1 << jsonValueHost2 << jsonValueHost3 << endl;
                        }
                    }
                    else
                    {
                        ui->lbNameServer->setStyleSheet("color: #d54623");
                        ui->lbNameServer->setText("未能查询到： HostName。");
                    }

                    QJsonValue jsonValueIps = objNameServers.value("ips");
                    if(!jsonValueIps.isNull())
                    {
                        QString strIPs = "";
                        if(jsonValueIps.isArray())
                        {
                            ui->lbIPs->setStyleSheet("color: #FFFFFF; font-weight:bold");
                            QJsonArray jsonArrayIp = jsonValueIps.toArray();
                            //QJsonArray jsonArrayIp = jsonValueHostName.toArray();
                            for (int i = 0; i < jsonArrayIp.size(); ++i)
                            {
                                QJsonValue jsonValueHosti = jsonValueHostName[i];
                                qDebug() << jsonValueHosti << endl;
                                strIPs +=  jsonValueHosti.toString() + ";  " ;
                            }
                            if(jsonArrayIp.size())
                                ui->lbIPs->setText(strIPs);
                            else
                                ui->lbIPs->setText("IP查询结果为空！");
                        }
                    }
                    else
                    {
                        ui->lbIPs->setStyleSheet("color: #d54623");
                        ui->lbIPs->setText("未能查询到： IP。");
                    }
                }
                //registryData
                QJsonValue jsonValueReDate = objRecord.value("registryData");
                if(!jsonValueReDate.isNull())
                {
                    if(jsonValueReDate.isObject())
                    {
                        QJsonObject objReDate = jsonValueReDate.toObject();
                        ui->lbReDateName->setStyleSheet("color: #FFFFFF; font-weight:bold");
                        ui->lbReDateIANAID->setStyleSheet("color: #FFFFFF; font-weight:bold");
                        ui->lbReDateWhois->setStyleSheet("color: #FFFFFF; font-weight:bold");
                        ui->lbReDateName->setText("RegistrarName:  " + objReDate.value("registrarName").toString());
                        ui->lbReDateIANAID->setText("RegistrarIANAID:  " + objReDate.value("registrarIANAID").toString());
                        ui->lbReDateWhois->setText("whoisServer:  " + objReDate.value("whoisServer").toString());
    //                    QJsonValue jsonValueAudit = objReDate.value("audit");
    //                    if(jsonValueAudit.isObject())
    //                    {
    //                        QJsonObject objAudit = jsonValueAudit.toObject();
    //                        ui->lbReDateName->setText(objAudit.value("registrarName").toString());
    //                        ui->lbReDateIANAID->setText(objAudit.value("registrarIANAID").toString());
    //                        ui->lbReDateWhois->setText(objAudit.value("whoisServer").toString());
    //                    }
                    }
                }
                else
                {
                   ui->lbReDateName->setStyleSheet("color: #d54623");
                   ui->lbReDateName->setText("未能查询到：registryData。");
                }

                QJsonValue jsonValueContactEmail = objRecord.value("contactEmail");
                if(!jsonValueContactEmail.isNull())
                {
                    ui->lbContactEmail->setStyleSheet("color: #FFFFFF; font-weight:bold");
                    ui->lbContactEmail->setText("ContactEmail:  " + objRecord.value("contactEmail").toString());
                }
                else
                {
                    ui->lbContactEmail->setStyleSheet("color: #d54623");
                    ui->lbContactEmail->setText("未能查询到： ContactEmail。");
                }

            }
         }
        else
        {
            qDebug() << "不是一个对象" << endl;
        }
    }
    else
    {
        qDebug() << "json 格式错误" << endl;
    }
}

void WebDirScan::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void WebDirScan::dropEvent(QDropEvent *event)
{
    isFile = 1;
    ui->CboxMineRoute->setCheckState(Qt::Checked);
    QList<QUrl> urls = event->mimeData()->urls();
    qDebug() << urls << endl;
    if(urls.isEmpty())
        return;
    foreach(QUrl url, urls)
    {
        QString fileName = url.toLocalFile();
        ui->leMessRoute->setText(fileName);
    }
}

void WebDirScan::showEvent(QShowEvent *)
{
    QPixmap ico(":/resouse/checklogo.png");
    QPixmap hideico(":/resouse/logo.png");
    switch (ui->tabWidget->currentIndex())
    {
     case 0 :
        ui->tabWidget->setTabIcon(0, ico);
        ui->tabWidget->setTabIcon(1,hideico);
        ui->tabWidget->setTabIcon(2,hideico);
        break;
     case 1 :
        ui->tabWidget->setTabIcon(1, ico);
        ui->tabWidget->setTabIcon(0,hideico);
        ui->tabWidget->setTabIcon(1,hideico);
        break;
     case 2 :
        ui->tabWidget->setTabIcon(2, ico);
        ui->tabWidget->setTabIcon(0,hideico);
        ui->tabWidget->setTabIcon(1,hideico);
        break;

    }
}

void WebDirScan::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    QPixmap ico(":/resouse/checklogo.png");
    QPixmap hideico(":/resouse/logo.png");
    switch (ui->tabWidget->currentIndex())
    {
     case 0 :
        ui->tabWidget->setTabIcon(0, ico);
        ui->tabWidget->setTabIcon(1,hideico);
        ui->tabWidget->setTabIcon(2,hideico);
        break;
     case 1 :

        ui->tabWidget->setTabIcon(0,hideico);
        ui->tabWidget->setTabIcon(1, ico);
        ui->tabWidget->setTabIcon(2,hideico);

        break;
     case 2 :

        ui->tabWidget->setTabIcon(0,hideico);
        ui->tabWidget->setTabIcon(2, ico);
        ui->tabWidget->setTabIcon(1,hideico);
        break;

    }
}

void WebDirScan::setHeader(int index)
{
    qDebug() << index << endl;
    switch (index) {
     case 0:
        ui->lbMessHeadText->setText("User-Agent:Mozilla/5.0 (compatible; Baiduspider/2.0;+http://www.baidu.com/search/spider.html）");
        break;
    case 1:
       ui->lbMessHeadText->setText("User-Agent:Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.117 Safari/537.36");
       break;
    case 2:
       ui->lbMessHeadText->setText("User-Agent:Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/534+ (KHTML, like Gecko) BingPreview/1.0b");
       break;
    case 3:
       ui->lbMessHeadText->setText("User-Agent: Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; 360SE)");
       break;
    case 4:
       ui->lbMessHeadText->setText("User-Agent: Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; Trident/4.0; SE 2.X MetaSr 1.0; SE 2.X MetaSr 1.0; .NET CLR 2.0.50727; SE 2.X MetaSr 1.0)");
       break;
    default :
       ui->lbMessHeadText->setText("User-Agent:Mozilla/5.0 (compatible; Baiduspider/2.0;+http://www.baidu.com/search/spider.html）");
       break;
     }

}
