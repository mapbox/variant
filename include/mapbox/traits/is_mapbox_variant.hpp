// Copyright 2016 Damien Buhl (alias daminetreg) for Fr. Sauter AG, CH-4016 BASEL
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
#ifndef MAPBOX_TRAITS_IS_MAPBOX_VARIANT_HPP
#define MAPBOX_TRAITS_IS_MAPBOX_VARIANT_HPP

#include <mapbox/variant.hpp>

namespace mapbox { namespace traits { 

  template <class... Types>
  struct is_mapbox_variant {
    using type = std::false_type;
    enum { value = false };
  };

  template <class... Types>
  struct is_mapbox_variant<mapbox::util::variant<Types...>> {
    using type = std::true_type;
    enum { value = true };
  };

  template <class... Types>
  struct is_mapbox_variant<const mapbox::util::variant<Types...>> {
    using type = std::true_type;
    enum { value = true };
  };


}}

#endif
