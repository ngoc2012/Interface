#include <iostream>
#include <cpprest/http_listener.h>
#include <cpprest/uri.h>
#include <cpprest/json.h>

using namespace utility;
using namespace web;
using namespace web::http;
// using namespace web::http::client;
// using namespace concurrency::streams;
using namespace web::http::experimental::listener;

void handle(http_request request) {
    std::cout << "Method: " << request.method() << std::endl;
    std::cout << "URI: " << http::uri::decode(request.relative_uri().path()) << std::endl;
    std::cout << "Query: " << http::uri::decode(request.relative_uri().query()) << std::endl << std::endl;

    request.extract_json().then([](json::value data) {
        // std::cout << "user : " << data[U("user")].as_string() << std::endl;
        // std::cout << "fileName : " << data[U("fileName")].as_string() << std::endl;
    });

    json::value jsonObject;
    jsonObject[U("firstName")] = json::value::string(U("atakan"));
    jsonObject[U("lastName")] = json::value::string(U("sarioglu"));

    std::cout << jsonObject.serialize() << std::endl;

    http_response response(status_codes::OK);
    
    //  "CORS_ORIGIN_WHITELIST": "('http://localhost:3000',)",
    //     "ALLOWED_HOSTS": "192.168.56.1"
    //     ALLOWED_HOSTS = [
    //     '192.168.56.1',
    //     'localhost'
    // ]
    // Without these headers, the client browser will likely refuse the data and eat it
    // response.headers().add(U("Access-Control-Allow-Origin"), U("http://localhost:9000")); // not work
    // response.headers().add(U("Access-Control-Allow-Origin"), U("http://localhost")); // not work
    // response.headers().add(U("Access-Control-Allow-Origin"), U("http://192.168.56.1")); // not work
    response.headers().add(U("Access-Control-Allow-Origin"), U("http://10.0.2.15"));
    // response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
    // response.headers().add("Access-Control-Allow-Origin", "*"); // not work
    // response.headers().add(U("Vary"), U("Origin")); // not work
    // response.headers().add(U("Access-Control-Allow-Methods"), U("POST, OPTIONS"));
    // response.headers().add(U("Access-Control-Allow-Headers"), U("Content-Type"));
    // response.headers().add(U("X-Content-Type-Options"), U("nosniff"));
    // response.headers().add(U("X-Frame-Options"), U("DENY"));
    // response.headers().add(U("Server"), U("WSGIServer/0.2 CPython/3.6.9"));

    // The C++ REST SDK uses a different string type dependent on the platform being targeted. For example for the Windows platforms utility::string_t is std::wstring using UTF-16, on Linux std::string using UTF-8. The 'U' macro can be used to create a string literal of the platform type. If you are using a library causing conflicts with the 'U' macro, for example Boost.Iostreams it can be turned off by defining the macro '_TURN_OFF_PLATFORM_STRING' before including the C++ REST SDK header files.

//     Server	WSGIServer/0.2 CPython/3.6.9
// Vary	Origin
// X-Content-Type-Options	nosniff
// X-Frame-Options	DENY
    response.set_body(jsonObject);
    // response.set_body("jsonObject");

    request.reply(response);
}

int main(int argc, char **argv)
{
    utility::string_t address = U("http://0.0.0.0:9000");
    http_listener listener(address);
 
    listener.support(methods::GET, handle);
    listener.support(methods::POST, handle);
    listener.support(methods::PUT, handle);
    listener.support(methods::DEL, handle);

    try{
        listener.open().wait();
        std::cout << "Listening for requests at: " << address << std::endl;

        // Force to wait
        std::string line;
        std::getline(std::cin, line);

        listener.close().wait();
    }
    catch (std::exception const & e)
    {
       std::cout << e.what() << std::endl;
    }

    return 0;
}