#ifndef BIGSCREENNETWORK_H
#define BIGSCREENNETWORK_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include "bigscreen_defines.hpp"

struct ApplicationItem
{
    QString name;
    QString launcher_script;
    Option<QString> qjoypad_profile;
};

class BigScreenNetwork: public QObject
{
   Q_OBJECT

public:
    BigScreenNetwork();
    void requestTwitchWebsites();
    void requestApplications();
    void requestTvChannels();

signals:
    void twitchListRetrieved(Result<QStringList, QString>& list);
    void applicationsRetrieved(Result<QVector<ApplicationItem>, QString>& list);
    void tvChannelsRetrieved(Result<QStringList, QString>& list);

private:
    QNetworkAccessManager qnam;
};

#endif // BIGSCREENNETWORK_H
