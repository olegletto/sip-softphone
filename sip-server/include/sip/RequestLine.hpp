#include <cstdint>
#include <sstream>
#include <string_view>

struct RequestLine {
    std::string method;
    std::string uri;
    std::string sipVersion;
};

RequestLine parseRequestLine(std::string_view line) {
    RequestLine requestLine;
    std::istringstream iss(line);
    iss >> requestLine.method >> requestLine.uri >> requestLine.sipVersion;
    return requestLine;
}