#include <iostream>
#include <cassert>

#include <mapbox/variant.hpp>
#include <mapbox/boost_spirit_karma.hpp>
#include <boost/spirit/include/karma.hpp>

typedef mapbox::util::variant<int, bool, std::string, float, long, uint64_t> frame;

int main() {
  typedef std::back_insert_iterator<std::string> Iterator;

  using namespace boost::spirit::karma;

  rule<Iterator, std::string()> mystring = *char_;

  rule<Iterator, frame()> r = 
      int_
    | bool_
    | mystring
  ;


  {
    frame frm = 12;
    std::string buf;
    Iterator iter(buf);
    auto s = generate(iter, r, frm); 

    std::cout << (s ? " success " : " error ") << " - " << mapbox::util::get<int>(frm) << " == " << buf << std::endl;
    assert(s);
    assert("12" == buf);
  }

  {
    frame frm = true;
    std::string buf;
    Iterator iter(buf);
    auto s = generate(iter, r, frm); 

    std::cout << (s ? " success " : " error ") << " - " << mapbox::util::get<bool>(frm) << " == " << buf << std::endl;
    assert(s);
    assert("true" == buf);
  }

  {
    frame frm = std::string{"this is a string"};
    std::string buf;
    Iterator iter(buf);
    auto s = generate(iter, r, frm); 

    std::cout << (s ? " success " : " error ") << " - " << mapbox::util::get<std::string>(frm) << " == " << buf << std::endl;
    assert(s);
    assert("this is a string" == buf);
  }

  return EXIT_SUCCESS;
}
