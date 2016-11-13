#ifndef MAPBOX_UTIL_VARIANT_VISITOR_HPP
#define MAPBOX_UTIL_VARIANT_VISITOR_HPP

namespace mapbox {
namespace util {

template <typename... Fns>
struct visitor;

template <typename Fn>
struct visitor<Fn> : Fn
{
    using type = Fn;

    using Fn::operator();

    visitor(Fn fn) : Fn(fn) {}
};

template <typename Fn, typename... Fns>
struct visitor<Fn, Fns...> : Fn, visitor<Fns...>::type
{
    using type = visitor;

    using Fn::operator();
    using visitor<Fns...>::type::operator();

    visitor(Fn fn, Fns... fns) : Fn(fn), visitor<Fns...>::type(fns...) {}
};

template <typename... Fns>
typename visitor<Fns...>::type make_visitor(Fns... fns)
{
    return visitor<Fns...>(fns...);
}

} // namespace util
} // namespace mapbox

#endif // MAPBOX_UTIL_VARIANT_VISITOR_HPP
