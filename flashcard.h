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
    ~Flashcard();
    Flashcard(QString, QString, int);
    void setQuestion(QString);
    void setAnswer(QString);
    void setCardNum(int);
    QString getQuestion();
    QString getAnswer();
    int getCardNum();

    void setEditBtn();


signals:

public slots:

private slots:
    void on_edit_card_btn_clicked();

    void on_set_front_btn_clicked();

    void on_pushButton_3_clicked();

    void on_flip_card_btn_clicked();

private:
    Ui::Flashcard *ui;
    QString back_text;
    QString front_text;
    int cardNum;
};

#endif // FLASHCARD_H
