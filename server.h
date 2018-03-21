#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QtNetwork>
#include <QTimer>
#include <QDateTime>

#include <sgwidget.h>
#include <sgtcpsocket.h>

/***
 *
 * Purely static class used for interfacing with StudyGroup Server.
 * This class needs to be initialized once in order to begin.
 *
 * It is meant to run in its own thread, and must be provided a <QString, SGWidget*> pair
 * to notify a class of incoming events based on its QString key received from the server.
 *
 * Key formats **must** be consistent for this to work properly.
 *
 * Example: Incoming socket is a write to a whiteboard in biology_3485. This class will search
 * its internal dictionary for this: <QString("biology_3485 whiteboard"), Whiteboard*> and
 * add this message to the Whiteboard*'s work queue. The SGWidget*'s job is to interpret this message.
 *
 */

class server : public QObject
{
    Q_OBJECT
public:
    const static char timestamp_utc = 2;  // Figure this out later
    const static char timestamp_local = 1;  // Figure this out later
    static void initialize();  // New one
    static void clear_memory();  // New thing
    void setTimestamps(char arg);  // Figure this out later

    // New stuff
    static void remove(QString class_key);  // Removes the key value pair from dict
    static void remove(SGWidget* object_pointer);  // Removes all keys with that value
    static void add(QString key, SGWidget* value);  // Adds key value to dict, overwriting if key already exists.
    static void send(const QString& outgoing_message);
    static void send(const QByteArray& outgoing_message);
    static void send(const char* outgoing_message);
    static bool request_response(QString outgoing_message, QString& response);
    static void test(QString key, QString test_message);
    //

    /* Server codes! */
    static const QString CREATE_ACCOUNT;
    static const QString LOGIN;
    static const QString LOGOUT;
    static const QString RECOVER_USERNAME;
    static const QString RECOVER_PASSWORD;
    static const QString GROUP_CREATE;
    static const QString GROUP_JOIN;
    static const QString GROUP_LEAVE;
    static const QString CHAT_SEND;
    static const QString WHITEBOARD_DRAW;
    static const QString WHITEBOARD_UPDATE;
    static const QString WHITEBOARD_SAVE;
    static const QString FLASHCARD_SET_FRONT;
    static const QString FLASHCARD_SET_BACK;
    static const QString HOMEPAGE_UPDATE;
    static const QString HOMEPAGE_NEW_FAVORITE;
    static const QString HOMEPAGE_REMOVE_FAVORITE;
    static const QString SOCIAL_SEARCH_USER;
    static const QString SOCIAL_ADD_FRIEND;
    static const QString SOCIAL_REMOVE_FRIEND;

protected:
    explicit server(QObject *parent = nullptr);  // Protect the constructor to prevent class instantiation

private slots:
    static void incoming_message(QString& object_name, QByteArray& work_message);

private:
    // New stuff
    static QMap<QString, SGWidget*> _object_dictionary;
    static SGTCPSocket sg_socket;
    //
    QDateTime timestamps;
    QString username;
    QByteArray format_socket_request(const QString& request_code, QString request_arg);
    QByteArray format_socket_request(const QString &request_code, const QByteArray& request_arg);
};

#endif // SERVER_H
