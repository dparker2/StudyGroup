#include "server.h"
#include <QDebug>
#include <QMessageBox>
#include <QDataStream>

// Testing changes
// More changes
// TESTING

QMap<QString, SGWidget*> server::_object_dictionary;
SGTCPSocket server::sg_socket;

const QString server::CREATE_ACCOUNT = "CACC";
const QString server::LOGIN = "LOGN";
const QString server::LOGOUT = "LOGT";
const QString server::RECOVER_USERNAME = "RUSR";
const QString server::RECOVER_PASSWORD = "RUSP";
const QString server::GROUP_CREATE = "CGRP";
const QString server::GROUP_JOIN = "JGRP";
const QString server::GROUP_LEAVE = "LGRP";
const QString server::CHAT_SEND = "GCHT";
const QString server::WHITEBOARD_DRAW = "WBLN";
const QString server::WHITEBOARD_UPDATE = "UPWB";
const QString server::WHITEBOARD_SAVE = "SVWB";
const QString server::FLASHCARD_SET_FRONT = "FCFT";
const QString server::FLASHCARD_SET_BACK = "FCBK";
const QString server::HOMEPAGE_UPDATE = "UPHP";
const QString server::HOMEPAGE_NEW_FAVORITE = "NWFG";
const QString server::HOMEPAGE_REMOVE_FAVORITE = "RMFG";

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
void server::initialize()
{
    sg_socket.connect_server();
    connect(&sg_socket, &SGTCPSocket::new_message, incoming_message);
}

void server::clear_memory()
{

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
    QMap<QString, SGWidget*>::iterator i = _object_dictionary.begin();
    while (i != _object_dictionary.end()) {
        if (i.value() == object_pointer)
            i = _object_dictionary.erase(i);
        else
            ++i;
    }
}

void server::send(const QString &outgoing_message)
{
    //qDebug() << "server sending" << outgoing_message;
    sg_socket.write(outgoing_message);
}

void server::send(const QByteArray &outgoing_message)
{
    sg_socket.write(outgoing_message);
}

void server::send(const char *outgoing_message)
{
    qDebug() << "server sending" << outgoing_message;
    sg_socket.write(QString(outgoing_message));
}

bool server::request_response(QString outgoing_message, QString &response)
{
    sg_socket.write(outgoing_message);
    return sg_socket.read_socket_helper(response);
}

void server::test(QString key, QString test_message)
{
    //_object_dictionary[key]->enqueue(test_message);
}

/***
 * SLOTS
 */

void server::incoming_message(QString& object_name, QByteArray& work_message)
{
    qDebug() << "Object name:" << object_name;
    qDebug() << "Message:" << work_message;
    qDebug() << "\n";
    if (_object_dictionary.contains(object_name) && (_object_dictionary[object_name] != nullptr))
    {
        _object_dictionary[object_name]->enqueue(work_message);
    }
}

void server::setTimestamps(char arg)
{
    if(arg == server::timestamp_local) {
        timestamps.setTimeSpec(Qt::LocalTime);
    }
    else if(arg == server::timestamp_utc) {
        timestamps.setTimeSpec(Qt::UTC);
    }
}

