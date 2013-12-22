#ifndef UTILS_H
#define UTILS_H

#include <map>
#include <QString>
#include <QStringList>

namespace casimiro {

typedef std::map<std::string, float> StringFloatMap;

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
    
}

#endif // UTILS_H