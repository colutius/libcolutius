/**
 * @file Message.h
 * @author ambition_echo (ambition_echo@outlook.com)
 * @brief  IRC消息类头文件
 * @version 0.1
 * @date 2022-05-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once
#include <QDebug>
#include <QObject>
/**
 * @brief IRC消息类
 * 
 */
class Message : public QObject
{
    Q_OBJECT
  public:
    Message(QObject *parent = nullptr);
    ~Message() override;
    static void sendMsg(QString msg);
};
