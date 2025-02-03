#include <chrono>
#include <string_view>
#include <semaphore>

#include "gtest/gtest.h"

#include "koioshttp/useful.h"

using namespace ::std::chrono_literals;
using namespace toolpex::ip_address_literals;
using namespace koios;
using namespace koios::http;

namespace
{

bool commu_success{}, version_success{}, method_success{};
::std::binary_semaphore sem{ 0 };

lazy_task<> recver()
{
    tcp_server s(8890);
    co_await s.start([&s](toolpex::unique_posix_fd fd) -> task<> { 
        {
            auto gen = koios::http::parse_request_from(fd, 10s);
            auto req_opt = (co_await gen.next_value_async()).value_or(::std::nullopt);
            commu_success = req_opt.has_value();       
            version_success = req_opt.value().get_version() == version::HTTP_1_1;
            method_success = req_opt.value().get_method() == method::GET;
        }
        
        s.stop();
    });
    sem.release();
    co_await s.until_done_async();
}

lazy_task<> send_dummy()
{
    ::std::string_view dummy_req =
        "GET /test HTTP/1.1\r\n"
        "Host: example.com\r\n"
        "User-Agent: curl/7.81.0\r\n"
        "Accept: */*\r\n"
        "\r\n";

    auto fd = co_await koios::uring::connect_get_sock("127.0.0.1"_ip, 8890);
    co_await koios::uring::append_all(fd, dummy_req);
}

} // namespace { annoymous }

TEST(req_parse, basic)
{
    auto f = recver().run_and_get_future();
    sem.acquire();
    send_dummy().result();
    f.get();
    ASSERT_TRUE(commu_success && version_success && method_success);
}
