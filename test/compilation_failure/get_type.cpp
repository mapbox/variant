// @EXPECTED: no matching member function for call to 'get'

#include <mapbox/variant.hpp>

int main()
{
    mapbox::util::variant<int, double> x;
    x.get<std::string>();
}
