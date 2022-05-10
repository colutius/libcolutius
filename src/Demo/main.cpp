#include <Message.h>
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    auto *msg = new Message("hgy!hgy@123.23.3.23 PRIVMSG #linux :llll");
    qDebug() << msg->getMsgSender();
    qDebug() << msg->getMsgType();
    qDebug() << msg->getRawMsg();
}
