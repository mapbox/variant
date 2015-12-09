#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "variant.hpp"
#include "variant_io.hpp"

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <limits>
#include <memory>
#include <ostream>
#include <sstream>
#include <string>

template <typename T>
struct mutating_visitor
{
    mutating_visitor(T & val)
        : val_(val) {}

    void operator()(T & val) const
    {
        val = val_;
    }

    template <typename T1>
    void operator()(T1 &) const {} // no-op

    T & val_;
};


TEST_CASE( "variant can be moved into vector", "[variant]" ) {
    using variant_type = mapbox::util::variant<bool,std::string>;
    variant_type v(std::string("test"));
    std::vector<variant_type> vec;
    vec.emplace_back(std::move(v));
    REQUIRE(v.get<std::string>() != std::string("test"));
    REQUIRE(vec.at(0).get<std::string>() == std::string("test"));
}

TEST_CASE( "variant should support built-in types", "[variant]" ) {
    SECTION( "bool" ) {
        mapbox::util::variant<bool> v(true);
        REQUIRE(v.valid());
        REQUIRE(v.is<bool>());
        REQUIRE(v.get_type_index() == 0);
        REQUIRE(v.get<bool>() == true);
        v.set<bool>(false);
        REQUIRE(v.get<bool>() == false);
        v = true;
        REQUIRE(v == mapbox::util::variant<bool>(true));
    }
    SECTION( "nullptr" ) {
        using value_type = std::nullptr_t;
        mapbox::util::variant<value_type> v(nullptr);
        REQUIRE(v.valid());
        REQUIRE(v.is<value_type>());
        REQUIRE(v.get_type_index() == 0);
        // TODO: commented since it breaks on windows: 'operator << is ambiguous'
        //REQUIRE(v.get<value_type>() == nullptr);
        // FIXME: does not compile: ./variant.hpp:340:14: error: use of overloaded operator '<<' is ambiguous (with operand types 'std::__1::basic_ostream<char>' and 'const nullptr_t')
        // https://github.com/mapbox/variant/issues/14
        //REQUIRE(v == mapbox::util::variant<value_type>(nullptr));
    }
    SECTION( "unique_ptr" ) {
        using value_type = std::unique_ptr<std::string>;
        mapbox::util::variant<value_type> v(value_type(new std::string("hello")));
        REQUIRE(v.valid());
        REQUIRE(v.is<value_type>());
        REQUIRE(v.get_type_index() == 0);
        REQUIRE(*v.get<value_type>().get() == *value_type(new std::string("hello")).get());
        REQUIRE(*v.get<value_type>() == "hello");
    }
    SECTION( "string" ) {
        using value_type = std::string;
        mapbox::util::variant<value_type> v(value_type("hello"));
        REQUIRE(v.valid());
        REQUIRE(v.is<value_type>());
        REQUIRE(v.get_type_index() == 0);
        REQUIRE(v.get<value_type>() == value_type("hello"));
        v.set<value_type>(value_type("there"));
        REQUIRE(v.get<value_type>() == value_type("there"));
        v = value_type("variant");
        REQUIRE(v == mapbox::util::variant<value_type>(value_type("variant")));
    }
    SECTION( "size_t" ) {
        using value_type = std::size_t;
        mapbox::util::variant<value_type> v(std::numeric_limits<value_type>::max());
        REQUIRE(v.valid());
        REQUIRE(v.is<value_type>());
        REQUIRE(v.get_type_index() == 0);
        REQUIRE(v.get<value_type>() == std::numeric_limits<value_type>::max());
        v.set<value_type>(value_type(0));
        REQUIRE(v.get<value_type>() == value_type(0));
        v = value_type(1);
        REQUIRE(v == mapbox::util::variant<value_type>(value_type(1)));
    }
    SECTION( "int8_t" ) {
        using value_type = std::int8_t;
        mapbox::util::variant<value_type> v(std::numeric_limits<value_type>::max());
        REQUIRE(v.valid());
        REQUIRE(v.is<value_type>());
        REQUIRE(v.get_type_index() == 0);
        REQUIRE(v.get<value_type>() == std::numeric_limits<value_type>::max());
        v.set<value_type>(0);
        REQUIRE(v.get<value_type>() == value_type(0));
        v = value_type(1);
        REQUIRE(v == mapbox::util::variant<value_type>(value_type(1)));
    }
    SECTION( "int16_t" ) {
        using value_type = std::int16_t;
        mapbox::util::variant<value_type> v(std::numeric_limits<value_type>::max());
        REQUIRE(v.valid());
        REQUIRE(v.is<value_type>());
        REQUIRE(v.get_type_index() == 0);
        REQUIRE(v.get<value_type>() == std::numeric_limits<value_type>::max());
        v.set<value_type>(0);
        REQUIRE(v.get<value_type>() == value_type(0));
        v = value_type(1);
        REQUIRE(v == mapbox::util::variant<value_type>(value_type(1)));
    }
    SECTION( "int32_t" ) {
        using value_type = std::int32_t;
        mapbox::util::variant<value_type> v(std::numeric_limits<value_type>::max());
        REQUIRE(v.valid());
        REQUIRE(v.is<value_type>());
        REQUIRE(v.get_type_index() == 0);
        REQUIRE(v.get<value_type>() == std::numeric_limits<value_type>::max());
        v.set<value_type>(0);
        REQUIRE(v.get<value_type>() == value_type(0));
        v = value_type(1);
        REQUIRE(v == mapbox::util::variant<value_type>(value_type(1)));
    }
    SECTION( "int64_t" ) {
        using value_type = std::int64_t;
        mapbox::util::variant<value_type> v(std::numeric_limits<value_type>::max());
        REQUIRE(v.valid());
        REQUIRE(v.is<value_type>());
        REQUIRE(v.get_type_index() == 0);
        REQUIRE(v.get<value_type>() == std::numeric_limits<value_type>::max());
        v.set<value_type>(0);
        REQUIRE(v.get<value_type>() == value_type(0));
        v = value_type(1);
        REQUIRE(v == mapbox::util::variant<value_type>(value_type(1)));
    }
}

