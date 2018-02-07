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
    static void send(const QString& outgoing_message);
    static bool request_response(QString outgoing_message, QString& response);
    static void test(QString key, QString test_message);
    //

protected:
    explicit server(QObject *parent = nullptr);  // Protect the constructor to prevent class instantiation

signals:
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
    // Group Slots
    void send_chat(QString& groupID, QString& message);
    // Whiteboard Slots
    void send_whiteboard_line(const QString& groupID, const QPoint& point1, const QPoint& point2, const QColor& pen_color, const int& pen_size);
    void send_whiteboard(QString& ip, QByteArray* whiteboard);
    void save_whiteboard(QString& group_id, QByteArray* whiteboard);
    void send_card(QString&, QString&, int&, int&);

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
