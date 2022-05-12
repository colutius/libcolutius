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
 * @brief 无参构造,采用这种构造方式之后，
 * 需要调用`setRawMsg()`和setMsgSender()手动设置其中的原始数据和消息发送者
 * @param parent 默认为nullptr
 */
Message::Message(QObject *parent) : QObject(parent)
{
    this->setTime();
}
/**
 * @brief 有参构造函数
 * @param msg 接收到的原始数据或即将发送的原始数据
 * @param sender 消息发送者,默认为User,
 * 如不确定消息发送者，默认即可
 * 注意:用户发送的消息必须传入Owner或手动设置为Owner
 * @param parent 默认为nullptr
 */
Message::Message(QString msg, Message::MsgSender sender, QObject *parent) : QObject(parent)
{
    this->setMsgSender(sender);
    this->setRawMsg(msg);
    this->setTime();
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
void Message::setMsgSender(Message::MsgSender sender)
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
 * @brief 解析消息原始数据，
 * 一般情况下，不需要手动调用，在设置了原始数据之后会自动调用
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
            this->setMsgSender(MsgSender::User);
        }
        else
        {
            this->setMsgSender(MsgSender::Server);
        }
    }
    switch (this->_sender)
    {
    case Server:
        //服务器消息有可能有多行消息
        buf = this->getRawMsg().split("\r\n");
        if (buf.length() > 1)
        {
            // TODO 处理服务器的多行消息
        }
        else
        {
            buf = this->getRawMsg().split(" ");
            // PING消息
            if (buf[0] == "PING")
            {
                this->setMsgType(MsgType::Ping);
                this->parseMainMsg(this->getRawMsg(), 1);
                break;
            }
            // ERROR消息
            if (buf[0] == "ERROR")
            {
                this->setMsgType(MsgType::Error);
                this->parseMainMsg(this->getRawMsg(), 1);
                break;
            }
            if (buf.length() > 1)
            {
                //判断是否为数字指令
                QRegularExpression num("[0-9].*");
                QRegularExpressionValidator isNum(num);
                int pos = 0;
                //数字指令
                if (isNum.validate(buf[1], pos) == QValidator::Acceptable)
                {
                    this->parseMainMsg(this->getRawMsg(), 2);
                    // TODO 分别解析不同的数字
                    switch (buf[1].toInt())
                    {
                    case 352:
                        return;
                    default:
                        return;
                    }
                }
                // NOTICE消息
                if (buf[1] == "NOTICE")
                {
                    this->setMsgType(MsgType::Notice);
                    this->parseMainMsg(this->getRawMsg(), 2);
                    break;
                }
            }
        }
        break;
    case User:
        if (buf[1] == "JOIN")
        {
            this->setMsgType(MsgType::Join);
            this->parseMainMsg(this->getRawMsg(), 2);
            this->parseMsgSender(buf[0]);
            break;
        }
        if (buf[1] == "PRIVMSG")
        {
            if (buf[2][0] == '#')
            {
                this->setMsgType(MsgType::Channel);
                this->_channel = buf[2];
                this->parseMainMsg(this->getRawMsg(), 3);
                this->parseMsgSender(buf[0]);
                break;
            }
            else
            {
                this->setMsgType(MsgType::Private);
                this->parseMainMsg(this->getRawMsg(), 3);
                this->parseMsgSender(buf[0]);
                break;
            }
        }
        break;
    case Owner:
        if (buf[0][0] != '/')
        {
            //这里无法判断是频道消息还是私信消息，需要外部手动设置
            this->_mainMsg = this->getRawMsg();
            break;
        }
        else
        {
            if (buf[0] == "/msg")
            {
                if (buf.length() < 3)
                {
                    qDebug() << "指令不合法";
                    this->setMsgType(MsgType::None);
                }
                else
                {
                    if (buf[1][0] == '#')
                    {
                        this->setMsgType(MsgType::Channel);
                        this->parseMainMsg(this->getRawMsg(), 2);
                    }
                    else
                    {
                        this->setMsgType(MsgType::Private);
                        this->parseMainMsg(this->getRawMsg(), 2);
                    }
                }
                break;
            }
            if (buf[0] == "/join")
            {
                if (buf.length() < 2)
                {
                    qDebug() << "指令不合法";
                    this->setMsgType(MsgType::None);
                }
                else
                {
                    this->_channel = buf[1];
                    this->setMsgType(MsgType::Join);
                }
                break;
            }
        }
    }
}
/**
 * @brief 获取一条消息的主体部分
 * @param msg 消息原始数据
 * @param index 将消息以空格分隔后，主题消息头所在的位置
 */
void Message::parseMainMsg(QString msg, int index)
{
    QStringList buf = msg.split(" ");
    if (index < 0 || index >= buf.length())
    {
        qDebug() << "索引不合法";
    }
    //去除头部的冒号
    if (buf[index][0] == ':')
    {
        buf[index] = buf[index].remove(0, 1);
    }
    QString mainMsg = "";
    for (int i = index; i < buf.length(); i++)
    {
        mainMsg += buf[i] + " ";
    }
    this->_mainMsg = mainMsg;
}
/**
 * @brief 获取消息主体信息
 * @return 原始消息中去掉头部及指令之后最后真正有用的主体信息
 */
QString Message::getMainMsg()
{
    if (this->_mainMsg.isEmpty())
    {
        qDebug() << "主体信息为空";
        return "";
    }
    return this->_mainMsg;
}
/**
 * @brief 解析消息发送者信息
 * @param msg 用户消息原始数据中空格分隔的第一部分
 * 例如：colutius!~colutius@123.123 PRIVMSG #colutius :hello?
 * 传入colutius!~colutius@123.123即可
 */
void Message::parseMsgSender(QString msg)
{
    QStringList buf = msg.split("!");
    //去除头部冒号
    if (buf[0][0] == ':')
    {
        buf[0] = buf[0].remove(0, 1);
    }
    this->_nick = buf[0];
    buf = msg.split("@");
    this->_ip = buf[1];
}
/**
 * @brief 设置消息发送者昵称
 * @param nick 消息发送者昵称
 */
void Message::setNick(QString nick)
{
    this->_nick = nick;
}
/**
 * @brief 获取频道名
 * @return 消息所在频道名
 */
QString Message::getChannel()
{
    return this->_channel;
}
/**
 * @brief 获取消息发送者昵称
 * @return 消息发送者昵称
 */
QString Message::getNick()
{
    return this->_nick;
}
/**
 * @brief 获取消息发送者ip
 * @return 消息发送者ip
 */
QString Message::getIp()
{
    return this->_ip;
}
/**
 * @brief 设置消息发送/接收时间
 */
void Message::setTime()
{
    this->_msgTime = QTime::currentTime();
}
