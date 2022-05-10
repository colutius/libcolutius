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
#include <QRegularExpression>
#include <QtGui/QValidator>
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
     */
    enum MsgType
    {
        Ping = 1,     ///<服务器定期Ping消息
        Pong = 2,     ///<返回给服务器的Pong消息
        Error = 3,    ///<报错信息
        Notice = 4,   ///<注意信息
        Join = 5,     ///<加入信息
        Nick = 6,     ///<昵称相关信息
        Kick = 7,     ///<踢人信息
        Num = 8,      ///<数字类信息，相当于状态码
        Private = 9,  ///<私信消息
        Channel = 10, ///<正常频道消息
        Own = 11,     ///<用户自己发送的消息
    };
    /**
     * @brief 消息发送者
     */
    enum MsgSender
    {
        Server = 1, ///<服务器发送的消息
        User = 2,   ///<其他用户发送的消息
        Owner = 3,  ///<用户自己发送的消息
    };
    explicit Message(QObject *parent = nullptr);
    Message(QString msg, MsgSender sender = MsgSender::User, QObject *parent = nullptr);
    ~Message() override;

    void setRawMsg(QString msg);        //设置原始数据
    QString getRawMsg();                //获取原始数据
    MsgType getMsgType();               //获取消息类型
    void setMsgSendr(MsgSender sender); //设置消息发送者
    MsgSender getMsgSender();           //获取消息发送者

  private:
    void setMsgType(MsgType type); //设置消息类型
    void parse();                  //解析消息原始数据
    MsgType _type;                 //消息类型
    MsgSender _sender;             //消息发送者
    QString _rawMsg;               //原始数据
    QString _mainMsg;              //消息主体信息
};
