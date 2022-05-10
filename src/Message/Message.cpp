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
/**
 * @brief 无参构造，采用这种构造方式之后，
 * 需要调用setRawMsg()和setMsgSender()手动设置其中的原始数据和消息发送者
 * @param parent 默认为nullptr
 */
Message::Message(QObject *parent) : QObject(parent)
{
}
/**
 * @brief 有参构造函数
 * @param msg 接收到的原始数据或即将发送的原始数据
 * @param sender 消息发送者,默认为User,注意:用户发送的消息必须传入Owner或手动设置为Owner
 * @param parent 默认为nullptr
 */
Message::Message(QString msg, Message::MsgSender sender, QObject *parent) : QObject(parent)
{
    this->setMsgSendr(sender);
    this->setRawMsg(msg);
}
/**
 * @brief 默认析构函数
 */
Message::~Message() = default;
/**
 * @brief 设置原始数据
 * @param msg 接收到的原始数据或即将发送的原始数据
 */
void Message::setRawMsg(QString msg)
{
    this->_rawMsg = msg;
    this->parse();
}
/**
 * @brief 获取原始数据
 * @return Qstring类型的消息原始数据，若没有设置原始数据，
 * 则返回空字符串
 */
QString Message::getRawMsg()
{
    if (!this->_rawMsg.isEmpty())
    {
        return this->_rawMsg;
    }
    else
    {
        return "";
    }
}
/**
 * @brief 设置消息类型
 * @param type 选择msgType中的枚举值
 */
void Message::setMsgType(Message::MsgType type)
{
    this->_type = type;
}
/**
 * @brief 获取此消息的类型
 * @return Message::MsgType类型的消息的类型
 */
Message::MsgType Message::getMsgType()
{
    return this->_type;
}
/**
 * @brief 设置消息的发送者
 * @param sender 选择msgSender中的枚举值
 */
void Message::setMsgSendr(Message::MsgSender sender)
{
    this->_sender = sender;
    if (this->getMsgSender() == MsgSender::Owner)
    {
        this->parse();
    }
}
/**
 * @brief 获取此消息的发送者
 * @return Message::MsgSender类型的消息发送者
 */
Message::MsgSender Message::getMsgSender()
{
    return this->_sender;
}
/**
 * @brief 解析消息原始数据
 * 不需要手动调用，在设置了原始数据之后会自动调用
 */
void Message::parse()
{
    //分割原始数据
    QStringList buf = this->getRawMsg().split(" ");
    //如果不是用户自己发送的消息，解析之前无法确切知道是服务器消息还是用户消息
    if (this->getMsgSender() != Owner)
    {
        //匹配用户消息的头部
        QRegularExpression user(".*!.*@.*");
        QRegularExpressionValidator isUser(user);
        int pos = 0;
        if (isUser.validate(buf[0], pos) == QValidator::Acceptable)
        {
            this->setMsgSendr(MsgSender::User);
        }
        else
        {
            this->setMsgSendr(MsgSender::Server);
        }
    }
    switch (this->_sender)
    {
    case Server:
        //服务器消息有可能有多行消息
        buf = this->getRawMsg().split("\r\n");
        if (buf.length() > 1)
        {
        }
        else
        {
            buf = this->getRawMsg().split(" ");
            if (buf[0] == "PING")
            {
                this->setMsgType(MsgType::Ping);
                break;
            }
            if (buf[0] == "ERROR")
            {
                this->setMsgType(MsgType::Error);
                break;
            }
            if (buf.length() > 1)
            {
                if (buf[1] == "NOTICE")
                {
                    this->setMsgType(MsgType::Notice);
                    break;
                }
            }
        }
        break;
    case User:
        if (buf[1] == "JOIN")
        {
            this->setMsgType(MsgType::Join);
            break;
        }
        if (buf[1] == "PRIVMSG")
        {
            if (buf[2][0] == '#')
            {
                this->setMsgType(MsgType::Channel);
                break;
            }
            else
            {
                this->setMsgType(MsgType::Private);
                break;
            }
        }
        break;
    case Owner:
        if (buf[0][0] != '/')
        {
            this->setMsgType(MsgType::Own);
            break;
        }
        else
        {
            if (buf[0] == "/msg")
            {
                this->setMsgType(MsgType::Own);
                break;
            }
            if (buf[0] == "/join")
            {
                this->setMsgType(MsgType::Join);
                break;
            }
        }
    }
}
