#include "koioshttp/parse.h"
#include "toolpex/buffer.h"

namespace koios::http
{

task<::std::optional<request>> 
parse_request_from(
    const toolpex::unique_posix_fd& fd, 
    ::std::chrono::system_clock::time_point timeout)
{
    toolpex::buffer buf;
    
    bool one_parse_complete{};
    while (!one_parse_complete)
    {
        auto recv_ret = co_await recv(fd, buf.writable_span(), 0, timeout);
        if (recv_ret.error_code())
            co_return {};
        buf.commit(recv_ret.nbytes_delivered());

        
    }
    
    co_return {};
}

} // namspace koios::http
