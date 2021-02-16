#include "bigscreennetwork.h"

BigScreenNetwork::BigScreenNetwork()
{

}

void BigScreenNetwork::requestTwitchWebsites()
{
    QStringList sl;
    sl.append("monstercat");
    sl.append("lrnzlrnzlrnz");
    sl.append("sabaku_no_sutoriimaa");
    sl.append("j_blow");
    sl.append("gittre");
    sl.append("fredrin");

    auto res = Result<QStringList, QString>::Ok(sl);

    emit twitchListRetrieved(res);
}
