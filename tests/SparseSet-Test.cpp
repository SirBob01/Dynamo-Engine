#include <Dynamo.hpp>
#include <catch2/catch_test_macros.hpp>

using CharSet = Dynamo::SparseSet<char>;

TEST_CASE("SparseSet insert", "[SparseSet]") {
    CharSet set;
    Dynamo::IdTracker tracker;

    Dynamo::Id a = tracker.generate();
    set.insert(a, 'a');
    REQUIRE(set.get(a) == 'a');

    Dynamo::Id b = tracker.generate();
    set.insert(b, 'b');
    REQUIRE(set.get(b) == 'b');

    Dynamo::Id c = tracker.generate();
    set.insert(c, 'c');
    REQUIRE(set.get(c) == 'c');

    REQUIRE(set.size() == 3);
}

TEST_CASE("SparseSet remove", "[SparseSet]") {
    CharSet set;
    Dynamo::IdTracker tracker;

    Dynamo::Id a = tracker.generate();
    set.insert(a, 'a');
    REQUIRE(set.find(a) == 0);
    REQUIRE(set.get(a) == 'a');

    set.remove(a);
    REQUIRE(set.find(a) == -1);
    REQUIRE(!set.exists(a));
    REQUIRE_THROWS(set.get(a));

    REQUIRE(set.size() == 0);
}

TEST_CASE("SparseSet clear", "[SparseSet]") {
    CharSet set;
    Dynamo::IdTracker tracker;

    Dynamo::Id a = tracker.generate();
    set.insert(a, 'a');
    REQUIRE(set.get(a) == 'a');

    Dynamo::Id b = tracker.generate();
    set.insert(b, 'b');
    REQUIRE(set.get(b) == 'b');

    Dynamo::Id c = tracker.generate();
    set.insert(c, 'c');
    REQUIRE(set.get(c) == 'c');

    set.clear();
    REQUIRE(set.size() == 0);
    REQUIRE(!set.exists(a));
    REQUIRE(!set.exists(b));
    REQUIRE(!set.exists(c));

    REQUIRE(set.find(a) == -1);
    REQUIRE(set.find(b) == -1);
    REQUIRE(set.find(c) == -1);

    REQUIRE_THROWS(set.get(a));
    REQUIRE_THROWS(set.get(b));
    REQUIRE_THROWS(set.get(c));
}

TEST_CASE("SparseSet mismatched id version", "[SparseSet]") {
    CharSet set;
    Dynamo::IdTracker tracker;

    Dynamo::Id a = tracker.generate();
    set.insert(a, 'a');
    REQUIRE(set.get(a) == 'a');

    tracker.discard(a);
    Dynamo::Id b = tracker.generate();
    REQUIRE(Dynamo::IdTracker::get_index(a) == Dynamo::IdTracker::get_index(b));
    REQUIRE_THROWS(set.get(b));

    set.insert(b, 'b');
    REQUIRE(set.get(b) == 'b');
    REQUIRE(set.size() == 1);
}

TEST_CASE("SparseSet foreach", "[SparseSet]") {
    CharSet set;
    Dynamo::IdTracker tracker;

    Dynamo::Id a = tracker.generate();
    set.insert(a, 'a');
    REQUIRE(set.get(a) == 'a');

    Dynamo::Id b = tracker.generate();
    set.insert(b, 'b');
    REQUIRE(set.get(b) == 'b');

    Dynamo::Id c = tracker.generate();
    set.insert(c, 'c');
    REQUIRE(set.get(c) == 'c');

    set.remove(a);
    Dynamo::Id d = tracker.generate();
    set.insert(d, 'd');

    REQUIRE(set.size() == 3);

    struct Triplet {
        char &item;
        Dynamo::Id id;
        int index;
    };
    std::vector<Triplet> triplets;
    set.forall([&](char &item, Dynamo::Id id, int index) {
        triplets.push_back({item, id, index});
    });

    REQUIRE(triplets[0].item == 'c');
    REQUIRE(triplets[0].id == c);
    REQUIRE(triplets[0].index == 0);

    REQUIRE(triplets[1].item == 'b');
    REQUIRE(triplets[1].id == b);
    REQUIRE(triplets[1].index == 1);

    REQUIRE(triplets[2].item == 'd');
    REQUIRE(triplets[2].id == d);
    REQUIRE(triplets[2].index == 2);

    REQUIRE(triplets.size() == set.size());
}