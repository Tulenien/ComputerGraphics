#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "settings.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionNew_scene_triggered();

    void on_actionScene_triggered();

private:
    Ui::MainWindow *ui;
    Settings *windowSettings = nullptr;
};
#endif // MAINWINDOW_H
