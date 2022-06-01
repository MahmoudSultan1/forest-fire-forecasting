#define CATCH_CONFIG_MAIN

#include "./Test_lib/catch.hpp"
#include "./classes/server.cpp"
#include "./classes/client.cpp"

TEST_CASE("Testing average over time")
{
    client x;
    // 10+15+25+35+40/5(sec) =
    x.setReadings(10);
    x.setReadings(15);
    x.setReadings(25);
    x.setReadings(35);
    x.setReadings(40);
    x.AvgOverTime();
    REQUIRE(x.getAvgOverTime() == 25);
}
TEST_CASE("Testing accumlation over time")
{
    client x;
    // 0 = 0 + 15 - 10 = 5
    x.AccOverTime(15, 10);
    REQUIRE(x.getAccOverTime() == 5);

    // 5 = 5 + 10 - 5 = 10
    x.AccOverTime(10, 5);
    REQUIRE(x.getAccOverTime() == 10);

    // negative values

    // 10 = 10 + 10 - 30 = -10
    x.AccOverTime(10, 30);
    REQUIRE(x.getAccOverTime() == -10);
}

TEST_CASE_METHOD(client, "Test client ")
{

    REQUIRE_THROWS(SendAdd());
    REQUIRE_THROWS(DoOperation());
    REQUIRE_NOTHROW(initClient());
}

TEST_CASE_METHOD(UDP_Server, "Test server ")
{
    REQUIRE_THROWS(SendTemp());
    REQUIRE_THROWS(DoOperation());
    REQUIRE_NOTHROW(initServer());
}