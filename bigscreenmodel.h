#ifndef BIGSCREENMODEL_H
#define BIGSCREENMODEL_H

#include <QString>
#include <QStringList>
#include "bigscreen_defines.hpp"

#include <cstdint>

struct BigScreenModel
{
public:
    QString debugText;
    Option<BigScreenTab> selectedTab;
    Option<int> selectedListItem;
    QStringList items;
    QString statusBarText;
    bool showFullScreen;
};

#endif // BIGSCREENMODEL_H
