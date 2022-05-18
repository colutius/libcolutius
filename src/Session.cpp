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
Session::Session(QString name, QObject *parent) : QObject(parent)
{
    this->_name = std::move(name);
}
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
Session::~Session() = default;
