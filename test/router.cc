#include "gtest/gtest.h"
#include "koioshttp/router.h"
#include "koioshttp/request.h"
#include "koioshttp/response.h"

using namespace koios::http;

namespace
{
    bool test_success[3]{};
}

TEST(router, basic)
{
    router r;
    r.insert("/hello/world",            [] (server::request) -> koios::task<server::response> { test_success[0] = true; co_return {}; });
    r.insert("/hello",                  [] (server::request) -> koios::task<server::response> { test_success[1] = true; co_return {}; });
    r.insert("/hello/wilson/jeremy",    [] (server::request) -> koios::task<server::response> { test_success[2] = true; co_return {}; });
    
    r.find("hello")({}).result();
    r.find("hello/wilson/jeremy")({}).result();
    r.find("hello/world")({}).result();

    ASSERT_TRUE(test_success[0]);
    ASSERT_TRUE(test_success[1]);
    ASSERT_TRUE(test_success[2]);
}
