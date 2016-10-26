// Copyright 2016 Damien Buhl (alias daminetreg) for Fr. Sauter AG, CH-4016 BASEL
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
#ifndef MAPBOX_DETAIL_BOOST_SPIRIT_ATTRIBUTES_HPP
#define MAPBOX_DETAIL_BOOST_SPIRIT_ATTRIBUTES_HPP

#include <mapbox/variant.hpp>

#include <mapbox/traits/is_mapbox_variant.hpp>
#include <mapbox/traits/is_type_in_variant.hpp>

#include <boost/spirit/home/support/attributes.hpp>

// Defines the trait for Boost.Spirit to treat mapbox::util::variant as a variant.
namespace boost { namespace spirit { namespace traits
{
    template <typename Domain, class... Types>
    struct not_is_variant<mapbox::util::variant<Types...>, Domain>
      : mpl::false_
    {};
}
}}

namespace boost { namespace spirit { namespace traits {

    template <class... Types>
    struct variant_which< mapbox::util::variant<Types...> >
    {
        static int call(mapbox::util::variant<Types...> const& v)
        {
            return v.which();
        }
    };

}}}

namespace boost { namespace spirit { namespace traits {

    template <typename Variant, typename Expected>
    struct compute_compatible_component_variant<Variant, Expected, mpl::false_
      , typename enable_if< typename mapbox::traits::is_mapbox_variant<Variant>::type >::type>
    {
        typedef typename traits::variant_type<Variant>::type variant_type;

        // true_ if the attribute matches one of the types in the variant
        typedef mapbox::traits::is_type_in_variant<Expected, Variant> type;
        enum { value = type::value };

        // return the type in the variant the attribute is compatible with
        typedef typename
            mpl::eval_if<type, mpl::identity<Expected>, mpl::identity<unused_type> >::type
        compatible_type;

        // return whether the given type is compatible with the Expected type
        static bool is_compatible(int which)
        {
          auto idx = (type::size - 1 - type::index); // Typelist is inverted and 0-based
          return which == idx;
        }
    };


}}}


#endif
