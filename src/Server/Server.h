/**
 * @file Server.h
 * @author ambition_echo (ambition_echo@outlook.com)
 * @brief  IRC服务器类头文件
 * @version 0.1
 * @date 2022-05-09
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once
#include <QDebug>
#include <QObject>
#include <QtNetwork/QSslSocket>
#include <QtNetwork/QTcpSocket>
/**
 * @brief IRC服务器类
 *
 */
class Server : public QObject
{
    Q_OBJECT
  signals:
    void connectedSuccess(); ///<服务器连接成功
    void connectedFail();    ///<服务器连接失败
    void disconnected();     ///<服务器断开连接
  public:
    enum Status
    {
        Connected,
        Disconnected,
    };
    enum Type
    {
        Tcp,
        Ssl,
    };
    Server(QObject *parent = nullptr);
    ~Server() override;

  private:
    QTcpSocket *_tcpsocket; // tcp socket
    QSslSocket *_sslsocket; // ssl socket
};
