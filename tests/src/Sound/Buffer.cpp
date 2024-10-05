#include <Dynamo.hpp>
#include <catch2/catch_test_macros.hpp>

#include "../Common.hpp"

struct Samples {
    Dynamo::Sound::Buffer mono;
    Dynamo::Sound::Buffer stereo;
    Dynamo::Sound::Buffer quad;
    Dynamo::Sound::Buffer s_51;

    Samples() {
        mono = Dynamo::Sound::Buffer(3, 1);
        for (unsigned i = 0; i < 3 * 1; i++) {
            mono.data()[i] = i;
        }

        stereo = Dynamo::Sound::Buffer(3, 2);
        for (unsigned i = 0; i < 3 * 2; i++) {
            stereo.data()[i] = i;
        }

        quad = Dynamo::Sound::Buffer(3, 4);
        for (unsigned i = 0; i < 3 * 4; i++) {
            quad.data()[i] = i;
        }

        s_51 = Dynamo::Sound::Buffer(3, 6);
        for (unsigned i = 0; i < 3 * 6; i++) {
            s_51.data()[i] = i;
        }
    }
};

Samples dataset;

TEST_CASE("Buffer construction", "[Buffer]") {
    Dynamo::Sound::Buffer buffer;
    REQUIRE(buffer.frames() == 0);
    REQUIRE(buffer.channels() == 0);

    buffer = Dynamo::Sound::Buffer(3, 1);
    REQUIRE(buffer.frames() == 3);
    REQUIRE(buffer.channels() == 1);

    std::array<float, 6> arr = {3, 2, 1, 0, -1, 4};
    buffer = Dynamo::Sound::Buffer(arr.data(), 3, 2);
    REQUIRE(buffer.frames() == 3);
    REQUIRE(buffer.channels() == 2);

    buffer = Dynamo::Sound::Buffer(arr.data(), 2, 3);
    REQUIRE(buffer.frames() == 2);
    REQUIRE(buffer.channels() == 3);

    // Verify samples
    REQUIRE(buffer[0][0] == 3);
    REQUIRE(buffer[0][1] == 2);

    REQUIRE(buffer[1][0] == 1);
    REQUIRE(buffer[1][1] == 0);

    REQUIRE(buffer[2][0] == -1);
    REQUIRE(buffer[2][1] == 4);

    Dynamo::Sound::Buffer copy = buffer;
    for (unsigned c = 0; c < buffer.channels(); c++) {
        for (unsigned f = 0; f < buffer.frames(); f++) {
            REQUIRE(buffer[c][f] == copy[c][f]);
        }
    }
}

TEST_CASE("Buffer silence", "[Buffer]") {
    std::array<float, 6> arr = {3, 2, 1, 0, -1, 4};
    Dynamo::Sound::Buffer buffer(arr.data(), 3, 2);
    buffer.silence();

    for (unsigned c = 0; c < buffer.channels(); c++) {
        for (unsigned f = 0; f < buffer.frames(); f++) {
            REQUIRE(buffer[c][f] == 0);
        }
    }
}

TEST_CASE("Buffer resize", "[Buffer]") {
    std::array<float, 6> arr = {3, 2, 1, 0, -1, 4};
    Dynamo::Sound::Buffer buffer(arr.data(), 3, 2);

    // Size up
    buffer.resize(5, 3);
    REQUIRE(buffer.frames() == 5);
    REQUIRE(buffer.channels() == 3);

    // Size down
    buffer.resize(4, 2);
    REQUIRE(buffer.frames() == 4);
    REQUIRE(buffer.channels() == 2);
}

TEST_CASE("Buffer channel index", "[Buffer]") {
    std::array<float, 6> arr = {3, 2, 1, 0, -1, 4};
    Dynamo::Sound::Buffer buffer(arr.data(), 3, 2);

    REQUIRE(buffer[0][0] == 3);
    REQUIRE(buffer[0][1] == 2);
    REQUIRE(buffer[0][2] == 1);

    REQUIRE(buffer[1][0] == 0);
    REQUIRE(buffer[1][1] == -1);
    REQUIRE(buffer[1][2] == 4);
}

TEST_CASE("Buffer remix 1 - 2", "[Buffer]") {
    Dynamo::Sound::Buffer src = dataset.mono;
    Dynamo::Sound::Buffer dst(src.frames(), 2);

    dst.silence();
    src.remix(dst);

    REQUIRE(dst[0][0] == src[0][0]);
    REQUIRE(dst[0][1] == src[0][1]);
    REQUIRE(dst[0][2] == src[0][2]);

    REQUIRE(dst[1][0] == src[0][0]);
    REQUIRE(dst[1][1] == src[0][1]);
    REQUIRE(dst[1][2] == src[0][2]);
}

