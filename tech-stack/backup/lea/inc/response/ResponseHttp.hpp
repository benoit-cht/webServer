#ifndef RESPONSEHTTP_HPP
#define RESPONSEHTTP_HPP

#include <string>
#include <map>
#include <sstream>

class ResponseHttp {
  public:
    ResponseHttp();
    virtual ~ResponseHttp();

  public:
    // Setters to build the response
    void        setStatusCode(int code, const std::string& message);
    void        setHeader(const std::string& key, const std::string& value);
    void        setBody(const std::string& body);

    // Generates the final raw string to be sent over the socket
    std::string buildResponse() const;

  private:
    int                                 _statusCode;
    std::string                         _statusMessage;
    std::map<std::string, std::string>  _headers;
    std::string                         _body;
};

#endif