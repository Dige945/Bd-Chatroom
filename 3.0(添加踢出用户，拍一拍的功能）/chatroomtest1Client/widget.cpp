#include "widget.h"
#include "ui_widget.h"
#include "privatechatwidget.h"
#include <QDebug>
#include <QString>
#include <QMenu>

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
    ui->qingxie->setIcon(QIcon(":/new/prefix1/qinxie.jpg"));
    ui->jiacu->setIcon(QIcon(":/new/prefix1/jiacu/jpg"));
    //设置窗口图标名称
    //发送登录信息


    QString loginmessage= "USERNAME:" +userid;
    socket->write(loginmessage.toUtf8());

    contextMenu = new QMenu(this);//用于存储右键菜单的实例
    QAction* pat = new QAction("拍一拍", this);
    //往menu里面添加拍一拍这个动作
    connect(pat,&QAction::triggered,this,&Widget::on_pat_clicked);
    contextMenu->addAction(pat);
    //实现右键打开菜单
    ui->usernametable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->usernametable, &QTableWidget::customContextMenuRequested, this, &Widget::showContextMenu);

    connect(ui->usernametable, &QTableWidget::itemDoubleClicked, this, &Widget::startprivatechat);
    //设置字体的倾斜和加粗的按钮与函数连接
    connect(ui->jiacu,&QToolButton::clicked,this,&Widget::on_jiacu_clicked);
    connect(ui->qingxie,&QToolButton::clicked,this,&Widget::on_qingxie_clicked);
    connect(ui->fontComboBox,&QFontComboBox::currentFontChanged,this,&Widget::on_fontComboBox_currentFontChanged);
    connect(ui->comboBox,&QComboBox::currentTextChanged,this,&Widget::on_FontSizeChanged);
}
//加粗与倾斜
void Widget::on_jiacu_clicked(){
    QString sendtext = ui->textEdit->toPlainText();
    sendtext = sendtext.replace("<b>", "").replace("</b>", "");
    QString newstyle = ui->jiacu->isChecked()?"<b>":"";
    sendtext = newstyle+sendtext+newstyle;
    ui->textEdit->setText(sendtext);
}
void Widget::on_qingxie_clicked(){
    QString sendtext = ui->textEdit->toPlainText();
    sendtext = sendtext.replace("<i>", "").replace("</i>", "");
    QString newstyle = ui->qingxie->isChecked()?"<i>":"";
    sendtext = newstyle+sendtext+newstyle;
    ui->textEdit->setText(sendtext);
}
//加粗与倾斜

//字体更换
void Widget::on_fontComboBox_currentFontChanged(const QFont &font) {
    QTextCharFormat charFormat = ui->textEdit->currentCharFormat();
    charFormat.setFont(font);
    ui->textEdit->setCurrentFont(font);
}

//字体大小的修改

void Widget::on_FontSizeChanged(const QString &size){
    int fontSize = size.toInt();
    QTextCharFormat charFormat = ui->textEdit->currentCharFormat();
    // 设置字体大小
    charFormat.setFontPointSize(fontSize);
    // 将新的文本格式应用到 textEdit
    ui->textEdit->setCurrentCharFormat(charFormat);
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
        qDebug() << "Received USERMESSAGE";
        QRegExp fontRegex("<font\\s+face=['\"](.*?)['\"]\\s+size=['\"](\\d+)['\"]>(.*?)</font>");
        if (fontRegex.indexIn(str) != -1) {
            QString fontName = fontRegex.cap(1);
            int fontSize = fontRegex.cap(2).toInt();
            // 可以在这里获取字体信息并进行处理

            // 构建包含字体信息的消息
            QString fontInfo = QString("<font face='%1' size='%2'>").arg(fontName).arg(fontSize);
            str = fontInfo + fontRegex.cap(3) + "</font>";
        }
        QString showstr=QDateTime::currentDateTime().toString("dddd.yyyy.MM.dd HH:mm:ss") +"<br>"+ str;
        ui->textBrowser->setHtml(ui->textBrowser->toHtml()+showstr);
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
    if(str.startsWith("PATMESSAGE:"))
    {
        str.remove("PATMESSAGE:");
        ui->textBrowser->append(str);
    }
}
void Widget::on_SendpushButton_clicked(){
    QString user;
    user = userid;
    QString message = ui->textEdit->toPlainText();
    //获取当前的文本格式（包含了所有信息，待会再从中提取具体信息）
    QTextCharFormat charFormat = ui->textEdit->currentCharFormat();
    //获取字体信息（华文，宋体等等）
    QString fontName = charFormat.font().family();
    //获取字体大小
    int fontSize = ui->comboBox->currentText().toInt();
    // 构建包含字体信息的消息
        QString formattedMessage = QString("<font face='%1' size='%2'>%3</font>")
            .arg(fontName)
            .arg(fontSize)
            .arg(message);
        //arg用于将%1%2%3等替换
    QString str = user + ':'+ formattedMessage;
    if(socket->isOpen() && socket->isValid()){
        socket->write("USERMESSAGE:"+str.toUtf8());
        ui->textEdit->clear();
    }

    QString showstring;
    showstring=QDateTime::currentDateTime().toString("dddd-yyyy-MM-dd hh:mm:ss")
            + "<br>" +str;
    ui->textBrowser->setHtml(ui->textBrowser->toHtml()+showstring);

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

void Widget::on_pat_clicked(){
    pat(currentRightClickedUsername);
}
void Widget::pat(const QString& username){
    QString str;
    str= "                   "+userid + " 拍了拍 " + username + " 的屁屁";
    socket->write("PATMESSAGE:"+str.toUtf8());
    ui->textBrowser->append(str);
}

void Widget::showContextMenu(const QPoint &pos){
    QTableWidgetItem *currentItem = ui->usernametable->itemAt(pos);
    if (currentItem != nullptr) {
        currentRightClickedUsername = currentItem->text();
        contextMenu->exec(ui->usernametable->mapToGlobal(pos));
    }
}
