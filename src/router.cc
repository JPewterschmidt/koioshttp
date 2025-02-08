#include "koioshttp/router.h"

#include <ranges>

namespace rv = ::std::ranges::views;
namespace r = ::std::ranges;

namespace koios::http
{

void router::recursive_delete(router_node* node) noexcept
{
    if (!node) return;
    
    auto& map = node->m_subseg;
    for (auto it = map.begin(); it != map.end(); ++it)
    {
        router_node* n = it->second;
        if (!n) continue;
        recursive_delete(n);
        delete n;
    }
}

router::~router() noexcept
{
    recursive_delete(m_root_node.get());
}

router::router()
    : m_root_node{ ::std::make_unique<router_node>() }
{
}

void router::insert(::std::string_view path, router::callback_t cb)
{
    auto splitted = path 
        | rv::split('/') 
        | rv::filter([](const auto& item) { return item.size() != 0; })
        | r::to<::std::vector<::std::string>>()
        ;

    router_node* current = m_root_node.get();
    for (auto str : splitted)
    {
        toolpex_assert(current);
        auto it = current->m_subseg.find(str);
        if (it != current->m_subseg.end())
        {
            current = it->second;
        }
        else
        {
            router_node* newnode =  new router_node{};
            current->m_subseg[str] = newnode;
            current = newnode;
        }
    }
    current->m_callback = ::std::move(cb);
}

router::callback_t router::find(::std::string_view path)
{
    auto splitted = path
        | rv::split('/')
        | rv::filter([](const auto& item) { return item.size() != 0; })
        | r::to<::std::vector<::std::string>>()
        ;

    router_node* current = m_root_node.get();
    for (auto str : splitted)
    {
        toolpex_assert(!!current);
        auto it = current->m_subseg.find(str);
        if (it == current->m_subseg.end())
            return {};
        current = it->second;
    }
    toolpex_assert(!!current);
    return current->m_callback;
}

} // namespace koios::http
