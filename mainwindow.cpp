#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->m_portLineEdit->setText("5550");
    //pushButton2->setShortcut(QKeySequence(QLatin1String(“Left”)));
    connect(ui->m_initSocketBtn, SIGNAL(clicked()), this, SLOT(OnBtnInitSocket()));
    connect(ui->m_sendData, SIGNAL(clicked()), this, SLOT(OnBtnSendData()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
/*------------------slots--------------------*/
void MainWindow::ServerNewConnection(){
    //通过nextPendingConnection()函数获得连接客户端的SOCKET套接字
    QTcpSocket *mp_TCPSocket = mp_TCPServer->nextPendingConnection(); //
    if(!mp_TCPSocket)
    {
        QMessageBox::information(this, "QT网络通信", "未正确获取客户端连接！");
        return;
    }
    else
    {
        //QMessageBox::information(this, "QT网络通信", "成功接受客户端的连接");
        ui->m_recvDataTextEdit->append("成功接受客户端");
        ui->m_recvDataTextEdit->append(QString("%1").arg(mp_TCPSocketList.count()));
        //QT通过信号SIGNAL:readyRead()来判断是否有数据传入
        connect(mp_TCPSocket, SIGNAL(readyRead()), this, SLOT(ServerReadData()));
        connect(mp_TCPSocket, SIGNAL(disconnected()), this, SLOT(sServerDisConnection()));
        mp_TCPSocketList.append(mp_TCPSocket);

    }
}
//接收数据
void MainWindow::ServerReadData(){
    char buffer[1024] = {0};
    QTcpSocket *mp_TCPSocket;
    int MsgSender=-1; //用于保存发送该信息的客户端在mp_TCPSocketList中的序号
    for(int i=0; i<mp_TCPSocketList.count(); i++)  //对每一个连接的客户端,接收数据
    {
        mp_TCPSocket = mp_TCPSocketList.at(i);
        buffer[1024] = {0};
        mp_TCPSocket->read(buffer, 1024);
        if(strlen(buffer) > 0)
        {
            QString newMsg = buffer;
            ui->m_recvDataTextEdit->append(newMsg);
            MsgSender=i;  //标记发送该信息的客户端在mp_TCPSocketList中的序号
            break;
        }
        else
        {
            //QMessageBox::information(this, "QT网络通信", QString("未正确接收数据%1").arg(i));
            //return;
        }
    }

    for(int i=0; i<mp_TCPSocketList.count(); i++)
    {
        //广播信息
        if(i==MsgSender) continue;
        mp_TCPSocket = mp_TCPSocketList.at(i);
        if(mp_TCPSocket->isValid())
        {
            int sendRe = mp_TCPSocket->write(buffer, strlen(buffer));
            if( -1 == sendRe)
            {
                QMessageBox::information(this, "QT网络通信", "服务端发送数据失败！");
            }
        }
        else
        {
            QMessageBox::information(this, "QT网络通信", "套接字无效！");
        }
    }
}
//初始化socket
void MainWindow::OnBtnInitSocket(){
    mp_TCPServer = new QTcpServer();
    int port = ui->m_portLineEdit->text().toInt();
    if(!mp_TCPServer->listen(QHostAddress::Any, port))
    {
        QMessageBox::information(this, "QT网络通信", "服务器端监听失败！");
        return;
    }
    else
    {
        QMessageBox::information(this, "QT网络通信", "服务器监听成功！");
    }
    //服务端通过信号 SIGNAL:newConnection() 来判断是否接收到了新的连接
    connect(mp_TCPServer, SIGNAL(newConnection()), this, SLOT(ServerNewConnection()));

}
//发送数据
void MainWindow::OnBtnSendData(){
    for(int i=0; i<mp_TCPSocketList.count(); i++)  //对每一个连接的客户端
    {
        QTcpSocket *mp_TCPSocket = mp_TCPSocketList.at(i);
        char sendMsgChar[1024] = {0};
        QString sendMsg = ui->m_recvDataTextEdit->toPlainText();
        if(sendMsg.isEmpty())
        {
            QMessageBox::information(this, "QT网络通信", "发送数据为空，请输入数据");
            return;
        }
        strcpy_s(sendMsgChar, sendMsg.toStdString().c_str());
        if(mp_TCPSocket->isValid())
        {
            int sendRe = mp_TCPSocket->write(sendMsgChar, strlen(sendMsgChar));
            if( -1 == sendRe)
            {
                QMessageBox::information(this, "QT网络通信", "服务端发送数据失败！");
            }
        }
        else
        {
            QMessageBox::information(this, "QT网络通信", "套接字无效！");
        }
    }
}
//连接断开提醒
void MainWindow::sServerDisConnection(){
    //QMessageBox::information(this, "QT网络通信", "与客户端的连接断开");
    ui->m_recvDataTextEdit->append("与客户端的连接断开");
    return;
}
