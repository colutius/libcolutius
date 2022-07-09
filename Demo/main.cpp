#include <Message.h>
#include <QCoreApplication>
#include <QObject>
#include <Server.h>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    auto *server = new Server(Server::Tcp);
    server->setHost("irc.ircnet.com");
    server->setPort(6667);
    server->setNick("hgy");
    server->setUser("hgy");

    server->login();
    return QCoreApplication::exec();
}