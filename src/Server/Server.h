/**
 * @file Server.h
 * @author ambition_echo (ambition_echo@outlook.com)
 * @brief  IRC服务器类头文件
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
 * @brief IRC服务器类
 * 
 */
class Server : public QObject
{
    Q_OBJECT
  public:
    Server(QObject *parent = nullptr);
    ~Server() override;
};
