#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QtNetwork>
#include <QTimer>
#include <QDateTime>

#include <sgwidget.h>

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
    void connect_server();  // Old one
    void initialize_connection();  // New one
    void setTimestamps(char arg);  // Figure this out later
    // Account Functions
    bool login(QString& username, QString& password, QString& email);  // Replace all of these with static functions
    bool create_account(QString& username, QString& password, QString& email);  //  |
    bool recover_user(QString& email, QString& user);                           //  |
    bool recover_pass(QString& username, QString& email, QString& pass);        //  |
    bool logout();                                                              //  |
    // Group Functions                                                          //  |
    bool create_group(QString& group_name, QString& group_id);                  //  |
    bool join_group(QString& group_id);                                         //  |
    bool leave_group(QString& group_id);                                        // <-

    // New stuff
    static void remove(QString class_key);  // Removes the key value pair from dict
    static void remove(SGWidget* object_pointer);  // Removes all keys with that value
    static void add(QString key, SGWidget* value);  // Adds key value to dict, overwriting if key already exists.
    static void test(QString key, QString test_message);
    //

protected:
    explicit server(QObject *parent = nullptr);  // Protect the constructor to prevent class instantiation

signals:
    void disconnected();
    void connected();
    // Group Signals
    void new_chat(QString,QString,QString);
    void users_changed();
    void user_joined(QString);
    void user_left(QString);
    // Whiteboard
    void whiteboard_draw_line(const QPoint&, const QPoint&, const QColor&, const int&);
    void get_whiteboard(QString);
    void update_whiteboard(QByteArray*);
    // Flashcard
    void new_flashcard(const int id, const QString&, const bool front);

public slots:
    // Socket helper functions
    void reconnect_socket(QAbstractSocket::SocketState);
    void read_socket_send_signal();  // These will be moved to custom TCPSocket class which is instantiated and so will send signals to this class
    // Group Slots
    void send_chat(QString& groupID, QString& message);
    // Whiteboard Slots
    void send_whiteboard_line(const QString& groupID, const QPoint& point1, const QPoint& point2, const QColor& pen_color, const int& pen_size);
    void send_whiteboard(QString& ip, QByteArray* whiteboard);
    void save_whiteboard(QString& group_id, QByteArray* whiteboard);
    void error(QAbstractSocket::SocketError);
    void send_card(QString&, QString&, int&, int&);

private:
    // New stuff
    static QMap<QString, SGWidget*> _object_dictionary;
    //

    QTcpSocket* my_socket;
    bool reconnecting;
    QDateTime timestamps;
    QString username;
    bool success_flag; // ***Always set this to 0 before checking, ONLY to be set to 1 by the read_socket_send_signal() function.
    bool fail_flag;    // ***Always set this to 0 before checking, ONLY to be set to 1 by the read_socket_send_signal() function.
    QString success_message; // ***Always set this to nullptr before checking, ONLY to be set by the read_socket_send_signal() function.

    QByteArray format_socket_request(const QString& request_code, QString request_arg);
    QByteArray format_socket_request(const QString &request_code, const QByteArray& request_arg);
    bool read_socket_helper(QString& out_message);
};

#endif // SERVER_H
