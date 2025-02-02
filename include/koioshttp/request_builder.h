#ifndef KOIOSHTTP_REQUEST_BUILDER_H
#define KOIOSHTTP_REQUEST_BUILDER_H

#include <memory>
#include <utility>

#include "koioshttp/message_details.h"
#include "koioshttp/request.h"
#include "koioshttp/version.h"
#include "koioshttp/methods.h"

namespace koios::http
{

template<typename MessageType>
class basic_request_builder
{
public:
    using message_type  = MessageType;
    using string_type   = typename message_type::string_type;
    using generate_type = basic_request<message_type>;
    using storage_type  = typename message_type::storage_type;

public:
    basic_request_builder() : m_data{ ::std::make_shared<message_type>() } {}

    basic_request_builder(storage_type&& stor) 
        : m_data{ ::std::make_shared<message_type>(::std::move(stor)) } {}

    basic_request_builder(::std::shared_ptr<storage_type> stor) 
        : m_data{ ::std::make_shared<message_type>(::std::move(stor)) } {}

    void set_version(version s) noexcept { m_data->m_version  = s;   }
    void set_method(method s)   noexcept { m_data->m_method   = s;   }

    template<typename... Args>
    void append_body(Args&&... s) 
    { 
        m_data->m_body_parts.emplace_back(::std::forward<Args>(s)...); 
    }

    void set_url(auto&& s)      { m_data->m_url      = string_type(::std::forward<decltype(s)>(s));   }
    void set_path(auto&& s)     { m_data->m_path     = string_type(::std::forward<decltype(s)>(s));   }
    void set_query(auto&& s)    { m_data->m_query    = string_type(::std::forward<decltype(s)>(s));   }
    void set_fragment(auto&& s) { m_data->m_fragment = string_type(::std::forward<decltype(s)>(s));   }

    void set_keep_alive(bool s = true) noexcept { m_data->m_keep_alive = s; }
    void set_web_socket(bool s = true) noexcept { m_data->m_web_socket = s; }

    template<typename Str1, typename Str2>
    requires (::std::convertible_to<Str1, string_type>
           && ::std::convertible_to<Str2, string_type>)
    void set_header(Str1&& key, Str2&& value)
    {
        m_data->m_headers.emplace( ::std::make_pair(
            string_type{::std::forward<decltype(key)>(key)}, 
            string_type{::std::forward<decltype(value)>(value)}
        ));
    }

    using header_item_trampoline_t = ::std::pair<string_type, string_type>;

    void set_header(header_item_trampoline_t t)
    {
        m_data->m_headers.emplace(::std::move(t));
    }

    generate_type generate_request() noexcept { return { m_data }; }
    auto generate() noexcept { return generate_request(); }
    auto& storage() noexcept { return m_data->storage(); }

    auto& header_item_trampoline() noexcept { return m_header_item; }

private: 
    ::std::shared_ptr<message_type> m_data;
    header_item_trampoline_t m_header_item;
};

namespace server { using request_builder = basic_request_builder<details::message_from_net>; }
namespace client { using request_builder = basic_request_builder<details::message_to_net>; }

} // namespace koios::http

#endif
