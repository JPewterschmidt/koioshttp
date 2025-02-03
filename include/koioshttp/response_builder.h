#ifndef KOIOS_HTTP_RESPONE_BUILDER_H
#define KOIOS_HTTP_RESPONE_BUILDER_H

#include <memory>
#include <vector>
#include <filesystem>
#include <type_traits>

#include "koioshttp/message_details.h"
#include "koioshttp/response.h"
#include "koioshttp/version.h"
#include "koioshttp/status.h"
#include "koioshttp/cookie.h"
#include "koioshttp/concepts.h"

namespace koios::http
{

template<typename MessageType>
class basic_response_builder
{
public:
    using message_type = MessageType;
    using string_type  = typename message_type::string_type;
    using generate_type = basic_response<message_type>;
    using storage_type = typename message_type::storage_type;
    using body_part_type = ::std::variant<string_type, ::std::filesystem::path>;

public:
    basic_response_builder() : m_data{ ::std::make_shared<message_type>() } {}
    basic_response_builder(storage_type&& stor) 
        : m_data{ ::std::make_shared<message_type>(::std::move(stor)) } {}
    basic_response_builder(::std::shared_ptr<storage_type> stor) 
        : m_data{ ::std::make_shared<message_type>(::std::move(stor)) } {}

    void set_version(version s)     noexcept { m_data->m_version = s;    }
    void set_status(status s)       noexcept { m_data->m_status  = s;    }

    void append_body(auto&& s) { m_data->m_body_parts.emplace_back(::std::forward<decltype(s)>(s)); }

    void set_url(auto s)      { m_data->m_url      = string_type(::std::move(s)); }
    void set_path(auto s)     { m_data->m_path     = string_type(::std::move(s)); }
    void set_query(auto s)    { m_data->m_query    = string_type(::std::move(s)); }
    void set_fragment(auto s) { m_data->m_fragment = string_type(::std::move(s)); }
    void set_keep_alive(bool s)     noexcept { m_data->m_keep_alive = s; }
    void set_web_socket(bool s)     noexcept { m_data->m_web_socket = s; }
    
    template<typename Str1, typename Str2>
    void set_header(Str1&& key, Str2&& value)
    {
        static_assert(::std::is_convertible_v<Str1, string_type> 
                   && ::std::is_convertible_v<Str2, string_type> 
                   || ::std::constructible_from<string_type, Str1> 
                   && ::std::constructible_from<string_type, Str2>, 
                   "requires that `string_type` must be constructable from `Str1` and `Str2`");
        m_data->m_headers.emplace( ::std::make_pair(
            string_type{::std::forward<decltype(key)>(key)}, 
            string_type{::std::forward<decltype(value)>(value)}
        ));
    }

    generate_type generate_response() noexcept { return { m_data }; }
    auto generate() noexcept { return generate_response(); }
    auto& storage() noexcept { return m_data->storage(); }

private:
    ::std::shared_ptr<message_type> m_data;
};

namespace server { using response_builder = basic_response_builder<details::message_to_net>; }
namespace client { using response_builder = basic_response_builder<details::message_from_net>; }

} // namespace koios::http

#endif
