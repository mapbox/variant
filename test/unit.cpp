#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

#include "variant.hpp"
#include <cstdint>
#include <limits>
#include <string>
#include <ostream>
#include <memory>

using namespace mapbox;

TEST_CASE( "variant version", "[variant]" ) {
    unsigned int version = VARIANT_VERSION;
    REQUIRE(version == 100);
    #if VARIANT_VERSION == 100
        REQUIRE(true);
    #else
        REQUIRE(false);
    #endif
}

TEST_CASE( "variant can be moved into vector", "[variant]" ) {
    typedef util::variant<bool,std::string> variant_type;
    variant_type v(std::string("test"));
    std::vector<variant_type> vec;
    vec.emplace_back(std::move(v));
    REQUIRE(v.get<std::string>() != std::string("test"));
    REQUIRE(vec.at(0).get<std::string>() == std::string("test"));
}

TEST_CASE( "variant should support built-in types", "[variant]" ) {
    SECTION( "bool" ) {
        util::variant<bool> v(true);
        REQUIRE(v.valid());
        REQUIRE(v.is<bool>());
        REQUIRE(v.get_type_index() == 0);
        REQUIRE(v.get<bool>() == true);
        v.set<bool>(false);
        REQUIRE(v.get<bool>() == false);
        v = true;
        REQUIRE(v == util::variant<bool>(true));
    }
    SECTION( "nullptr" ) {
        typedef std::nullptr_t value_type;
        util::variant<value_type> v(nullptr);
        REQUIRE(v.valid());
        REQUIRE(v.is<value_type>());
        REQUIRE(v.get_type_index() == 0);
        // TODO: commented since it breaks on windows: 'operator << is ambiguous'
        //REQUIRE(v.get<value_type>() == nullptr);
        // FIXME: does not compile: ./variant.hpp:340:14: error: use of overloaded operator '<<' is ambiguous (with operand types 'std::__1::basic_ostream<char>' and 'const nullptr_t')
        // https://github.com/mapbox/variant/issues/14
        //REQUIRE(v == util::variant<value_type>(nullptr));
    }
    SECTION( "unique_ptr" ) {
        typedef std::unique_ptr<std::string> value_type;
        util::variant<value_type> v(value_type(new std::string("hello")));
        REQUIRE(v.valid());
        REQUIRE(v.is<value_type>());
        REQUIRE(v.get_type_index() == 0);
        REQUIRE(*v.get<value_type>().get() == *value_type(new std::string("hello")).get());
    }
    SECTION( "string" ) {
        typedef std::string value_type;
        util::variant<value_type> v(value_type("hello"));
        REQUIRE(v.valid());
        REQUIRE(v.is<value_type>());
        REQUIRE(v.get_type_index() == 0);
        REQUIRE(v.get<value_type>() == value_type("hello"));
        v.set<value_type>(value_type("there"));
        REQUIRE(v.get<value_type>() == value_type("there"));
        v = value_type("variant");
        REQUIRE(v == util::variant<value_type>(value_type("variant")));
    }
    SECTION( "size_t" ) {
        typedef std::size_t value_type;
        util::variant<value_type> v(std::numeric_limits<value_type>::max());
        REQUIRE(v.valid());
        REQUIRE(v.is<value_type>());
        REQUIRE(v.get_type_index() == 0);
        REQUIRE(v.get<value_type>() == std::numeric_limits<value_type>::max());
        v.set<value_type>(value_type(0));
        REQUIRE(v.get<value_type>() == value_type(0));
        v = value_type(1);
        REQUIRE(v == util::variant<value_type>(value_type(1)));
    }
    SECTION( "int8_t" ) {
        typedef std::int8_t value_type;
        util::variant<value_type> v(std::numeric_limits<value_type>::max());
        REQUIRE(v.valid());
        REQUIRE(v.is<value_type>());
        REQUIRE(v.get_type_index() == 0);
        REQUIRE(v.get<value_type>() == std::numeric_limits<value_type>::max());
        v.set<value_type>(0);
        REQUIRE(v.get<value_type>() == value_type(0));
        v = value_type(1);
        REQUIRE(v == util::variant<value_type>(value_type(1)));
    }
    SECTION( "int16_t" ) {
        typedef std::int16_t value_type;
        util::variant<value_type> v(std::numeric_limits<value_type>::max());
        REQUIRE(v.valid());
        REQUIRE(v.is<value_type>());
        REQUIRE(v.get_type_index() == 0);
        REQUIRE(v.get<value_type>() == std::numeric_limits<value_type>::max());
        v.set<value_type>(0);
        REQUIRE(v.get<value_type>() == value_type(0));
        v = value_type(1);
        REQUIRE(v == util::variant<value_type>(value_type(1)));
    }
    SECTION( "int32_t" ) {
        typedef std::int32_t value_type;
        util::variant<value_type> v(std::numeric_limits<value_type>::max());
        REQUIRE(v.valid());
        REQUIRE(v.is<value_type>());
        REQUIRE(v.get_type_index() == 0);
        REQUIRE(v.get<value_type>() == std::numeric_limits<value_type>::max());
        v.set<value_type>(0);
        REQUIRE(v.get<value_type>() == value_type(0));
        v = value_type(1);
        REQUIRE(v == util::variant<value_type>(value_type(1)));
    }
    SECTION( "int64_t" ) {
        typedef std::int64_t value_type;
        util::variant<value_type> v(std::numeric_limits<value_type>::max());
        REQUIRE(v.valid());
        REQUIRE(v.is<value_type>());
        REQUIRE(v.get_type_index() == 0);
        REQUIRE(v.get<value_type>() == std::numeric_limits<value_type>::max());
        v.set<value_type>(0);
        REQUIRE(v.get<value_type>() == value_type(0));
        v = value_type(1);
        REQUIRE(v == util::variant<value_type>(value_type(1)));
    }
}

