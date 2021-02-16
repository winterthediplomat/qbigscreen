#ifndef BIGSCREENNETWORK_H
#define BIGSCREENNETWORK_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include "bigscreen_defines.hpp"

class BigScreenNetwork: public QObject
{
   Q_OBJECT

public:
    BigScreenNetwork();
    void requestTwitchWebsites();

signals:
    void twitchListRetrieved(Result<QStringList, QString>& list);

private:
    QNetworkAccessManager qnam;
};

#endif // BIGSCREENNETWORK_H
