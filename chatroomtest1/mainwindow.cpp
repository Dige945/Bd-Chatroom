#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "privatechatwindow.h"
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->listenbtn->setStyleSheet("color:blue");
    server=new QTcpServer(this);
    this->enumAllIp();
    this->setFixedSize(1114,772);
    this->setWindowTitle("Bd Chatroom_Server");
    this->setWindowIcon(QIcon(":/new/prefix1/ico.ico"));
    connect(server,&QTcpServer::newConnection,this,&MainWindow::newConnectSlot);
    connect(ui->listenbtn,&QPushButton::clicked,this,&MainWindow::on_listeningbtn_clicked);
    connect(this,&MainWindow::usernumbermodify,this,&MainWindow::on_usernumbermodify);
    connect(this,&MainWindow::userlistupdated,this,&MainWindow::updateuserlist);

}

MainWindow::~MainWindow()
{
    delete ui;
}

//时间暂停
void MainWindow::pause(int milliseconds) {
    QTimer timer;
    QEventLoop loop;
    QObject::connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    timer.start(milliseconds);
    loop.exec();
}
//时间暂停装置
int MainWindow::clientnum = 0;
void MainWindow::enumAllIp(){
    QList <QHostAddress> addressList = QNetworkInterface::allAddresses();
    QStringList addressList_str;
    for(int i=0;i<addressList.size();i++){
        if(addressList.at(i).isNull()) continue;
        if(addressList.at(i).protocol()!=QAbstractSocket::IPv4Protocol) continue;
        addressList_str.append(addressList.at(i).toString());
    }
    ui->IPbox->addItems(addressList_str);
}
void MainWindow::newConnectSlot(){
    clientsock = server->nextPendingConnection();
    clientList_sock.push_back(clientsock);
    QString str=clientsock->peerAddress().toString() + ":" + QString::number(clientsock->peerPort())+"connect!";
    ui->listWidget->addItem(str);
    connect(clientsock, &QTcpSocket::readyRead, this ,&MainWindow::readMessage);
    connect(clientsock,&QTcpSocket::disconnected,this,&MainWindow::disConnectSlot);
    //在线人数++
    clientnum++;
    QString showclientnum;
    showclientnum = clientnum;
    ui->ClientNumber->setText(QString::number(clientnum));

    emit usernumbermodify();
    pause(1000);
    clientsock->flush();
    //发送在线人数给客户端
    //在线人数
}
void MainWindow::readMessage()
{
    QTcpSocket *currentClient = NULL;
    QByteArray arr;
    QString str;
    if(!clientList_sock.isEmpty()){
        for(int i=0;i<clientList_sock.count();i++){
            arr=clientList_sock.at(i)->readAll();
            str=arr.data();
            if(arr.isNull()) continue;
            //用户名的传输
            if(str.startsWith("USERNAME:")){
                str.remove("USERNAME:");
                userEnter(str);
                qDebug()<<str;
                ui->listWidget->addItem(str);
                socketUserMap[clientList_sock.at(i)] = str;
                str.clear();
                clientList_sock.at(i)->flush();
                pause(500);
                emit userlistupdated();
                break;
            }
            //用户发送公聊信息的传输
            if(str.startsWith("USERMESSAGE:"))
            {
            currentClient=clientList_sock.at(i);
            str.remove("USERMESSAGE:");
            QString showstr=QDateTime::currentDateTime().toString("dddd.yyyy.MM.dd HH:mm:ss") +'\n'+ str;
            ui->chatwidget->append(showstr);
            for(int i=0;i<clientList_sock.count();i++){
                if(currentClient == clientList_sock.at(i)){
                    continue;
                }
            clientList_sock.at(i)->write("USERMESSAGE:"+showstr.toUtf8());
            break;
            }
            }
            //用户发送私聊信息的处理
            if(str.startsWith("PRIVATECHAT")){
                QString chatusername;
                QString privatemessage;
                QString userid;
                str.remove("PRIVATECHAT");
                QStringList parts = str.split("::");
                if (parts.size() >= 2) {
                chatusername = parts[0];
                privatemessage = parts[1];
                userid=parts[2];
                }
                for(int j=0;j<clientList_sock.count();j++){
                    QString username = socketUserMap[clientList_sock.at(j)];
                    if(username == chatusername){
                        QString privatestr;
                        privatestr="【"+userid+"对你发的私聊信息"+"】"+"\n"+privatemessage;
                        clientList_sock.at(j)->write("PRIVATEMESSAGE:"+privatestr.toUtf8());
                        break;
                    }
                }
            }
        }
    }
}
void MainWindow::disConnectSlot(){
    QString closestr= QDateTime::currentDateTime().toString("dddd.yyyy.MM.dd HH:mm::ss")+ ":" +"disnected";
    ui->listWidget->addItem(closestr);
    //在线人数--
    clientnum--;
    ui->ClientNumber->setText(QString::number(clientnum));
    //在线人数
    emit usernumbermodify();
    clientsock->flush();
    pause(1000);
    QTcpSocket* client = qobject_cast<QTcpSocket*>(sender());
        if (client && socketUserMap.contains(client)) {
            QString username = socketUserMap[client];
            userout(username);
            socketUserMap.remove(client);
        }

    emit userlistupdated();
    clientsock->flush();
}

