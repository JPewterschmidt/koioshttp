#ifndef KOIOSHTTP_STATUS_H 
#define KOIOSHTTP_STATUS_H 

#include <string_view>
#include <cstddef>
#include <cstdint>

#include "koioshttp/macros.h"

namespace koios::http
{
    enum class status : uint16_t
    {
#define XX(code, name, string) name = code,
        KH_HTTP_STATUS_MAP(XX)
#undef XX
        INVALID_STATUS,
    };

    ::std::string_view to_string(status s);
    status to_status(::std::string_view str);
    status to_status(uint16_t code);
}

#endif
