#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QProcess>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(this->ui->lst_stuff, SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(itemClicked(QListWidgetItem*)));

    connect(&controller, SIGNAL(modelUpdated(BigScreenModel&)),
            this, SLOT(modelUpdated(BigScreenModel&)));

    controller.initialize();
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
        // net.requestTwitchWebsites();
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
    this->controller.tabClicked(Twitch);
}
void MainWindow::on_pb_apps_clicked() {
    this->controller.tabClicked(Twitch);
}
void MainWindow::on_pb_tv_clicked() {
    this->controller.tabClicked(Twitch);
}
void MainWindow::on_pb_youtube_clicked() {
    this->controller.tabClicked(Twitch);
}

void MainWindow::itemClicked(QListWidgetItem*)
{
    auto row = this->ui->lst_stuff->currentRow();
    this->controller.listItemClicked(row);
}

void MainWindow::modelUpdated(BigScreenModel& model) {

    // update tab
    if(model.selectedTab.isNone()) {
        colorizeAsNotSelected(this->ui->pb_apps);
        colorizeAsNotSelected(this->ui->pb_tv);
        colorizeAsNotSelected(this->ui->pb_twitch);
        colorizeAsNotSelected(this->ui->pb_youtube);
    }
    else {
        this->markAsSelected(model.selectedTab.value());
    }

    // update list
    this->ui->lst_stuff->clear();
    this->ui->lst_stuff->addItems(model.items);

    // update list selection
    if(model.selectedListItem.isNone()) {
    }
    else {
       this->ui->lst_stuff->setCurrentRow(model.selectedListItem.value());
    }

    // update status bar
    this->statusBar()->showMessage(model.statusBarText);

    // update debug view
    this->ui->pte_debug->clear();
    this->ui->pte_debug->appendPlainText(model.debugText);

    // fullscreen
    if(model.showFullScreen) {
        this->showMaximized();
    }
    else {
        this->showNormal();
    }

    // freeze interface if requested
    this->ui->pb_apps->setEnabled(!model.freezeInterface);
    this->ui->pb_tv->setEnabled(!model.freezeInterface);
    this->ui->pb_twitch->setEnabled(!model.freezeInterface);
    this->ui->pb_youtube->setEnabled(!model.freezeInterface);
    this->ui->lst_stuff->setEnabled(!model.freezeInterface);
}
