#include "LoginController.h"
#include <argon2.h> 
#include <random>

namespace Passwd {

auto hash(std::string passwd) {
    constexpr size_t itCost = 1;
    constexpr size_t memCost = 64;
    constexpr size_t parallel = 2;
    constexpr size_t bufSz = 128;
    constexpr size_t saltSz = 16;
    constexpr size_t hashSz = 32;

    char buf[bufSz];
    char salt[saltSz];

    std::random_device rndgen;
    for (size_t i = 0; i < saltSz; i++) {
        salt[i] = rndgen() - rndgen.min();
    }

    int res = argon2id_hash_encoded(itCost, memCost, parallel, passwd.c_str(), passwd.length(), salt, saltSz, hashSz, buf, bufSz);
    if (res != ARGON2_OK) {
        throw std::runtime_error(std::string("error occured on argon2id_hash_encoded(): ") + std::to_string(res));
    }

    return std::string(buf);
}

bool verify(std::string passwd, std::string hash) {
    auto res = argon2id_verify(hash.c_str(), passwd.c_str(), passwd.length());
    if (res == ARGON2_OK)
        return true;
    if (res == ARGON2_VERIFY_MISMATCH)
        return false;
    throw std::runtime_error(std::string("error occured on argon2id_verify(): ") + std::to_string(res));
}

}; // namespace Passwd

// Add definition of your processing function here
void LoginController::login(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) const {
    const auto req_json = req->getJsonObject();
    if(!req_json) {
        Json::Value res;
        res["status"] = "error";
        res["message"] = "only json";
        callback(drogon::HttpResponse::newHttpJsonResponse(std::move(res)));
        return;
    }

    if (req->session()->find("logined") && req->session()->get<bool>("logined")) {
        Json::Value res;
        res["status"] = "success";
        callback(drogon::HttpResponse::newHttpJsonResponse(std::move(res)));
        return;
    }

    auto id = req_json->get("id", "").asString();
    auto password = req_json->get("password", "").asString();
    
    Json::Value res;
    if (id == "hoge" && password == "fuga") {
        req->session()->insert("logined", true);
        res["status"] = "success";
    } else {
        res["status"] = "failed";
    }

    callback(drogon::HttpResponse::newHttpJsonResponse(std::move(res)));
}

void LoginController::logout(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) const {
    req->session()->clear();

    Json::Value res;
    res["status"] = "success";
    callback(drogon::HttpResponse::newHttpJsonResponse(std::move(res)));
}
