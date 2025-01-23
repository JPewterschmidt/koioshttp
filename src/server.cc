#include "koioshttp/server.h"

namespace koios::http
{

void server::start()
{
    m_tcps.start([](toolpex::unique_posix_fd fd) -> lazy_task<> { 
        return make_lazy(m_callback, ::std::move(fd));
    }).run();
}

} // namespace koios::http
