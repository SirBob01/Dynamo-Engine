#include <Dynamo.hpp>
#include <catch2/catch_test_macros.hpp>

#include "../Common.hpp"

using WaveForm = std::vector<Dynamo::Sound::WaveSample>;

struct Samples {
    Dynamo::Sound::Sound mono;
    Dynamo::Sound::Sound stereo;
    Dynamo::Sound::Sound quad;
    Dynamo::Sound::Sound s_51;

    Samples() {
        WaveForm mono_buf;
        for (unsigned i = 0; i < 3 * 1; i++) {
            mono_buf.push_back(i);
        }
        mono = Dynamo::Sound::Sound(mono_buf, 1, 44100);

        WaveForm stereo_buf;
        for (unsigned i = 0; i < 3 * 2; i++) {
            stereo_buf.push_back(i);
        }
        stereo = Dynamo::Sound::Sound(stereo_buf, 2, 44100);

        WaveForm quad_buf;
        for (unsigned i = 0; i < 3 * 4; i++) {
            quad_buf.push_back(i);
        }
        quad = Dynamo::Sound::Sound(quad_buf, 4, 44100);

        WaveForm s_51_buf;
        for (unsigned i = 0; i < 3 * 6; i++) {
            s_51_buf.push_back(i);
        }
        s_51 = Dynamo::Sound::Sound(s_51_buf, 6, 44100);
    }
};

Samples dataset;

TEST_CASE("Sound remix 1 - 2", "[Sound]") {
    Dynamo::Sound::Sound src = dataset.mono;
    Dynamo::Sound::Sound dst(src.frames(), 2);

    src.remix(dst, 0, 0, src.frames());

    REQUIRE(dst[0][0] == src[0][0]);
    REQUIRE(dst[0][1] == src[0][1]);
    REQUIRE(dst[0][2] == src[0][2]);

    REQUIRE(dst[1][0] == src[0][0]);
    REQUIRE(dst[1][1] == src[0][1]);
    REQUIRE(dst[1][2] == src[0][2]);
}

TEST_CASE("Sound remix 1 - 4", "[Sound]") {
    Dynamo::Sound::Sound src = dataset.mono;
    Dynamo::Sound::Sound dst(src.frames(), 4);

    src.remix(dst, 0, 0, src.frames());

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

TEST_CASE("Sound remix 1 - 6", "[Sound]") {
    Dynamo::Sound::Sound src = dataset.mono;
    Dynamo::Sound::Sound dst(src.frames(), 6);

    src.remix(dst, 0, 0, src.frames());

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

TEST_CASE("Sound remix 2 - 1", "[Sound]") {
    Dynamo::Sound::Sound src = dataset.stereo;
    Dynamo::Sound::Sound dst(src.frames(), 1);

    src.remix(dst, 0, 0, src.frames());

    REQUIRE(dst[0][0] == 0.5 * (src[0][0] + src[1][0]));
    REQUIRE(dst[0][1] == 0.5 * (src[0][1] + src[1][1]));
    REQUIRE(dst[0][2] == 0.5 * (src[0][2] + src[1][2]));
}

TEST_CASE("Sound remix 2 - 4", "[Sound]") {
    Dynamo::Sound::Sound src = dataset.stereo;
    Dynamo::Sound::Sound dst(src.frames(), 4);

    src.remix(dst, 0, 0, src.frames());

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

TEST_CASE("Sound remix 2 - 6", "[Sound]") {
    Dynamo::Sound::Sound src = dataset.stereo;
    Dynamo::Sound::Sound dst(src.frames(), 6);

    src.remix(dst, 0, 0, src.frames());

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

TEST_CASE("Sound remix 4 - 1", "[Sound]") {
    Dynamo::Sound::Sound src = dataset.quad;
    Dynamo::Sound::Sound dst(src.frames(), 1);

    src.remix(dst, 0, 0, src.frames());

    REQUIRE(dst[0][0] ==
            0.25 * (src[0][0] + src[1][0] + src[2][0] + src[3][0]));
    REQUIRE(dst[0][1] ==
            0.25 * (src[0][1] + src[1][1] + src[2][1] + src[3][1]));
    REQUIRE(dst[0][2] ==
            0.25 * (src[0][2] + src[1][2] + src[2][2] + src[3][2]));
}

TEST_CASE("Sound remix 4 - 2", "[Sound]") {
    Dynamo::Sound::Sound src = dataset.quad;
    Dynamo::Sound::Sound dst(src.frames(), 2);

    src.remix(dst, 0, 0, src.frames());

    REQUIRE(dst[0][0] == 0.5 * (src[0][0] + src[2][0]));
    REQUIRE(dst[0][1] == 0.5 * (src[0][1] + src[2][1]));
    REQUIRE(dst[0][2] == 0.5 * (src[0][2] + src[2][2]));

    REQUIRE(dst[1][0] == 0.5 * (src[1][0] + src[3][0]));
    REQUIRE(dst[1][1] == 0.5 * (src[1][1] + src[3][1]));
    REQUIRE(dst[1][2] == 0.5 * (src[1][2] + src[3][2]));
}

TEST_CASE("Sound remix 4 - 6", "[Sound]") {
    Dynamo::Sound::Sound src = dataset.quad;
    Dynamo::Sound::Sound dst(src.frames(), 6);

    src.remix(dst, 0, 0, src.frames());

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

TEST_CASE("Sound remix 6 - 1", "[Sound]") {
    Dynamo::Sound::Sound src = dataset.s_51;
    Dynamo::Sound::Sound dst(src.frames(), 1);

    src.remix(dst, 0, 0, src.frames());

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

TEST_CASE("Sound remix 6 - 2", "[Sound]") {
    Dynamo::Sound::Sound src = dataset.s_51;
    Dynamo::Sound::Sound dst(src.frames(), 2);

    src.remix(dst, 0, 0, src.frames());

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

TEST_CASE("Sound remix 6 - 4", "[Sound]") {
    Dynamo::Sound::Sound src = dataset.s_51;
    Dynamo::Sound::Sound dst(src.frames(), 4);

    src.remix(dst, 0, 0, src.frames());

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

TEST_CASE("Sound remix discrete upmix", "[Sound]") {
    Dynamo::Sound::Sound src = dataset.stereo;
    Dynamo::Sound::Sound dst(src.frames(), 3);

    src.remix(dst, 0, 0, src.frames());

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

TEST_CASE("Sound remix discrete downmix", "[Sound]") {
    Dynamo::Sound::Sound src = dataset.s_51;
    Dynamo::Sound::Sound dst(src.frames(), 3);

    src.remix(dst, 0, 0, src.frames());

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

TEST_CASE("Sound remix discrete equal", "[Sound]") {
    Dynamo::Sound::Sound src = dataset.s_51;
    Dynamo::Sound::Sound dst(src.frames(), 6);

    src.remix(dst, 0, 0, src.frames());

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