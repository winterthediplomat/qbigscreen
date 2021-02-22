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
    sl.append("liabellula");
    sl.append("motokosu");

    auto res = Result<QStringList, QString>::Ok(sl);

    emit twitchListRetrieved(res);
}

void BigScreenNetwork::requestApplications()
{
   QVector<ApplicationItem> l;
   l.append(ApplicationItem{ "Deezer", "launch-deezer.py", Option<QString>::None() });
   l.append(ApplicationItem{ "Steam", "launch-steam.py", Option<QString>::None() });

   auto res = Result<QVector<ApplicationItem>, QString>::Ok(l);

   emit applicationsRetrieved(res);
}

void BigScreenNetwork::requestTvChannels()
{
    QStringList sl;
    sl.append("rai1");
    sl.append("rai2");
    sl.append("rai3");
    sl.append("rai4");
    sl.append("rai5");
    sl.append("rainews24");

    auto res = Result<QStringList, QString>::Ok(sl);

    emit tvChannelsRetrieved(res);
}