TEST_CASE("Buffer remix 1 - 4", "[Buffer]") {
    Dynamo::Sound::Buffer src = dataset.mono;
    Dynamo::Sound::Buffer dst(src.frames(), 4);

    dst.silence();
    src.remix(dst);

    REQUIRE(dst[0][0] == src[0][0]);
    REQUIRE(dst[0][1] == src[0][1]);
    REQUIRE(dst[0][2] == src[0][2]);

    REQUIRE(dst[1][0] == src[0][0]);
    REQUIRE(dst[1][1] == src[0][1]);
    REQUIRE(dst[1][2] == src[0][2]);

    REQUIRE(dst[2][0] == 0);
    REQUIRE(dst[2][1] == 0);
    REQUIRE(dst[2][2] == 0);

    REQUIRE(dst[3][0] == 0);
    REQUIRE(dst[3][1] == 0);
    REQUIRE(dst[3][2] == 0);
}

TEST_CASE("Buffer remix 1 - 6", "[Buffer]") {
    Dynamo::Sound::Buffer src = dataset.mono;
    Dynamo::Sound::Buffer dst(src.frames(), 6);

    dst.silence();
    src.remix(dst);

    REQUIRE(dst[0][0] == 0);
    REQUIRE(dst[0][1] == 0);
    REQUIRE(dst[0][2] == 0);

    REQUIRE(dst[1][0] == 0);
    REQUIRE(dst[1][1] == 0);
    REQUIRE(dst[1][2] == 0);

    REQUIRE(dst[2][0] == src[0][0]);
    REQUIRE(dst[2][1] == src[0][1]);
    REQUIRE(dst[2][2] == src[0][2]);

    REQUIRE(dst[3][0] == 0);
    REQUIRE(dst[3][1] == 0);
    REQUIRE(dst[3][2] == 0);

    REQUIRE(dst[4][0] == 0);
    REQUIRE(dst[4][1] == 0);
    REQUIRE(dst[4][2] == 0);

    REQUIRE(dst[5][0] == 0);
    REQUIRE(dst[5][1] == 0);
    REQUIRE(dst[5][2] == 0);
}

TEST_CASE("Buffer remix 2 - 1", "[Buffer]") {
    Dynamo::Sound::Buffer src = dataset.stereo;
    Dynamo::Sound::Buffer dst(src.frames(), 1);

    dst.silence();
    src.remix(dst);

    REQUIRE(dst[0][0] == 0.5 * (src[0][0] + src[1][0]));
    REQUIRE(dst[0][1] == 0.5 * (src[0][1] + src[1][1]));
    REQUIRE(dst[0][2] == 0.5 * (src[0][2] + src[1][2]));
}

TEST_CASE("Buffer remix 2 - 4", "[Buffer]") {
    Dynamo::Sound::Buffer src = dataset.stereo;
    Dynamo::Sound::Buffer dst(src.frames(), 4);

    dst.silence();
    src.remix(dst);

    REQUIRE(dst[0][0] == src[0][0]);
    REQUIRE(dst[0][1] == src[0][1]);
    REQUIRE(dst[0][2] == src[0][2]);

    REQUIRE(dst[1][0] == src[1][0]);
    REQUIRE(dst[1][1] == src[1][1]);
    REQUIRE(dst[1][2] == src[1][2]);

    REQUIRE(dst[2][0] == 0);
    REQUIRE(dst[2][1] == 0);
    REQUIRE(dst[2][2] == 0);

    REQUIRE(dst[3][0] == 0);
    REQUIRE(dst[3][1] == 0);
    REQUIRE(dst[3][2] == 0);
}

TEST_CASE("Buffer remix 2 - 6", "[Buffer]") {
    Dynamo::Sound::Buffer src = dataset.stereo;
    Dynamo::Sound::Buffer dst(src.frames(), 6);

    dst.silence();
    src.remix(dst);

    REQUIRE(dst[0][0] == src[0][0]);
    REQUIRE(dst[0][1] == src[0][1]);
    REQUIRE(dst[0][2] == src[0][2]);

    REQUIRE(dst[1][0] == src[1][0]);
    REQUIRE(dst[1][1] == src[1][1]);
    REQUIRE(dst[1][2] == src[1][2]);

    REQUIRE(dst[2][0] == 0);
    REQUIRE(dst[2][1] == 0);
    REQUIRE(dst[2][2] == 0);

    REQUIRE(dst[3][0] == 0);
    REQUIRE(dst[3][1] == 0);
    REQUIRE(dst[3][2] == 0);

    REQUIRE(dst[4][0] == 0);
    REQUIRE(dst[4][1] == 0);
    REQUIRE(dst[4][2] == 0);

    REQUIRE(dst[5][0] == 0);
    REQUIRE(dst[5][1] == 0);
    REQUIRE(dst[5][2] == 0);
}

