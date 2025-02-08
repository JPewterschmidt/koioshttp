#ifndef KOIOSHTTP_ROUTER_H
#define KOIOSHTTP_ROUTER_H

#include <unordered_map>
#include <functional>
#include <memory>
#include <string>
#include <string_view>

#include "koios/task.h"
#include "koioshttp/request.h"
#include "koioshttp/response.h"

namespace koios::http
{

class router
{
public:
    using callback_t = ::std::function<task<server::response>(server::request)>;

public:
    router();

    router(router&& other) noexcept = default;
    router& operator=(router&& other) noexcept = default;

    ~router() noexcept;

private:
    struct router_node
    {
        ::std::unordered_map<::std::string, router_node*> m_subseg;
        callback_t m_callback;
    };

private:
    void recursive_delete(router_node* node) noexcept;

public:
    void insert(::std::string_view path, callback_t cb);
    callback_t find(::std::string_view path) const;

private:
    ::std::unique_ptr<router_node> m_root_node;
};

} // namespace koios::http

#endif
