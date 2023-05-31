#include <drogon/drogon.h>

auto loadEnv(const std::string &name) {
    auto tmp = std::getenv(name.c_str());
    if (tmp) {
        return std::string(tmp);
    } else {
        throw std::runtime_error(name + " is not set");
    }
}

auto loadEnv(const std::string &name, const std::string &default_value) {
    auto tmp = std::getenv(name.c_str());
    if (tmp) {
        return std::string(tmp);
    } else {
        return std::string(default_value);
    }
}

int main() {
    // Set HTTP listener address and port
    drogon::app().addListener("0.0.0.0", 3000);

    auto mariadb_hostname = loadEnv("MARIADB_HOSTNAME", "127.0.0.1");
    auto mariadb_database = loadEnv("MARIADB_DATABASE", "larncycl");
    auto mariadb_username = loadEnv("MARIADB_USERNAME", "larncycl");
    auto mariadb_password = loadEnv("MARIADB_PASSWORD", "password");

    drogon::app().createDbClient("mysql", mariadb_hostname, 3306, mariadb_database, mariadb_username, mariadb_password);

    // Load config file
    // drogon::app().loadConfigFile("../config.json");
    // Run HTTP framework,the method will block in the internal event loop
    drogon::app().enableSession(std::chrono::hours(6)).run();
    return 0;
}
