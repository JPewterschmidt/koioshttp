#include <queue>
#include <type_traits>
#include <vector>
#include <string_view>
#include <cstring>
#include <string>
#include <filesystem>
#include <sstream>
#include <variant>

#include "koioshttp/message_details.h"

using namespace ::std::string_view_literals;
namespace rv = ::std::ranges::views;

namespace koios::http::details
{

::std::vector<::std::string_view> message_to_net::message_parts() const
{
    ::std::vector<::std::string_view> result; 
    
    // response line
    result.emplace_back(to_string(m_version));
    result.emplace_back(" ");
    auto status_str_code = ::std::to_string(
        static_cast<::std::underlying_type_t<status>>(m_status)
    );
    result.emplace_back(status_str_code);
    result.emplace_back(" ");
    result.emplace_back(to_string(m_status));
    result.emplace_back(crlf);

    // headers
    for (const auto& [k, v] : m_headers)
    {
        result.emplace_back(k);
        result.emplace_back(": ");
        result.emplace_back(v);  
        result.emplace_back(crlf);
    }
    result.emplace_back(crlf);
    
    // body
    result.append_range(m_body_parts | rv::transform([](auto&& str) { return ::std::string_view(str); }));
    result.emplace_back(crlf);
    result.emplace_back(crlf);

    return result; 
}

toolpex::buffer message_to_net::fill_to_buffer() const
{
    toolpex::buffer result;
    for (auto str : message_parts())
    {
        result.append(str);
    }
    return result;
}

} // namespace koios::http
