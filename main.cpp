#include <iostream>
#include <vector>
#include <string>
#include <boost/variant.hpp>
#include <boost/timer/timer.hpp>
#include "variant.hpp"

#define TEXT "Testing various variant implementations with a longish string ........................................."
//#define BOOST_VARIANT_MINIMIZE_SIZE
namespace test {

template <typename V>
struct Holder
{
    typedef V value_type;
    std::vector<value_type> data;

    template <typename T>
    void append_move( T && obj)
    {
        data.emplace_back(std::move(obj));
    }

    template <typename T>
    void append(T const& obj)
    {
        data.push_back(obj);
    }
};

}


struct print
{
    template <typename T>
    void operator() (T const& val) const
    {
        std::cerr << val << ":" << typeid(T).name() <<  std::endl;
    }
};


template <typename V>
struct dummy : boost::static_visitor<>
{
    dummy(V & v)
        : v_(v) {}

    template <typename T>
    void operator() (T const& val) const
    {
        v_ = val;
    }
    V & v_;
};

template <typename V>
struct dummy2 : boost::static_visitor<>
{
    dummy2(V & v)
        : v_(v) {}

    template <typename T>
    void operator() (T const& val) const
    {
        v_.template set<T>(val);
    }
    V & v_;
};

int main (int argc, char** argv)
{
    if (argc!=2)
    {
        std::cerr << "Usage:" << argv[0] << " <num-runs>" << std::endl;
        return 1;
    }
    const int NUM_RUNS=std::stol(argv[1]);

#if 1
    {
        boost::timer::auto_cpu_timer t;
        test::Holder<util::variant<int,double,std::string> > h;
        h.data.reserve(NUM_RUNS);
        for (int i=0; i< NUM_RUNS;++i)
        {
            h.append_move(std::string(TEXT));
            h.append_move(123);
            h.append_move(3.14159);

        }
        std::cerr << h.data.size() << std::endl;
        util::variant<int,double,std::string> v;
        for (auto const& v2 : h.data)
        {
            dummy2<util::variant<int,double,std::string> > d(v);
            util::apply_visitor(v2, d);
        }
    }
#endif

#if 1
    {
        boost::timer::auto_cpu_timer t;
        test::Holder<boost::variant<int,double,std::string>> h;
        for (int i=0; i< NUM_RUNS;++i)
        {
            h.append_move(std::string(TEXT));
            h.append_move(123);
            h.append_move(3.14159);
        }

        std::cerr << h.data.size() << std::endl;
        boost::variant<int,double,std::string> v;
        for (auto const& v2 : h.data)
        {
            dummy<boost::variant<int,double,std::string> > d(v);
            boost::apply_visitor(d, v2);
        }
    }
#endif

#if 1

    std::cerr << util::detail::type_traits<bool, bool, int, double, std::string>::id << std::endl;
    std::cerr << util::detail::type_traits<int, bool, int, double, std::string>::id << std::endl;
    std::cerr << util::detail::type_traits<double, bool, int, double, std::string>::id << std::endl;
    std::cerr << util::detail::type_traits<std::string, bool, int, double, std::string>::id << std::endl;
    std::cerr << util::detail::type_traits<long, bool, int, double, std::string>::id << std::endl;
    std::cerr << util::detail::type_traits<std::vector<int>, bool, int, double, std::string>::id << std::endl;

    typedef util::variant<bool,int, double, std::string> variant_type;
    variant_type v(std::string("test"));
    util::apply_visitor(v, print());

    v = std::string("ABC");
    util::apply_visitor(v, print());

    std::vector<variant_type> vec;
    vec.emplace_back(std::move(v));
    for (auto const& e : vec)
    {
        util::apply_visitor(e, print());
    }


    v=std::string("test");
    util::apply_visitor(v, print());
    v=123.345;
    util::apply_visitor(v, print());
    variant_type v2(std::string("testing a bit more"));
    util::apply_visitor(v2, print());
    variant_type v3(444);
    util::apply_visitor(v3, print());

    std::cerr << sizeof(v) << std::endl;
    std::cerr << sizeof(v2) << std::endl;
    std::cerr << sizeof(v3) << std::endl;
    std::cerr << sizeof(boost::variant<bool,int, double, std::string>) << std::endl;
#endif
    return EXIT_SUCCESS;

}
