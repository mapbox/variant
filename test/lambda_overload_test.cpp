#include <iostream>
#include <vector>

#include <mapbox/variant.hpp>
#include <mapbox/variant_visitor.hpp>

#if __cplusplus >= 201402L
#define HAS_CPP14_SUPPORT
#endif

using namespace mapbox::util;

template <typename Left, typename Right>
using Either = mapbox::util::variant<Left, Right>;

struct Response
{
};

struct Error
{
};

void test_lambda_overloads()
{
    Either<Error, Response> rv;

    rv = Response{};

    auto visitor = make_visitor([](Response) { std::cout << "Response\n"; }, //
                                [](Error) { std::cout << "Error\n"; });      //
    apply_visitor(visitor, rv);
}

void test_lambda_overloads_capture()
{
    Either<Error, Response> rv;

    rv = Error{};

    int ok = 0;
    int err = 0;

    auto visitor = make_visitor([&](Response) { ok += 1; }, //
                                [&](Error) { err += 1; });  //
    apply_visitor(visitor, rv);

    std::cout << "Got " << ok << " ok, " << err << " err" << std::endl;
}

void test_singleton_variant()
{

    variant<int> singleton;
    apply_visitor(make_visitor([](int) {}), singleton);
}

#ifdef HAS_CPP14_SUPPORT
void test_lambda_overloads_sfinae()
{
    variant<int, float, std::vector<int>> var;

    auto visitor = make_visitor([](auto range) -> decltype(std::begin(range), void()) {
                                    for (auto each : range)
                                        std::cout << each << ' '; },
                                [](auto x) -> decltype(std::cout << x, void()) {
                                    std::cout << x << std::endl;
                                });

    var = 1;
    apply_visitor(visitor, var);

    var = 2.f;
    apply_visitor(visitor, var);

    var = std::vector<int>{4, 5, 6};
    apply_visitor(visitor, var);
}
#endif

int main()
{
    test_lambda_overloads();
    test_singleton_variant();
    test_lambda_overloads_capture();

#ifdef HAS_CPP14_SUPPORT
    test_lambda_overloads_sfinae();
#endif
}

#undef HAS_CPP14_SUPPORT
