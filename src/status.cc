#include <cstring>
#include <string_view>

#include "koioshttp/status.h"

using namespace ::std::string_view_literals;

namespace koios::http
{

::std::string_view to_string(status s)
{
    switch (s) 
    {
#define XX(code, name, str) case status::name: return str##sv;
        KH_HTTP_STATUS_MAP(XX)
#undef XX
        default : return "invalid status"sv;
    }
}

status to_status(::std::string_view sv)
{
#define XX(code, name, str) \
    if (::strncasecmp(str, sv.data(), sv.size()) == 0) \
        return static_cast<status>(code);
    
    KH_HTTP_STATUS_MAP(XX)
#undef XX
    return {};
}

status to_status(uint16_t code)
{
    return static_cast<status>(code);
}

} // namespace koios::http
