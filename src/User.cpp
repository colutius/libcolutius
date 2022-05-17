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

User::User(Session *parent) : Session(parent)
{
}
User::~User() = default;
