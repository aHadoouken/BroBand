#pragma once

template<typename Handler, typename Response, typename Request>
class Router {
public:
    void AddHandler(std::string method, Handler handler) {
        m_Handlers.emplace(method, handler);
    }

    Response ProcessRoute(const std::string &method, const Request &request) {
        auto handler_it = m_Handlers.find(method);
        if (handler_it != m_Handlers.end()) {
            return (handler_it->second)(request);
        }

        Response response;
        response.status_code = 404;
        response.status_message = "Not Found";
        response.http_version = "HTTP/1.0";
        response.body = "Not Found";
        return response;
    }

private:
    std::map<std::string, Handler> m_Handlers;
};
