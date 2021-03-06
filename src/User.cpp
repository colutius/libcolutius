/**
 * @file User.cpp
 * @author ambition_echo (ambition_echo@outlook.com)
 * @brief IRC用户类实现
 * @version 0.1
 * @date 2022-05-09
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "User.h"

User::User(QString name, Session *parent) : Session(std::move(name), parent)
{
}
QString User::getType()
{
    return "User";
}
User::~User() = default;
