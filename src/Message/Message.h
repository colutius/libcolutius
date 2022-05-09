#pragma once
#include <QDebug>
#include <QObject>

class Message : public QObject
{
    Q_OBJECT
  public:
    Message(QObject *parent = nullptr);
    ~Message() override;
    static void sendMsg();
};
