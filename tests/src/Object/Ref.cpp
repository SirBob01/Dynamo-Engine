#include <Dynamo.hpp>
#include <catch2/catch_test_macros.hpp>

// Dummy test object
class Dummy : public Dynamo::Object {
    unsigned _value;
    unsigned *_deletes;

  public:
    Dummy(unsigned value, unsigned *deletes) : _value(value), _deletes(deletes) {}
    ~Dummy() { (*_deletes)++; }

    unsigned get_value() { return _value; }
};

TEST_CASE("Ref default constructor", "[Ref]") {
    Dynamo::Ref<Dummy> ref;
    REQUIRE(ref.empty());
    REQUIRE(ref.ref_count() == 0);
}

TEST_CASE("Ref create and access", "[Ref]") {
    unsigned deletes = 0;
    Dynamo::Ref<Dummy> ref = Dynamo::Ref<Dummy>::create(69, &deletes);
    REQUIRE(!ref.empty());
    REQUIRE(ref.ref_count() == 1);

    // Pointer access
    REQUIRE(ref->get_value() == 69);

    // Dereference access
    REQUIRE((*ref).get_value() == 69);
}

TEST_CASE("Ref copy constructor", "[Ref]") {
    unsigned deletes = 0;
    Dynamo::Ref<Dummy> ref = Dynamo::Ref<Dummy>::create(69, &deletes);
    Dynamo::Ref<Dummy> copy(ref);

    REQUIRE(!ref.empty());
    REQUIRE(ref.ref_count() == 2);
    REQUIRE(ref->get_value() == 69);

    REQUIRE(!copy.empty());
    REQUIRE(copy.ref_count() == 2);
    REQUIRE(copy->get_value() == 69);

    REQUIRE(copy == ref);
}

TEST_CASE("Ref move constructor", "[Ref]") {
    unsigned deletes = 0;
    Dynamo::Ref<Dummy> ref = Dynamo::Ref<Dummy>::create(69, &deletes);
    Dynamo::Ref<Dummy> move(std::move(ref));

    REQUIRE(ref.empty());
    REQUIRE(ref.ref_count() == 0);

    REQUIRE(!move.empty());
    REQUIRE(move.ref_count() == 1);
    REQUIRE(move->get_value() == 69);

    REQUIRE(move != ref);
}

TEST_CASE("Ref copy assignment", "[Ref]") {
    unsigned deletes = 0;
    Dynamo::Ref<Dummy> ref = Dynamo::Ref<Dummy>::create(69, &deletes);
    Dynamo::Ref<Dummy> copy = Dynamo::Ref<Dummy>::create(12, &deletes);
    Dynamo::Ref<Dummy> copy1;

    copy = ref;
    copy1 = ref;

    REQUIRE(!ref.empty());
    REQUIRE(ref.ref_count() == 3);
    REQUIRE(ref->get_value() == 69);

    REQUIRE(!copy.empty());
    REQUIRE(copy.ref_count() == 3);
    REQUIRE(copy->get_value() == 69);

    REQUIRE(!copy1.empty());
    REQUIRE(copy1.ref_count() == 3);
    REQUIRE(copy1->get_value() == 69);

    REQUIRE(copy == ref);
    REQUIRE(copy1 == ref);
    REQUIRE(copy == copy1);

    REQUIRE(deletes == 1);
}

TEST_CASE("Ref move assignment", "[Ref]") {
    unsigned deletes = 0;
    Dynamo::Ref<Dummy> ref = Dynamo::Ref<Dummy>::create(69, &deletes);
    Dynamo::Ref<Dummy> move = Dynamo::Ref<Dummy>::create(12, &deletes);
    Dynamo::Ref<Dummy> move1;

    move = std::move(ref);

    REQUIRE(ref.empty());
    REQUIRE(ref.ref_count() == 0);

    REQUIRE(!move.empty());
    REQUIRE(move.ref_count() == 1);
    REQUIRE(move->get_value() == 69);

    REQUIRE(move != ref);

    move1 = std::move(move);

    REQUIRE(move.empty());
    REQUIRE(move.ref_count() == 0);

    REQUIRE(!move1.empty());
    REQUIRE(move1.ref_count() == 1);
    REQUIRE(move1->get_value() == 69);

    REQUIRE(move1 != move);

    REQUIRE(deletes == 1);
}

TEST_CASE("Ref destructor", "[Ref, Object]") {
    unsigned deletes = 0;
    {
        Dynamo::Ref<Dummy> a;
        Dynamo::Ref<Dummy> b;

        REQUIRE(deletes == 0);
        {
            a = Dynamo::Ref<Dummy>::create(69, &deletes);
            b = Dynamo::Ref<Dummy>::create(12, &deletes);

            {
                // c is a copy of a
                Dynamo::Ref<Dummy> c = a;
            }

            // no deletes yet
            REQUIRE(deletes == 0);

            // d now owns the object held by b, b is empty
            Dynamo::Ref<Dummy> d = std::move(b);
        }

        // delete d
        REQUIRE(deletes == 1);
    }

    // delete a
    REQUIRE(deletes == 2);
}

TEST_CASE("Ref as function argument", "[Ref, Object]") {
    unsigned deletes = 0;

    auto copy_fn = [](Dynamo::Ref<Dummy> ref) {
        REQUIRE(!ref.empty());
        REQUIRE(ref.ref_count() == 2);
        REQUIRE(ref->get_value() == 69);
    };

    auto const_ref_fn = [](const Dynamo::Ref<Dummy> &ref) {
        REQUIRE(!ref.empty());
        REQUIRE(ref.ref_count() == 1);
        REQUIRE(ref->get_value() == 69);
    };

    auto move_fn = [](Dynamo::Ref<Dummy> &&ref) {
        REQUIRE(!ref.empty());
        REQUIRE(ref.ref_count() == 1);
        REQUIRE(ref->get_value() == 69);
    };

    Dynamo::Ref<Dummy> a = Dynamo::Ref<Dummy>::create(69, &deletes);

    // Pass as copy
    copy_fn(a);

    // Pass as a const reference
    const_ref_fn(a);

    REQUIRE(!a.empty());
    REQUIRE(a.ref_count() == 1);
    REQUIRE(a->get_value() == 69);

    // Pass as an r-value, should still copy
    move_fn(std::move(a));

    REQUIRE(!a.empty());
    REQUIRE(a.ref_count() == 1);
    REQUIRE(a->get_value() == 69);

    REQUIRE(deletes == 0);
}
