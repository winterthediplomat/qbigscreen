#include "bigscreencontroller.h"
#include <QtGamepad/QGamepad>

BigScreenController::BigScreenController(QObject *parent) : QObject(parent)
{
    internal.state = InternalState::TabSelection;
}

void BigScreenController::initialize() {
    auto gamepads = QGamepadManager::instance()->connectedGamepads();
    if(!gamepads.empty()) {
        this->updateStatusBarText("gamepad found");
        this->pad = new QGamepad(*gamepads.begin(), this);
    }
    else {
        this->updateStatusBarText("gamepad is not available!");
        this->pad = nullptr;
    }

    connect(pad, SIGNAL(axisLeftXChanged(double)),
            this, SLOT(joypadLeftRight(double)));
    connect(pad, SIGNAL(axisLeftYChanged(double)),
            this, SLOT(joypadUpDown(double)));
    connect(pad, SIGNAL(buttonXChanged(bool)),
            this, SLOT(buttonXChanged(bool)));

    connect(&net, &BigScreenNetwork::twitchListRetrieved,
            this, &BigScreenController::twitchListRetrieved);

    connect(&livestreamer, SIGNAL(started()),
            this, SLOT(processStarted()));
    connect(&livestreamer, SIGNAL(finished(int, QProcess::ExitStatus)),
            this, SLOT(processFinished(int, QProcess::ExitStatus)));
}

void BigScreenController::updateStatusBarText(QString text) {
    model.statusBarText = text;
    emit modelUpdated(this->model);
}



void BigScreenController::joypadLeftRight(double value)
{
    if(value == -1) {
        this->joypadLeft();
    }
    else if(value == 1) {
        this->joypadRight();
    }
}

void BigScreenController::joypadUpDown(double value)
{
    if(value == -1) {
        this->joypadUp();
    }
    else if(value == 1) {
        this->joypadDown();
    }
}

void BigScreenController::buttonXChanged(bool pressed)
{
    if(pressed) {
        this->btnX();
    }
}

void BigScreenController::joypadUp() {
    bool updated = false;

    switch(internal.state) {
    case ApplicationRunning:
    case TabSelection:
        break;
    case ListItemSelection:
        updated = true;
        if(model.selectedListItem.isNone()) {
            model.selectedListItem = Option<int>::Some(0);
        }
        else {
             auto value = model.selectedListItem.value();
             value -= 1;
             value = 0 <= value ? value : 0;
             model.selectedListItem = Option<int>::Some(value);
        }
        break;
    }

    if(updated) {
        emit this->modelUpdated(this->model);
    }
}

void BigScreenController::joypadDown() {
    bool updated = false;

    switch(internal.state) {
    case ApplicationRunning:
    case TabSelection:
        break;
    case ListItemSelection:
        updated = true;
        if(model.selectedListItem.isNone()) {
            model.selectedListItem = Option<int>::Some(0);
        }
        else {
             auto value = model.selectedListItem.value();
             value += 1;
             value = value < model.items.size() ? value : model.items.size();
             model.selectedListItem = Option<int>::Some(value);
        }
        break;
    }

    if(updated) {
        emit this->modelUpdated(this->model);
    }
}

void BigScreenController::joypadLeft() {
    bool updated = false;

    switch(internal.state) {
    case ApplicationRunning:
    case ListItemSelection:
        break;
    case TabSelection:
        updated = true;
        if(model.selectedTab.isNone()) {
            model.selectedTab = Option<BigScreenTab>::Some(Twitch);
        }
        else {
            int value = static_cast<int>(model.selectedTab.value());
            value = value == static_cast<int>(Twitch) ? static_cast<int>(Twitch) : value-1;
            model.selectedTab = Option<BigScreenTab>::Some(static_cast<BigScreenTab>(value));
        }
        break;
    }

    if(updated) {
        emit this->modelUpdated(this->model);
    }
}

void BigScreenController::joypadRight() {
    bool updated = false;

    switch(internal.state) {
    case ApplicationRunning:
    case ListItemSelection:
        break;
    case TabSelection:
        updated = true;
        if(model.selectedTab.isNone()) {
            model.selectedTab = Option<BigScreenTab>::Some(Twitch);
        }
        else {
            int value = static_cast<int>(model.selectedTab.value());
            value = value == static_cast<int>(BigScreenTab::Applications) ? static_cast<int>(BigScreenTab::Applications) : value+1;
            model.selectedTab = Option<BigScreenTab>::Some(static_cast<BigScreenTab>(value));
        }
        break;
    }

    if(updated) {
        emit this->modelUpdated(this->model);
    }
}

void BigScreenController::btnX() {
    bool updated = false;

    switch(internal.state) {
    case ApplicationRunning:
        break;
    case TabSelection:
        updated = true;
        if(!model.selectedTab.isNone()) {
            this->executeTab(model.selectedTab.value());
        }
        break;
    case ListItemSelection:
        updated = true;
        if(!model.selectedListItem.isNone()) {
            // invoke the right stuff
            switch(model.selectedTab.value()) {
            case Twitch:
                this->startTwitchStream();
                break;
            default:
                model.debugText.append("the selected tab is not handled yet");
                break;
            }
        }
        break;
    }

    if(updated) {
        emit this->modelUpdated(this->model);
    }

}

void BigScreenController::tabClicked(BigScreenTab tab)
{
    if(internal.state == TabSelection) {
        model.selectedTab = Option<BigScreenTab>::Some(tab);
        this->executeTab(tab);
    }
}

void BigScreenController::listItemClicked(size_t row)
{
    model.selectedListItem = Option<int>::Some(row);
    this->btnX();
}

void BigScreenController::executeTab(BigScreenTab tab)
{
    switch(tab) {
    case Youtube:
    case Television:
    case Applications:
        this->updateStatusBarText("not supported");
        break;
    case Twitch:
        model.debugText.append("requested twitch channels");
        this->net.requestTwitchWebsites();
        break;
    }
}

void BigScreenController::startTwitchStream() {
    auto currentIndex = model.selectedListItem.value();
    auto twitch_username = model.items.at(currentIndex);

    QString program = "streamlink";
    QStringList args;
    args << "--player";
    args << "mpv";
    args << "https://twitch.tv/" + twitch_username;
    args << "720p";

    model.statusBarText = "starting: " + program + " " + args.join(" ");

    livestreamer.start(program, args);
}

void BigScreenController::processStarted()
{
    internal.state = ApplicationRunning;
}


void BigScreenController::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    // this->ui->pte_debug->appendPlainText("process finished - (" + QString::number(exitCode) + ", status: " + QString::number(exitStatus) + ")");

    internal.state = ListItemSelection;
    if(exitStatus == QProcess::CrashExit) {
        this->updateStatusBarText("[twitch] the stream is not active");
    }
    else {
        this->updateStatusBarText("[twitch] you stopped watching this stream");
    }
}

void BigScreenController::twitchListRetrieved(Result<QStringList, QString>& channels)
{
    model.debugText.append("twitchListRequested called");
    model.items = channels.value();
    internal.state = ListItemSelection;
    emit modelUpdated(model);
}

