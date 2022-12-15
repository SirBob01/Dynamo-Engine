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

TEST_CASE("RingBuffer write buffer", "[RingBuffer]") {
    Dynamo::RingBuffer<short, 4> buffer;
    short x[6] = {1, 2, 3, 4, 5, 6};

    REQUIRE(buffer.size() == 0);
    REQUIRE(buffer.remaining() == 4);

    REQUIRE(buffer.write(x, 1) == 1);
    REQUIRE(buffer.size() == 1);
    REQUIRE(buffer.remaining() == 3);

    REQUIRE(buffer.write(x + 1, 2) == 2);
    REQUIRE(buffer.size() == 3);
    REQUIRE(buffer.remaining() == 1);

    REQUIRE(buffer.write(x + 3, 3) == 1);
    REQUIRE(buffer.size() == 4);
    REQUIRE(buffer.remaining() == 0);
    REQUIRE(buffer.is_full());

    CHECK_THROWS(buffer.write(5));
    for (int i = 0; i < 4; i++) {
        REQUIRE(buffer.read() == i + 1);
    }
}

TEST_CASE("RingBuffer read buffer", "[RingBuffer]") {
    Dynamo::RingBuffer<short, 4> buffer;
    short x[4] = {1, 2, 3, 4};
    short y[4] = {0, 0, 0, 0};
    buffer.write(x, 4);

    REQUIRE(buffer.read(y, 1) == 1);
    REQUIRE(y[0] == 1);
    REQUIRE(buffer.size() == 3);
    REQUIRE(buffer.remaining() == 1);

    REQUIRE(buffer.read(y + 1, 2) == 2);
    REQUIRE(y[1] == 2);
    REQUIRE(y[2] == 3);
    REQUIRE(buffer.size() == 1);
    REQUIRE(buffer.remaining() == 3);

    REQUIRE(buffer.read(y + 3, 1) == 1);
    REQUIRE(y[3] == 4);
    REQUIRE(buffer.size() == 0);
    REQUIRE(buffer.remaining() == 4);
    REQUIRE(buffer.is_empty());

    CHECK_THROWS(buffer.read());
    for (int i = 0; i < 4; i++) {
        REQUIRE(y[i] == i + 1);
    }
}

TEST_CASE("RingBuffer overlapping read write", "[RingBuffer]") {
    Dynamo::RingBuffer<short, 4> buffer;
    short x[6] = {1, 2, 3, 4, 5, 6};
    short y[6] = {0, 0, 0, 0, 0, 0};

    REQUIRE(buffer.write(x, 2) == 2);
    REQUIRE(buffer.size() == 2);
    REQUIRE(buffer.remaining() == 2);

    REQUIRE(buffer.read(y, 2) == 2);
    REQUIRE(y[0] == 1);
    REQUIRE(y[1] == 2);
    REQUIRE(buffer.size() == 0);
    REQUIRE(buffer.remaining() == 4);

    REQUIRE(buffer.write(x + 2, 3) == 3);
    REQUIRE(buffer.size() == 3);
    REQUIRE(buffer.remaining() == 1);

    REQUIRE(buffer.read(y + 2, 4) == 3);
    REQUIRE(y[2] == 3);
    REQUIRE(y[3] == 4);
    REQUIRE(buffer.size() == 0);
    REQUIRE(buffer.remaining() == 4);
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
