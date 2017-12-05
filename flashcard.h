#ifndef FLASHCARD_H
#define FLASHCARD_H

#include <QWidget>
#include <QHBoxLayout>

namespace Ui {
class Flashcard;
}

class Flashcard : public QWidget
{
    Q_OBJECT
public:
    explicit Flashcard(QWidget *parent = nullptr);
    explicit Flashcard(QString, QString, int, QWidget *parent = nullptr);
    ~Flashcard();
    void setFront(QString);
    void setBack(QString);
    void setCardNum(int);
    QString getFront();
    QString getBack();
    int getCardNum();

    void setEditBtn();
    void emit_init_signal();

signals:


    void check_set_card(Flashcard*, QString&, int&, int);

public slots:

private slots:
    void on_edit_card_btn_clicked();

    void on_set_front_btn_clicked();

    void on_cancel_btn_clicked();

    void on_flip_card_btn_clicked();

private:
    Ui::Flashcard *ui;
    QString back_text;
    QString front_text;
    int cardNum;
};

#endif // FLASHCARD_H
