#include "server.h"
#include <QDebug>

// Testing changes
// More changes
// TESTING

server::server(QObject *parent) : QObject(parent)
{
    // Initialize socket stuff
    my_socket = new QTcpSocket(this);
    // Try to reconnect socket whenever it disconnects
    connect(my_socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(reconnect_socket(QAbstractSocket::SocketState)));
    //connect(my_socket, SIGNAL())
    // Prints any socket errors to console
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

bool server::connectServer()
{
    //Connect the socket to the host
    my_socket->connectToHost("18.221.67.202", 9001); // CSCI 150 SERVER
    if (my_socket->waitForConnected(5000))
    {
        qDebug() << "Connected.";
        return true;
    } else {
        qDebug() << "Not connected."; // Timeout
        return false;
    }
}

bool server::login(QString& username, QString& password)
{
    // Socket connected at this point, pass through info
    my_socket->write(QString("LOGIN "+username+" "+password).toLatin1());
    qDebug() << "Sending info...";
    if (my_socket->waitForReadyRead(5000)) {
        qDebug() << "Reading info...";
        QString server_response = my_socket->readAll();
        qDebug() << "Server response: " << server_response;

        if(server_response == "success message") // INSERT REAL MESSAGE HERE
        {
            return true;
        } else {
            return false; // Wrong info
        }
    }

    return false; // Timeout
}

/***
 * Sends info to server to create
 * a new account.
 */

bool server::createAccount(QString& email, QString& username, QString& password)
{
    // Socket connected at this point, pass through info
    my_socket->write(QString("CREATE "+username+" "+password+" "+email).toLatin1());
    qDebug() << "Sending info...";
    if (my_socket->waitForReadyRead(5000)) {
        qDebug() << "Reading info...";
        QString server_response = my_socket->readAll();
        qDebug() << "Server response: " << server_response;

        if(server_response == "success message") // INSERT REAL MESSAGE HERE
        {
            return true;
        } else {
            return false; // Wrong info
        }
    }

    return false; // Timeout
}

void server::reconnect_socket(QAbstractSocket::SocketState current_state) {

    qDebug() << current_state;

    if(QAbstractSocket::UnconnectedState == current_state)
    {
        // Here because connection with the server was severed
        // Either by server going offline or client internet going out
        // So, try to continually reconnect until successful.
        emit disconnected();
        bool connected = false;

        while(!connected)
        {
            qDebug() << "Trying to reconnect...";
            connected = connectServer();
        }
    }
}

void server::error(QAbstractSocket::SocketError err)
{
   qDebug() << my_socket->errorString();
}
