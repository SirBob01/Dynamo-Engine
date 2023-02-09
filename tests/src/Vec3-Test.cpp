#include "./Common.hpp"
#include <Dynamo.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Vec3 length squared", "[Vec3]") {
    Dynamo::Vec3 a(1, 2, 2);
    REQUIRE(a.length_squared() == 9);
}

TEST_CASE("Vec3 length", "[Vec3]") {
    Dynamo::Vec3 a(1, 2, 2);
    REQUIRE(a.length() == 3);
}

TEST_CASE("Vec3 add", "[Vec3]") {
    Dynamo::Vec3 a(1, 2, 5);
    Dynamo::Vec3 b(3, 4, 6);
    Dynamo::Vec3 c = a + b;
    REQUIRE(c.x == 4);
    REQUIRE(c.y == 6);
    REQUIRE(c.z == 11);
}

TEST_CASE("Vec3 subtract", "[Vec3]") {
    Dynamo::Vec3 a(1, 2, 5);
    Dynamo::Vec3 b(3, 4, 6);
    Dynamo::Vec3 c = a - b;
    REQUIRE(c.x == -2);
    REQUIRE(c.y == -2);
    REQUIRE(c.z == -1);
}

TEST_CASE("Vec3 multiply by scalar", "[Vec3]") {
    Dynamo::Vec3 a(1, 2, 3);
    Dynamo::Vec3 b = a * 3.2;
    REQUIRE(b.x == 3.2f);
    REQUIRE(b.y == 6.4f);
    REQUIRE(b.z == 9.6f);
}

TEST_CASE("Vec3 divide by scalar", "[Vec3]") {
    Dynamo::Vec3 a(3.2f, 6.4f, 9.6f);
    Dynamo::Vec3 b = a / 3.2f;
    REQUIRE(b.x == 1);
    REQUIRE(b.y == 2);
    REQUIRE(b.z == 3);
}

TEST_CASE("Vec3 negation", "[Vec3]") {
    Dynamo::Vec3 a(1, 2, 3);
    Dynamo::Vec3 b = -a;
    REQUIRE(b.x == -1);
    REQUIRE(b.y == -2);
    REQUIRE(b.z == -3);
}

TEST_CASE("Vec3 add in-place", "[Vec3]") {
    Dynamo::Vec3 a(1, 2, 5);
    Dynamo::Vec3 b(3, 4, 6);
    a += b;
    REQUIRE(a.x == 4);
    REQUIRE(a.y == 6);
    REQUIRE(a.z == 11);
}

TEST_CASE("Vec3 subtract in-place", "[Vec3]") {
    Dynamo::Vec3 a(1, 2, 5);
    Dynamo::Vec3 b(3, 4, 6);
    a -= b;
    REQUIRE(a.x == -2);
    REQUIRE(a.y == -2);
    REQUIRE(a.z == -1);
}

TEST_CASE("Vec3 multiply by scalar in-place", "[Vec3]") {
    Dynamo::Vec3 a(1, 2, 3);
    a *= 3.2;
    REQUIRE(a.x == 3.2f);
    REQUIRE(a.y == 6.4f);
    REQUIRE(a.z == 9.6f);
}

TEST_CASE("Vec3 divide by scalar in-place", "[Vec3]") {
    Dynamo::Vec3 a(3.2f, 6.4f, 9.6f);
    a /= 3.2f;
    REQUIRE(a.x == 1);
    REQUIRE(a.y == 2);
    REQUIRE(a.z == 3);
}

TEST_CASE("Vec3 normalize", "[Vec3]") {
    Dynamo::Vec3 a(1, 2, 2);
    a.normalize();
    REQUIRE(a.x == 1.0f / 3);
    REQUIRE(a.y == 2.0f / 3);
    REQUIRE(a.z == 2.0f / 3);
}

TEST_CASE("Vec3 transform", "[Vec3]") {
    Dynamo::Vec3 a(1, 2, 2);
    Dynamo::Matrix<4> mat;
    a.transform(mat);
    REQUIRE(a.x == 1);
    REQUIRE(a.y == 2);
    REQUIRE(a.z == 2);
}

TEST_CASE("Vec3 dot product", "[Vec3]") {
    Dynamo::Vec3 a(1, 2, -3);
    Dynamo::Vec3 b(5, 1, 2);
    float dot = a * b;
    REQUIRE(dot == 1);
}

TEST_CASE("Vec3 cross product", "[Vec3]") {
    Dynamo::Vec3 a(1, 2, -3);
    Dynamo::Vec3 b(5, 1, 2);
    Dynamo::Vec3 cross = a.cross(b);
    REQUIRE(cross.x == 7);
    REQUIRE(cross.y == -17);
    REQUIRE(cross.z == -9);
}

TEST_CASE("Vec3 equality", "[Vec3]") {
    Dynamo::Vec3 a(2, 3, 4);
    Dynamo::Vec3 b(2, 3, 4);
    REQUIRE(a == b);
}

TEST_CASE("Vec3 inequality", "[Vec3]") {
    Dynamo::Vec3 a(1, 2, 3);
    Dynamo::Vec3 b(3, 2, 1);
    Dynamo::Vec3 c(1, 4, 7);
    Dynamo::Vec3 d = b + c;
    REQUIRE(a != b);
    REQUIRE(a != c);
    REQUIRE(a != d);
}

TEST_CASE("Vec3 hash", "[Vec3]") {
    REQUIRE_NOTHROW(std::unordered_set<Dynamo::Vec3>());
}

TEST_CASE("Vec3 performance", "[Vec3]") {
    std::vector<Dynamo::Vec3> binary;
    for (unsigned i = 0; i < 1000; i++) {
        float x = Dynamo::Random::range(0, 10);
        float y = Dynamo::Random::range(0, 10);
        float z = Dynamo::Random::range(0, 10);
        binary.push_back(Dynamo::Vec3(x, y, z));
    }
    BENCHMARK("Vec3 add benchmark") {
        Dynamo::Vec3 c;
        for (unsigned i = 0; i < binary.size(); i++) {
            for (unsigned j = i + 1; j < binary.size(); j++) {
                do_not_optimize(c = binary[i] + binary[j]);
            }
        }
    };
    BENCHMARK("Vec3 subtract benchmark") {
        Dynamo::Vec3 c;
        for (unsigned i = 0; i < binary.size(); i++) {
            for (unsigned j = i + 1; j < binary.size(); j++) {
                do_not_optimize(c = binary[i] - binary[j]);
            }
        }
    };
    BENCHMARK("Vec3 dot benchmark") {
        float dot;
        for (unsigned i = 0; i < binary.size(); i++) {
            for (unsigned j = i + 1; j < binary.size(); j++) {
                do_not_optimize(dot = binary[i] * binary[j]);
            }
        }
    };

    std::vector<Dynamo::Vec3> unary;
    for (unsigned i = 0; i < 100000; i++) {
        float x = Dynamo::Random::range(0, 10);
        float y = Dynamo::Random::range(0, 10);
        float z = Dynamo::Random::range(0, 10);
        unary.push_back(Dynamo::Vec3(x, y, z));
    }
    BENCHMARK("Vec3 scalar multiply benchmark") {
        Dynamo::Vec3 c;
        for (unsigned i = 0; i < unary.size(); i++) {
            do_not_optimize(c = unary[i] * 69);
        }
    };
    BENCHMARK("Vec3 scalar divide benchmark") {
        Dynamo::Vec3 c;
        for (unsigned i = 0; i < unary.size(); i++) {
            do_not_optimize(c = unary[i] / 69);
        }
    };
}