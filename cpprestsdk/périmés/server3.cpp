#include <iostream>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/uri.h>
#include <cpprest/json.h>

using namespace utility;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace concurrency::streams;

int main(int argc, char **argv)
{
    auto postJson = pplx::create_task([]() {
        json::value jsonObject;
        jsonObject[U("first_name")] = json::value::string(U("atakan"));
        jsonObject[U("last_name")] = json::value::string(U("sarioglu"));
        //https://reqres.in/api/users/1
        return http_client(U("http://0.0.0.0:9000"))
            .request(methods::POST,
                uri_builder(U("rido")).append_path(U("projet")).to_string(),
                jsonObject.serialize(), U("application/json"));
    })
    
    // Get the response.
    .then([](http_response response) {
        // Check the status code.
        if (response.status_code() != 201) {
            throw std::runtime_error("Returned " + std::to_string(response.status_code()));
        }

        // Convert the response body to JSON object.
        return response.extract_json();
    })
    
    // Parse the user details.
    .then([](json::value jsonObject) {
        std::cout << jsonObject[U("first_name")].as_string()
        << " " << jsonObject[U("last_name")].as_string()
        << " (" << jsonObject[U("id")].as_string() << ")"
        << std::endl;
    });

    postJson.open().wait();
    handler.close().wait();
    // // Wait for the concurrent tasks to finish.
    // try {
    //     while (!requestTask.is_done()) { std::cout << "."; }
    //     } catch (const std::exception &e) {
    //     printf("Error exception:%s\n", e.what());
    // }
    // return 0;
}