void MainWindow::on_listeningbtn_clicked(){
    QString currentIP = ui->IPbox->currentText();
    quint16 currentport = ui->Port->toPlainText().toInt();
    QHostAddress currentHostIP = QHostAddress(currentIP);
    if(server->isListening()){
        server->close();
        ui->listenbtn->setText("Listen");
    }
    else{
        if(server->listen(currentHostIP,currentport))
            ui->listenbtn->setText("close Listening");
        else
            QMessageBox::warning(this,"listen error",server->errorString());
    }
}
void MainWindow::on_sendbtn_clicked(){
    QString sendstr = ui->sendtext->toPlainText();
    QString showstr = QDateTime::currentDateTime().toString("dddd.yyyy.MM.dd HH:mm:ss") + ":" + '\n' + sendstr;
    ui->chatwidget->append(showstr);
    for(int i=0;i<clientList_sock.count();i++){
        clientList_sock.at(i)->write("USERMESSAGE:"+showstr.toUtf8());
        ui->sendtext->clear();
    }
}

void MainWindow::on_usernumbermodify(){
    for(int i=0;i<clientList_sock.count();i++){
        clientList_sock.at(i)->write("USERCOUNT:"+QString::number(clientnum).toUtf8());
        clientList_sock.at(i)->flush();
        clientList_sock.at(i)->write("\n");

    }
}

//加载客户端信息到窗口左边表格
void MainWindow::userEnter(QString username){
        //qDebug<<username;
        qDebug() << "Adding user: " << username;
        int row = ui->usernametable->rowCount(); // 获取当前行数
        ui->usernametable->setRowCount(row+1);
        ui->usernametable->setItem(row, 0, new QTableWidgetItem(username));
        // 在新行的第一列设置用户名
        connect(ui->usernametable,&QTableWidget::itemDoubleClicked,this,&MainWindow::startPrivatechat);
        qApp->processEvents();
        updateuserlist();
}

void MainWindow::userout(QString username) {
    for (int row = 0; row < ui->usernametable->rowCount(); ++row) {
        if (ui->usernametable->item(row, 0)->text() == username) {
            ui->usernametable->removeRow(row);
            break;
        }
    }
    clientsock->flush();
}
void MainWindow::updateuserlist(){
    QStringList userList;
        for (int i = 0; i < clientList_sock.size(); ++i) {
            QTcpSocket* socket = clientList_sock.at(i);
            QString username = socketUserMap[socket];
            if(username != "")
            userList << username;
        }
        //发送用户名信息给客户端
     QString userListMessage = "USERLISTUPDATE:" + userList.join(",");
         for (int i = 0; i < clientList_sock.size(); ++i) {
            QTcpSocket* socket = clientList_sock.at(i);
            socket->write(userListMessage.toUtf8());
            socket->flush();  // 添加这一行以刷新socke
            socket->write("\n");
        }
    pause(500);
}

void MainWindow::startPrivatechat(QTableWidgetItem *item){
    QString selectusername = item->text();
    //开始遍历socket的链表寻找目标socket
    for(int i=0;i<clientList_sock.count();i++){
        QTcpSocket *socket = clientList_sock.at(i);
        QString username = socketUserMap[socket];
        for(int i=0;i<clientList_sock.count();i++){
            if(selectusername == username){
                initPrivatechat(socket,selectusername,this);
                break;
            }
        }
    }
}

void MainWindow::initPrivatechat(QTcpSocket *selectedsocket,const QString& selectusername,MainWindow *server){
    selectedsocket->write("PRIVATECHAT:" + QByteArray::number(selectedsocket->socketDescriptor()));
    selectedsocket->flush();

    PrivateChatWindow *privateChat = new PrivateChatWindow(selectedsocket,server);
    privateChat->setWindowTitle("与 " + selectusername + " 私聊");
    privateChat->show();


}
