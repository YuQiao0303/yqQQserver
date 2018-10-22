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
    mp_TCPSocket = mp_TCPServer->nextPendingConnection(); //
    if(!mp_TCPSocket)
    {
        QMessageBox::information(this, "QT网络通信", "未正确获取客户端连接！");
        return;
    }
    else
    {
        QMessageBox::information(this, "QT网络通信", "成功接受客户端的连接");
        //QT通过信号SIGNAL:readyRead()来判断是否有数据传入
        connect(mp_TCPSocket, SIGNAL(readyRead()), this, SLOT(ServerReadData()));
        connect(mp_TCPSocket, SIGNAL(disconnected()), this, SLOT(sServerDisConnection()));
    }
}
//接收数据
void MainWindow::ServerReadData(){
    char buffer[1024] = {0};
    mp_TCPSocket->read(buffer, 1024);
    if( strlen(buffer) > 0)
    {
        QString newMsg = buffer;
        ui->m_recvDataTextEdit->append(newMsg);
        //广播信息
        if(ui->m_recvDataTextEdit->toPlainText().isEmpty())
        {
            //QMessageBox::information(this, "QT网络通信", "发送数据为空，请输入数据");
            return;
        }
        //char sendMsgChar[1024] = {0};
        strcpy_s(buffer, ui->m_recvDataTextEdit->toPlainText().toStdString().c_str());
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
    else
    {
        QMessageBox::information(this, "QT网络通信", "未正确接收数据");
        return;
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
//连接断开提醒
void MainWindow::sServerDisConnection(){
    QMessageBox::information(this, "QT网络通信", "与客户端的连接断开");
    return;
}