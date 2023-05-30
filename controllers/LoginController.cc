#include "LoginController.h"
#include <argon2.h>

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
