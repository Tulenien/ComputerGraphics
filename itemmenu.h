#ifndef ITEMMENU_H
#define ITEMMENU_H

#include <QWidget>
#include "scene.h"

namespace Ui { class itemMenu; }

class itemMenu : public QWidget
{
    Q_OBJECT
public:
    itemMenu(QWidget *parent = nullptr);
    ~itemMenu();
    void setCurrentIndex(int index);
    int getCurrentIndex() const { return currentIndex; }
    void setScene(Scene &scene);

private:
    Ui::itemMenu *ui;
    int currentIndex;
    Scene *scene;
};


#endif // ITEMMENU_H
