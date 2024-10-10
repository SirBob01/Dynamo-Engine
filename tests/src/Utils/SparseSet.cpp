#include <Dynamo.hpp>
#include <catch2/catch_test_macros.hpp>

using CharSet = Dynamo::SparseSet<char>;

TEST_CASE("SparseSet []", "[SparseSet]") {
    CharSet set;
    Dynamo::IdTracker tracker;

    set.insert(tracker.generate(), 'a');
    set.insert(tracker.generate(), 'b');
    set.insert(tracker.generate(), 'c');

    REQUIRE(set[0] == 'a');
    REQUIRE(set[1] == 'b');
    REQUIRE(set[2] == 'c');
}

TEST_CASE("SparseSet const []", "[SparseSet]") {
    CharSet set;
    Dynamo::IdTracker tracker;

    set.insert(tracker.generate(), 'a');
    set.insert(tracker.generate(), 'b');
    set.insert(tracker.generate(), 'c');

    const CharSet &const_set = set;

    REQUIRE(const_set[0] == 'a');
    REQUIRE(const_set[1] == 'b');
    REQUIRE(const_set[2] == 'c');
}

TEST_CASE("SparseSet get", "[SparseSet]") {
    CharSet set;
    Dynamo::IdTracker tracker;

    Dynamo::Id a = tracker.generate();
    Dynamo::Id b = tracker.generate();
    Dynamo::Id c = tracker.generate();

    set.insert(a, 'a');
    set.insert(b, 'b');
    set.insert(c, 'c');

    REQUIRE(set.get(a) == 'a');
    REQUIRE(set.get(b) == 'b');
    REQUIRE(set.get(c) == 'c');
}

TEST_CASE("SparseSet const get", "[SparseSet]") {
    CharSet set;
    Dynamo::IdTracker tracker;

    Dynamo::Id a = tracker.generate();
    Dynamo::Id b = tracker.generate();
    Dynamo::Id c = tracker.generate();

    set.insert(a, 'a');
    set.insert(b, 'b');
    set.insert(c, 'c');

    const CharSet &const_set = set;

    REQUIRE(const_set.get(a) == 'a');
    REQUIRE(const_set.get(b) == 'b');
    REQUIRE(const_set.get(c) == 'c');
}

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
    REQUIRE(!set.empty());
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
    REQUIRE(set.empty());
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
    REQUIRE(set.empty());

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
    REQUIRE(!set.empty());
}

TEST_CASE("SparseSet forall", "[SparseSet]") {
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
    REQUIRE(!set.empty());

    struct Pair {
        char &item;
        Dynamo::Id id;
    };
    std::vector<Pair> triplets;
    std::vector<char> items;
    std::vector<Dynamo::Id> ids;

    set.forall([&](char &item, Dynamo::Id id) { triplets.push_back({item, id}); });
    set.forall_items([&](char &item) { items.push_back(item); });
    set.forall_ids([&](Dynamo::Id id) { ids.push_back(id); });

    REQUIRE(triplets[0].item == 'c');
    REQUIRE(triplets[0].id == c);
    REQUIRE(items[0] == 'c');
    REQUIRE(ids[0] == c);

    REQUIRE(triplets[1].item == 'b');
    REQUIRE(triplets[1].id == b);
    REQUIRE(items[1] == 'b');
    REQUIRE(ids[1] == b);

    REQUIRE(triplets[2].item == 'd');
    REQUIRE(triplets[2].id == d);
    REQUIRE(items[2] == 'd');
    REQUIRE(ids[2] == d);

    REQUIRE(triplets.size() == set.size());
    REQUIRE(items.size() == set.size());
    REQUIRE(ids.size() == set.size());
}