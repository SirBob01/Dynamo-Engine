#include <Dynamo.hpp>
#include <catch2/catch_test_macros.hpp>

int sum_fast(int x, int y) { return x + y; }

int sum_slow(int x, int y) {
    int i = 0;
    for (i = 0; i < 1000000; i++) {
    }
    return x + y;
}

TEST_CASE("ThreadPool create and destroy", "[ThreadPool]") {
    {
        Dynamo::ThreadPool pool_10(10);
        Dynamo::ThreadPool pool_device;
    }
}

TEST_CASE("ThreadPool submit", "[ThreadPool]") {
    Dynamo::ThreadPool pool;
    std::future<int> a = pool.submit(sum_fast, 2, 3);
    std::future<int> b = pool.submit(sum_slow, 5, 6);
    std::future<int> c = pool.submit(sum_fast, a.get(), b.get());
    REQUIRE(c.get() == 16);
}

TEST_CASE("ThreadPool wait", "[ThreadPool]") {
    Dynamo::ThreadPool pool;
    std::future<int> a = pool.submit(sum_fast, 2, 3);
    std::future<int> b = pool.submit(sum_slow, 5, 6);
    std::future<int> c = pool.submit(sum_fast, 10, 3);
    pool.wait_all();
    REQUIRE(a.get() == 5);
    REQUIRE(b.get() == 11);
    REQUIRE(c.get() == 13);
}
