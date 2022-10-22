#include <Dynamo.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("RingBuffer write and read", "[RingBuffer]") {
    Dynamo::RingBuffer<short, 3> buffer;
    REQUIRE(buffer.size() == 0);
    buffer.write(1);
    REQUIRE(buffer.size() == 1);
    REQUIRE(buffer.remaining() == 1);
    buffer.write(2);
    REQUIRE(buffer.size() == 2);
    REQUIRE(buffer.remaining() == 0);
    buffer.write(3);
    REQUIRE(buffer.size() == 2);

    REQUIRE(buffer.read() == 1);
    REQUIRE(buffer.size() == 1);
    REQUIRE(buffer.read() == 2);
    REQUIRE(buffer.size() == 0);

    CHECK_THROWS(buffer.read());
}

TEST_CASE("RingBuffer clear", "[RingBuffer]") {
    Dynamo::RingBuffer<short, 3> buffer;
    buffer.write(1);
    buffer.write(2);
    buffer.write(3);
    buffer.clear();
    REQUIRE(buffer.size() == 0);
}
