#ifndef KOIOSHTTP_PARSE_H
#define KOIOSHTTP_PARSE_H

#include <optional>

#include "koios/generator.h"

#include "koioshttp/context.h"
#include "koioshttp/request.h"

namespace koios::http
{

/**
 * @brief Parses an HTTP request from a given file descriptor with a timeout.
 *
 * This function attempts to read and parse a request from the specified 
 * file descriptor within the given timeout duration. If successful, it 
 * yields a parsed request; otherwise, it returns an empty optional.
 *
 * @param fd The unique file descriptor from which the request is read.
 * @param timeout The maximum time allowed for reading and parsing a single request.
 * @return A generator yielding an optional request. If parsing fails or times out, returns std::nullopt.
 */
generator<::std::optional<server::request>> 
parse_request_from(
    const toolpex::unique_posix_fd& fd, 
    ::std::chrono::milliseconds timeout 
);

} // namespace koios::http

#endif
