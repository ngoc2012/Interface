#include <cpprest/http_client.h>
#include <cpprest/json.h>
#pragma comment(lib, "cpprest110_1_1")
 
using namespace web;
using namespace web::http;
using namespace web::http::client;
 
#include <iostream>
using namespace std;
 
// void display_field_map_json(json::value & jvalue)
// {
//    if (!jvalue.is_null())
//    {
//       for (auto const & e : jvalue)
//       {
//          wcout << e.first.as_string() << L" : " << e.second.as_string() << endl;
//       }
//    }
// }
 
pplx::task<http_response> make_task_request(http_client & client, 
                                            method mtd, 
                                            json::value const & jvalue)
{
   return (mtd == methods::GET || mtd == methods::HEAD) ? 
      // client.request(mtd, L"/restdemo") : 
      // client.request(mtd, L"/restdemo", jvalue);
      client.request(mtd, U("/restdemo")) : 
      client.request(mtd, U("/restdemo"), jvalue);
}
 
void make_request(http_client & client, method mtd, json::value const & jvalue)
{
   make_task_request(client, mtd, jvalue)
      .then([](http_response response)
      {
         if (response.status_code() == status_codes::OK)
         {
            return response.extract_json();
         }
         return pplx::task_from_result(json::value());
      })
      .then([](pplx::task<json::value> previousTask)
      {
         try
         {
            // display_field_map_json(previousTask.get());
            cout << "display_field_map_json" << endl;
         }
         catch (http_exception const & e)
         {
            wcout << e.what() << endl;
         }
      })
      .wait();
}

int main()
{
   http_client client(U("http://localhost:9000"));
 
   // json::value::field_map putvalue;
   // putvalue.push_back(make_pair(json::value(L"one"), json::value(L"100")));
   // putvalue.push_back(make_pair(json::value(L"two"), json::value(L"200")));
 
   // wcout << L"\nput values\n";
   // make_request(client, methods::PUT, json::value::object(putvalue));
 
   auto getvalue = json::value::array();
   getvalue[0] = json::value(L"one");
   getvalue[1] = json::value(L"two");
   getvalue[2] = json::value(L"three");
 
   wcout << L"\nget values (POST)\n";
   make_request(client, methods::POST, getvalue);
 
   auto delvalue = json::value::array();
   delvalue[0] = json::value(L"one");
 
   wcout << L"\ndelete values\n";
   make_request(client, methods::DEL, delvalue);
 
   wcout << L"\nget values (POST)\n";
   make_request(client, methods::POST, getvalue);
 
   wcout << L"\nget values (GET)\n";
   make_request(client, methods::GET, json::value::null());
 
   return 0;
}