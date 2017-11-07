#include "server.h"
#include <QDebug>
#include <QMessageBox>

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
    reconnecting = false;
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
    //my_socket->connectToHost("localhost", 9001);
    // If it ever disconnects (including while trying this), the socket will
    // continuously try to reconnect. See reconnect_socket().
}

/*
 * ACCOUNTS
 */

bool server::login(QString& username, QString& password)
{
    // Socket connected at this point, pass through info
    my_socket->write(format_socket_request("LOGN", QString(username+" "+password)));
    QString reply;
    return read_socket_helper(reply); // TODO: Handle receiving the email when it is passed back in reply
}

bool server::create_account(QString& username, QString& password, QString& email)
{
    // Socket connected at this point, pass through info
    my_socket->write(format_socket_request("CACC", QString(username+" "+password+" "+email)));
    QString message;
    bool ret = read_socket_helper(message);
    if(ret)
    {
        QMessageBox success_box;
        success_box.setText(message);
        success_box.exec();
    }
    return ret;
}

bool server::logout()
{
    my_socket->write(format_socket_request("LOGT", ""));
    return true;
}

/*
 * GROUPS
 */

bool server::create_group(QString& group_name, QString& group_id)
{
    // Pass through info
    my_socket->write(format_socket_request("CGRP", QString(group_name)));
    return read_socket_helper(group_id);
}

bool server::join_group(QString &group_id)
{
    my_socket->write(format_socket_request("JGRP", QString(group_id)));
    QString _str;
    return read_socket_helper(_str);
}

bool server::leave_group(QString &group_id)
{
    my_socket->write(format_socket_request("LGRP", QString(group_id)));
    QString _str;
    return read_socket_helper(_str);
}

/*
 *
 *
 * SLOTS
 *
 *
 */

void server::reconnect_socket(QAbstractSocket::SocketState current_state)
{
    qDebug() << current_state;

    if(QAbstractSocket::UnconnectedState == current_state)
    {
        // Here because connection with the server was severed
        // Either by server going offline or client internet going out
        // So, try to continually reconnect until successful.
        if(reconnecting == false) {
            // First time we are trying to reconnect
            emit disconnected();
            // Show error message only initally
            QMessageBox reconnect_box;
            reconnect_box.setText("No internet connection to server.");
            reconnect_box.setIcon(QMessageBox::Warning);
            reconnect_box.exec();
        }
        reconnecting = true;

        qDebug() << "Trying to reconnect...";
        connect_server();
    }

    if(QAbstractSocket::ConnectedState == current_state)
    {
        if(reconnecting == true) {
            QMessageBox connected_box;
            connected_box.setText("Connection to server has been reestablished.");
            connected_box.exec();
            reconnecting = false;
        }
    }
}

void server::read_socket_send_signal()
{
    while(!my_socket->atEnd())
    {
        qDebug() << "Receiving info...";
        QString message_size_str = my_socket->read(5);  // Read first 5 bytes, which is the serialized message size
        qDebug() << "Message size: " << message_size_str;

        if(message_size_str == "SUCC\n") // Until the messages are all updated. Backwards compatibility.
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
            qDebug() << "Server message: " << success_message;
        }
        else if (server_code == "FAIL") // Fail code
        {
            // Set the fail flag
            fail_flag = true;
            // Display the failure message to user
            QMessageBox timeout_box;
            timeout_box.setText("Error");
            timeout_box.setInformativeText(message_stream.readAll());
            timeout_box.setIcon(QMessageBox::Warning);
            timeout_box.exec();
        }
        else if (server_code == "USCH") // User List has CHANGED
        {
            emit users_changed();
        }
        else if (server_code == "NUSR") // New User code
        {
            QString new_username = message_stream.readAll();
            qDebug() << "New user: " << new_username;
            emit user_joined(new_username);
        }
        else if (server_code == "NCHT")
        {
            QString message = message_stream.readAll();
            QString username = message.section(' ', 0, 0);
            QString time = message.section(' ', 1, 1);
            QString chat = message.section(' ', 2, -1);
            emit new_chat(username, time, chat);
        }
        else if (server_code  == "WBLN")
        {
            QString line_str = message_stream.readAll();
            qDebug() << line_str;
            QString x1 = line_str.section(' ', 0, 0);
            QString y1 = line_str.section(' ', 1, 1);
            QString x2 = line_str.section(' ', 2, 2);
            QString y2 = line_str.section(' ', 3, -1);
            QPoint point1(x1.toInt(), y1.toInt());
            QPoint point2(x2.toInt(), y2.toInt());
            qDebug() << point1 << point2;
            emit whiteboard_draw_line(point1, point2);
        }
    }

    return;
    // TODO: Implement if statements and send signals based on what was received.
}

void server::send_chat(QString& groupID, QString& message)
{
    my_socket->write(format_socket_request("GCHT", QString(groupID+" "+message)));
    QString _str;
    read_socket_helper(_str);
}

void server::send_whiteboard_line(QString& groupID, QPoint point1, QPoint point2)
{
    my_socket->write(format_socket_request("WBLN", QString(groupID+" "+
                                                           QString::number(point1.x())+" "+
                                                           QString::number(point1.y())+" "+
                                                           QString::number(point2.x())+" "+
                                                           QString::number(point2.y()))));
}

/*
 *
 *
 * PRIVATE
 *
 *
 */

QByteArray server::format_socket_request(const QString &request_code, QString request_arg)
{
    QString full_request = request_code + request_arg;
    QString request_length = QString::number(full_request.size());
    full_request = full_request.prepend(request_length.rightJustified(5, '0', true));
    qDebug() << "Sending: " << full_request;
    return full_request.toLatin1();
}

bool server::read_socket_helper(QString& out_message)
{
    qDebug() << "Sending info...";
    success_flag = false;
    fail_flag = false;
    success_message = nullptr;
    if((QAbstractSocket::ConnectedState == my_socket->state()) && (my_socket->waitForReadyRead(5000)))
    {
        if(success_flag)
        {
            out_message = success_message;
            return true;
        }

        if(fail_flag)
        {
            return false; // Wrong info
        }
        return true; // Don't care
    } else {
        QMessageBox timeout_box;
        timeout_box.setText("Network Operation Timeout");
        timeout_box.setInformativeText("Either you aren't connected to the internet, or the server is down.");
        timeout_box.setIcon(QMessageBox::Warning);
        timeout_box.exec();
        return false; // Timeout
    }
}

void server::error(QAbstractSocket::SocketError err)
{
   qDebug() << my_socket->errorString();
}
