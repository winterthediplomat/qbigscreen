#include "bigscreencontroller.h"
#include <QtGamepad/QGamepad>
// for path checking
#include <QFile>
#include <QDir>

BigScreenController::BigScreenController(QObject *parent) : QObject(parent)
{
    internal.state = InternalState::TabSelection;
}

void BigScreenController::initialize() {
    model.showFullScreen = true;
    model.freezeInterface = false;

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
    connect(pad, SIGNAL(buttonBChanged(bool)),
            this, SLOT(buttonBChanged(bool)));

    connect(&net, &BigScreenNetwork::twitchListRetrieved,
            this, &BigScreenController::twitchListRetrieved);
    connect(&net, &BigScreenNetwork::applicationsRetrieved,
            this, &BigScreenController::applicationsRetrieved);
    connect(&net, &BigScreenNetwork::tvChannelsRetrieved,
            this, &BigScreenController::tvChannelsRetrieved);

    connect(&livestreamer, &QProcess::started,
            this, &BigScreenController::processStarted);
    // TODO(wha): how should I use the &QProcess::finished syntax in case of overloads?
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


void BigScreenController::buttonBChanged(bool pressed)
{
    if(pressed) {
        this->btnB();
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
            model.selectedListItem = Option<int>::None();
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
            case Television:
                this->startTvChannelStream();
                break;
            case Applications:
                this->executeSelectedApplication();
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

void BigScreenController::btnB()
{
    bool updated = false;

    switch(internal.state) {
    case ApplicationRunning:
    case TabSelection:
        break;
    case ListItemSelection:
        internal.state = TabSelection;
        model.selectedListItem = Option<int>::None();
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
        this->updateStatusBarText("not supported");
        break;
    case Television:
        model.debugText.append("\nrequested tv channels");
        this->net.requestTvChannels();
        break;
    case Applications:
        model.debugText.append("\nrequested applications");
        this->net.requestApplications();
        break;
    case Twitch:
        model.debugText.append("\nrequested twitch channels");
        this->net.requestTwitchWebsites();
        break;
    }
}

void BigScreenController::startTwitchStream() {
    auto currentIndex = model.selectedListItem.value();
    auto twitch_username = model.items.at(currentIndex);

    QString program = this->launcherScriptPath("launch-via-streamlink.py");
    QStringList args;
    args << "https://twitch.tv/" + twitch_username;
    args << "720p";

    model.statusBarText = "starting: " + program + " " + args.join(" ");
    model.debugText.append("\nstarting: " + program + " " + args.join(" "));
    model.showFullScreen = false;
    model.freezeInterface = true;

    livestreamer.start(program, args);
}

void BigScreenController::startTvChannelStream() {
    auto currentIndex = model.selectedListItem.value();
    auto tv_channel_name = model.items.at(currentIndex);

    QString program = this->launcherScriptPath("launch-via-streamlink.py");
    QStringList args;
    args << "https://raiplay.it/dirette/" + tv_channel_name;
    args << "576p";

    model.statusBarText = "starting: " + program + " " + args.join(" ");
    model.debugText.append("\nstarting: " + program + " " + args.join(" "));
    model.showFullScreen = false;
    model.freezeInterface = true;

    livestreamer.start(program, args);
}


void BigScreenController::executeSelectedApplication()
{
    auto currentIndex = model.selectedListItem.value();
    auto application = internal.applications.at(currentIndex);

    auto app_launch_script = application.launcher_script;
    QString program = this->launcherScriptPath(app_launch_script);
    QStringList args;

    model.statusBarText = "starting: " + program + " " + args.join(" ");
    model.debugText.append("\nstarting: " + program + " " + args.join(" "));
    model.showFullScreen = false;
    model.freezeInterface = true;

    livestreamer.start(program, args);
}

QString BigScreenController::launcherScriptPath(QString scriptName)
{
    QString GLOBAL_DEPLOY_PATH("/opt/qbigscreen/");
    QString DEV_DEPLOY_PATH(QDir::homePath() + QString("/.config/qbigscreen/scripts/"));


    QString global_version_path = GLOBAL_DEPLOY_PATH + scriptName;
    QFile global_version(global_version_path);
    QString local_version_path = DEV_DEPLOY_PATH + scriptName;
    QFile local_version(local_version_path);

    QString program;
    if(local_version.exists()) {
        program = local_version_path;
    }
    else if(global_version.exists()) {
        program = global_version_path;
    }
    else {
        assert(false && "how tf did you install this app?");
    }

    return program;
}

void BigScreenController::processStarted()
{
    internal.state = ApplicationRunning;
}


void BigScreenController::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    // this->ui->pte_debug->appendPlainText("process finished - (" + QString::number(exitCode) + ", status: " + QString::number(exitStatus) + ")");

    internal.state = ListItemSelection;
    model.showFullScreen = true;
    model.freezeInterface = false;
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

void BigScreenController::applicationsRetrieved(Result<QVector<ApplicationItem>, QString> &list)
{
    model.debugText.append("\napplicationsRetrievedCalled");
    internal.state = ListItemSelection;
    internal.applications = list.value();

    model.items.clear();
    for(ApplicationItem ai: internal.applications) {
        model.items.append(ai.name);
    }

    emit modelUpdated(model);
}

void BigScreenController::tvChannelsRetrieved(Result<QStringList, QString> &channels)
{
    model.debugText.append("\ntvChannelRetrieved called");
    model.items = channels.value();
    internal.state = ListItemSelection;
    emit modelUpdated(model);

}

