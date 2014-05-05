#include <iostream>
#include <vector>
#include <thread>
#include <string>
#include <utility>
#include <boost/variant.hpp>
#include <boost/timer/timer.hpp>
#include "variant.hpp"

namespace test {

struct javascript_equal_visitor
{
    template <typename T>
    bool operator() (T lhs, T rhs) const
    {
        return lhs == rhs;
    }

    bool operator() (int lhs, std::string const& rhs) const
    {
        return lhs == std::stol(rhs);
    }

    bool operator() (std::string const& lhs, int rhs) const
    {
        return std::stol(lhs) == rhs;
    }
};

template <typename T>
struct javascript_equal
{
    javascript_equal(T const& lhs)
        : lhs(lhs) {}

    bool operator() (T const& rhs) const
    {
        return util::apply_visitor(lhs, rhs, test::javascript_equal_visitor());
    }
    T const& lhs;
};

}

int main (int argc, char** argv)
{
    typedef util::variant<int, std::string> variant_type;
    variant_type v0(123);
    variant_type v1(std::string("123"));

    std::cerr << v0 << " == " << v1 << " -> "
              << std::boolalpha << util::apply_visitor(v0, v1, test::javascript_equal_visitor()) << std::endl;


    std::vector<variant_type> vec;

    vec.emplace_back(1);
    vec.push_back(variant_type(2));
    vec.push_back(variant_type(3));
    vec.push_back(std::string("123"));

    //auto itr = std::find_if(vec.begin(), vec.end(), [&v0](variant_type const& val) {
    //        return util::apply_visitor(v0, val, test::javascript_equal_visitor());
    //    });

    auto itr = std::find_if(vec.begin(), vec.end(),test::javascript_equal<variant_type>(v0));

    if (itr != std::end(vec))
    {
        std::cout << "found " << *itr << std::endl;
    }
    else
    {
        std::cout << "can't find " << v0 << '\n';
    }

    return EXIT_SUCCESS;
}
