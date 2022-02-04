// #include <iostream>
// #include <vector>
// #include <array>
// #include <math.h>       /* sqrt */

// #include <any>
// #include <iostream>

// #include <map>
// #include <string>
// #include <any>

#include <nlohmann/json.hpp>

// for convenience
using json = nlohmann::json;

// using namespace std;

int main(int argc, char **argv)
{
    // // [9 8 7 6 5 4 3 2 1 0]
    // std::array<std::size_t,10> a;
    // for (std::size_t i = 0; i != a.size(); ++i)
    //     a[i] = i;
    // for (std::size_t i = a.size()-1; i < a.size(); --i)
    //     std::cout << a[i] << " ";
    
    // // map contain
    // std::cout << "mymap contains:";
    // for (auto& x: mymap)
    //     std::cout << " [" << x.first << ':' << x.second << ']';
    // std::cout << '\n';

    // std::map<std::string, std::any> notebook;

    // std::string name{ "Pluto" };
    // int year = 2015;

    // notebook["PetName"] = name;
    // notebook["Born"] = year;

    // std::string name2 = std::any_cast<std::string>(notebook["PetName"]); // = "Pluto"
    // int year2 = std::any_cast<int>(notebook["Born"]); // = 2015

    // if( __cplusplus == 201103L ) std::cout << "C++11\n" ;
    // else if( __cplusplus == 19971L ) std::cout << "C++98\n" ;
    // else if( __cplusplus == 201703L ) std::cout << "C++17\n" ;
    // else std::cout << "pre-standard C++\n" ;

    // std::cout << std::boolalpha;
 
    // // any type
    // std::any a = 1;
    // std::cout << a.type().name() << ": " << std::any_cast<int>(a) << '\n';
    // a = 3.14;
    // std::cout << a.type().name() << ": " << std::any_cast<double>(a) << '\n';
    // a = true;
    // std::cout << a.type().name() << ": " << std::any_cast<bool>(a) << '\n';
 
    // // bad cast
    // try
    // {
    //     a = 1;
    //     std::cout << std::any_cast<float>(a) << '\n';
    // }
    // catch (const std::bad_any_cast& e)
    // {
    //     std::cout << e.what() << '\n';
    // }
 
    // // has value
    // a = 1;
    // if (a.has_value())
    // {
    //     std::cout << a.type().name() << '\n';
    // }
 
    // // reset
    // a.reset();
    // if (!a.has_value())
    // {
    //     std::cout << "no value\n";
    // }
 
    // // pointer to contained data
    // a = 1;
    // int* i = std::any_cast<int>(&a);
    // std::cout << *i << "\n";

}