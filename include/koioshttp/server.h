#ifndef KOIOSHTTP_SERVER_H
#define KOIOSHTTP_SERVER_H

#include "koios/tcp_server.h"
#include "koioshttp/context.h"

namespace koios::http
{

class server
{
public:
    server(toolpex::ip_address::ptr addr, 
           ::in_port_t port, ::std::move_only_function<task<>(context)> callback)
        : m_tcps{ ::std::move(addr), port }, m_callback{ ::std::move(callback) }
    {
        start();
    }

    server(::in_port_t port, ::std::function<task<>(context)> callback)
        : m_tcps{ port }, m_callback{ ::std::move(callback) }
    {
        start();
    }

private:
    void start();

private:
    tcp_server m_tcps;   
    ::std::move_only_function<task<>(context)> m_callback;
};

} // namespace koios::http

#endif