TEST_CASE("Buffer remix 4 - 1", "[Buffer]") {
    Dynamo::Sound::Buffer src = dataset.quad;
    Dynamo::Sound::Buffer dst(src.frames(), 1);

    dst.silence();
    src.remix(dst);

    REQUIRE(dst[0][0] ==
            0.25 * (src[0][0] + src[1][0] + src[2][0] + src[3][0]));
    REQUIRE(dst[0][1] ==
            0.25 * (src[0][1] + src[1][1] + src[2][1] + src[3][1]));
    REQUIRE(dst[0][2] ==
            0.25 * (src[0][2] + src[1][2] + src[2][2] + src[3][2]));
}

TEST_CASE("Buffer remix 4 - 2", "[Buffer]") {
    Dynamo::Sound::Buffer src = dataset.quad;
    Dynamo::Sound::Buffer dst(src.frames(), 2);

    dst.silence();
    src.remix(dst);

    REQUIRE(dst[0][0] == 0.5 * (src[0][0] + src[2][0]));
    REQUIRE(dst[0][1] == 0.5 * (src[0][1] + src[2][1]));
    REQUIRE(dst[0][2] == 0.5 * (src[0][2] + src[2][2]));

    REQUIRE(dst[1][0] == 0.5 * (src[1][0] + src[3][0]));
    REQUIRE(dst[1][1] == 0.5 * (src[1][1] + src[3][1]));
    REQUIRE(dst[1][2] == 0.5 * (src[1][2] + src[3][2]));
}

TEST_CASE("Buffer remix 4 - 6", "[Buffer]") {
    Dynamo::Sound::Buffer src = dataset.quad;
    Dynamo::Sound::Buffer dst(src.frames(), 6);

    dst.silence();
    src.remix(dst);

    REQUIRE(dst[0][0] == src[0][0]);
    REQUIRE(dst[0][1] == src[0][1]);
    REQUIRE(dst[0][2] == src[0][2]);

    REQUIRE(dst[1][0] == src[1][0]);
    REQUIRE(dst[1][1] == src[1][1]);
    REQUIRE(dst[1][2] == src[1][2]);

    REQUIRE(dst[2][0] == 0);
    REQUIRE(dst[2][1] == 0);
    REQUIRE(dst[2][2] == 0);

    REQUIRE(dst[3][0] == 0);
    REQUIRE(dst[3][1] == 0);
    REQUIRE(dst[3][2] == 0);

    REQUIRE(dst[4][0] == src[2][0]);
    REQUIRE(dst[4][1] == src[2][1]);
    REQUIRE(dst[4][2] == src[2][2]);

    REQUIRE(dst[5][0] == src[3][0]);
    REQUIRE(dst[5][1] == src[3][1]);
    REQUIRE(dst[5][2] == src[3][2]);
}

TEST_CASE("Buffer remix 6 - 1", "[Buffer]") {
    Dynamo::Sound::Buffer src = dataset.s_51;
    Dynamo::Sound::Buffer dst(src.frames(), 1);

    dst.silence();
    src.remix(dst);

    REQUIRE_THAT(dst[0][0],
                 Approx(M_SQRT1_2 * (src[0][0] + src[1][0]) + src[2][0] +
                        0.5 * (src[4][0] + src[5][0])));
    REQUIRE_THAT(dst[0][1],
                 Approx(M_SQRT1_2 * (src[0][1] + src[1][1]) + src[2][1] +
                        0.5 * (src[4][1] + src[5][1])));
    REQUIRE_THAT(dst[0][2],
                 Approx(M_SQRT1_2 * (src[0][2] + src[1][2]) + src[2][2] +
                        0.5 * (src[4][2] + src[5][2])));
}

TEST_CASE("Buffer remix 6 - 2", "[Buffer]") {
    Dynamo::Sound::Buffer src = dataset.s_51;
    Dynamo::Sound::Buffer dst(src.frames(), 2);

    dst.silence();
    src.remix(dst);

    REQUIRE_THAT(dst[0][0],
                 Approx(src[0][0] + M_SQRT1_2 * (src[2][0] + src[4][0]), 1e-5));
    REQUIRE_THAT(dst[0][1],
                 Approx(src[0][1] + M_SQRT1_2 * (src[2][1] + src[4][1]), 1e-5));
    REQUIRE_THAT(dst[0][2],
                 Approx(src[0][2] + M_SQRT1_2 * (src[2][2] + src[4][2]), 1e-5));

    REQUIRE_THAT(dst[1][0],
                 Approx(src[1][0] + M_SQRT1_2 * (src[2][0] + src[5][0]), 1e-5));
    REQUIRE_THAT(dst[1][1],
                 Approx(src[1][1] + M_SQRT1_2 * (src[2][1] + src[5][1]), 1e-5));
    REQUIRE_THAT(dst[1][2],
                 Approx(src[1][2] + M_SQRT1_2 * (src[2][2] + src[5][2]), 1e-5));
}