struct MissionInteger
{
    using value_type = uint64_t;
    value_type val_;
    public:
      MissionInteger(uint64_t val) :
       val_(val) {}

    bool operator==(MissionInteger const& rhs) const
    {
        return (val_ == rhs.get());
    }

    uint64_t get() const
    {
        return val_;
    }
};

std::ostream& operator<<(std::ostream & os, MissionInteger const& rhs)
{
    os << rhs.get();
    return os;
}

TEST_CASE( "variant should support custom types", "[variant]" ) {
    // http://www.missionintegers.com/integer/34838300
    mapbox::util::variant<MissionInteger> v(MissionInteger(34838300));
    REQUIRE(v.valid());
    REQUIRE(v.is<MissionInteger>());
    REQUIRE(v.get_type_index() == 0);
    REQUIRE(v.get<MissionInteger>() == MissionInteger(34838300));
    REQUIRE(v.get<MissionInteger>().get() == MissionInteger::value_type(34838300));
    // TODO: should both of the set usages below compile?
    v.set<MissionInteger>(MissionInteger::value_type(0));
    v.set<MissionInteger>(MissionInteger(0));
    REQUIRE(v.get<MissionInteger>().get() == MissionInteger::value_type(0));
    v = MissionInteger(1);
    REQUIRE(v == mapbox::util::variant<MissionInteger>(MissionInteger(1)));
}

TEST_CASE( "variant should correctly index types", "[variant]" ) {
    using variant_type = mapbox::util::variant<bool,std::string,std::uint64_t,std::int64_t,double,float>;
    // Index is in reverse order
    REQUIRE(5 == variant_type(true).get_type_index());
    REQUIRE(4 == variant_type(std::string("test")).get_type_index());
    REQUIRE(3 == variant_type(std::uint64_t(0)).get_type_index());
    REQUIRE(2 == variant_type(std::int64_t(0)).get_type_index());
    REQUIRE(1 == variant_type(double(0.0)).get_type_index());
    REQUIRE(0 == variant_type(float(0.0)).get_type_index());
}

TEST_CASE( "variant::which() returns zero based index of stored type", "[variant]" ) {
    using variant_type = mapbox::util::variant<bool,std::string,std::uint64_t,std::int64_t,double,float>;
    // Index is in forward order
    REQUIRE(0 == variant_type(true).which());
    REQUIRE(1 == variant_type(std::string("test")).which());
    REQUIRE(2 == variant_type(std::uint64_t(0)).which());
    REQUIRE(3 == variant_type(std::int64_t(0)).which());
    REQUIRE(4 == variant_type(double(0.0)).which());
    REQUIRE(5 == variant_type(float(0.0)).which());
}

TEST_CASE( "get with wrong type (here: double) should throw", "[variant]" ) {
    using variant_type = mapbox::util::variant<int, double>;
    variant_type var = 5;
    REQUIRE(var.get<int>() == 5);
    REQUIRE_THROWS(var.get<double>());
}

TEST_CASE( "get with wrong type (here: int) should throw", "[variant]" ) {
    using variant_type = mapbox::util::variant<int, double>;
    variant_type var = 5.0;
    REQUIRE(var.get<double>() == 5.0);
    REQUIRE_THROWS(var.get<int>());
}

