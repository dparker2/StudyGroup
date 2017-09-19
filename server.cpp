#include "server.h"
#include <QDebug>

server::server(QObject *parent) : QObject(parent)
{
    my_socket = new QTcpSocket(this);
    connect(my_socket, SIGNAL(connected()), this, SLOT(socket_connected()));
    connect(my_socket, SIGNAL(readyRead()), this, SLOT(socket_readyRead()));
    connect(my_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));

    //my_socket->connectToHost("18.221.67.202", 9001); // CSCI 150 SERVER
    //my_socket->connectToHost("localhost", 24680);
    //my_socket->connectToHost("52.53.198.189", 24680); // David's AWS server
}

/***
 * Public
 * Functions
 * API
 */

/***
 * bool verifyUserInfo(QString& username, QString& password)
 *  returns true if found
 *          false if not
 */

bool server::verifyUserInfo(QString& username, QString& password)
{
    my_socket->connectToHost("18.221.67.202", 9001); // CSCI 150 SERVER
    if (my_socket->waitForConnected(5000)) {
        qDebug() << "Connected.";
    } else {
        qDebug() << "Not connected.";
    }

    // Socket connected at this point, pass through info
    my_socket->write(QString("0"+username+" "+password+"\n").toLocal8Bit());

    return true; // Always return true for now
}

bool server::createAccount(QString& email, QString& username, QString& password)
{
    my_socket->connectToHost("18.221.67.202", 9001); // CSCI 150 SERVER
    if (my_socket->waitForConnected(5000)) {
        qDebug() << "Connected.";
    } else {
        qDebug() << "Not connected.";
    }

    // Socket connected at this point, pass through info
    my_socket->write(QString("1"+email+" "+username+" "+password+"\n").toLocal8Bit());

    return true; // Always return true for now
}

void server::socket_connected()
{
    qDebug() << "Connected!";
}

void server::socket_readyRead() {
    qDebug() << my_socket->readAll();
}

void server::error(QAbstractSocket::SocketError err)
{
   qDebug() << my_socket->errorString();
}
