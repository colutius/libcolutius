/**
 * @file Channel.h
 * @author ambition_echo (ambition_echo@outlook.com)
 * @brief  IRC频道类头文件
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
 * @brief IRC频道类
 * 
 */
class Channel : public QObject
{
    Q_OBJECT
  public:
    Channel(QObject *parent = nullptr);
    ~Channel() override;
};
