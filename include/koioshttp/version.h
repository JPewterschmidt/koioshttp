#ifndef KOIOSHTTP_VERSION_H
#define KOIOSHTTP_VERSION_H

#include <string_view>
#include <cstddef>

namespace koios::http
{

enum class version : uint8_t
{
    HTTP_1_0 = (1U << 0),
    HTTP_1_1 = (1U << 1),
    HTTP_2   = (1U << 2),
    HTTP_3   = (1U << 3),
    INVALID_VERSION,
};

::std::string_view to_string(version v);
version to_version(::std::string_view str);
version to_version(uint8_t major, uint8_t minor);

} // namespace koios::http

#endif
