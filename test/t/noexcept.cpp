#include "catch.hpp"
#include "variant.hpp"

#include <type_traits>

// helper macros for concise checks, will be expanded in failure output
#define nothrow_def_c(type, ...)        (std::is_nothrow_default_constructible<type<__VA_ARGS__>>::value)
#define nothrow_copy_a(type, ...)       (std::is_nothrow_copy_assignable<type<__VA_ARGS__>>::value)
#define nothrow_copy_c(type, ...)       (std::is_nothrow_copy_constructible<type<__VA_ARGS__>>::value)
#define nothrow_move_a(type, ...)       (std::is_nothrow_move_assignable<type<__VA_ARGS__>>::value)
#define nothrow_move_c(type, ...)       (std::is_nothrow_move_constructible<type<__VA_ARGS__>>::value)
#define nothrow_destruct(type, ...)     (detail::is_nothrow_destructible<type<__VA_ARGS__>>::value)

namespace detail {

#if defined(__GNUC__) && (100 * __GNUC__ + __GNUC_MINOR__ < 408)
    // GCC 4.7 doesn't have std::is_nothrow_destructible

    template <typename T>
    struct is_nothrow_destructible
    {
        static constexpr bool value = noexcept(std::declval<T&>().~T());
    };

#else

    using std::is_nothrow_destructible;

#endif

} // namespace detail

namespace { // internal

struct throwing_default_ctor
{
    throwing_default_ctor() noexcept(false)
    {
        throw std::runtime_error("throwing_default_ctor");
    }
};

struct throwing_copy
{
    throwing_copy() = default;
    throwing_copy(throwing_copy && ) = default;
    throwing_copy(throwing_copy const& ) noexcept(false)
    {
        throw std::runtime_error("throwing_copy:constructor");
    }
    throwing_copy & operator=(throwing_copy && ) = default;
    throwing_copy & operator=(throwing_copy const& ) noexcept(false)
    {
        throw std::runtime_error("throwing_copy:assignment");
        return *this;
    }
};

struct throwing_move
{
    throwing_move() = default;
    throwing_move(throwing_move const& ) = default;
    throwing_move(throwing_move && ) noexcept(false)
    {
        throw std::runtime_error("throwing_move:constructor");
    }
    throwing_move & operator=(throwing_move const& ) = default;
    throwing_move & operator=(throwing_move && ) noexcept(false)
    {
        throw std::runtime_error("throwing_move:assignment");
        return *this;
    }
};

struct throwing_dtor
{
    throwing_dtor() = default;
    throwing_dtor(throwing_dtor const& ) = default;
    throwing_dtor(throwing_dtor && ) = default;
    throwing_dtor & operator=(throwing_dtor const& ) = default;
    throwing_dtor & operator=(throwing_dtor && ) = default;
    ~throwing_dtor() noexcept(false)
    {
        throw std::runtime_error("throwing_dtor");
    }
};

} // namespace

