#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "bigscreen_defines.hpp"
#include "bigscreennetwork.h"

#include "bigscreencontroller.h"

#include <QListWidgetItem>
#include <QProcess>

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
    void on_pb_twitch_clicked();
    void on_pb_tv_clicked();
    void on_pb_apps_clicked();
    void on_pb_youtube_clicked();

    void itemClicked(QListWidgetItem *item);

    void modelUpdated(BigScreenModel& model);
private:
    Ui::MainWindow *ui;
    BigScreenNetwork net;

    BigScreenTab currentTab;
    BigScreenController controller;

    void markAsSelected(BigScreenTab tab);

};
#endif // MAINWINDOW_H
