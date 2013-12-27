#ifndef UTILS_H
#define UTILS_H

#include <map>
#include <QString>
#include <QStringList>
#include <vector>
#include <fstream>
#include <sstream>

namespace casimiro {

typedef std::map<std::string, float> StringFloatMap;
typedef std::map<std::string, int> StringIntMap;
typedef std::vector<StringIntMap> StringIntMaps;

inline StringFloatMap BuildProfileFromString(const QString &_content)
{
    StringFloatMap profile;
    auto pairs = _content.split(" ");
    for(auto pair : pairs)
    {
        auto values = pair.split(":");
        profile[values.at(0).toStdString()] = atof(values.at(1).toStdString().c_str());
    }
    return profile;
}

inline StringIntMap BuildTopicLifeSpanMapFromFile(const std::string& _fileName)
{
    StringIntMap topics;
    
    std::ifstream file(_fileName);
    std::string line;
    
    while(std::getline(file, line))
    {
        std::istringstream iss(line);
        std::vector<std::string> tokens{std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}};
        topics.insert(std::make_pair(tokens.at(0), atoi(tokens.at(1).c_str())));
    }
    
    return topics;
}

}
#endif // UTILS_H