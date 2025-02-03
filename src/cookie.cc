#include <ranges>
#include <string_view>
#include <string>
#include <type_traits>
#include <iterator>
#include <format>

#include "toolpex/string_ext.h"

#include "koioshttp/cookie.h"

namespace rv = ::std::ranges::views;
using namespace ::std::string_view_literals;

// TODO : port to koioshttp

namespace koios::http
{

cookie::cookie(::std::string key, ::std::string value)
    : m_key{ ::std::move(key) }, m_value{ ::std::move(value) }
{
}

void cookie::set_value(::std::string_view val)
{
    m_value = ::std::string{ val };
}

::std::string_view cookie::payload_header_field() const noexcept
{
    return version() == 1 ? "Set-Cookie2" : "Set-Cookie";
}

::std::string cookie::payload_header_value() const
{
    ::std::string out;
    if (m_version) ::std::format_to(::std::back_inserter(out), "{}; ", m_version);
    if (m_comment) ::std::format_to(::std::back_inserter(out), "{}; ", *m_comment);
    if (m_comment_url) ::std::format_to(::std::back_inserter(out), "{}; ", *m_comment_url);
    if (m_discard) ::std::format_to(::std::back_inserter(out), "{}; ", "Discard");
    if (m_domain) ::std::format_to(::std::back_inserter(out), "{}; ", *m_domain);
    if (m_max_age) ::std::format_to(::std::back_inserter(out), "{}; ", *m_max_age);
    if (m_path) ::std::format_to(::std::back_inserter(out), "{}; ", m_path->string());
    if (m_port) ::std::format_to(::std::back_inserter(out), "{}; ", *m_port);
    if (m_secure) ::std::format_to(::std::back_inserter(out), "{}", "Secure");

    return out;
}

void cookie::set_comment(const ::std::string& comment)
{
    m_comment = comment;
}

void cookie::set_comment_url(const ::std::string& comment_url)
{
    m_comment_url = comment_url;
}

void cookie::set_discard(bool v)
{
    m_discard = v;
}

void cookie::set_domain(const ::std::string& comment)
{
    m_domain = comment;
}

void cookie::set_max_age(int val)
{
    m_max_age = val;
}

void cookie::set_path(const ::std::filesystem::path& p)
{
    m_path = p;
}

void cookie::set_path(const ::std::string& str)
{
    set_path(::std::filesystem::path{ str });
}

void cookie::set_secure(bool v)
{
    m_secure = v;
}

void cookie::set_port(in_port_t port)
{
    m_port = port;
}

void cookie::set_original_string(const ::std::string& str)
{
    m_original_str = str;
}

static ::std::pair<bool, typename ::std::string_view::size_type> 
equal_oper_idx(::std::string_view str)
{
    using size_type = typename ::std::string_view::size_type;
    auto ret = str.find('=');
    if (ret == ::std::string_view::npos) return ::std::make_pair(false, size_type{});
    return ::std::make_pair(true, size_type{ ret });
}

cookie to_cookie(::std::string_view str) noexcept
{
    if (auto idx = str.find(':'); idx != ::std::string_view::npos)
        str = str.substr(idx + 1);
    str = toolpex::trim(str);

    auto strs = rv::split(str, ';');
    ::std::ranges::subrange temp = *strs.begin();
    ::std::string_view kv_pair{ temp.data(), temp.size() };
    kv_pair = toolpex::trim(kv_pair);
    auto eq_oper_idx = kv_pair.find('=');
    if (eq_oper_idx == ::std::string_view::npos)
        return {};
    cookie result{ 
        ::std::string{ kv_pair.substr(0, eq_oper_idx) }, 
        ::std::string{ toolpex::remove_quotation_mark(kv_pair.substr(eq_oper_idx + 1)) }
    };
    result.set_original_string(::std::string{str});
    for (const auto& item : strs | rv::drop(1))
    {
        ::std::string_view item_str{ item.data(), item.size() };
        item_str = toolpex::trim(item_str);
        item_str = toolpex::remove_quotation_mark(item_str);
        auto [has_eq_oper, idx] = equal_oper_idx(item_str);
        if (has_eq_oper)
        {
            auto k = item_str.substr(0, idx);
            auto v = toolpex::remove_quotation_mark(item_str.substr(idx + 1));
            if (toolpex::str_case_equal(k, "Comment"sv))
                result.set_comment(::std::string{ v });
            else if (toolpex::str_case_equal(k, "CommentUrl"sv))
                result.set_comment_url(::std::string{ v });
            else if (toolpex::str_case_equal(k, "domain"sv))
                result.set_domain(::std::string{ v });
            else if (toolpex::str_case_equal(k, "MaxAge"sv))
                result.set_max_age(atoi(v.data()));
            else if (toolpex::str_case_equal(k, "Path"sv))
                result.set_path(::std::string{ v });
            else if (toolpex::str_case_equal(k, "Port"sv))
                result.set_port(static_cast<in_port_t>(atoi(v.data())));
        }
        else
        {
            if (toolpex::str_case_equal(item_str, "Discard"sv))
                result.set_discard();
            else if (toolpex::str_case_equal(item_str, "Secure"sv))
                result.set_secure();
        }
    }
    return result;
}

} // namespace koios::http
