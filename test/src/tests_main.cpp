#include <gtest/gtest.h>

#include <QList>
#include "Application.h"
#include <QMetaType>
#include <QCoreApplication>
Q_DECLARE_METATYPE(Application);

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    qRegisterMetaType<Application>("Application");
    qRegisterMetaType<QList<Application>>("ApplicationList");

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}