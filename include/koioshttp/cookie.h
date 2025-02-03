#ifndef KOIOS_HTTP_COOKIE_H
#define KOIOS_HTTP_COOKIE_H

#include <string>
#include <string_view>
#include <functional>
#include <map>
#include <optional>
#include <filesystem>
#include <netinet/in.h>

namespace koios::http
{

class cookie
{
public:
    cookie() = default;
    cookie(::std::string key, ::std::string value);

public:
    void set_value(::std::string_view val);
    const ::std::string& key() const noexcept { return m_key; }
    const ::std::string& value() const noexcept { return m_value; }
    int version() const noexcept { return m_version; }

    void set_comment(const ::std::string& comment);
    void set_comment_url(const ::std::string& comment_url);
    void set_discard(bool v = true);
    void set_domain(const ::std::string& comment);
    void set_max_age(int val);
    void set_path(const ::std::filesystem::path& p);
    void set_path(const ::std::string& p);
    void set_secure(bool val = true);
    void set_port(in_port_t port);

    ::std::string payload_header_value() const;
    ::std::string_view payload_header_field() const noexcept;
    ::std::string_view original_string() const noexcept { return m_original_str; }

    bool empty() const noexcept { return m_key == ""; }

private:
    friend cookie to_cookie(::std::string_view str) noexcept;
    void set_original_string(const ::std::string& str);
    
private:
    ::std::string m_key;
    ::std::string m_value;

    int                             m_version{ 1 }; // 0 or 1(current)
    ::std::optional<::std::string>  m_comment;
    ::std::optional<::std::string>  m_comment_url;
    bool                            m_discard{ false };
    ::std::optional<::std::string>  m_domain;
    ::std::optional<int>            m_max_age; // seconds
    ::std::optional<::std::filesystem::path> m_path;
    ::std::optional<in_port_t>      m_port;
    bool                            m_secure{ false };

    ::std::string m_original_str;
};

cookie to_cookie(::std::string_view str) noexcept;

} // namespace koios::http

template<>
struct std::hash<::koios::http::cookie>
{
    size_t operator()(::koios::http::cookie& c) const noexcept
    {
        return ::std::hash<::std::string>{}(c.value());
    }
};

#endif
