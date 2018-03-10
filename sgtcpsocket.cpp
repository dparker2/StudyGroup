#include "sgtcpsocket.h"
#include <QMessageBox>
#include <QDateTime>

SGTCPSocket::SGTCPSocket(QObject *parent) : QObject(parent)
{
    my_tcp_socket = new QTcpSocket();
    connect(my_tcp_socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(reconnect_socket(QAbstractSocket::SocketState)));
    connect(my_tcp_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
    connect(my_tcp_socket, SIGNAL(readyRead()), this, SLOT(read_socket_send_signal()));

    success_flag = false;
    fail_flag = false;
    success_message = nullptr;
    reconnecting = false;
}

SGTCPSocket::SGTCPSocket(const SGTCPSocket &sock) : QObject()
{
    qDebug() << "UHHH";
}

SGTCPSocket::~SGTCPSocket()
{
    my_tcp_socket->deleteLater();
}

void SGTCPSocket::connect_server()
{
    my_tcp_socket->connectToHost("52.14.84.3", 9001); // CSCI 150 SERVER
    // Connect the socket to the host
    //my_tcp_socket->connectToHost("52.14.84.3", 9001); // CSCI 150 SERVER
    //my_tcp_socket->connectToHost("localhost", 9001);
    // If it ever disconnects (including while trying this), the socket will
    // continuouslyq try to reconnect. See reconnect_socket().
}

void SGTCPSocket::write(QString message)
{
    QString message_length = QString::number(message.size());
    message = message.prepend(message_length.rightJustified(5, '0', true));
    my_tcp_socket->write(message.toLatin1());
}

void SGTCPSocket::write(QByteArray message)
{
    QByteArray message_length = QByteArray::number(message.size());
    message = message.prepend(message_length.rightJustified(5, '0', true));
    my_tcp_socket->write(message);
}

void SGTCPSocket::error(QAbstractSocket::SocketError err)
{
    qDebug() << my_tcp_socket->errorString();
}

void SGTCPSocket::reconnect_socket(QAbstractSocket::SocketState current_state)
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
        my_tcp_socket->connectToHost("52.14.84.3", 9001); // CSCI 150 SERVER
    }

    if(QAbstractSocket::ConnectedState == current_state)
    {
        if(reconnecting == true) {
            QMessageBox connected_box;
            connected_box.setText("Connection to server has been reestablished.");
            connected_box.exec();
            reconnecting = false;
        }
        emit connected();
    }
}

bool SGTCPSocket::read_socket_helper(QString& out_message)
{
    qDebug() << "Sending info...";
    success_flag = false;
    fail_flag = false;
    success_message = nullptr;
    if((QAbstractSocket::ConnectedState == my_tcp_socket->state()) && (my_tcp_socket->waitForReadyRead(5000)))
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
    } else {
        QMessageBox timeout_box;
        timeout_box.setText("Network Operation Timeout");
        timeout_box.setInformativeText("Either you aren't connected to the internet, or the server is down.");
        timeout_box.setIcon(QMessageBox::Warning);
        timeout_box.exec();
        return false; // Timeout
    }
}

QByteArray SGTCPSocket::single_message()
{
    qDebug() << "Receiving info...";
    QString message_size_str = my_tcp_socket->read(5);  // Read first 5 bytes, which is the serialized message size
    qDebug() << "Message size: " << message_size_str;

    int message_size = message_size_str.toInt();    // Convert the size to an integer
    while((my_tcp_socket->bytesAvailable() < message_size) && (my_tcp_socket->waitForReadyRead())) {
        qDebug() << "Message not here yet... bytes: " << my_tcp_socket->bytesAvailable();
        my_tcp_socket->waitForBytesWritten();
        // Do nothing until the right amount is available!
    }
    return my_tcp_socket->read(message_size); // Read the message, which is the next size bytes
}

