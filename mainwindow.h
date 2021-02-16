#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "bigscreen_defines.hpp"
#include "bigscreennetwork.h"

#include <QListWidgetItem>
#include <QProcess>
#include <QtGamepad/QGamepad>

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
    void twitchListRetrieved(Result<QStringList, QString>& list);

    void itemClicked(QListWidgetItem *item);

    void processStarted();
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void joypadUpDown(double value);
    void buttonXChanged(bool pressed);
private:
    Ui::MainWindow *ui;
    BigScreenNetwork net;
    QProcess proc;

    BigScreenTab currentTab;

    QGamepad* pad;

    void markAsSelected(BigScreenTab tab);

};
#endif // MAINWINDOW_H