TEST_CASE("exception-specification : simple variant")
{
    // variant delegates noexcept to tuple in some cases; that's why we check
    // tuple as well as variant here -- when a check on variant fails, seeing
    // what the tuple's noexcept is can help us locate the culprit
    using std::tuple;
    using mapbox::util::variant;

    SECTION("default constructor")
    {
        CHECK_NOFAIL(nothrow_def_c(tuple, int, float) == true);
        CHECK_NOFAIL(nothrow_def_c(tuple, int, throwing_default_ctor) == false);
        CHECK_NOFAIL(nothrow_def_c(tuple, throwing_default_ctor, int) == false);
        CHECK_NOFAIL(nothrow_def_c(tuple, throwing_default_ctor) == false);
        CHECK_NOFAIL(nothrow_def_c(tuple, throwing_copy, throwing_move) == true);
        CHECK_NOFAIL(nothrow_def_c(tuple, throwing_default_ctor, throwing_copy, throwing_move) == false);

        CHECK(nothrow_def_c(variant, int, float) == true);
        CHECK(nothrow_def_c(variant, int, throwing_default_ctor) == true);
        CHECK(nothrow_def_c(variant, throwing_default_ctor, int) == false);
        CHECK(nothrow_def_c(variant, throwing_default_ctor) == false);
        CHECK(nothrow_def_c(variant, throwing_copy, throwing_move) == true);
        CHECK(nothrow_def_c(variant, throwing_default_ctor, throwing_copy, throwing_move) == false);
    }

    SECTION("copy constructor")
    {
        CHECK_NOFAIL(nothrow_copy_c(tuple, int, float) == true);
        CHECK_NOFAIL(nothrow_copy_c(tuple, int, throwing_copy) == false);
        CHECK_NOFAIL(nothrow_copy_c(tuple, throwing_copy) == false);
        CHECK_NOFAIL(nothrow_copy_c(tuple, throwing_default_ctor, throwing_move) == true);
        CHECK_NOFAIL(nothrow_copy_c(tuple, throwing_default_ctor, throwing_copy, throwing_move) == false);

        CHECK(nothrow_copy_c(variant, int, float) == true);
        CHECK(nothrow_copy_c(variant, int, throwing_copy) == false);
        CHECK(nothrow_copy_c(variant, throwing_copy) == false);
        CHECK(nothrow_copy_c(variant, throwing_default_ctor, throwing_move) == true);
        CHECK(nothrow_copy_c(variant, throwing_default_ctor, throwing_copy, throwing_move) == false);
    }

    SECTION("move constructor")
    {
        CHECK_NOFAIL(nothrow_move_c(tuple, int, float) == true);
        CHECK_NOFAIL(nothrow_move_c(tuple, int, throwing_move) == false);
        CHECK_NOFAIL(nothrow_move_c(tuple, throwing_move) == false);
        CHECK_NOFAIL(nothrow_move_c(tuple, throwing_default_ctor, throwing_copy) == true);
        CHECK_NOFAIL(nothrow_move_c(tuple, throwing_default_ctor, throwing_copy, throwing_move) == false);

        CHECK(nothrow_move_c(variant, int, float) == true);
        CHECK(nothrow_move_c(variant, int, throwing_move) == false);
        CHECK(nothrow_move_c(variant, throwing_move) == false);
        CHECK(nothrow_move_c(variant, throwing_default_ctor, throwing_copy) == true);
        CHECK(nothrow_move_c(variant, throwing_default_ctor, throwing_copy, throwing_move) == false);
    }

    SECTION("copy assignment")
    {
        CHECK_NOFAIL(nothrow_copy_a(tuple, int, float) == true);
        CHECK_NOFAIL(nothrow_copy_a(tuple, int, throwing_copy) == false);
        CHECK_NOFAIL(nothrow_copy_a(tuple, throwing_copy) == false);
        CHECK_NOFAIL(nothrow_copy_a(tuple, throwing_default_ctor, throwing_move) == true);
        CHECK_NOFAIL(nothrow_copy_a(tuple, throwing_default_ctor, throwing_copy, throwing_move) == false);

        CHECK(nothrow_copy_a(variant, int, float) == true);
        CHECK(nothrow_copy_a(variant, int, throwing_copy) == false);
        CHECK(nothrow_copy_a(variant, throwing_copy) == false);
        CHECK(nothrow_copy_a(variant, throwing_default_ctor, throwing_move) == true);
        CHECK(nothrow_copy_a(variant, throwing_default_ctor, throwing_copy, throwing_move) == false);
    }

    SECTION("move assignment")
    {
        CHECK_NOFAIL(nothrow_move_a(tuple, int, float) == true);
        CHECK_NOFAIL(nothrow_move_a(tuple, int, throwing_move) == false);
        CHECK_NOFAIL(nothrow_move_a(tuple, throwing_move) == false);
        CHECK_NOFAIL(nothrow_move_a(tuple, throwing_default_ctor, throwing_copy) == true);
        CHECK_NOFAIL(nothrow_move_a(tuple, throwing_default_ctor, throwing_copy, throwing_move) == false);

        CHECK(nothrow_move_a(variant, int, float) == true);
        CHECK(nothrow_move_a(variant, int, throwing_move) == false);
        CHECK(nothrow_move_a(variant, throwing_move) == false);
        CHECK(nothrow_move_a(variant, throwing_default_ctor, throwing_copy) == true);
        CHECK(nothrow_move_a(variant, throwing_default_ctor, throwing_copy, throwing_move) == false);
    }

    SECTION("destructor")
    {
        CHECK_NOFAIL(nothrow_destruct(tuple, int, float) == true);
        CHECK_NOFAIL(nothrow_destruct(tuple, int, throwing_dtor) == false);
        CHECK_NOFAIL(nothrow_destruct(tuple, throwing_dtor) == false);

        // ~variant() is noexcept(true), regardless of exception-specification
        // on stored alternatives' destructors
        CHECK(nothrow_destruct(variant, int, float) == true);
        CHECK(nothrow_destruct(variant, int, throwing_dtor) == true);
        CHECK(nothrow_destruct(variant, throwing_dtor) == true);
    }
}

