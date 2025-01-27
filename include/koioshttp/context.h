#ifndef KOIOSHTTP_CONTEXT_H
#define KOIOSHTTP_CONTEXT_H

#include "toolpex/buffer.h"
#include "toolpex/unique_posix_fd.h"

#include "koios/generator.h"

#include "koioshttp/request.h"
#include "koioshttp/response.h"

namespace koios::http
{

class context
{
public:
    context(toolpex::unique_posix_fd fd) noexcept
        : m_fd{ ::std::move(fd) }
    {
    }

    toolpex::ip_address::ptr peer_ip() const;
    
    operator const toolpex::unique_posix_fd& () const noexcept { return m_fd; }
    operator int () noexcept { return m_fd; }

    generator<request> requests();
    task<> send(response rsp);

private:
    friend class server;

    toolpex::unique_posix_fd m_fd;
    toolpex::buffer m_buffer;
};

} // namespace koios::http

#endif
