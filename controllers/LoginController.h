#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

class LoginController : public drogon::HttpController<LoginController> {
  public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(LoginController::login, "/api/login", Post);
    ADD_METHOD_TO(LoginController::logout, "/api/logout", Get);

    METHOD_LIST_END
    void login(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) const;
    void logout(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) const;
};
