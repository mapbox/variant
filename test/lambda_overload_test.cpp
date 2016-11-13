#include <iostream>

#include <mapbox/variant.hpp>
#include <mapbox/variant_visitor.hpp>

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

int main()
{
    test_lambda_overloads();
    test_singleton_variant();
    test_lambda_overloads_capture();
}
