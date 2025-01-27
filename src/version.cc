#include "version.h"

using namespace ::std::string_view_literals;

namespace wilson::http
{

::std::string_view to_string(version v)
{
    switch (v)
    {
        case version::HTTP_1_0: return "HTTP/1.0"sv;
        case version::HTTP_1_1: return "HTTP/1.1"sv;
        case version::HTTP_2  : return "HTTP/2"sv;
        case version::HTTP_3  : return "HTTP/3"sv;
        default: return "invalid http version"sv;
    }
}

version to_version(::std::string_view str)
{
    /**/ if (str.find("HTTP/1.1"))  return version::HTTP_1_1;
    else if (str.find("HTTP/1.0"))  return version::HTTP_1_0;
    else if (str.find("HTTP/2"))    return version::HTTP_2;
    else if (str.find("HTTP_3"))    return version::HTTP_3;

    return version::INVALID_VERSION;
}

version to_version(uint8_t major, uint8_t minor)
{
    if (major == 1)
    {
        if (minor == 1) [[likely]] return version::HTTP_1_1;
        else return version::HTTP_1_0;
    }
    else if (major == 2) return version::HTTP_2;
    else if (major == 3) return version::HTTP_3;

    return version::INVALID_VERSION;
}

} // namespace koios::version
