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
#include <QTime>
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
    enum Type
    {
        None,   ///<未知消息
        Ping,   ///<服务器定期Ping消息
        Error,  ///<报错信息
        Notice, ///<注意信息
        Join,   ///<加入信息
        Quit,   ///<退出频道消息
        Nick,   ///<昵称相关信息
        Kick,   ///<踢人信息
        Num,    ///<数字类信息，相当于状态码
        Common, ///<常规消息
    };
    /**
     * @brief 消息发送者
     */
    enum Who
    {
        Server,  ///<服务器发送的消息
        Channel, ///<频道消息
        User,    ///<其他用户发送的消息
        Me,      ///<用户自己发送的消息
    };
    explicit Message(QObject *parent = nullptr);
    ~Message() override;
    Type msgType();
    Message *msgType(Type type);
    QString typeStr();
    Message *typeStr(QString type);
    QString server();
    Message *server(QString server);
    Who from();
    Message *from(Who from);
    QString fromName();
    Message *fromName(QString fromName);
    Who to();
    Message *to(Who to);
    QString toName();
    Message *toName(QString toName);
    QString rawMsg();
    Message *rawMsg(QString rawMsg);
    QString mainMsg();
    Message *mainMsg(QString mainMsg);
    QString myNick();
    Message *myNick(QString myNick);
    int time();
    Message *parse(); //解析消息原始数据

  private:
    void parseType(QString msg);
    void parseFrom(QString msg);
    void parseTo(QString msg);
    void parseSpecial(QString msg);

    /*Type*/
    QString _typeStr; //消息类型字符串
    Type _msgType;    //消息类型
    /*Who*/
    QString _server; //消息所在服务器
    Who _from;       //消息从哪来
    QString _fromName;
    Who _to; //消息到哪去
    QString _toName;
    /*Main*/
    QString _rawMsg;  //原始数据
    QString _mainMsg; //消息主体信息
    /*Additional*/
    int _time;       //消息发送时间 时间戳形式
    QString _myNick; //我的昵称
};
