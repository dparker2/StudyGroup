#include "sgtcpsocket.h"

SGTCPSocket::SGTCPSocket(QObject *parent) : QObject(parent)
{
    _socket->connectToHost("18.221.67.202", 9001); // CSCI 150 SERVER
}
