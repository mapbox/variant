#include <iostream>
#include <vector>
#include <thread>
#include <string>
#include <sstream>
#include <utility>
#include <type_traits>
#include <boost/variant.hpp>
#include <boost/timer/timer.hpp>
#include "variant.hpp"

namespace test {

struct add;
struct sub;
template <typename OpTag> struct binary_op;

typedef util::variant<int ,
                      std::unique_ptr<binary_op<add>>,
                      std::unique_ptr<binary_op<sub>>
                      > expression;

template <typename Op>
struct binary_op
{
    expression left;  // variant instantiated here...
    expression right;

    binary_op( expression && lhs, expression && rhs )
        : left(std::move(lhs)), right(std::move(rhs)) {}
};

struct print : util::static_visitor<void>
{
    template <typename T>
    void operator() (T const& val) const
    {
        std::cerr << val << ":" << typeid(T).name() <<  std::endl;
    }
};


struct test : util::static_visitor<std::string>
{
    template <typename T>
    std::string operator() (T const& obj) const
    {
        return std::string("TYPE_ID=") + typeid(obj).name();
    }
};

struct calculator : public util::static_visitor<int>
{
public:

    int operator()(int value) const
    {
        return value;
    }

    int operator()(std::unique_ptr<binary_op<add>> const& binary) const
    {
        return util::apply_visitor( binary->left, calculator())
            + util::apply_visitor( binary->right, calculator());
    }

    int operator()(std::unique_ptr<binary_op<sub>> const& binary) const
    {
        return util::apply_visitor( binary->left, calculator())
            - util::apply_visitor( binary->right, calculator());
    }
};

struct to_string : public util::static_visitor<std::string>
{
public:

    std::string operator()(int value) const
    {
        return std::to_string(value);
    }

    std::string operator()(std::unique_ptr<binary_op<add>> const& binary) const
    {
        return util::apply_visitor( binary->left, to_string()) + std::string("+")
            + util::apply_visitor( binary->right, to_string());
    }

    std::string operator()(std::unique_ptr<binary_op<sub>> const& binary) const
    {
        return util::apply_visitor( binary->left, to_string() ) + std::string("-")
            + util::apply_visitor( binary->right, to_string() );
    }

};

}

int main (int argc, char** argv)
{
    test::expression sum(std::unique_ptr<test::binary_op<test::add>>(new test::binary_op<test::add>(2,3)));
    test::expression result(std::unique_ptr<test::binary_op<test::sub>>(new test::binary_op<test::sub>(std::move(sum),10)));
    std::cerr << "TYPE OF RESULT-> " << util::apply_visitor(result, test::test()) << std::endl;
    for (int i=0; i<10;++i)
    {
        std::cerr << util::apply_visitor(result, test::to_string()) << "=" << util::apply_visitor(result, test::calculator()) << std::endl;
    }
    return EXIT_SUCCESS;
}
