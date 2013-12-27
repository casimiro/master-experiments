#include <gmock/gmock.h>
#include <QString>
#include "../utils.h"

using namespace casimiro;
using namespace ::testing;

TEST(UtilsTest, BuildProfileFromString)
{
    QString profileString = "0:0.3 3:8.34 4:0.345";
    auto profile = BuildProfileFromString(profileString);
    
    ASSERT_EQ(profile.size(), 3);
    
    ASSERT_NEAR(0.3, profile["0"], 0.01);
    ASSERT_NEAR(8.34, profile["3"], 0.01);
    ASSERT_NEAR(0.345, profile["4"], 0.01);
}

TEST(UtilsTest, BuildTopicLifeSpanMapFromFile)
{
    auto topicLifeSpanMap = BuildTopicLifeSpanMapFromFile("topic_timings");
    
    ASSERT_EQ(114501, topicLifeSpanMap["0"]);
    ASSERT_EQ(137716, topicLifeSpanMap["1"]);
    ASSERT_EQ(77130, topicLifeSpanMap["2"]);
}