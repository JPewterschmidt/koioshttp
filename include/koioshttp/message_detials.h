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

namespace koios::http
{

extern constexpr static ::std::string_view crlf{"\r\n"};

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

// OK
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

    auto& storage() noexcept { return *m_storage; }

private:
    ::std::shared_ptr<toolpex::buffer> m_storage{};
};

// OK
class message_to_net : public message_object<::std::string>
{
public:
    using storage_type = toolpex::buffer;
    
public:
    ::std::vector<::std::string_view> message_parts() const;
    toolpex::buffer fill_to_buffer() const;
};

} // namespace koios::http

#endif
