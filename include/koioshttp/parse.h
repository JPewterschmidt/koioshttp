#ifndef KOIOSHTTP_PARSE_H
#define KOIOSHTTP_PARSE_H

#include <optional>

#include "koioshttp/context.h"
#include "koioshttp/request.h"

namespace koios::http
{

task<::std::optional<server::request>> 
parse_request_from(
    const toolpex::unique_posix_fd& fd, 
    ::std::chrono::system_clock::time_point timeout
);

} // namespace koios::http

#endif
