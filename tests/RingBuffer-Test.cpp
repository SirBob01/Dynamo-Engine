#include <Dynamo.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("RingBuffer write", "[RingBuffer]") {
    Dynamo::RingBuffer<short, 4> buffer;
    REQUIRE(buffer.size() == 0);
    REQUIRE(buffer.remaining() == 4);

    buffer.write(1);
    REQUIRE(buffer.size() == 1);
    REQUIRE(buffer.remaining() == 3);

    buffer.write(2);
    REQUIRE(buffer.size() == 2);
    REQUIRE(buffer.remaining() == 2);

    buffer.write(3);
    REQUIRE(buffer.size() == 3);
    REQUIRE(buffer.remaining() == 1);

    buffer.write(4);
    REQUIRE(buffer.size() == 4);
    REQUIRE(buffer.remaining() == 0);
    REQUIRE(buffer.is_full());

    CHECK_THROWS(buffer.write(5));
}

TEST_CASE("RingBuffer read", "[RingBuffer]") {
    Dynamo::RingBuffer<short, 4> buffer;
    buffer.write(1);
    buffer.write(2);
    buffer.write(3);
    buffer.write(4);

    REQUIRE(buffer.read() == 1);
    REQUIRE(buffer.size() == 3);
    REQUIRE(buffer.remaining() == 1);

    REQUIRE(buffer.read() == 2);
    REQUIRE(buffer.size() == 2);
    REQUIRE(buffer.remaining() == 2);

    REQUIRE(buffer.read() == 3);
    REQUIRE(buffer.size() == 1);
    REQUIRE(buffer.remaining() == 3);

    REQUIRE(buffer.read() == 4);
    REQUIRE(buffer.size() == 0);
    REQUIRE(buffer.remaining() == 4);
    REQUIRE(buffer.is_empty());

    CHECK_THROWS(buffer.read());
}

TEST_CASE("RingBuffer pop", "[RingBuffer]") {
    Dynamo::RingBuffer<short, 4> buffer;
    buffer.write(1);
    buffer.write(2);
    buffer.write(3);
    buffer.write(4);

    buffer.pop();
    REQUIRE(buffer.size() == 3);
    REQUIRE(buffer.remaining() == 1);

    buffer.pop();
    REQUIRE(buffer.size() == 2);
    REQUIRE(buffer.remaining() == 2);

    buffer.pop();
    REQUIRE(buffer.size() == 1);
    REQUIRE(buffer.remaining() == 3);

    buffer.pop();
    REQUIRE(buffer.size() == 0);
    REQUIRE(buffer.remaining() == 4);
    REQUIRE(buffer.is_empty());

    CHECK_THROWS(buffer.pop());
}

TEST_CASE("RingBuffer clear", "[RingBuffer]") {
    Dynamo::RingBuffer<short, 2> buffer;
    buffer.write(1);
    buffer.write(2);

    buffer.clear();
    REQUIRE(buffer.size() == 0);
    REQUIRE(buffer.is_empty());

    buffer.write(5);
    buffer.write(6);

    REQUIRE(buffer.read() == 5);

    buffer.clear();
    REQUIRE(buffer.size() == 0);
    REQUIRE(buffer.is_empty());
}
