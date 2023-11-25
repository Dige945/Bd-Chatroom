#include "widget.h"
#include "ui_widget.h"
#include "privatechatwidget.h"
#include <QDebug>

Widget::Widget(QString i,QWidget *parent)
    : QWidget(parent),userid(i)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->UserName->setText(userid);
    qDebug()<<userid;
    socket = new QTcpSocket;
    connectstatement=false;
    this->setFixedSize(1184,891);
    //设置固定窗口大小
    this->setWindowTitle("Bd Chatroom_Client");
    //设置窗口名称

    //连接服务器；v
    socket->connectToHost("127.0.0.1",8001);
    pause(500);

    connect(socket,&QTcpSocket::readyRead,this,&Widget::readMessage);
    connect(socket,&QTcpSocket::disconnected,this,&Widget::disConnectslot);
    //connect(ui->Connect_pushButton,&QPushButton::clicked,this,&Widget::on_ConnectpushButton_clicked);
    connect(ui->Send_pushButton,&QPushButton::clicked,this,&Widget::on_SendpushButton_clicked);
    this->setWindowIcon(QIcon(":/new/prefix1/ico.ico"));
    //设置窗口图标名称
    //发送登录信息
    QString loginmessage= "USERNAME:" +userid;
    socket->write(loginmessage.toUtf8());

    connect(ui->usernametable, &QTableWidget::itemDoubleClicked, this, &Widget::startprivatechat);


}


//时间暂停
void Widget::pause(int milliseconds) {
    QTimer timer;
    QEventLoop loop;
    QObject::connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    timer.start(milliseconds);
    loop.exec();
}
//时间暂停装置
void Widget::readMessage(){
    QByteArray arr = socket->readAll();
    QString str = arr.data();
    //读取在线人数
    if (str.startsWith("USERCOUNT:"))
    {
        str.remove("USERCOUNT:");
        ui->ClientNumber->setText(str);
        socket->flush();

    }
    if (str.startsWith("USERLISTUPDATE:")) {
            str.remove("USERLISTUPDATE:");
            updateuserlist(str);
            socket->flush();
    }
    if(str.startsWith("USERMESSAGE:"))
    {
        str.remove("USERMESSAGE:");
        ui->textBrowser->append(str);
    }
    if(str.startsWith("SERVERMESSAGE:"))
    {
        str.remove("SERVERMESSAGE:");
        str= "【来自服务器管理员的私聊信息】\n" + str;
        ui->textBrowser->append(str);
    }
    if(str.startsWith("PRIVATEMESSAGE:"))
    {
        str.remove("PRIVATEMESSAGE:");
        ui->textBrowser->append(str);
    }
}
void Widget::on_SendpushButton_clicked(){
    QString user;
    user = userid;
    QString str = user + ':'+ '\n'+ ui->textEdit->toPlainText();
    if(socket->isOpen() && socket->isValid()){
        socket->write("USERMESSAGE:"+str.toUtf8());
        ui->textEdit->clear();
    }

    QString showstring;
    showstring=QDateTime::currentDateTime().toString("dddd-yyyy-MM-dd hh:mm:ss")
            + '\n' +str;
    ui->textBrowser->append(showstring);

}

/*void Widget::on_ConnectpushButton_clicked(){

    QString ipaddress = ui->IPAddress->toPlainText();
    quint16 port = ui -> Port->toPlainText().toUInt();
    if(connectstatement==false){
        socket->connectToHost(ipaddress,port);
        if(socket->waitForConnected()){
            ui->Connect_pushButton->setText("Disconnect");
            QString strconnect = QDateTime::currentDateTime().toString("dddd-yyyy-MM-dd hh:mm:ss")
                    + '\n' + "Successful connecting!";
            ui->textBrowser->append(strconnect);
            connectstatement=true;
        }
        else QMessageBox::information(this,"Warning","Connecting failed!");
    }
    else
    {
        connectstatement=false;
        socket->disconnectFromHost();
        ui->Connect_pushButton->setText("Connect");
        QString strdisconnect = QDateTime::currentDateTime().toString("dddd-yyyy-MM-dd hh:mm:ss")
                + '\n' + "Already disconnecting!";
        ui->textBrowser->append(strdisconnect);

    }

}
*/
void Widget::disConnectslot(){
    ui->textBrowser->append("Client disconnected.");
}

void Widget::setuserid(QString &id){
    userid = id;
}

void Widget::updateuserlist(const QString& userListStr) {
    ui->usernametable->clear();
    QStringList userList = userListStr.split(",");
    ui->usernametable->setRowCount(userList.size());
    for (int i = 0; i < userList.size(); ++i) {
        ui->usernametable->setItem(i, 0, new QTableWidgetItem(userList[i]));
    }
}


void Widget::startprivatechat(QTableWidgetItem* item){
    QString username = item->text();
    Privatechatwidget *privatechat = new Privatechatwidget(username,socket,userid);
    privatechat->setWindowTitle("与"+username+"聊天");
    privatechat->show();
}
Widget::~Widget()
{
    delete ui;
}

