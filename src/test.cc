#include <iostream>

#include "json.hh"

#define STR1(x) #x
#define STR(x) STR1(x)
#define TEST(var) [&](auto&& f){ \
    std::cout << "\033[33m" STR(__LINE__) ": \033[36m" #var ":\033[0m "; \
    try { std::cout << f(); } \
    catch (const std::exception& e) { \
      std::cout << "\033[31m" << e.what() << "\033[0m"; \
    } \
    std::cout << std::endl; \
  }([&]{ return var; });

using std::cout;
using std::endl;
using ivanp::json;

void test(std::string_view str) {
  cout << "\n`" << str << '`' << endl;
  try {
    json j(str);
    cout << "\033[36m" << j.type_name() << "\033[0m\n";
    cout << j << endl;
  } catch (const json::error& e) {
    cout << "\033[31m" << e.what() << "\033[0m" << endl;
  }
}

int main(int argc, char* argv[]) {
  TEST(sizeof(json))
  TEST(sizeof(json::object_t))
  TEST(sizeof(json::array_t))
  TEST(sizeof(std::variant<bool,char>))
  TEST(sizeof(std::variant<char,short>))
  TEST(sizeof(std::variant<char,int>))
  TEST(sizeof(std::variant<char,size_t>))

  test("null");
  test(" null");
  test("null ");
  test("nullX");
  test("Xnull");
  test("5");
  test("5.5");
  test("5e5");
  test("\"5e5\"");
  test("\"5e5\",");
  test(R"("5\e5")");
  test(R"("5\\e5")");
  test(R"("5\\\e5")");
  test("[]");
  test("[null,5]");
  test("[null,5f]");
  test("[null,f]");
  test("[null,\"f\"]");
  test("[[[[]]]]");
  test("[false,[ [[] ],true]  ]");
  test("[false,[ null,true]  ]");
  test("[false,[[],true]]");
  test("[false,[[],[]]]");
  test("[[[],[]]]");
  test("[[],[[],[]]]");
  test(R"({ })");
  test(R"({ 1: 5 })");
  test(R"({ "a" })");
  test(R"({ "a": 5 })");
  test(R"({ "a": 5, "b" :[6] })");

  // TEST(ivanp::json("[null,5]")[1].type_name())
  // TEST(std::get<json::array_t>(json("[null,5]").node).size())

  TEST((bool)json(R"(3.14)"))
  TEST((bool)json(R"(null)"))
  TEST((bool)json(R"(0.)"))
  TEST((bool)json(R"(false)"))
  TEST((bool)json(R"(true)"))
  TEST((bool)json(R"([])"))
  TEST((bool)json(R"({})"))
  TEST((bool)json(R"([[]])"))
  TEST((double)json(R"(3.14)"))
  TEST((float)json(R"(3.14)"))
  TEST((short)json(R"(3.14)"))
  TEST((float)json(R"(3)"))
  TEST((int)json(R"({})"))
  TEST(json(R"("test")").get<std::string&>())
  TEST(json(R"("test")").get<const std::string&>())

  TEST((const double&)json(R"(2.2)"))
  { json j("\"3.3\"");
    TEST(j)
    std::string& x = j.get<std::string&>();
    // std::string& x = j;
    x = "test";
    TEST(j)
    TEST(x)
  }

}
