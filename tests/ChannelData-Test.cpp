#include <Dynamo.hpp>
#include <catch2/catch_test_macros.hpp>

using IntegerBuffer = Dynamo::ChannelData<int>;

TEST_CASE("ChannelData construction", "[ChannelData]") {
    IntegerBuffer buffer;
    REQUIRE(buffer.frames() == 0);
    REQUIRE(buffer.channels() == 0);

    buffer = IntegerBuffer(1);
    REQUIRE(buffer.frames() == 0);
    REQUIRE(buffer.channels() == 1);

    buffer = IntegerBuffer(1, 3);
    REQUIRE(buffer.frames() == 3);
    REQUIRE(buffer.channels() == 1);

    buffer = IntegerBuffer({3, 2, 1, 0, -1, 4}, 2);
    REQUIRE(buffer.frames() == 3);
    REQUIRE(buffer.channels() == 2);

    IntegerBuffer copy = buffer;
    for (int c = 0; c < buffer.channels(); c++) {
        for (int f = 0; f < buffer.frames(); f++) {
            REQUIRE(buffer.at(f, c) == copy.at(f, c));
        }
    }

    CHECK_THROWS(IntegerBuffer({1, 2, 3}, 2));
}

TEST_CASE("ChannelData clear", "[ChannelData]") {
    IntegerBuffer buffer({3, 2, 1, 0, -1, 4}, 2);
    buffer.clear();

    auto &data = buffer.data();
    for (int v : data) {
        REQUIRE(v == 0);
    }
}

TEST_CASE("ChannelData set channels", "[ChannelData]") {
    IntegerBuffer buffer({3, 2, 1, 0, -1, 4}, 2);
    auto &data = buffer.data();

    buffer.set_channels(4);
    REQUIRE(buffer.channels() == 4);
    REQUIRE(data.size() == 12);
}

TEST_CASE("ChannelData resize", "[ChannelData]") {
    IntegerBuffer buffer({3, 2, 1, 0, -1, 4}, 2);
    auto &data = buffer.data();

    buffer.resize(5);
    REQUIRE(buffer.frames() == 5);
    REQUIRE(data.size() == 10);
}

TEST_CASE("ChannelData reading", "[ChannelData]") {
    IntegerBuffer buffer({3, 2, 1, 0, -1, 4}, 2);
    REQUIRE(buffer.at(2, 0) == 1);
    REQUIRE(buffer.at_interleaved(2, 0) == -1);

    REQUIRE(buffer.at(2, 1) == 4);
    REQUIRE(buffer.at_interleaved(2, 1) == 4);

    REQUIRE_THROWS(buffer.at(2, 2));
    REQUIRE_THROWS(buffer.at_interleaved(2, 2));

    REQUIRE_THROWS(buffer.at(2, 2));
    REQUIRE_THROWS(buffer.at_interleaved(2, 2));
}