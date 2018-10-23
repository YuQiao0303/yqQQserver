#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTcpServer *mp_TCPServer;
    //QTcpSocket *mp_TCPSocket;
    QList<QTcpSocket *> mp_TCPSocketList;

private slots:
    void OnBtnInitSocket();
    void OnBtnSendData();
    void ServerReadData();
    void ServerNewConnection();
    void sServerDisConnection();
};

#endif // MAINWINDOW_H
