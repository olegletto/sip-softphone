#include <string>
#include <string_view>

struct RequestLine {
    std::string method;
    std::string uri;
    std::string sipVersion;
};

RequestLine parseRequestLine(std::string_view line);