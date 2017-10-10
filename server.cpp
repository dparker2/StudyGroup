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

    // New information -> send appropriate signal
    connect(my_socket, SIGNAL(readyRead()), this, SLOT(read_socket_send_signal()));

    // Prints any socket errors to console
    connect(my_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));

    //my_socket->connectToHost("18.221.67.202", 9001); // CSCI 150 SERVER
    //my_socket->connectToHost("localhost", 24680);
    //my_socket->connectToHost("52.53.198.189", 24680); // David's AWS server

    success_flag = false;
    fail_flag = false;
    success_message = nullptr;
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

void server::connect_server()
{
    // Connect the socket to the host
    my_socket->connectToHost("18.221.67.202", 9001); // CSCI 150 SERVER
    // If it ever disconnects (including while trying this), the socket will
    // continuously try to reconnect. See reconnect_socket().
}

/*
 * ACCOUNTS
 */

bool server::login(QString& username, QString& password)
{
    // Socket connected at this point, pass through info

    // Determine size of message, which is length(username)+length(password)+6 (for 2 spaces and 4 letter code)
    //QString message_length = username.length() + password.length() + 6;

    my_socket->write(QString("LOGIN "+username+" "+password).toLatin1());
    qDebug() << "Sending info...";
    success_flag = false;
    fail_flag = false;
    success_message = nullptr;
    if(my_socket->waitForReadyRead(5000))
    {
        if(success_flag)
        {
            return true;
        }

        if(fail_flag)
        {
            return false; // Wrong info
        }
    }

    return false; // Timeout
}

bool server::create_account(QString& username, QString& password, QString& email)
{
    // Socket connected at this point, pass through info
    my_socket->write(QString("CREATE "+username+" "+password+" "+email).toLatin1());
    qDebug() << "Sending info...";
    if (my_socket->waitForReadyRead(5000)) {
        qDebug() << "Reading info...";
        QString server_response = my_socket->readAll();
        qDebug() << "Server response: " << server_response;

        if(server_response == "SUCC\n") // INSERT REAL MESSAGE HERE
        {
            return true;
        } else {
            return false; // Wrong info
        }
    }

    return false; // Timeout
}

/*
 * GROUPS
 */

bool server::create_group(QString& group_name, QString& group_id)
{
    // Pass through info
    my_socket->write(QString("CREATEGRP "+group_name+" "+this->username).toLatin1());
    qDebug() << "Sending info...";
    success_flag = false;
    fail_flag = false;
    success_message = nullptr;
    if(my_socket->waitForReadyRead(5000))
    {
        if(success_flag)
        {
            group_id = success_message; // Return the group ID given from server
            return true;
        }

        if(fail_flag)
        {
            return false; // Wrong info
        }
    }

    return false;
}

bool server::join_group(QString &group_id)
{
    my_socket->write(QString("JOINGRP "+group_id+" "+this->username).toLatin1());
    qDebug() << "Sending info...";
    success_flag = false;
    fail_flag = false;
    success_message = nullptr;
    if(my_socket->waitForReadyRead(5000))
    {
        if(success_flag)
        {
            group_id = success_message; // Return the group ID given from server
            return true;
        }

        if(fail_flag)
        {
            return false; // Wrong info
        }
    }

    return false;

}

QString server::get_username()
{
    return this->username;
}

void server::set_username(QString& username)
{
    this->username = username;
}

/*
 *
 *
 * SLOTS
 *
 *
 */

void server::reconnect_socket(QAbstractSocket::SocketState current_state) {

    qDebug() << current_state;

    if(QAbstractSocket::UnconnectedState == current_state)
    {
        // Here because connection with the server was severed
        // Either by server going offline or client internet going out
        // So, try to continually reconnect until successful.
        emit disconnected();

        qDebug() << "Trying to reconnect...";
        connect_server();
    }
}

void server::read_socket_send_signal()
{
    QString message_size_str = my_socket->read(5);  // Read first 5 bytes, which is the serialized message size

    if(message_size_str == "SUCC\n")
    {
        success_flag = true;
        return;
    }

    int message_size = message_size_str.toInt();    // Convert the size to an integer
    QTextStream message_stream(my_socket->read(message_size));   // Read the message, which is the next size bytes
    QString server_code = message_stream.read(4);   // Get the server code
    qDebug() << "Server code: " << server_code;

    // Hinge on server_code
    if (server_code == "SUCC")      // Success code
    {
        // Set the success flag and message
        success_flag = true;
        success_message = message_stream.readAll();
    }
    else if (server_code == "FAIL") // Fail code
    {
        // Set the fail flag
        fail_flag = true;
    }

    return;
    // TODO: Implement if statements and send signals based on what was received.
}

void server::error(QAbstractSocket::SocketError err)
{
   qDebug() << my_socket->errorString();
}
