#include "catch.hpp"

#include <mapbox/variant.hpp>
#include <mapbox/variant_io.hpp>

#include <string>
#include <iostream>

TEST_CASE("variant alternative", "[types]")
{
    using variant_type =  mapbox::util::variant<int, double, std::string>;
    using type_0 = mapbox::util::variant_alternative<0, variant_type>::type;
    using type_1 = mapbox::util::variant_alternative<1, variant_type>::type;
    using type_2 = mapbox::util::variant_alternative<2, variant_type>::type;

    std::cerr << typeid(type_0).name() << std::endl;
    std::cerr << typeid(type_1).name() << std::endl;
    std::cerr << typeid(type_2).name() << std::endl;
    constexpr bool check_0 = std::is_same<int, type_0>::value;
    constexpr bool check_1 = std::is_same<double, type_1>::value;
    constexpr bool check_2 = std::is_same<std::string, type_2>::value;
    CHECK(check_0);
}
