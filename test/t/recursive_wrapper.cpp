
#include "catch.hpp"

#include "recursive_wrapper.hpp"

#include <utility>

using rwi = mapbox::util::recursive_wrapper<int>;
using rwp = mapbox::util::recursive_wrapper<std::pair<int, int>>;

TEST_CASE("recursive wrapper of int") {

    // this fails, POD types are not initialized. is this okay?
/*    SECTION("default constructed") {
        rwi a;
        REQUIRE(a.get() == 0);
    }*/

    SECTION("construct with value") {
        rwi a{7};
        REQUIRE(a.get() == 7);
        REQUIRE(*a.get_pointer() == 7);

        a = 8;
        REQUIRE(a.get() == 8);

        rwi b{a};
        REQUIRE(b.get() == 8);

        rwi c;
        c = b;
        REQUIRE(b.get() == 8);
        REQUIRE(c.get() == 8);

        c = 9;
        REQUIRE(c.get() == 9);

        b = std::move(c);
        REQUIRE(b.get() == 9);
    }

    SECTION("implicit conversion to reference of underlying type") {

        SECTION("const") {
            rwi const a{7};
            REQUIRE(a.get() == 7);
            REQUIRE(*a.get_pointer() == 7);

            rwi::type const& underlying = a;
            REQUIRE(underlying == 7);
        }

        SECTION("non const") {
            rwi a{7};
            REQUIRE(a.get() == 7);
            REQUIRE(*a.get_pointer() == 7);

            rwi::type& underlying = a;
            REQUIRE(underlying == 7);
            a = 8;
            REQUIRE(underlying == 8);
        }

    }

}

// XXX !!! handling of move is inconsistent
TEST_CASE("move of recursive wrapper") {
    rwi a{1};

    SECTION("null out operand in move constructor") {
        rwi b{std::move(a)};
        REQUIRE(b.get() == 1);
        REQUIRE(a.get_pointer() == nullptr);
    }

    SECTION("swap with operand in operator=") {
        rwi b{2};
        b = std::move(a);
        REQUIRE(b.get() == 1);
        REQUIRE(a.get() == 2);
    }

}

TEST_CASE("recursive wrapper of pair<int, int>") {

    SECTION("default constructed") {
        rwp a;
        REQUIRE(a.get().first == 0);
        REQUIRE(a.get().second == 0);
    }

    SECTION("construct with value") {
        rwp a{std::make_pair(1, 2)};

        REQUIRE(a.get().first == 1);
        REQUIRE(a.get().second == 2);

        REQUIRE(a.get_pointer()->first == 1);
        REQUIRE(a.get_pointer()->second == 2);

        a = {3, 4};
        REQUIRE(a.get().first == 3);
        REQUIRE(a.get().second == 4);

        rwp b{a};
        REQUIRE(b.get().first == 3);
        REQUIRE(b.get().second == 4);

        rwp c;
        c = b;
        REQUIRE(b.get().first == 3);
        REQUIRE(b.get().second == 4);
        REQUIRE(c.get().first == 3);
        REQUIRE(c.get().second == 4);

        c = {5, 6};
        REQUIRE(c.get().first == 5);
        REQUIRE(c.get().second == 6);

        b = std::move(c);
        REQUIRE(b.get().first == 5);
        REQUIRE(b.get().second == 6);
//        REQUIRE(c.get_pointer() == nullptr);

    }

}

