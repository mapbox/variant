#ifndef MAPBOX_UTIL_OPTIONAL_HPP
#define MAPBOX_UTIL_OPTIONAL_HPP

#include <type_traits>

#include "variant.hpp"

namespace mapbox { namespace util {

    namespace detail {
        struct none_helper{};
    }
    typedef int detail::none_helper::*none_t ;
    none_t const none = (static_cast<none_t>(0)) ;

    template<typename T>
    class optional {
        static_assert(!std::is_reference<T>::value, "optional doesn't support references");

        variant<none_t, T> variant_;

      public:
        optional() = default;

        explicit optional(optional const& rhs) {
            variant_ = rhs.variant_;
        }

        explicit optional(T const& v) {
            variant_ = v;
        }

        bool operator!() const {
            return variant_.template is<none_t>();
        }

        T const& get() const {
            return variant_.template get<T>();
        }

        T&       get() {
            return variant_.template get<T>();
        }

        T const& operator *() const { return this->get() ; }
        T        operator *()       { return this->get() ; }

        optional& operator = ( T const& v ) {
            variant_ = v;
            return *this;
        }

        optional& operator = ( optional const& rhs ) {
            variant_ = rhs.variant_;
            return *this;
        }

        template<typename... Args>
        void emplace(Args&&... args) {
            variant_ = T{std::forward<Args>(args)...};
        }

        void reset() {
            variant_ = none;
        }
    };
}
}

#endif
