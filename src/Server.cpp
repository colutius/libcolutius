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
    this->_socket = new QTcpSocket;
    this->_initConnect();
}
/**
 * @brief 有参构造
 * @param type 服务器连接类型 Tcp或Ssl
 * @param parent 默认nullptr
 */
Server::Server(Server::Type type, QObject *parent)
{
    this->_type = type;
    switch (this->_type)
    {
    case Tcp:
        this->_socket = new QTcpSocket;
    case Ssl:
        this->_socket = new QSslSocket;
    }
    this->_initConnect();
}
/**
 * @brief 默认析构函数
 */
Server::~Server() = default;
/**
 * @brief 初始化信号槽
 */
void Server::_initConnect()
{
    //网络检查正常后连接到服务器
    connect(this, &Server::networkOk, this, &Server::_connect);
    //连接成功后发送登录信息
    connect(this->_socket, &QAbstractSocket::connected, this, &Server::_login);
    //接收数据
    connect(this->_socket, &QAbstractSocket::readyRead, this, &Server::receiveData);
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
    QHostInfo::lookupHost(this->_host, this, SLOT(_checkNetWork(QHostInfo)));
}
void Server::_connect()
{
    this->_socket->connectToHost(this->_host, this->_port);
    qDebug() << "开始登录";
}
/**
 * @brief 检查网络连接状态
 */
void Server::_checkNetWork(QHostInfo host)
{
    if (host.error() != QHostInfo::NoError)
    {
        qDebug() << "网络连接异常" << host.errorString();
        emit networkError();
    }
    else
    {
        qDebug() << "网络连接正常,开始连接到服务器";
        emit networkOk();
    }
}
void Server::_login()
{
    if (!this->_passwd.isEmpty())
    {
        this->_sendData("PASS " + this->_passwd);
    }
    this->_sendData("NICK " + this->_nick);
    this->_sendData("USER " + this->_user + " 8 * :Colutius IRC Client");
}
/**
 * @brief
 */
void Server::_sendData(QString data)
{
    QByteArray buf = (data + "\r\n").toUtf8();
    this->_socket->write(buf);
}
void Server::setHost(QString host)
{
    this->_host = host;
}
void Server::setPort(int port)
{
    this->_port = port;
}
void Server::setNick(QString nick)
{
    this->_nick = nick;
}
void Server::setUser(QString user)
{
    this->_user = user;
}
void Server::setPasswd(QString passwd)
{
    this->_passwd = passwd;
}
/**
 * @brief 接收数据
 */
void Server::receiveData()
{
    //分割多行消息
    QStringList buf = QString(this->_socket->readAll()).split("\r\n");
    //遍历多行消息
    foreach (auto i, buf)
    {
        //确保此行消息非空
        if (i.isEmpty())
        {
            continue;
        }
        //创建消息实例
        auto *message = new Message(i);
        //按消息发送者分类处理
        switch (message->getMsgSender())
        {
            //服务器消息
        case Message::Server:
            switch (message->getMsgType())
            {
            case Message::None:
                qDebug() << message->getRawMsg();
                break;
            case Message::Ping:
                _sendData("PONG :" + message->getMainMsg());
                qDebug() << "PONG :" + message->getMainMsg();
                break;
            case Message::Error:
                qDebug() << "ERROR " + message->getMainMsg();
                break;
            case Message::Notice:
                qDebug() << "NOTICE " + message->getMainMsg();
                break;
            case Message::Join:
                break;
            case Message::Nick:
                break;
            case Message::Kick:
                break;
            case Message::Num:
                qDebug() << QString::number(message->getNum()) + " " + message->getMainMsg();
                break;
            case Message::Private:
                break;
            case Message::Channel:
                break;
            default:
                break;
            }
            break;
            //用户消息
        case Message::User:
            switch (message->getMsgType())
            {
            case Message::Channel:
                break;
            case Message::Private:
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
}
