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
 * @brief 有参构造，
 * @param type 服务器连接类型，Tcp或Ssl,默认为Tcp
 * @param host 服务器地址，默认为空
 * @param port 服务器端口号，默认为0
 * @param nick 昵称，默认为空
 * @param user 用户名，默认为空
 * @param passwd 密码，默认为空
 * @param parent 默认为nullptr
 */
Server::Server(Server::Type type, QString host, int port, QString nick, QString user, QString passwd, QObject *parent)
{
    this->_status = Disconnected;
    this->_type = type;
    switch (this->_type)
    {
    case Tcp:
        this->_socket = new QTcpSocket;
    case Ssl:
        this->_socket = new QSslSocket;
    }
    this->setHost(std::move(host));
    this->setPort(port);
    this->setNick(std::move(nick));
    this->setUser(std::move(user));
    this->setPasswd(std::move(passwd));
    this->_initConnect();
}
/**
 * @brief 默认析构函数
 */
Server::~Server()
{
    //断开连接
    this->disconnect();
}
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
/**
 * @brief 连接服务器
 */
void Server::_connect()
{
    this->_socket->connectToHost(this->_host, this->_port);
    this->_status = Connected;
    qDebug() << "开始登录";
}
/**
 * @brief 检查网络连接状态
 */
void Server::_checkNetWork(const QHostInfo &host)
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
/**
 * @brief 发送登录信息
 */
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
 * @brief 发送数据
 */
void Server::_sendData(const QString &data)
{
    QByteArray buf = (data + "\r\n").toUtf8();
    this->_socket->write(buf);
}
/**
 * @brief 设置服务器地址
 * @param host 服务器地址
 */
void Server::setHost(QString host)
{
    this->_host = std::move(host);
}
/**
 * @brief 设置端口号
 * @param port 端口号
 */
void Server::setPort(int port)
{
    this->_port = port;
}
/**
 * @brief 设置昵称
 * @param nick 昵称
 */
void Server::setNick(QString nick)
{
    this->_nick = std::move(nick);
}
/**
 * @brief 设置用户名
 * @param user 用户名
 */
void Server::setUser(QString user)
{
    this->_user = std::move(user);
}
/**
 * @brief 设置密码
 * @param passwd 密码
 */
void Server::setPasswd(QString passwd)
{
    this->_passwd = std::move(passwd);
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
            //按消息类型分类处理
            switch (message->getMsgType())
            {
            case Message::None:
                this->_messageList.append(message);
                qDebug() << message->getRawMsg();
                break;
            case Message::Ping:
                _sendData("PONG :" + message->getMainMsg());
                qDebug() << "Ping?PONG!";
                break;
            case Message::Error:
                this->_messageList.append(message);
                qDebug() << "ERROR " + message->getMainMsg();
                break;
            case Message::Notice:
                this->_messageList.append(message);
                qDebug() << "NOTICE " + message->getMainMsg();
                break;
            case Message::Num:
                //按数字值分别处理
                switch (message->getNum())
                {
                case 376:
                    emit loginSuccess();
                    break;
                case 366:
                    emit addSessionCallBack();
                    break;
                default:
                    break;
                }
                break;
            default:
                this->_messageList.append(message);
                qDebug() << message->getRawMsg();
                break;
            }
            break;
            //用户消息
        case Message::User:
            //获取消息来自的会话
            if (message->getMsgType() == Message::Private)
            {
                QString name = message->getNick();
                foreach (auto *session, this->_sessionList)
                {
                    if (session->getName() == name)
                    {
                        session->addMessage(message);
                        break;
                    }
                }
            }
            else
            {
                QString channel = message->getChannel();
                foreach (auto *session, this->_sessionList)
                {
                    if (session->getName() == channel)
                    {
                        session->addMessage(message);
                        break;
                    }
                }
            }
            break;
        default:
            break;
        }
    }
}
/**
 * @brief 断开服务器连接
 */
void Server::disconnect()
{
    if (this->_socket->isOpen())
    {
        this->_socket->disconnectFromHost();
        this->_status = Disconnected;
    }
}
/**
 * @brief 获取昵称
 * @return 当前昵称
 */
QString Server::getNick()
{
    return this->_nick;
}
/**
 * @brief 获取服务器地址
 * @return 服务器地址
 */
QString Server::getHost()
{
    return this->_host;
}
/**
 * @brief 添加会话,加入成功后发射addSessionSucess信号
 * @param name 会话名称
 */
void Server::addSession(const QString &name)
{
    //发送加入信息
    this->_sendData("JOIN " + name);
    //会话加入成功
    connect(this, SIGNAL(addSessionCallBack), this, SLOT(_addSessionCallBack(name)));
}
/**
 * @brief 添加会话成功
 * @param name 会话名称
 */
void Server::_addSessionCallBack(QString name)
{
    Session *newSession;
    if (name[0] == '#')
    {
        newSession = new Channel(name);
    }
    else
    {
        newSession = new User(name);
    }
    this->_sessionList.append(newSession);
    //发射成功信号
    emit addSessionSucess();
}
