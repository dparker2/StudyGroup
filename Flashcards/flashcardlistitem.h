#ifndef FLASHCARDLISTITEM_H
#define FLASHCARDLISTITEM_H

#include <QWidget>
#include <QLabel>

namespace Ui {
class FlashcardListItem;
}

class FlashcardListItem : public QWidget
{
    Q_OBJECT

public:
    explicit FlashcardListItem(QWidget *parent = 0);
    ~FlashcardListItem();

    void set_index(QString& index);
    void set_front(QString& text);
    void set_back(QString& text);

signals:
    void card_clicked(int);

protected:
    void mouseReleaseEvent(QMouseEvent* event);

private:
    void put_ellipsis(QLabel* label, QString& text);
    Ui::FlashcardListItem *ui;
};

#endif // FLASHCARDLISTITEM_H
