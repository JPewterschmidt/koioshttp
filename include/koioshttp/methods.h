#ifndef KOIOSHTTP_METHODS_H_
#define KOIOSHTTP_METHODS_H_

#include "macros.h"
#include <string_view>
#include <cstddef>
#include <llhttp.h>

namespace koios::http
{
    enum method : uint16_t
    {
#define XX(num, name, string) name = num,
        CC_HTTP_METHOD_MAP(XX)
#undef XX
        INVALID_METHOD,
    };

    ::std::string_view to_string(method m);
    method to_method(::std::string_view str);
    method to_method(uint16_t code);
    method to_method_llhttp(uint8_t code);
} // namespace koios::http

#endif
