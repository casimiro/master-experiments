#include <iostream>
#include <QSqlDatabase>
#include <fstream>
#include "twitteruser.h"
#include "evaluation.h"
#include "utils.h"

using namespace casimiro;

QDateTime START_PROFILE = QDateTime::fromString("2013-01-01 00:00:00", "yyyy-MM-dd HH:mm:ss");
QDateTime END_PROFILE = QDateTime::fromString("2013-05-01 00:00:00", "yyyy-MM-dd HH:mm:ss");

QDateTime START_RETWEETS = QDateTime::fromString("2013-05-01 00:00:01", "yyyy-MM-dd HH:mm:ss");
QDateTime END_RETWEETS = QDateTime::fromString("2013-08-01 00:00:00", "yyyy-MM-dd HH:mm:ss");

int CANDIDATE_PERIOD_IN_HOURS = 48;

int main(int /*argc*/, char** /* argv*/) {
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("192.168.25.33");
    db.setDatabaseName("tweets");
    db.setUserName("tweets");
    db.setPassword("zxc123");

    if(!db.open())
    {
        std::cerr << "Could not open database" << std::endl;
        return -1;
    }
    
    StringIntMaps topicLifeSpanMaps;
    topicLifeSpanMaps.push_back(BuildTopicLifeSpanMapFromFile("data/timings_60"));
    topicLifeSpanMaps.push_back(BuildTopicLifeSpanMapFromFile("data/timings_70"));
    topicLifeSpanMaps.push_back(BuildTopicLifeSpanMapFromFile("data/timings_80"));
    topicLifeSpanMaps.push_back(BuildTopicLifeSpanMapFromFile("data/timings_90"));
    
    Evaluation evaluation;
    evaluation.evaluateSystem("data/users.csv", START_PROFILE, END_PROFILE, START_RETWEETS, END_RETWEETS, CANDIDATE_PERIOD_IN_HOURS, "data/results.csv", topicLifeSpanMaps);
    
    return 0;
}
