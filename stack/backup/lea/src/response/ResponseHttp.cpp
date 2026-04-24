#include "../../include/response/ResponseHttp.hpp"

// Default constructor initializes to a standard 200 OK
ResponseHttp::ResponseHttp() : _statusCode(200), _statusMessage("OK") {}
ResponseHttp::~ResponseHttp() {}

void ResponseHttp::setStatusCode(int code, const std::string& message) {
    _statusCode = code;
    _statusMessage = message;
}

void ResponseHttp::setHeader(const std::string& key, const std::string& value) {
    _headers[key] = value;
}

// Sets the body and automatically calculates the Content-Length header
void ResponseHttp::setBody(const std::string& body) {
    _body = body;
    
    // In C++98, we use ostringstream to convert integer to string
    std::ostringstream ss;
    ss << _body.size();
    setHeader("Content-Length", ss.str());
}

// Assembles all parts into a valid HTTP response string
std::string ResponseHttp::buildResponse() const {
    std::ostringstream response;

    // Status Line (e.g., "HTTP/1.1 200 OK\r\n")
    response << "HTTP/1.1 " << _statusCode << " " << _statusMessage << "\r\n";

    // Headers
    std::map<std::string, std::string>::const_iterator it;
    for (it = _headers.begin(); it != _headers.end(); ++it) {
        response << it->first << ": " << it->second << "\r\n";
    }

    // Mandatory empty line separating headers from body
    response << "\r\n";

    // The actual content
    response << _body;

    return response.str();
}