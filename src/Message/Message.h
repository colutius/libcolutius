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
 * 将原始数据传入其中，由该类对接收到的消息进行解析、分类、格式化
 */
class Message : public QObject
{
    Q_OBJECT
  public:
    /**
     * @brief 消息类型
     * @enum msgType
     */
    enum msgType
    {
        Ping,    ///<服务器定期Ping消息
        Error,   ///<报错信息
        Notice,  ///<注意信息
        Join,    ///<加入信息
        Nick,    ///<昵称相关信息
        Kick,    ///<踢人信息
        Num,     ///<数字类信息相当于状态码
        Private, ///<私信消息
        Channel, ///<频道消息
    };
    explicit Message(QObject *parent = nullptr);
    Message(QString msg, QObject *parent = nullptr);
    ~Message() override;

    void setRawMsg(QString msg);
    QString getRawMsg();

  private:
    QString rawMsg;
};
