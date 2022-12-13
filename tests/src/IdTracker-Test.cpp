#include <Dynamo.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("IdTracker get index and version", "[IdTracker]") {
    uint64_t index = 1;
    index = (index << 32) - 1;

    uint64_t version = 1;
    version = (version << 31) - 10;

    Dynamo::Id id = (index << 32) | version;
    REQUIRE(Dynamo::IdTracker::get_index(id) == index);
    REQUIRE(Dynamo::IdTracker::get_version(id) == version);
}

TEST_CASE("IdTracker generate", "[IdTracker]") {
    Dynamo::IdTracker tracker;
    Dynamo::Id a = tracker.generate();
    Dynamo::Id b = tracker.generate();

    REQUIRE(Dynamo::IdTracker::get_index(a) == 0);
    REQUIRE(Dynamo::IdTracker::get_version(a) == 0);

    REQUIRE(Dynamo::IdTracker::get_index(b) == 1);
    REQUIRE(Dynamo::IdTracker::get_version(b) == 0);
}

TEST_CASE("IdTracker discard", "[IdTracker]") {
    Dynamo::IdTracker tracker;
    Dynamo::Id a = tracker.generate();
    tracker.discard(a);
    Dynamo::Id b = tracker.generate();
    tracker.discard(b);
    Dynamo::Id c = tracker.generate();
    Dynamo::Id d = tracker.generate();

    REQUIRE(Dynamo::IdTracker::get_index(a) == 0);
    REQUIRE(Dynamo::IdTracker::get_version(a) == 0);

    REQUIRE(Dynamo::IdTracker::get_index(b) == 0);
    REQUIRE(Dynamo::IdTracker::get_version(b) == 1);

    REQUIRE(Dynamo::IdTracker::get_index(c) == 0);
    REQUIRE(Dynamo::IdTracker::get_version(c) == 2);

    REQUIRE(Dynamo::IdTracker::get_index(d) == 1);
    REQUIRE(Dynamo::IdTracker::get_version(d) == 0);
}

TEST_CASE("IdTracker is_active", "[IdTracker]") {
    Dynamo::IdTracker tracker;
    Dynamo::Id a = tracker.generate();

    REQUIRE(tracker.is_active(a));
    tracker.discard(a);
    REQUIRE(!tracker.is_active(a));
}

TEST_CASE("IdTracker clear", "[IdTracker]") {
    Dynamo::IdTracker tracker;
    for (int i = 0; i < 100; i++) {
        tracker.generate();
    }
    tracker.clear();

    Dynamo::Id a = tracker.generate();
    REQUIRE(Dynamo::IdTracker::get_index(a) == 0);
    REQUIRE(Dynamo::IdTracker::get_version(a) == 0);
}