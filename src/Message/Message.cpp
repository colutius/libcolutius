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
 * 需要调用setRawMsg()手动设置其中的原始数据
 * @param parent 默认参数，不需要管他
 */
Message::Message(QObject *parent) : QObject(parent)
{
}
/**
 * @brief 有参构造函数，
 * 使用时直接传入接收到的原始数据或即将发送的原始数据
 * @param msg 接收到的原始数据或即将发送的原始数据
 * @param parent 默认参数，不用管
 */
Message::Message(QString msg, QObject *parent)
{
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
    this->rawMsg = msg;
}
/**
 * @brief 获取原始数据
 * @return 返回消息的原始数据，若没有设置原始数据，
 * 则返回"[libcolutius-message]->ERROR!"
 */
QString Message::getRawMsg()
{
    if (!this->rawMsg.isEmpty())
    {
        return this->rawMsg;
    }
    else
    {
        qDebug() << "未设置rawMsg！";
        return "[libcolutius-message]->ERROR!";
    }
}
