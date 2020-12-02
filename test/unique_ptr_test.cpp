
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <typeinfo>
#include <utility>

#include "auto_cpu_timer.hpp"

#include <mapbox/variant.hpp>

using namespace mapbox;

namespace test {

struct add;
struct sub;

template <typename OpTag>
struct binary_op;

typedef util::variant<int,
                      std::unique_ptr<binary_op<add>>,
                      std::unique_ptr<binary_op<sub>>>
    expression;

template <typename Op>
struct binary_op
{
    expression left; // variant instantiated here...
    expression right;

    binary_op(expression&& lhs, expression&& rhs)
        : left(std::move(lhs)), right(std::move(rhs))
    {
    }
};

struct print
{
    template <typename T>
    void operator()(T const& val) const
    {
        std::cerr << val << ":" << typeid(T).name() << std::endl;
    }
};

struct test
{
    template <typename T>
    std::string operator()(T const& obj) const
    {
        return std::string("TYPE_ID=") + typeid(obj).name();
    }
};

struct calculator
{
  public:
    int operator()(int value) const
    {
        return value;
    }

    int operator()(std::unique_ptr<binary_op<add>> const& binary) const
    {
        return util::apply_visitor(calculator(), binary->left) + util::apply_visitor(calculator(), binary->right);
    }

    int operator()(std::unique_ptr<binary_op<sub>> const& binary) const
    {
        return util::apply_visitor(calculator(), binary->left) - util::apply_visitor(calculator(), binary->right);
    }
};

struct to_string
{
  public:
    std::string operator()(int value) const
    {
        return std::to_string(value);
    }

    std::string operator()(std::unique_ptr<binary_op<add>> const& binary) const
    {
        return util::apply_visitor(to_string(), binary->left) + std::string("+") + util::apply_visitor(to_string(), binary->right);
    }

    std::string operator()(std::unique_ptr<binary_op<sub>> const& binary) const
    {
        return util::apply_visitor(to_string(), binary->left) + std::string("-") + util::apply_visitor(to_string(), binary->right);
    }
};

template <typename Op, typename Node = binary_op<Op>>
std::unique_ptr<Node> make_binary(expression&& lhs, expression&& rhs)
{
    return std::unique_ptr<Node>(new Node(std::move(lhs), std::move(rhs)));
}

void bench_large_expression(std::size_t num)
{
    std::cerr << "----- sum of " << num << " ones -----" << std::endl;
    expression sum = 0;
    {
        std::cerr << "construction ";
        auto_cpu_timer t;
        for (std::size_t i = 0; i < num; ++i)
        {
            sum = make_binary<add>(std::move(sum), 1);
        }
    }
    int total = 0;
    {
        std::cerr << "calculation ";
        auto_cpu_timer t;
        for (std::size_t i = 0; i < num; ++i)
        {
            total += util::apply_visitor(calculator(), sum);
        }
    }
    std::cerr << "total=" << total << std::endl;
}

} // namespace test

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage" << argv[0] << " <num-iter>" << std::endl;
        return EXIT_FAILURE;
    }

    const std::size_t NUM_ITER = static_cast<std::size_t>(std::stol(argv[1]));

    test::expression sum = test::make_binary<test::add>(2, 3);
    test::expression result = test::make_binary<test::sub>(std::move(sum), 4);

    std::cerr << "TYPE OF RESULT-> " << util::apply_visitor(test::test(), result) << std::endl;

    int total = 0;
    {
        auto_cpu_timer t;
        for (std::size_t i = 0; i < NUM_ITER; ++i)
        {
            total += util::apply_visitor(test::calculator(), result);
        }
    }
    std::cerr << "total=" << total << std::endl;

    std::cerr << util::apply_visitor(test::to_string(), result) << "=" << util::apply_visitor(test::calculator(), result) << std::endl;

    test::bench_large_expression(1000);
    test::bench_large_expression(5000);

    return EXIT_SUCCESS;
}
