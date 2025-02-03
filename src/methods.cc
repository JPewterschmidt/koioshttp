#include <string_view>
#include <cstring>

#include "koioshttp/methods.h"

using namespace ::std::string_view_literals;

namespace koios::http
{

::std::string_view to_string(method m)
{
    switch (m) 
    {
#define XX(code, name, str) case method::name: return str##sv;
        KH_HTTP_METHOD_MAP(XX)
#undef XX
        default : return "invalid method"sv;
    }
}

method to_method(::std::string_view sv)
{
#define XX(code, name, str) \
    if (::strncasecmp(str, sv.data(), sv.size()) == 0) \
        return static_cast<method>(code);
    
    KH_HTTP_METHOD_MAP(XX)
#undef XX
    return {};
}

method to_method(uint16_t code)
{
    return static_cast<method>(code);
}

method to_method_llhttp(uint8_t code)
{
    return to_method(code);
}

} // namespace koios::http
