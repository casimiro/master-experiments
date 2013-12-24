#ifndef COMMON_H
#define COMMON_H

#include <QSqlDatabase>
#include <QDateTime>
#include <fstream>
#include <QSqlQuery>

namespace casimiro {

extern QSqlDatabase DB;

inline void DropTables()
{
    QSqlQuery query;
    query.exec("DROP TABLE IF EXISTS relationship");
    query.exec("DROP TABLE IF EXISTS tweet_topics");
}

inline void CleanUpTables()
{
    QSqlQuery query;
    query.exec("DELETE FROM relationship");
    query.exec("DELETE FROM tweet_topics");
}
    
inline void CreateDbStructure()
{
    QSqlQuery query;
    std::ifstream file("tweet_topics.sql");
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    query.exec(content.c_str());
    file.close();
    
    file.open("relationship.sql");
    content = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    query.exec(content.c_str());
}

}

#endif // COMMON_H