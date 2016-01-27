
#include "catch.hpp"

#include "variant.hpp"
#include "variant_io.hpp"

#include <string>

struct some_visitor
{
    int var_;

    some_visitor(int init)
        : var_(init) {}

    int operator()(int val) {
        return var_ + val;
    }

    int operator()(double val) {
        return var_ + int(val);
    }

    template <typename T1>
    int operator()(T1 &) const {
        return 0;
    }

};

TEST_CASE( "simple visitor works on const variants", "[visitor][unary visitor]" ) {
    using variant_type = const mapbox::util::variant<int, double, std::string>;
    variant_type var1(123);
    variant_type var2(3.2);
    variant_type var3("foo");
    REQUIRE(var1.get<int>() == 123);
    REQUIRE(var2.get<double>() == Approx(3.2));
    REQUIRE(var3.get<std::string>() == "foo");

    some_visitor visitor(1);

    REQUIRE(mapbox::util::apply_visitor(visitor, var1) == 124);
    REQUIRE(mapbox::util::apply_visitor(visitor, var2) == 4);
    REQUIRE(mapbox::util::apply_visitor(visitor, var3) == 0);
}

struct total_sizeof
{
   total_sizeof() : total_(0) { }

   template<class Value>
   int operator()(const Value&) const
   {
      total_ += int(sizeof(Value));
      return total_;
   }

   int result() const
   {
      return total_;
   }

   mutable int total_;

}; // total_sizeof

TEST_CASE( "changes in visitor should be visible", "[visitor][unary visitor]" ) {
    typedef mapbox::util::variant< int, std::string, double > t_var1;
    t_var1 v1;
    total_sizeof ts;

    v1 = 5.9;
    REQUIRE(mapbox::util::apply_visitor(ts, v1) == sizeof(double));
    REQUIRE(ts.result() == sizeof(double));
}

