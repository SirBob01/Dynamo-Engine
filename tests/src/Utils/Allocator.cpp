#include <Dynamo.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Allocator Reserve", "[Allocator]") {
    Dynamo::Allocator allocator(10);
    Dynamo::Log::info("Test Reserve - {}", allocator.print());

    std::optional<unsigned> a = allocator.reserve(7, 1);
    REQUIRE(a.has_value());
    REQUIRE(a.value() == 0);
    Dynamo::Log::info("Test Reserve - {}", allocator.print());

    std::optional<unsigned> b = allocator.reserve(3, 1);
    REQUIRE(b.has_value());
    REQUIRE(b.value() == 7);
    Dynamo::Log::info("Test Reserve - {}", allocator.print());

    std::optional<unsigned> c = allocator.reserve(1, 1);
    REQUIRE(!c.has_value());
    Dynamo::Log::info("Test Reserve - {}", allocator.print());
}

TEST_CASE("Allocator Reserve Alignment", "[Allocator]") {
    Dynamo::Allocator allocator(10);
    Dynamo::Log::info("Test Reserve Alignment - {}", allocator.print());

    std::optional<unsigned> a = allocator.reserve(5, 3);
    REQUIRE(a.has_value());
    REQUIRE(a.value() == 0);
    Dynamo::Log::info("Test Reserve Alignment - {}", allocator.print());

    std::optional<unsigned> b = allocator.reserve(3, 3);
    REQUIRE(b.has_value());
    REQUIRE(b.value() == 6);
    Dynamo::Log::info("Test Reserve Alignment - {}", allocator.print());

    std::optional<unsigned> c = allocator.reserve(4, 3);
    REQUIRE(!c.has_value());
    Dynamo::Log::info("Test Reserve Alignment - {}", allocator.print());

    std::optional<unsigned> d = allocator.reserve(1, 1);
    REQUIRE(d.has_value());
    REQUIRE(d.value() == 5);
    Dynamo::Log::info("Test Reserve Alignment - {}", allocator.print());

    std::optional<unsigned> e = allocator.reserve(1, 1);
    REQUIRE(e.has_value());
    REQUIRE(e.value() == 9);
    Dynamo::Log::info("Test Reserve Alignment - {}", allocator.print());

    // Memory full
    std::optional<unsigned> f = allocator.reserve(1, 1);
    REQUIRE(!f.has_value());
    Dynamo::Log::info("Test Reserve Alignment - {}", allocator.print());
}

TEST_CASE("Allocator Free", "[Allocator]") {
    Dynamo::Allocator allocator(10);
    Dynamo::Log::info("Test Free - {}", allocator.print());

    std::optional<unsigned> a = allocator.reserve(5, 1);
    REQUIRE(a.has_value());
    REQUIRE(a.value() == 0);
    Dynamo::Log::info("Test Free - {}", allocator.print());

    std::optional<unsigned> b = allocator.reserve(5, 1);
    REQUIRE(b.has_value());
    REQUIRE(b.value() == 5);
    allocator.print();
    Dynamo::Log::info("Test Free - {}", allocator.print());

    REQUIRE_NOTHROW(allocator.free(a.value()));
    Dynamo::Log::info("Test Free - {}", allocator.print());

    std::optional<unsigned> c = allocator.reserve(5, 1);
    REQUIRE(c.has_value());
    REQUIRE(c.value() == 0);
    allocator.print();
    Dynamo::Log::info("Test Free - {}", allocator.print());
}

TEST_CASE("Allocator Resize", "[Allocator]") {
    Dynamo::Allocator allocator(10);
    Dynamo::Log::info("Test Resize - {}", allocator.print());

    std::optional<unsigned> a = allocator.reserve(7, 5);
    REQUIRE(a.has_value());
    REQUIRE(a.value() == 0);
    Dynamo::Log::info("Test Resize - {}", allocator.print());

    std::optional<unsigned> b = allocator.reserve(2, 2);
    REQUIRE(b.has_value());
    REQUIRE(b.value() == 8);
    Dynamo::Log::info("Test Resize - {}", allocator.print());

    allocator.grow(20);
    Dynamo::Log::info("Test Resize - {}", allocator.print());

    std::optional<unsigned> d = allocator.reserve(6, 2);
    REQUIRE(d.has_value());
    REQUIRE(d.value() == 10);
    Dynamo::Log::info("Test Resize - {}", allocator.print());
}

TEST_CASE("Allocator Resize Full", "[Allocator]") {
    Dynamo::Allocator allocator(10);
    std::optional<unsigned> e = allocator.reserve(10, 1);
    REQUIRE(e.has_value());
    REQUIRE(e.value() == 0);
    Dynamo::Log::info("Test Resize Full - {}", allocator.print());

    allocator.grow(20);
    std::optional<unsigned> f = allocator.reserve(5, 1);
    REQUIRE(f.has_value());
    REQUIRE(f.value() == 10);
    Dynamo::Log::info("Test Resize Full - {}", allocator.print());

    allocator.grow(40);
    std::optional<unsigned> g = allocator.reserve(10, 1);
    REQUIRE(g.has_value());
    REQUIRE(g.value() == 15);
    Dynamo::Log::info("Test Resize Full - {}", allocator.print());
}

TEST_CASE("Allocator is allocated", "[Allocator]") {
    Dynamo::Allocator allocator(10);
    std::optional<unsigned> a = allocator.reserve(7, 5);
    REQUIRE(allocator.is_reserved(a.value()));

    allocator.free(a.value());

    REQUIRE(!allocator.is_reserved(a.value()));
}

TEST_CASE("Allocator capacity", "[Allocator]") {
    Dynamo::Allocator allocator(10);
    REQUIRE(allocator.capacity() == 10);

    allocator.grow(20);
    REQUIRE(allocator.capacity() == 20);

    REQUIRE_THROWS(allocator.grow(3));
}

TEST_CASE("Allocator block size", "[Allocator]") {
    Dynamo::Allocator allocator(10);
    Dynamo::Log::info("Test Block Size - {}", allocator.print());

    std::optional<unsigned> a = allocator.reserve(7, 1);
    REQUIRE(allocator.size(a.value()) == 7);
    Dynamo::Log::info("Test Block Size - {}", allocator.print());

    std::optional<unsigned> b = allocator.reserve(3, 1);
    REQUIRE(allocator.size(b.value()) == 3);
    Dynamo::Log::info("Test Block Size - {}", allocator.print());

    std::optional<unsigned> c = allocator.reserve(1, 1);
    REQUIRE_THROWS(allocator.size(c.value()));
    Dynamo::Log::info("Test Block Size - {}", allocator.print());
}