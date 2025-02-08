#include "gtest/gtest.h"
#include "koioshttp/router.h"
#include "koioshttp/request.h"
#include "koioshttp/response.h"

using namespace koios::http;

namespace
{
    bool test_success[3]{};
    router get_router()
    {
        router r;
        r.insert("/hello/world",            [] (server::request) -> koios::task<server::response> { test_success[0] = true; co_return {}; });
        r.insert("/hello",                  [] (server::request) -> koios::task<server::response> { test_success[1] = true; co_return {}; });
        r.insert("/hello/wilson/jeremy",    [] (server::request) -> koios::task<server::response> { test_success[2] = true; co_return {}; });
        return r;
    }
}

TEST(router, basic)
{
    router r = get_router();
    (void)r.find("hello")({}).result();
    (void)r.find("hello/wilson/jeremy")({}).result();
    (void)r.find("hello/world")({}).result();

    ASSERT_TRUE(test_success[0]);
    ASSERT_TRUE(test_success[1]);
    ASSERT_TRUE(test_success[2]);
}

TEST(router, remove)
{
    router r = get_router();
    ASSERT_EQ(r.remove("/bye"), nullptr);
    ASSERT_TRUE(r.remove("/hello") != nullptr);
    ASSERT_TRUE(r.remove("/hello") == nullptr);
}
