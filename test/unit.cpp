#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

#include "variant.hpp"
#include <cstdint>
#include <limits>
#include <string>
#include <ostream>
#include <memory>


TEST_CASE( "variant should support built-in types", "[variant]" ) {
    SECTION( "bool" ) {
        util::variant<bool> v(true);
        REQUIRE(v.valid());
        REQUIRE(v.is<bool>());
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
        REQUIRE(v.get<value_type>() == nullptr);
        // FIXME: does not compile: ./variant.hpp:340:14: error: use of overloaded operator '<<' is ambiguous (with operand types 'std::__1::basic_ostream<char>' and 'const nullptr_t')
        // https://github.com/mapbox/variant/issues/14
        //REQUIRE(v == util::variant<value_type>(nullptr));
    }    
    SECTION( "unique_ptr" ) {
        typedef std::unique_ptr<std::string> value_type;
        util::variant<value_type> v(value_type(new std::string("hello")));
        REQUIRE(v.valid());
        REQUIRE(v.is<value_type>());
        REQUIRE(*v.get<value_type>().get() == *value_type(new std::string("hello")).get());
    }
    SECTION( "string" ) {
        typedef std::string value_type;
        util::variant<value_type> v(value_type("hello"));
        REQUIRE(v.valid());
        REQUIRE(v.is<value_type>());
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
    REQUIRE(v.get<MissionInteger>() == MissionInteger(34838300));
    REQUIRE(v.get<MissionInteger>().get() == MissionInteger::value_type(34838300));
    // TODO: should both of the set usages below compile?
    v.set<MissionInteger>(MissionInteger::value_type(0));
    v.set<MissionInteger>(MissionInteger(0));
    REQUIRE(v.get<MissionInteger>().get() == MissionInteger::value_type(0));
    v = MissionInteger(1);
    REQUIRE(v == util::variant<MissionInteger>(MissionInteger(1)));
}

int main (int argc, char* const argv[])
{
    int result = Catch::Session().run( argc, argv );
    if (!result) printf("\x1b[1;32m ✓ \x1b[0m\n");
    return result;
}
