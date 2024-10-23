#include <Dynamo.hpp>
#include <catch2/catch_test_macros.hpp>

DYN_DEFINE_ID_TYPE(Id);

TEST_CASE("IdTracker get index and version", "[IdTracker]") {
    uintptr_t index = 1;
    index = (index << 32) - 1;

    uintptr_t version = 1;
    version = (version << 31) - 10;

    Id id = reinterpret_cast<Id>((index << 32) | version);
    REQUIRE(Dynamo::IdTracker<Id>::get_index(id) == index);
    REQUIRE(Dynamo::IdTracker<Id>::get_version(id) == version);
}

TEST_CASE("IdTracker generate", "[IdTracker]") {
    Dynamo::IdTracker<Id> tracker;
    Id a = tracker.generate();
    Id b = tracker.generate();

    REQUIRE(Dynamo::IdTracker<Id>::get_index(a) == 0);
    REQUIRE(Dynamo::IdTracker<Id>::get_version(a) == 0);

    REQUIRE(Dynamo::IdTracker<Id>::get_index(b) == 1);
    REQUIRE(Dynamo::IdTracker<Id>::get_version(b) == 0);
}

TEST_CASE("IdTracker discard", "[IdTracker]") {
    Dynamo::IdTracker<Id> tracker;
    Id a = tracker.generate();
    tracker.discard(a);
    Id b = tracker.generate();
    tracker.discard(b);
    Id c = tracker.generate();
    Id d = tracker.generate();

    REQUIRE(Dynamo::IdTracker<Id>::get_index(a) == 0);
    REQUIRE(Dynamo::IdTracker<Id>::get_version(a) == 0);

    REQUIRE(Dynamo::IdTracker<Id>::get_index(b) == 0);
    REQUIRE(Dynamo::IdTracker<Id>::get_version(b) == 1);

    REQUIRE(Dynamo::IdTracker<Id>::get_index(c) == 0);
    REQUIRE(Dynamo::IdTracker<Id>::get_version(c) == 2);

    REQUIRE(Dynamo::IdTracker<Id>::get_index(d) == 1);
    REQUIRE(Dynamo::IdTracker<Id>::get_version(d) == 0);
}

TEST_CASE("IdTracker is_active", "[IdTracker]") {
    Dynamo::IdTracker<Id> tracker;
    Id a = tracker.generate();

    REQUIRE(tracker.is_active(a));
    tracker.discard(a);
    REQUIRE(!tracker.is_active(a));
}

TEST_CASE("IdTracker clear", "[IdTracker]") {
    Dynamo::IdTracker<Id> tracker;
    for (int i = 0; i < 100; i++) {
        tracker.generate();
    }
    tracker.clear();

    Id a = tracker.generate();
    REQUIRE(Dynamo::IdTracker<Id>::get_index(a) == 0);
    REQUIRE(Dynamo::IdTracker<Id>::get_version(a) == 0);
}