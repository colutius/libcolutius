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
#include <QSslSocket>
#include <QTcpSocket>
/**
 * @brief IRC服务器类
 *
 */
class Server : public QObject
{
    Q_OBJECT
  signals:
    void loginSuccess(); ///<服务器连接成功
    void loginFail();    ///<服务器连接失败
    void disconnected(); ///<服务器断开连接
  public:
    /**
     * @brief 服务器当前状态
     */
    enum Status
    {
        Connected,    ///<服务器已连接
        Disconnected, ///<服务器断开连接
    };
    /**
     * 服务器连接类型
     */
    enum Type
    {
        Tcp, ///< TCP Socket
        Ssl, ///< SSL Socket
    };
    explicit Server(QObject *parent = nullptr);
    Server(Type type, QObject *parent = nullptr);
    ~Server() override;
    void login();
    void setServerInfo();
    void setHost();
    void setPort();
    void setNick();
    void setUser();
    void setPasswd();

  private:
    void _checkNetWork();   //检查网络连接情况
    void _sendData();       //向服务器发送数据
    void _initConnect();    //初始化信号槽
    void _connect();        //连接到服务器
    void _login();          //登录到服务器
    QString _host;          //服务器地址
    int _port = 0;          //服务器端口号
    QString _nick;          //昵称
    QString _user;          //用户名
    QString _passwd;        //密码
    QTcpSocket *_tcpsocket; // tcp socket
    QSslSocket *_sslsocket; // ssl socket
    QAbstractSocket *_socket;
    Status _status; //服务器状态
    Type _type;     //服务器连接类型
};
