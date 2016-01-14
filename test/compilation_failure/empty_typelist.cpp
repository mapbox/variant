
#include <variant.hpp>

// Empty type list should not work.

int main() {
    mapbox::util::variant<> x;
}

