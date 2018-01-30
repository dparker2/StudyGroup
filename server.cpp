#include "server.h"
#include <QDebug>
#include <QMessageBox>
#include <QDataStream>

// Testing changes
// More changes
// TESTING

QMap<QString, SGWidget*> server::_object_dictionary;
SGTCPSocket server::my_socket;

server::server(QObject *parent) : QObject(parent)
{
    //my_socket->connectToHost("18.221.67.202", 9001); // CSCI 150 SERVER
    //my_socket->connectToHost("localhost", 24680);
    //my_socket->connectToHost("52.53.198.189", 24680); // David's AWS server
}

/***
 * Public
 * Functions
 * API
 */
void server::initialize_connection()
{
    connect(&sg_socket, SGTCPSocket::new_message, incoming_message);
}

void server::add(QString key, SGWidget *value)
{
    server::_object_dictionary.insert(key, value);
    qDebug() << "Server: added (" << key << "," << value << ")";
}

void server::remove(QString class_key)
{
    server::_object_dictionary.remove(class_key);
}

void server::remove(SGWidget *object_pointer)
{
    for (QMap<QString, SGWidget*>::iterator i = _object_dictionary.begin(); i != _object_dictionary.end(); i++) {
        if (i.value() == object_pointer) {
            _object_dictionary.remove(i.key());
        }
    }
}

void server::test(QString key, QString test_message)
{
    _object_dictionary[key]->enqueue(test_message);
}

/***
 * SLOTS
 */

void server::incoming_message()
{

}

/*********************************************************************************/

/***
 * bool verifyUserInfo(QString& username, QString& password)
 *  returns true if found
 *          false if not
 */

void server::setTimestamps(char arg)
{
    if(arg == server::timestamp_local) {
        timestamps.setTimeSpec(Qt::LocalTime);
    }
    else if(arg == server::timestamp_utc) {
        timestamps.setTimeSpec(Qt::UTC);
    }
}

/*
 * ACCOUNTS
 */

bool server::login(QString& username, QString& password, QString& email)
{
    // Socket connected at this point, pass through info
    //my_socket->write(format_socket_request("LOGN", QString(username+" "+password)));
    //return read_socket_helper(email); // TODO: Handle receiving the email when it is passed back in reply
}

bool server::create_account(QString& username, QString& password, QString& email)
{
    // Socket connected at this point, pass through info
    //my_socket->write(format_socket_request("CACC", QString(username+" "+password+" "+email)));
    QString message;
    //bool ret = read_socket_helper(message);
    //if(ret)
    {
        QMessageBox success_box;
        success_box.setText(message);
        success_box.exec();
    }
    //return ret;
}

bool server::recover_user(QString& email, QString& user){
    //my_socket->write(format_socket_request("RUSR", QString(email)));
    //return read_socket_helper(user);
}

bool server::recover_pass(QString& username, QString& email, QString& pass){
    //my_socket->write(format_socket_request("RUSP", QString(username+" "+email)));
    //return read_socket_helper(pass);
}

bool server::logout()
{
    //my_socket->write(format_socket_request("LOGT", QString("")));
    return true;
}

/*
 * GROUPS
 */

bool server::create_group(QString& group_name, QString& group_id)
{
    // Pass through info
    //my_socket->write(format_socket_request("CGRP", QString(group_name)));
    //return read_socket_helper(group_id);
}

bool server::join_group(QString &group_id)
{
    //my_socket->write(format_socket_request("JGRP", QString(group_id)));
    QString _str;
    //return read_socket_helper(_str);
}

bool server::leave_group(QString &group_id)
{
    //my_socket->write(format_socket_request("LGRP", QString(group_id)));
    QString _str;
    //return read_socket_helper(_str);
}

void server::send_chat(QString& groupID, QString& message)
{
    //my_socket->write(format_socket_request("GCHT", QString(groupID+" "+message)));
    if (message.startsWith("devhack ")) {
        message.remove(0, 8);
        qDebug() << format_socket_request("", message);
        //my_socket->write(format_socket_request("", message));
    }
    // No success
}

void server::send_whiteboard_line(const QString &groupID, const QPoint &point1, const QPoint &point2, const QColor &pen_color, const int &pen_size)
{
    /*my_socket->write(format_socket_request("WBLN", QString(groupID+" "+
                                                           pen_color.name()+" "+
                                                           QString::number(pen_size)+" "+
                                                           QString::number(point1.x())+" "+
                                                           QString::number(point1.y())+" "+
                                                           QString::number(point2.x())+" "+
                                                           QString::number(point2.y()))));*/
    // No success message
}

void server::send_whiteboard(QString& ip, QByteArray* whiteboard)
{
    qDebug() << "Whiteboard ByteArray size: " << whiteboard->size();
    ip.append(" ");
    QByteArray arg = ip.toLatin1();
    arg += *whiteboard;
    //my_socket->write(format_socket_request("UPWB", arg));
    delete whiteboard; // Delete the byte array from heap
    // No success message
}

void server::save_whiteboard(QString& group_id, QByteArray *whiteboard)
{
    QString group_arg = group_id+" ";
    QByteArray arg = group_arg.toLatin1();
    arg += *whiteboard;
    //my_socket->write(format_socket_request("SVWB", arg));
    delete whiteboard;
}