TEST_CASE("Buffer remix 6 - 4", "[Buffer]") {
    Dynamo::Sound::Buffer src = dataset.s_51;
    Dynamo::Sound::Buffer dst(src.frames(), 4);

    dst.silence();
    src.remix(dst);

    REQUIRE_THAT(dst[0][0], Approx(src[0][0] + M_SQRT1_2 * src[2][0], 1e-5));
    REQUIRE_THAT(dst[0][1], Approx(src[0][1] + M_SQRT1_2 * src[2][1], 1e-5));
    REQUIRE_THAT(dst[0][2], Approx(src[0][2] + M_SQRT1_2 * src[2][2], 1e-5));

    REQUIRE_THAT(dst[1][0], Approx(src[1][0] + M_SQRT1_2 * src[2][0], 1e-5));
    REQUIRE_THAT(dst[1][1], Approx(src[1][1] + M_SQRT1_2 * src[2][1], 1e-5));
    REQUIRE_THAT(dst[1][2], Approx(src[1][2] + M_SQRT1_2 * src[2][2], 1e-5));

    REQUIRE(dst[2][0] == src[4][0]);
    REQUIRE(dst[2][1] == src[4][1]);
    REQUIRE(dst[2][2] == src[4][2]);

    REQUIRE(dst[3][0] == src[5][0]);
    REQUIRE(dst[3][1] == src[5][1]);
    REQUIRE(dst[3][2] == src[5][2]);
}

TEST_CASE("Buffer remix discrete upmix", "[Buffer]") {
    Dynamo::Sound::Buffer src = dataset.stereo;
    Dynamo::Sound::Buffer dst(src.frames(), 3);

    dst.silence();
    src.remix(dst);

    REQUIRE(dst[0][0] == src[0][0]);
    REQUIRE(dst[0][1] == src[0][1]);
    REQUIRE(dst[0][2] == src[0][2]);

    REQUIRE(dst[1][0] == src[1][0]);
    REQUIRE(dst[1][1] == src[1][1]);
    REQUIRE(dst[1][2] == src[1][2]);

    REQUIRE(dst[2][0] == 0);
    REQUIRE(dst[2][1] == 0);
    REQUIRE(dst[2][2] == 0);
}

TEST_CASE("Buffer remix discrete downmix", "[Buffer]") {
    Dynamo::Sound::Buffer src = dataset.s_51;
    Dynamo::Sound::Buffer dst(src.frames(), 3);

    dst.silence();
    src.remix(dst);

    REQUIRE(dst[0][0] == src[0][0]);
    REQUIRE(dst[0][1] == src[0][1]);
    REQUIRE(dst[0][2] == src[0][2]);

    REQUIRE(dst[1][0] == src[1][0]);
    REQUIRE(dst[1][1] == src[1][1]);
    REQUIRE(dst[1][2] == src[1][2]);

    REQUIRE(dst[2][0] == src[2][0]);
    REQUIRE(dst[2][1] == src[2][1]);
    REQUIRE(dst[2][2] == src[2][2]);
}

TEST_CASE("Buffer remix discrete equal", "[Buffer]") {
    Dynamo::Sound::Buffer src = dataset.s_51;
    Dynamo::Sound::Buffer dst(src.frames(), 6);

    dst.silence();
    src.remix(dst);

    REQUIRE(dst[0][0] == src[0][0]);
    REQUIRE(dst[0][1] == src[0][1]);
    REQUIRE(dst[0][2] == src[0][2]);

    REQUIRE(dst[1][0] == src[1][0]);
    REQUIRE(dst[1][1] == src[1][1]);
    REQUIRE(dst[1][2] == src[1][2]);

    REQUIRE(dst[2][0] == src[2][0]);
    REQUIRE(dst[2][1] == src[2][1]);
    REQUIRE(dst[2][2] == src[2][2]);

    REQUIRE(dst[3][0] == src[3][0]);
    REQUIRE(dst[3][1] == src[3][1]);
    REQUIRE(dst[3][2] == src[3][2]);

    REQUIRE(dst[4][0] == src[4][0]);
    REQUIRE(dst[4][1] == src[4][1]);
    REQUIRE(dst[4][2] == src[4][2]);

    REQUIRE(dst[5][0] == src[5][0]);
    REQUIRE(dst[5][1] == src[5][1]);
    REQUIRE(dst[5][2] == src[5][2]);
}