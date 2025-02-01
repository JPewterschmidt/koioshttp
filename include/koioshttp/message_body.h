#ifndef KOIOSHTTP_MESSAGE_BODY_H
#define KOIOSHTTP_MESSAGE_BODY_H

#include <string_view>
#include <vector>
#include <variant>
#include <chrono>

#include "toolpex/not_implemented.h"

#include "koios/task.h"
#include "koios/iouring_awaitables.h"


namespace koios::http
{

template<typename String>
class message_body
{
public:
    using string_type = String; 

    auto& append_body(::std::string_view val)
    {
        m_body_parts.emplace_back(string_type(val));
        return *this;
    }

    auto& append_body(toolpex::unique_posix_fd fd)
    {
        m_body_parts.emplace_back(::std::move(fd));
        return *this;
    }
    
    task<::std::error_code> sendto(const toolpex::unique_posix_fd& fd, ::std::chrono::system_clock::time_point tp)
    {
        for (auto& item : m_body_parts)
        {
            ::std::error_code ec{};
            const size_t idx = item.index();
            switch (idx)
            {
                case 0: co_await uring::append_all(fd, get<0>(item), ec); break;
                case 1: [[fallthrough]];
                default: toolpex::not_implemented(); break; 
            }
            if (ec) co_return ec;
        }

        co_return {};
    }

private:
    ::std::vector<::std::variant<string_view, toolpex::unqiue_posix_fd>> m_body_parts;
};

} // namespace koios::http

#endif
