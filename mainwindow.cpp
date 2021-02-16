#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QProcess>

#include <QtGamepad/QGamepad>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&net, SIGNAL(twitchListRetrieved(Result<QStringList, QString>&)),
            this, SLOT(twitchListRetrieved(Result<QStringList, QString>&)));

    connect(this->ui->lst_stuff, SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(itemClicked(QListWidgetItem*)));

    connect(&proc, SIGNAL(started()),
            this, SLOT(processStarted()));
    connect(&proc, SIGNAL(finished(int, QProcess::ExitStatus)),
            this, SLOT(processFinished(int, QProcess::ExitStatus)));

    auto gamepads = QGamepadManager::instance()->connectedGamepads();
    if(!gamepads.empty()) {
        this->ui->pte_debug->appendPlainText("got a gamepad!");
        this->pad = new QGamepad(*gamepads.begin(), this);
    }
    else {
        this->ui->pte_debug->appendPlainText("no gamepads found");
        this->pad = nullptr;
    }

    connect(pad, SIGNAL(axisLeftYChanged(double)),
            this, SLOT(joypadUpDown(double)));
    connect(pad, SIGNAL(buttonXChanged(bool)),
            this, SLOT(buttonXChanged(bool)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void colorizeAsSelected(QPushButton* lbl) {
    lbl->setStyleSheet("QPushButton { color: red; }");
}

void colorizeAsNotSelected(QPushButton* lbl) {
    lbl->setStyleSheet("QPushButton { color: lightgrey; }");
}

void MainWindow::markAsSelected(BigScreenTab tab) {
    colorizeAsNotSelected(this->ui->pb_apps);
    colorizeAsNotSelected(this->ui->pb_tv);
    colorizeAsNotSelected(this->ui->pb_twitch);
    colorizeAsNotSelected(this->ui->pb_youtube);

    currentTab = tab;

    switch(tab) {
    case Twitch: {
        colorizeAsSelected(this->ui->pb_twitch);
        net.requestTwitchWebsites();
        break;
    }
    case Youtube: {
        colorizeAsSelected(this->ui->pb_youtube);
        break;
    }
    case Applications: {
        colorizeAsSelected(this->ui->pb_apps);
        break;
    }
    case Television: {
        colorizeAsSelected(this->ui->pb_tv);
        break;
    }
    }
}

// slots
void MainWindow::on_pb_twitch_clicked() {
    this->markAsSelected(Twitch);
}
void MainWindow::on_pb_apps_clicked() {
    this->markAsSelected(Applications);
}
void MainWindow::on_pb_tv_clicked() {
    this->markAsSelected(Television);
}
void MainWindow::on_pb_youtube_clicked() {
    this->markAsSelected(Youtube);
}


void MainWindow::twitchListRetrieved(Result<QStringList, QString> &list) {
    if(!list.isError()) {
        this->ui->lst_stuff->clear();
        for(auto f: list.value()) {
            this->ui->lst_stuff->addItem(f);
        }
    }
}

void MainWindow::itemClicked(QListWidgetItem* item)
{
    if(currentTab == Twitch) {
        QString program = "streamlink";
        QStringList args;
        args << "--player";
        args << "mpv";
        args << "https://twitch.tv/" + item->text();
        args << "720p";

        this->ui->lst_stuff->setEnabled(false);
        this->ui->pb_apps->setEnabled(false);
        this->ui->pb_tv->setEnabled(false);
        this->ui->pb_twitch->setEnabled(false);
        this->ui->pb_youtube->setEnabled(false);

        this->ui->statusbar->showMessage("starting: " + program + " " + args.join(" "));

        proc.start(program, args);
    }
}

void MainWindow::processStarted()
{
    this->ui->pte_debug->appendPlainText("process started");
}

void MainWindow::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    this->ui->lst_stuff->setEnabled(true);
    this->ui->pb_apps->setEnabled(true);
    this->ui->pb_tv->setEnabled(true);
    this->ui->pb_twitch->setEnabled(true);
    this->ui->pb_youtube->setEnabled(true);
    this->ui->pte_debug->appendPlainText("process finished - (" + QString::number(exitCode) + ", status: " + QString::number(exitStatus) + ")");

    if(currentTab == Twitch) {
       if(exitStatus == QProcess::CrashExit) {
           this->ui->statusbar->showMessage("[twitch] the stream is not active");
       }
       else {
           this->ui->statusbar->showMessage("[twitch] you stopped watching this stream");
       }
    }
}

void MainWindow::joypadUpDown(double value)
{
    auto currentRow = this->ui->lst_stuff->currentRow();
//    this->ui->pte_debug->appendPlainText("[joypadUpDown] " + QString::number(value));
    if(value == -1) {
        this->ui->lst_stuff->setCurrentRow(std::max(currentRow-1, 0));
    }
    else if(value == 1) {
        this->ui->lst_stuff->setCurrentRow(std::min(currentRow+1, this->ui->lst_stuff->count()));
    }
}

void MainWindow::buttonXChanged(bool pressed)
{
    if(pressed) {
        auto currentItem = this->ui->lst_stuff->currentItem();
        this->itemClicked(currentItem);
    }
}
