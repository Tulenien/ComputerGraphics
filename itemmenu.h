#ifndef ITEMMENU_H
#define ITEMMENU_H

#include <QWidget>

namespace Ui {
class itemMenu;
}

class itemMenu : public QWidget
{
    Q_OBJECT

public:
    explicit itemMenu(QWidget *parent = nullptr);
    ~itemMenu();
    void setCurrentIndex(int index);

private:
    Ui::itemMenu *ui;
    int currentIndex;
};

#endif // ITEMMENU_H
