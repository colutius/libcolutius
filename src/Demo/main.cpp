#include <Message.h>
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    auto *msg = new Message();
    // msg->setRawMsg("hello?");
    qDebug() << msg->getRawMsg();
}
