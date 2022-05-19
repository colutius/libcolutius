#include <Message.h>
#include <QCoreApplication>
#include <QObject>
#include <Server.h>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    auto *server = new Server(Server::Tcp);
    server->setHost("irc.libera.chat");
    server->setPort(6667);
    server->setNick("colutius");
    server->setUser("colutius");
    server->login();
    return QCoreApplication::exec();
}