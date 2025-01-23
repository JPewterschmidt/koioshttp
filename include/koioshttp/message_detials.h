#ifndef KOIOSHTTP_MESSAGE_DETIALS_H 
#define KOIOSHTTP_MESSAGE_DETIALS_H 

#include <string_view>
#include <map>
#include <memory>
#include <variant>
#include <vector>

#include "toolpex/buffer.h"
#include "toolpex/unqiue_posix_fd.h"

#include "koioshttp/version.h"
#include "koioshttp/status.h"
#include "koioshttp/method.h"

#define CRLF "\r\n"
#define CRLF_SV ::std::string_view{ CRLF }

namespace koios::http
{

template<typename StringLike>
struct message_object
{
    using string_type = StringLike;
    http::version   m_version{version::HTTP_1_1};
    http::status    m_status{};
    http::method    m_method{};
    string_type     m_body;
    string_type     m_url;
    string_type     m_path;
    string_type     m_query;
    string_type     m_fragment;
    bool            m_keep_alive{};
    bool            m_web_socket{};
    ::std::multimap<string_type, string_type> m_headers;
};

class message_from_net : public message_object<::std::string_view>
{
public:
    using storage_type = toolpex::buffer;

public:
    message_from_net() = default;
    message_from_net(storage_type&& stor) : message_from_net(::std::make_shared<storage_type>(::std::move(stor))) {}
    message_from_net(::std::shared_ptr<storage_type> stor) noexcept : m_storage{ ::std::move(stor) } {}
    void set_storage(storage_type&& stor) { ::std::make_shared<storage_type>(::std::move(stor)); }
    void set_storage(::std::shared_ptr<storage_type> stor) noexcept
        { m_storage = stor; }
    auto storage_pointer() noexcept { return m_storage; }
    auto& storage() noexcept { return *m_storage; }
    ::std::span<const ::std::byte> to_payload() const;
    ::std::string_view to_payload_string() const;

private:
    ::std::shared_ptr<storage_type> m_storage{};
};

class message_to_net : public message_object<::std::string>
{
public:
    using storage_type = toolpex::buffer;
    
public:
    void fill_in_storage() const;
    auto storage_pointer() noexcept { return m_storage; }
    auto& storage() noexcept { return *m_storage; }
    ::std::span<const ::std::byte> to_payload() const;
    ::std::string_view to_payload_string() const;
    
private:
    mutable bool m_filled{ false };
    ::std::shared_ptr<storage_type> m_storage{ 
        ::std::make_shared<storage_type>() 
    };
};

} // namespace koios::http

#endif