/*
 *
 *
 * SOCKET SLOTS
 *
 *
 */

//void server::read_socket_send_signal()
//{
    /*while(my_socket->bytesAvailable() >= 5)
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
        while((my_socket->bytesAvailable() < message_size) && (my_socket->waitForReadyRead())) {
            qDebug() << "Message not here yet... bytes: " << my_socket->bytesAvailable();
            my_socket->waitForBytesWritten();
            // Do nothing until the right amount is available!
        }
        QByteArray message_ba = my_socket->read(message_size); // Read the message, which is the next size bytes
        QString server_code = message_ba.left(4);   // Get the server code
        message_ba.remove(0, 4); // Remove the server code
        qDebug() << "Server code: " << server_code;

        // Hinge on server_code
        if (server_code == "SUCC")      // Success code
        {
            // Set the success flag and message
            success_flag = true;
            success_message = message_ba;
            qDebug() << "Server message: " << success_message;
        }
        else if (server_code == "FAIL") // Fail code
        {
            // Set the fail flag
            fail_flag = true;
            // Display the failure message to user
            QMessageBox timeout_box;
            timeout_box.setText("Error");
            timeout_box.setInformativeText(message_ba);
            timeout_box.setIcon(QMessageBox::Warning);
            timeout_box.exec();
        }
        else if (server_code == "USCH") // User List has CHANGED
        {
            emit users_changed();
        }
        else if (server_code == "NUSR") // New User code
        {
            QString new_username = message_ba;
            qDebug() << "New user: " << new_username;
            emit user_joined(new_username);
        }
        else if (server_code == "NCHT")
        {
            QString message = message_ba;
            qDebug() << message;
            QString username = message.section(' ', 0, 0);
            QString str_date_time = message.section(' ', 1, 2);
            QDateTime date_time = QDateTime::fromString(str_date_time, "yyyy-MM-dd HH:mm:ss");
            date_time.setTimeSpec(Qt::UTC);
            QDateTime updated_date_time(date_time.toTimeSpec(timestamps.timeSpec()));
            QString updated_time = updated_date_time.toString("yyyy-MM-dd hh:mm:ss AP");
            QString chat = message.section(' ', 3, -1);
            emit new_chat(username, updated_time, chat);
        }
        else if (server_code  == "WBLN")
        {
            QString line_str = message_ba;
            qDebug() << line_str;
            QColor pen_color(line_str.section(' ', 0, 0));
            int pen_size = line_str.section(' ', 1, 1).toInt();
            QString x1 = line_str.section(' ', 2, 2);
            QString y1 = line_str.section(' ', 3, 3);
            QString x2 = line_str.section(' ', 4, 4);
            QString y2 = line_str.section(' ', 5, -1);
            QPoint point1(x1.toInt(), y1.toInt());
            QPoint point2(x2.toInt(), y2.toInt());
            qDebug() << point1 << point2;
            emit whiteboard_draw_line(point1, point2, pen_color, pen_size);
        }
        else if (server_code == "NUWB")
        {
            QString user_needs_wb = message_ba;
            qDebug() << "server.cpp NUWB";
            emit get_whiteboard(user_needs_wb);
        }
        else if (server_code == "WBUP")
        {
            QByteArray wb_string = message_ba;
            qDebug() << "wb string size: " << wb_string.size();
            emit update_whiteboard(&wb_string);
        }
        else if (server_code == "FCFT")
        {
            QString flash_str = message_ba;
            int flashcard_id = flash_str.section(' ', 0, 0).toInt();
            QString flashcard_front = flash_str.section(' ', 1, -1);
            emit new_flashcard(flashcard_id, flashcard_front, true);
            qDebug() << flashcard_id << flashcard_front << "front";
        }
        else if (server_code == "FCBK")
        {
            QString flash_str = message_ba;
            int flashcard_id = flash_str.section(' ', 0, 0).toInt();
            QString flashcard_back = flash_str.section(' ', 1, -1);
            emit new_flashcard(flashcard_id, flashcard_back, false);
            qDebug() << flashcard_id << flashcard_back << "back";
        }
    }

    return;
    // TODO: Implement if statements and send signals based on what was received.*/
//}

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

QByteArray server::format_socket_request(const QString &request_code, const QByteArray &request_arg)
{
    QByteArray full_request = request_code.toLatin1() + request_arg;
    qDebug() << full_request;
    QString request_length = QString::number(full_request.size());
    full_request = full_request.prepend(request_length.rightJustified(5, '0', true).toLatin1());
    //qDebug() << "Sending: " << full_request;
    return full_request;
}


void server::send_card(QString& groupID, QString& card_text, int& card_num, int& card_side)
{
    qDebug() << "SEND CARD" << endl;
    if(card_side == 0){
        //my_socket->write(format_socket_request("FCFT", groupID+" "+QString::number(card_num)+" "+card_text));
    }
    else{
        //my_socket->write(format_socket_request("FCBK", groupID+" "+QString::number(card_num)+" "+card_text));
    }
    QString returned_index;
    //read_socket_helper(returned_index);
    card_num = returned_index.toInt();
    qDebug() << card_num;
}