struct MissionInteger
{
    typedef uint64_t value_type;
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

// TODO - remove after https://github.com/mapbox/variant/issues/14
std::ostream& operator<< (std::ostream& os, MissionInteger const& rhs)
{
    os << rhs.get();
    return os;
}

TEST_CASE( "variant should support custom types", "[variant]" ) {
    // http://www.missionintegers.com/integer/34838300
    util::variant<MissionInteger> v(MissionInteger(34838300));
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
    REQUIRE(v == util::variant<MissionInteger>(MissionInteger(1)));
}

// Test internal api
TEST_CASE( "variant should correctly index types", "[variant]" ) {
    typedef util::variant<bool,std::string,std::uint64_t,std::int64_t,double,float> variant_type;
    // Index is in reverse order
    REQUIRE(variant_type(true).get_type_index() == 5);
    REQUIRE(variant_type(std::string("test")).get_type_index() == 4);
    REQUIRE(variant_type(std::uint64_t(0)).get_type_index() == 3);
    REQUIRE(variant_type(std::int64_t(0)).get_type_index() == 2);
    REQUIRE(variant_type(double(0.0)).get_type_index() == 1);
    REQUIRE(variant_type(float(0.0)).get_type_index() == 0);
}

TEST_CASE( "variant type traits", "[variant::detail]" ) {
    // Users should not create variants with duplicated types
    // however our type indexing should still work
    // Index is in reverse order
    REQUIRE((util::detail::type_traits<bool, bool, int, double, std::string>::id == 3));
    REQUIRE((util::detail::type_traits<int, bool, int, double, std::string>::id == 2));
    REQUIRE((util::detail::type_traits<double, bool, int, double, std::string>::id == 1));
    REQUIRE((util::detail::type_traits<std::string, bool, int, double, std::string>::id == 0));
    REQUIRE((util::detail::type_traits<long, bool, int, double, std::string>::id == util::detail::invalid_value));
    REQUIRE((util::detail::type_traits<std::vector<int>, bool, int, double, std::string>::id == util::detail::invalid_value));
}

TEST_CASE( "variant default constructor", "variant()" ) {
    // By default variant is initilised with (default constructed) first type in template parameters pack
    // As a reusult first type in Types... must be defaul constructable
    // NOTE: index in reverse order -> index = N - 1
    REQUIRE((util::variant<int, double, std::string>().get_type_index() == 2));
}

int main (int argc, char* const argv[])
{
    int result = Catch::Session().run( argc, argv );
    if (!result) printf("\x1b[1;32m ✓ \x1b[0m\n");
    return result;
}