TEST_CASE( "implicit conversion", "[variant][implicit conversion]" ) {
    using variant_type = mapbox::util::variant<int>;
    variant_type var(5.0); // converted to int
    REQUIRE(var.get<int>() == 5);
    var = 6.0; // works for operator=, too
    REQUIRE(var.get<int>() == 6);
}

TEST_CASE( "implicit conversion to first type in variant type list", "[variant][implicit conversion]" ) {
    using variant_type = mapbox::util::variant<long, char>;
    variant_type var = 5.0; // converted to long
    REQUIRE(var.get<long>() == 5);
    REQUIRE_THROWS(var.get<char>());
}

TEST_CASE( "implicit conversion to unsigned char", "[variant][implicit conversion]" ) {
    using variant_type = mapbox::util::variant<unsigned char>;
    variant_type var = 100.0;
    CHECK(var.get<unsigned char>() == static_cast<unsigned char>(100.0));
    CHECK(var.get<unsigned char>() == static_cast<unsigned char>(static_cast<unsigned int>(100.0)));
}

struct dummy {};

TEST_CASE( "variant value traits", "[variant::detail]" ) {
    // Users should not create variants with duplicated types
    // however our type indexing should still work
    // Index is in reverse order
    REQUIRE((mapbox::util::detail::value_traits<bool, bool, int, double, std::string>::index == 3));
    REQUIRE((mapbox::util::detail::value_traits<int, bool, int, double, std::string>::index == 2));
    REQUIRE((mapbox::util::detail::value_traits<double, bool, int, double, std::string>::index == 1));
    REQUIRE((mapbox::util::detail::value_traits<std::string, bool, int, double, std::string>::index == 0));
    REQUIRE((mapbox::util::detail::value_traits<dummy, bool, int, double, std::string>::index == mapbox::util::detail::invalid_value));
    REQUIRE((mapbox::util::detail::value_traits<std::vector<int>, bool, int, double, std::string>::index == mapbox::util::detail::invalid_value));
}

TEST_CASE( "variant default constructor", "[variant][default constructor]" ) {
    // By default variant is initialised with (default constructed) first type in template parameters pack
    // As a result first type in Types... must be default constructable
    // NOTE: index in reverse order -> index = N - 1
    using variant_type = mapbox::util::variant<int, double, std::string>;
    REQUIRE(variant_type{}.get_type_index() == 2);
    REQUIRE(variant_type{}.valid());
    REQUIRE(variant_type{mapbox::util::no_init()}.get_type_index() == mapbox::util::detail::invalid_value);
    REQUIRE_FALSE(variant_type{mapbox::util::no_init()}.valid());
}

TEST_CASE( "variant visitation", "[visitor][unary visitor]" ) {
    mapbox::util::variant<int, double, std::string> var(123);
    REQUIRE(var.get<int>() == 123);
    int val = 456;
    mutating_visitor<int> visitor(val);
    mapbox::util::apply_visitor(visitor, var);
    REQUIRE(var.get<int>() == 456);
}

TEST_CASE( "variant printer", "[visitor][unary visitor][printer]" ) {
    using variant_type = mapbox::util::variant<int, double, std::string>;
    std::vector<variant_type> var = {2.1, 123, "foo", 456};
    std::stringstream out;
    std::copy(var.begin(), var.end(), std::ostream_iterator<variant_type>(out, ","));
    out << var[2];
    REQUIRE(out.str() == "2.1,123,foo,456,foo");
}

TEST_CASE( "swapping variants should do the right thing", "[variant]" ) {
    using variant_type = mapbox::util::variant<int, double, std::string>;
    variant_type a = 7;
    variant_type b = 3;
    variant_type c = 3.141;
    variant_type d = "foo";
    variant_type e = "a long string that is longer than small string optimization";

    using std::swap;
    swap(a, b);
    REQUIRE(a.get<int>() == 3);
    REQUIRE(a.which() == 0);
    REQUIRE(b.get<int>() == 7);
    REQUIRE(b.which() == 0);

    swap(b, c);
    REQUIRE(b.get<double>() == Approx(3.141));
    REQUIRE(b.which() == 1);
    REQUIRE(c.get<int>() == 7);
    REQUIRE(c.which() == 0);

    swap(b, d);
    REQUIRE(b.get<std::string>() == "foo");
    REQUIRE(b.which() == 2);
    REQUIRE(d.get<double>() == Approx(3.141));
    REQUIRE(d.which() == 1);

    swap(b, e);
    REQUIRE(b.get<std::string>() == "a long string that is longer than small string optimization");
    REQUIRE(b.which() == 2);
    REQUIRE(e.get<std::string>() == "foo");
    REQUIRE(e.which() == 2);
}

