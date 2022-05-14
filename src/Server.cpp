/**
 * @file Server.cpp
 * @author ambition_echo (ambition_echo@outlook.com)
 * @brief IRC服务器类实现
 * @version 0.1
 * @date 2022-05-09
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "Server.h"
/**
 * @brief 无参构造，
 * 后续使用需要手动设置服务器host,port等参数
 * @param parent 默认nullptr
 */
Server::Server(QObject *parent) : QObject(parent)
{
    this->_tcpsocket = new QTcpSocket;
    this->_sslsocket = new QSslSocket;
}
Server::~Server() = default;
