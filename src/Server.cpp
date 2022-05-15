/**
 * @file Server.cpp
 * @author ambition_echo (ambition_echo@outlook.com)
 * @brief IRC服务器类实现
 * @version 0.1
 * @date 2022-05-09
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "Server.h"
/**
 * @brief 无参构造，
 * 后续使用需要手动设置服务器host,port等参数
 * @param parent 默认nullptr
 */
Server::Server(QObject *parent) : QObject(parent)
{
    this->_tcpsocket = new QTcpSocket;
    this->_sslsocket = new QSslSocket;
}
Server::Server(Server::Type type, QObject *parent)
{
    this->_tcpsocket = new QTcpSocket;
    this->_sslsocket = new QSslSocket;
    this->_type = type;
    switch (this->_type)
    {
    case Tcp:
        this->_socket = this->_tcpsocket;
    case Ssl:
        this->_socket = this->_sslsocket;
    }
}
/**
 * @brief 默认析构函数
 */
Server::~Server() = default;
void Server::_initConnect()
{
    //连接成功后发送登录信息
    connect(this->_socket, &QAbstractSocket::connected, this, &Server::_login);
}
/**
 * @brief 连接到服务器并向服务器发送登录信息，如果登录成功，则发射loginSuccess信号
 */
void Server::login()
{
    //检查host、port
    if (this->_host.isEmpty() || this->_port == 0)
    {
        qDebug() << "没有设置host或port";
        return;
    }
    //检查nick、user
    if (this->_nick.isEmpty() || this->_user.isEmpty())
    {
        qDebug() << "没有设置nick或user";
        return;
    }
    //检查网络连接
    // TODO 检查网络连接
    this->_checkNetWork();
}
void Server::_connect()
{
    this->_socket->connectToHost(this->_host, this->_port);
}
void Server::_checkNetWork()
{
}