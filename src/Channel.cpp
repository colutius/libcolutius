/**
 * @file Channel.cpp
 * @author ambition_echo (ambition_echo@outlook.com)
 * @brief IRC频道类实现
 * @version 0.1
 * @date 2022-05-09
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "Channel.h"

Channel::Channel(QString name, Session *parent) : Session(std::move(name), parent)
{
}
QString Channel::getType()
{
    return "Channel";
}
void Channel::quit()
{
    emit _quit(this->getName());
}
Channel::~Channel() = default;
