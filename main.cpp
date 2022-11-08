#include <cpr/cpr.h>
#include <iostream>
#include <nlohmann/json.hpp>

// for convenience
using json = nlohmann::json;

int main(int, char **) {
  cpr::Response response = cpr::Get(
      cpr::Url{"https://api.github.com/repos/whoshuu/cpr/contributors"},
      cpr::Authentication{"user", "pass", cpr::AuthMode::BASIC},
      cpr::Parameters{{"anon", "true"}, {"key", "value"}});

  if (response.status_code != 200) {
    // TODO: Decide what happens when the program breaks
    return 1; // Error code
  }

  json respose_data = json::parse(response.text);

  auto user_list = respose_data.get<json::array_t>();
  auto first_user = user_list[0].get<json::object_t>();
  std::string login = first_user["login"].get<json::string_t>();

  std::string some_string = "Test";

  std::cout << login << std::endl;
  std::cout << some_string << std::endl;
}