namespace { // internal

template <typename T> struct wrapped_alternative;
template <typename T> using wrap_alternative = mapbox::util::recursive_wrapper<wrapped_alternative<T>>;
template <typename T> using recursive_variant = mapbox::util::variant<wrap_alternative<T>>;

template <typename T>
struct wrapped_alternative : T
{
    recursive_variant<T> var;
};

} // namespace

TEST_CASE("exception-specification : recursive variant")
{
    using mapbox::util::recursive_wrapper;

    SECTION("default constructor")
    {
        // default-constructed wrapper allocates new T (default-constructed)
        CHECK(nothrow_def_c(recursive_variant, throwing_default_ctor) == false);
        CHECK(nothrow_def_c(recursive_variant, throwing_copy) == false);
        CHECK(nothrow_def_c(recursive_variant, throwing_move) == false);
        CHECK(nothrow_def_c(recursive_variant, throwing_dtor) == false);
    }

    SECTION("copy constructor")
    {
        // wrapper copy-constructor allocates new T (copy-constructed)
        CHECK(nothrow_copy_c(recursive_variant, throwing_default_ctor) == false);
        CHECK(nothrow_copy_c(recursive_variant, throwing_copy) == false);
        CHECK(nothrow_copy_c(recursive_variant, throwing_move) == false);
        CHECK(nothrow_copy_c(recursive_variant, throwing_dtor) == false);
    }

    SECTION("move constructor")
    {
        // wrapper move-constructor allocates new T (move-constructed)
        CHECK(nothrow_move_c(recursive_variant, throwing_default_ctor) == false);
        CHECK(nothrow_move_c(recursive_variant, throwing_copy) == false);
        CHECK(nothrow_move_c(recursive_variant, throwing_move) == false);
        CHECK(nothrow_move_c(recursive_variant, throwing_dtor) == false);
    }

    SECTION("copy assignment")
    {
        // wrapper copy-assignment copies wrapped value
        CHECK(nothrow_copy_a(recursive_wrapper, throwing_default_ctor) == true);
        CHECK(nothrow_copy_a(recursive_wrapper, throwing_copy) == false);
        CHECK(nothrow_copy_a(recursive_wrapper, throwing_move) == true);
        CHECK(nothrow_copy_a(recursive_wrapper, throwing_dtor) == true);

        // variant copy-assignment destroys the original wrapper and then
        // copy-constructs a new wrapper in its place, so we're actually
        // testing recursive_wrapper's copy-constructor here
        CHECK(nothrow_copy_a(recursive_variant, throwing_default_ctor) == false);
        CHECK(nothrow_copy_a(recursive_variant, throwing_copy) == false);
        CHECK(nothrow_copy_a(recursive_variant, throwing_move) == false);
        CHECK(nothrow_copy_a(recursive_variant, throwing_dtor) == false);
    }

    SECTION("move assignment")
    {
        // wrapper move-assignment swaps internal pointers
        CHECK(nothrow_move_a(recursive_wrapper, throwing_default_ctor) == true);
        CHECK(nothrow_move_a(recursive_wrapper, throwing_copy) == true);
        CHECK(nothrow_move_a(recursive_wrapper, throwing_move) == true);
        CHECK(nothrow_move_a(recursive_wrapper, throwing_dtor) == true);

        // variant move-assignment destroys the original wrapper and then
        // move-constructs a new wrapper in its place, so we're actually
        // testing recursive_wrapper's move-constructor here
        CHECK(nothrow_move_a(recursive_variant, throwing_default_ctor) == false);
        CHECK(nothrow_move_a(recursive_variant, throwing_copy) == false);
        CHECK(nothrow_move_a(recursive_variant, throwing_move) == false);
        CHECK(nothrow_move_a(recursive_variant, throwing_dtor) == false);
    }

    SECTION("conversion assignment")
    {
        CHECK((std::is_nothrow_assignable<recursive_wrapper<throwing_copy> & , throwing_copy const& >::value) == false);
        CHECK((std::is_nothrow_assignable<recursive_wrapper<throwing_copy> & , throwing_copy && >::value) == true);
        CHECK((std::is_nothrow_assignable<recursive_wrapper<throwing_move> & , throwing_move const& >::value) == true);
        CHECK((std::is_nothrow_assignable<recursive_wrapper<throwing_move> & , throwing_move && >::value) == false);
    }

    SECTION("destructor")
    {
        // ~variant() is always noexcept(true)
        CHECK(nothrow_destruct(recursive_variant, throwing_default_ctor) == true);
        CHECK(nothrow_destruct(recursive_variant, throwing_copy) == true);
        CHECK(nothrow_destruct(recursive_variant, throwing_move) == true);
        CHECK(nothrow_destruct(recursive_variant, throwing_dtor) == true);
    }
}
