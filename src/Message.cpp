/**
 * @file Message.cpp
 * @author ambition_echo (ambition_echo@outlook.com)
 * @brief IRC消息类实现
 * @version 0.1
 * @date 2022-05-09
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "Message.h"
#include <qglobal.h>
/**
 * @brief 有参构造函数
 * @param msg 接收到的原始数据或即将发送的原始数据
 * @param sender 消息发送者,默认为User,
 * 如不确定消息发送者，默认即可
 * 注意:用户发送的消息必须传入Owner或手动设置为Owner
 * @param parent 默认为nullptr
 */
Message::Message(QObject *parent) : QObject(parent)
{
}

/**
 * @brief 默认析构函数
 */
Message::~Message() = default;
Message::Type Message::msgType()
{
    return this->_msgType;
}
Message *Message::msgType(Type type)
{
    this->_msgType = type;
    return this;
}
QString Message::typeStr()
{
    return this->_typeStr;
}
Message *Message::typeStr(QString type)
{
    this->_typeStr = type;
    return this;
}
QString Message::server()
{
    return this->_server;
}
Message *Message::server(QString server)
{
    this->_server = server;
    return this;
}
Message::Who Message::from()
{
    return this->_from;
}
Message *Message::from(Who from)
{
    this->_from = from;
    return this;
}
QString Message::fromName()
{
    return this->_fromName;
}
Message *Message::fromName(QString fromName)
{
    this->_fromName = fromName;
    return this;
}
Message::Who Message::to()
{
    return this->_to;
}
Message *Message::to(Who to)
{
    this->_to = to;
    return this;
}
QString Message::toName()
{
    return this->_toName;
}
Message *Message::toName(QString toName)
{
    this->_toName = toName;
    return this;
}
QString Message::rawMsg()
{
    return this->_rawMsg;
}
Message *Message::rawMsg(QString rawMsg)
{
    this->_rawMsg = rawMsg;
    return this;
}
QString Message::mainMsg()
{
    return this->_mainMsg;
}
Message *Message::mainMsg(QString mainMsg)
{
    this->_mainMsg = mainMsg;
    return this;
}
QString Message::myNick()
{
    return this->_myNick;
}
Message *Message::myNick(QString myNick)
{
    this->_myNick = myNick;
    return this;
}
int Message::time()
{
    return this->_time;
}
Message *Message::parse()
{
    //分割原始数据
    QStringList buf = this->rawMsg().split(" ");
    int n = 0;
    foreach (auto msg, buf)
    {
        switch (n)
        {
        case 0: {
            n++;
            if (msg == "PING" || msg == "ERROR")
            {
                parseSpecial(msg);
                return this;
            }
            else if (buf.count() <= 3)
            {
                mainMsg(rawMsg());
                return this;
            }
            else
            {
                parseFrom(msg);
            }
            break;
        }
        case 1: {
            n++;
            parseType(msg);
            break;
        }
        case 2: {
            n++;
            parseTo(msg);
            break;
        }
        case 3: {
            n++;
            mainMsg(msg.remove(0, 1));
            break;
        }
        default: {
            mainMsg(mainMsg() + " " + msg);
            break;
        }
        }
    }
    QDateTime time = QDateTime::currentDateTime();
    this->_time = int(time.toSecsSinceEpoch());
    return this;
}
void Message::parseFrom(QString msg)
{
    QRegularExpression user(".*!.*@.*");
    QRegularExpressionValidator isUser(user);
    int pos = 0;
    if (isUser.validate(msg, pos) == QValidator::Acceptable)
    {
        QStringList buf = msg.split("!");
        //去除头部冒号
        if (buf[0][0] == ':')
        {
            buf[0] = buf[0].remove(0, 1);
        }
        from(Message::Who::User);
        fromName(buf[0]);
    }
    else
    {
        from(Message::Who::Server);
        fromName(server());
    }
}
void Message::parseSpecial(QString msg)
{
    if (msg == "PING")
    {
        msgType(Message::Type::Ping);
    }
    else if (msg == "ERROR")
    {
        msgType(Message::Type::Error);
    }
    else
    {
        msgType(Message::Type::None);
    }
    mainMsg(rawMsg());
    from(Message::Who::Server);
    fromName(server());
    to(Message::Who::Me);
    toName(myNick());
}

void Message::parseType(QString msg)
{
    if (msg == "PRIVMSG")
    {
        msgType(Message::Type::Common);
    }
    if (msg == "JOIN")
    {
        msgType(Message::Type::Join);
    }
    else
    {
        msgType(Message::Type::None);
    }
}
void Message::parseTo(QString msg)
{
    if (msg[0] == '#')
    {
        to(Message::Who::Channel);
    }
    else
    {
        if (msg == myNick())
        {
            to(Message::Who::Me);
        }
        else
        {
            to(Message::Who::User);
        }
    }
    toName(msg);
}