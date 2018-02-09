#include "groupwidget.h"
#include "ui_groupwidget.h"
#include <QTime>
#include <QDebug>
#include <QLine>
#include <QVBoxLayout>

GroupWidget::GroupWidget(QString id, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GroupWidget)
{
    ui->setupUi(this);
    group_id = id;

    // Begin set whiteboard
    whiteboard = new Whiteboard(group_id + " whiteboard", nullptr, ui->save_whiteboard_button);

    //qDebug() << connect(whiteboard, SIGNAL(line_drawn(QPoint,QPoint,QColor,int)), this, SLOT(send_line_drawn(QPoint,QPoint,QColor,int)));
    //qDebug() << connect(this, SIGNAL(whiteboard_draw_line(QPoint,QPoint,QColor,int)), whiteboard, SLOT(draw_line(QPoint,QPoint,QColor,int)));

    ui->study_mode->addWidget(whiteboard);
    ui->study_mode->setCurrentWidget(whiteboard);
    ui->study_menu->setCurrentIndex(0);
    whiteboard->set_pen_color(QColor("#000"));
    QString pen_string = ui->comboBox_pen_size->currentText();
    pen_string.chop(2);
    whiteboard->set_pen_size(pen_string.toInt());
    //ui->study_mode->setStyleSheet("background-color: #ffffff;");
    // End set whiteboard
    // Flaschard

    deck = new Deck();
    ui->study_mode->insertWidget(1, deck);
    setFlashcardUI();

    connect(deck, SIGNAL(set_card(QString,int&,int)), this, SLOT(set_card(QString,int&,int)));


}

/********
 *
 * PUBLIC
 *
 */

QString GroupWidget::get_groupID()
{
    return group_id;
}

Whiteboard* GroupWidget::whiteboard_ptr()
{
    return whiteboard;
}

/********
 *
 * SLOTS
 *
 */

void GroupWidget::new_chat(QString username, QString time, QString message)
{
    qDebug() << message;
    QDate new_date = QDate::fromString(time.section(' ', 0, 0), "yyyy-MM-dd");
    qDebug() << time.section(' ', 0, 0);
    time.remove(0, 11);
    qDebug() << new_date.toString("dddd MMM dd yyyy");
    qDebug() << last_date_printed.toString("dddd MMM dd yyyy");
    if(new_date != last_date_printed)
    {
        ui->chat_box->append(new_date.toString("dddd MMM dd yyyy"));
        last_date_printed = new_date;
    }
    ui->chat_box->append(time+" - "+username+": "+message);
}

void GroupWidget::users_changed()
{
    QLayoutItem* item;
    while ( ( item = ui->username_layout->takeAt( 0 ) ) != NULL )
    {
        delete item->widget();
        delete item;
    }
}

void GroupWidget::user_joined(QString username)
{
    QLabel* username_label = new QLabel(username);
    username_label->setStyleSheet("color: white");
    username_label->setFont(QFont("Trebuchet MS", 20));
    ui->username_layout->addWidget(username_label);
}

void GroupWidget::set_groupID(QString &groupID)
{
    group_id = groupID;
    ui->groupid_label->setText("GroupID: "+groupID);
}

void GroupWidget::send_line_drawn(const QPoint& first_mouse_pos, const QPoint& second_mouse_pos, const QColor& pen_color, const int& pen_size)
{
    ui->save_whiteboard_button->setEnabled(true);
    emit line_drawn(group_id, first_mouse_pos, second_mouse_pos, pen_color, pen_size);
}

/********
 *
 * UI SLOTS
 *
 */

void GroupWidget::on_submit_chat_released()
{
    QString chat_message = ui->chat_input->text();
    if(chat_message != "") {
        ui->chat_input->setText("");
        QString groupID = group_id;
        emit send_chat(groupID, chat_message); // Send the chat signal
    }
}

void GroupWidget::on_save_whiteboard_button_released()
{
    qDebug() << "saving_whiteboard";
    emit save_whiteboard(group_id, whiteboard->whiteboard_ba());
    ui->save_whiteboard_button->setEnabled(false);
}

void GroupWidget::on_comboBox_pen_color_currentTextChanged(const QString &pen_color)
{
    if(pen_color == "Black")
    {
        whiteboard->set_pen_color(QColor("#000"));
    }
    else if(pen_color == "Red")
    {
        whiteboard->set_pen_color(QColor("#f00"));
    }
    else if(pen_color == "Green")
    {
        whiteboard->set_pen_color(QColor("#0f0"));
    }
    else if(pen_color == "Blue")
    {
        whiteboard->set_pen_color(QColor("#00f"));
    }
}

void GroupWidget::on_comboBox_pen_size_currentTextChanged(const QString &pen_size)
{
    QString p_size = pen_size;
    p_size.chop(2);
    whiteboard->set_pen_size(p_size.toInt());
}

void GroupWidget::setFlashcardUI(){
    //deck->card_label->hide();
}

void GroupWidget::on_comboBox_study_mode_currentIndexChanged(int index)
{
    if(index){
        ui->study_mode->setCurrentWidget(deck);
        ui->study_menu->setCurrentIndex(1);
    }
    else{
        ui->study_mode->setCurrentWidget(whiteboard);
        ui->study_menu->setCurrentIndex(0);
    }
}

void GroupWidget::on_add_card_button_clicked()
{
    deck->add_card();
}

void GroupWidget::set_card(QString front, int& card_num, int side){
    QString groupID = get_groupID();
    qDebug() << "SET FRONT" << endl;
    emit send_card(groupID, front, card_num, side);
}

void GroupWidget::incoming_card(int card_index, QString text, bool front)
{
    deck->init_card(card_index, text, front); // Will edit or create new card
}

void GroupWidget::on_pushButton_clicked()
{
    ui->study_menu->setCurrentIndex(0);
    ui->study_mode->setCurrentWidget(whiteboard);
    ui->comboBox_study_mode->setCurrentIndex(0);
}

void GroupWidget::on_quiz_button_toggled(bool checked)
{
    deck->set_quiz(checked);
}
