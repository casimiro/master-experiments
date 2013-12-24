#include <gmock/gmock.h>
#include <QSqlDatabase>
#include "common.h"

QSqlDatabase casimiro::DB = QSqlDatabase::addDatabase("QSQLITE");

int main(int argc, char** argv) {
    testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
