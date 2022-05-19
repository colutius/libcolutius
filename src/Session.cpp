/**
 * @file Session.cpp
 * @author ambition_echo (ambition_echo@outlook.com)
 * @brief IRC会话类实现
 * @version 0.1
 * @date 2022-05-17
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "Session.h"
/**
 * @brief 构造函数
 * @param name 会话名称
 * @param parent 默认为nullptr
 */
Session::Session(QString name, QObject *parent) : QObject(parent)
{
    this->_name = std::move(name);
}
Session::~Session() = default;
/**
 * @brief 获取会话名称
 * @return 会话名称
 */
QString Session::getName()
{
    return this->_name;
}
/**
 * @brief 添加消息并发射newMsg信号
 * @param message Message类型的消息
 */
void Session::addMessage(Message *message)
{
    this->_messageList.append(message);
    emit newMsg();
}
QString Session::getType()
{
    return "Session";
}
QList<Message *> Session::getMessageList()
{
    return this->_messageList;
}
/**
 * @brief 根据索引获取消息实例
 * @param index 消息索引号
 * @return Message类型的消息实例
 */
Message *Session::getMessage(int index)
{
    if (index < 0 || index >= this->getMessageNum())
    {
        qDebug() << "会话消息列表索引越界";
        return nullptr;
    }
    return this->_messageList[index];
}
/**
 * @brief 获取消息数量
 * @return 消息数量
 */
int Session::getMessageNum()
{
    return this->_messageList.length();
}
