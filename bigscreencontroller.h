#ifndef BIGSCREENCONTROLLER_H
#define BIGSCREENCONTROLLER_H

#include <QObject>
#include "bigscreenmodel.h"
#include "bigscreennetwork.h"
#include <QtGamepad/QGamepad>
#include <QProcess>

class BigScreenController : public QObject
{
    Q_OBJECT
public:
    explicit BigScreenController(QObject *parent = nullptr);
    void initialize();

public slots:
    void joypadLeft();
    void joypadRight();
    void joypadUp();
    void joypadDown();

    void btnX();

    void tabClicked(BigScreenTab tab);
    void listItemClicked(size_t row);

signals:
    void modelUpdated(BigScreenModel& model);

private:
    enum InternalState {
        TabSelection,
        ListItemSelection,
        ApplicationRunning
    };

    struct InternalModel
    {
        InternalState state;
    };

    BigScreenModel model;
    InternalModel internal;
    QGamepad* pad;
    BigScreenNetwork net;

    QProcess livestreamer;

    // graphical support
    void updateStatusBarText(QString text);

    // mouse support
    void executeTab(BigScreenTab tab);

    // process
    void startTwitchStream();

private slots:
    void joypadUpDown(double value);
    void joypadLeftRight(double value);
    void buttonXChanged(bool pressed);

    void processStarted();
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);

    void twitchListRetrieved(Result<QStringList, QString>& channels);

};

#endif // BIGSCREENCONTROLLER_H
