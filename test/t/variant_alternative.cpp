#include "catch.hpp"

#include <mapbox/variant.hpp>
#include <mapbox/variant_io.hpp>

#include <string>

TEST_CASE("variant alternative", "[types]")
{
    using variant_type =  mapbox::util::variant<int, double, std::string>;
    using type_0 = mapbox::util::variant_alternative<0, variant_type>::type;
    using type_1 = mapbox::util::variant_alternative<1, variant_type>::type;
    using type_2 = mapbox::util::variant_alternative<2, variant_type>::type;

    constexpr bool check_0 = std::is_same<int, type_0>::value;
    constexpr bool check_1 = std::is_same<double, type_1>::value;
    constexpr bool check_2 = std::is_same<std::string, type_2>::value;

    CHECK(check_0);
    CHECK(check_1);
    CHECK(check_2);
}
