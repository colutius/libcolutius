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

Message::Message(QObject *parent) : QObject(parent)
{
}
Message::~Message() = default;
/**
 * @brief 自动文档生成测试函数
 *        发送消息
 * 
 * @param msg 
 */
void Message::sendMsg(QString msg)
{
    qDebug() << "hello shared library"+msg;
}
