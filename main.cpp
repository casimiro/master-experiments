#include <iostream>
#include <QSqlDatabase>
#include <fstream>
#include "twitteruser.h"
#include "evaluation.h"
#include "utils.h"

using namespace casimiro;

QDateTime START_PROFILE = QDateTime::fromString("2013-01-01 00:00:00", "yyyy-MM-dd HH:mm:ss");
QDateTime END_PROFILE = QDateTime::fromString("2013-07-01 00:00:00", "yyyy-MM-dd HH:mm:ss");

QDateTime START_RETWEETS = QDateTime::fromString("2013-07-01 00:00:01", "yyyy-MM-dd HH:mm:ss");
QDateTime END_RETWEETS = QDateTime::fromString("2013-08-01 00:00:00", "yyyy-MM-dd HH:mm:ss");

int CANDIDATE_PERIOD_IN_HOURS = 48;

void mainTopics()
{
    std::cout << "Evaluating regular topics system" << std::endl;
    StringIntMaps topicLifeSpanMaps;
    topicLifeSpanMaps.push_back(BuildTopicLifeSpanMapFromFile("data/timings_10"));
    topicLifeSpanMaps.push_back(BuildTopicLifeSpanMapFromFile("data/timings_20"));
    topicLifeSpanMaps.push_back(BuildTopicLifeSpanMapFromFile("data/timings_30"));
    topicLifeSpanMaps.push_back(BuildTopicLifeSpanMapFromFile("data/timings_40"));
    topicLifeSpanMaps.push_back(BuildTopicLifeSpanMapFromFile("data/timings_50"));
    topicLifeSpanMaps.push_back(BuildTopicLifeSpanMapFromFile("data/timings_60"));
    topicLifeSpanMaps.push_back(BuildTopicLifeSpanMapFromFile("data/timings_70"));
    topicLifeSpanMaps.push_back(BuildTopicLifeSpanMapFromFile("data/timings_80"));
    topicLifeSpanMaps.push_back(BuildTopicLifeSpanMapFromFile("data/timings_90"));
    
    bool ignoreRetweetsWithNoTimedTopic = false;
    bool usePersonalisedLifeSpanMaps = true;
    Evaluation evaluation;
    evaluation.evaluateSystem(
        "data/users.csv", 
        TopicProfile, 
        START_PROFILE, 
        END_PROFILE, 
        START_RETWEETS, 
        END_RETWEETS, 
        CANDIDATE_PERIOD_IN_HOURS, 
        "data/results20140623.csv", 
        topicLifeSpanMaps,
        ignoreRetweetsWithNoTimedTopic,
        usePersonalisedLifeSpanMaps
    );
}

void mainBOW()
{
    std::cout << "Evaluating BOW system" << std::endl;
    StringIntMaps topicLifeSpanMaps;
    topicLifeSpanMaps.push_back(BuildTopicLifeSpanMapFromFile("data/bow_timings_10"));
    topicLifeSpanMaps.push_back(BuildTopicLifeSpanMapFromFile("data/bow_timings_20"));
    topicLifeSpanMaps.push_back(BuildTopicLifeSpanMapFromFile("data/bow_timings_30"));
    topicLifeSpanMaps.push_back(BuildTopicLifeSpanMapFromFile("data/bow_timings_40"));
    topicLifeSpanMaps.push_back(BuildTopicLifeSpanMapFromFile("data/bow_timings_50"));
    topicLifeSpanMaps.push_back(BuildTopicLifeSpanMapFromFile("data/bow_timings_60"));
    topicLifeSpanMaps.push_back(BuildTopicLifeSpanMapFromFile("data/bow_timings_70"));
    topicLifeSpanMaps.push_back(BuildTopicLifeSpanMapFromFile("data/bow_timings_80"));
    topicLifeSpanMaps.push_back(BuildTopicLifeSpanMapFromFile("data/bow_timings_90"));
    
    bool ignoreRetweetsWithNoTimedTopic = true;
    bool usePersonalisedLifeSpanMaps = true;
    Evaluation evaluation;
    evaluation.evaluateSystem(
        "data/users.csv", 
        BOWProfile, 
        START_PROFILE, 
        END_PROFILE, 
        START_RETWEETS, 
        END_RETWEETS, 
        CANDIDATE_PERIOD_IN_HOURS, 
        "data/bow_results.csv", 
        topicLifeSpanMaps, 
        ignoreRetweetsWithNoTimedTopic,
        usePersonalisedLifeSpanMaps
    );
}

void mainSVM()
{
    std::cout << "Evaluating SVM system" << std::endl;
    StringIntMaps topicLifeSpanMaps;
    topicLifeSpanMaps.push_back(BuildTopicLifeSpanMapFromFile("data/timings_10"));
    topicLifeSpanMaps.push_back(BuildTopicLifeSpanMapFromFile("data/timings_20"));
    topicLifeSpanMaps.push_back(BuildTopicLifeSpanMapFromFile("data/timings_30"));
    topicLifeSpanMaps.push_back(BuildTopicLifeSpanMapFromFile("data/timings_40"));
    topicLifeSpanMaps.push_back(BuildTopicLifeSpanMapFromFile("data/timings_50"));
    topicLifeSpanMaps.push_back(BuildTopicLifeSpanMapFromFile("data/timings_60"));
    topicLifeSpanMaps.push_back(BuildTopicLifeSpanMapFromFile("data/timings_70"));
    topicLifeSpanMaps.push_back(BuildTopicLifeSpanMapFromFile("data/timings_80"));
    topicLifeSpanMaps.push_back(BuildTopicLifeSpanMapFromFile("data/timings_90"));
    
    bool ignoreRetweetsWithNoTimedTopic = true;
    bool usePersonalisedLifeSpanMaps = false;
    Evaluation evaluation;
    evaluation.evaluateSystem(
        "data/users.csv", 
        SVMProfile, 
        START_PROFILE, 
        END_PROFILE, 
        START_RETWEETS, 
        END_RETWEETS, 
        CANDIDATE_PERIOD_IN_HOURS, 
        "data/svm_results.csv", 
        topicLifeSpanMaps,
        ignoreRetweetsWithNoTimedTopic,
        usePersonalisedLifeSpanMaps
    );
}



int main(int /*argc*/, char** /* argv*/) 
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("192.168.1.150");
    db.setDatabaseName("tweets");
    db.setUserName("tweets");
    db.setPassword("zxc123");

    if(!db.open())
    {
        std::cerr << "Could not open database" << std::endl;
        return -1;
    }
    
    //mainBOW();
    //mainTopics();
    mainSVM();

    
    return 0;
}
