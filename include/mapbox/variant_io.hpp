#ifndef MAPBOX_UTIL_VARIANT_IO_HPP
#define MAPBOX_UTIL_VARIANT_IO_HPP

#include <iosfwd>
#include <type_traits>

#include <mapbox/variant.hpp>

namespace mapbox {
namespace util {

namespace detail {
// operator<< helper
template <typename Out>
class printer
{
public:
    explicit printer(Out& out)
        : out_(out) {}
    printer& operator=(printer const&) = delete;

    // visitor
    template <typename T>
    typename std::enable_if<std::is_empty<T>::value, void>::type operator()(T const&) const
    {
        out_ << "[]";
    }

    // visitor
    template <typename T>
    typename std::enable_if<!std::is_empty<T>::value, void>::type operator()(T const& operand) const
    {
        out_ << operand;
    }

private:
    Out& out_;
};
}

// operator<<
template <typename CharT, typename Traits, typename... Types>
VARIANT_INLINE std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& out, variant<Types...> const& rhs)
{
    detail::printer<std::basic_ostream<CharT, Traits>> visitor(out);
    apply_visitor(visitor, rhs);
    return out;
}
} // namespace util
} // namespace mapbox

#endif // MAPBOX_UTIL_VARIANT_IO_HPP
