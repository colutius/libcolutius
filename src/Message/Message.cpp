#include "Message.h"

Message::Message(QObject *parent) : QObject(parent)
{
}
Message::~Message() = default;
/*!
 * @brief 测试函数
 */
void Message::sendMsg()
{
    qDebug() << "hello shared library";
}