QString SGTCPSocket::get_object_name(QByteArray &message)
{
    QString message_string(message);
    QString first_section = message_string.section(' ', 0, 0);
    qDebug() << "*** CHECKING First section: " << first_section;
    QString code = first_section.left(4);  // Get the code
    first_section.remove(0, 4);  // Remove the code
    qDebug() << "before remove" << message;

    qDebug() << "after remove" << message;
    if ((code == "USCH") || (code == "NUSR") || (code == "NCHT"))
    {
        message.remove(4, first_section.length());
        return first_section;
    }
    else if((code == "WBLN") || (code == "NUWB") || (code == "WBUP"))
    {
        message.remove(4, first_section.length());
        return first_section + " whiteboard";
    }
    else if((code == "FCFT") || (code == "FCBK"))
    {
        message.remove(4, first_section.length());
        return first_section + " flashcard";
    }
    else if(code == "REQQ")
    {
        return "startpage";
    }
    else if (code == "RUSR"){
        message.remove(4, first_section.length());
        return "recover username";
    }
    else if(code == "RPWD"){
        qDebug() << "Recover password message";
        return "reset password";
    }

}

void SGTCPSocket::read_socket_send_signal()
{
    while(my_tcp_socket->bytesAvailable() >= 5)
    {
        QByteArray message_ba = single_message();

        QString server_code = QString(); // Remove the server code
        qDebug() << "Server code: " << server_code;

        // First short circuit in case its just a succ or fail message
        if (QString(message_ba).left(4) == "SUCC")
        {
            qDebug() << "SUCCESSSSSSS";
            // Set the success flag and message
            success_flag = true;
            message_ba.remove(0, 4);
            success_message = message_ba;
            qDebug() << "Server message: " << success_message;
        }
        else if (QString(message_ba).left(4) == "FAIL")
        {
            // Set the fail flag
            fail_flag = true;
            message_ba.remove(0, 4);
            // Display the failure message to user
            QMessageBox timeout_box;
            timeout_box.setText("Error");
            timeout_box.setInformativeText(message_ba);
            timeout_box.setIcon(QMessageBox::Warning);
            timeout_box.exec();
        }
        else {
            QString object_name = get_object_name(message_ba);
            emit new_message(object_name, message_ba);
        }

        /*if (server_code == "USCH") // User List has CHANGED
        {
            // Notify the relevant group widget
        }
        else if (server_code == "NUSR") // New User code
        {
            QString new_username = message_ba;
            qDebug() << "New user: " << new_username;
            //emit user_joined(new_username);
        }
        else if (server_code == "NCHT")
        {
            QString message = message_ba;
            qDebug() << message;
            QString username = message.section(' ', 0, 0);
            QString str_date_time = message.section(' ', 1, 2);
            QDateTime date_time = QDateTime::fromString(str_date_time, "yyyy-MM-dd HH:mm:ss");
            date_time.setTimeSpec(Qt::UTC);
            //QDateTime updated_date_time(date_time.toTimeSpec(timestamps.timeSpec()));
            //QString updated_time = updated_date_time.toString("yyyy-MM-dd hh:mm:ss AP");
            QString chat = message.section(' ', 3, -1);
            //emit new_chat(username, updated_time, chat);
        }
        else if (server_code == "FCFT")
        {
            QString flash_str = message_ba;
            int flashcard_id = flash_str.section(' ', 0, 0).toInt();
            QString flashcard_front = flash_str.section(' ', 1, -1);
            //emit new_flashcard(flashcard_id, flashcard_front, true);
            qDebug() << flashcard_id << flashcard_front << "front";
        }
        else if (server_code == "FCBK")
        {
            QString flash_str = message_ba;
            int flashcard_id = flash_str.section(' ', 0, 0).toInt();
            QString flashcard_back = flash_str.section(' ', 1, -1);
            //emit new_flashcard(flashcard_id, flashcard_back, false);
            qDebug() << flashcard_id << flashcard_back << "back";
        }*/
    }

    return;
}
