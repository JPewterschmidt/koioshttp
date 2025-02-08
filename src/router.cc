#include "koioshttp/router.h"

#include <ranges>
#include <utility>
#include <vector>
#include <list>

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

static auto make_segments_vec(::std::string_view path)
{
    return path
        | rv::split('/') 
        | rv::filter([](const auto& item) { return item.size() != 0; })
        | r::to<::std::vector<::std::string>>()
        ;
}

void router::insert(::std::string_view path, router::callback_t cb)
{
    auto splitted = make_segments_vec(path);

    router_node* current = m_root_node.get();
    for (auto str : splitted)
    {
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

router::callback_t router::find(::std::string_view path) const
{
    auto splitted = make_segments_vec(path);

    router_node* current = m_root_node.get();
    for (auto str : splitted)
    {
        auto it = current->m_subseg.find(str);
        if (it == current->m_subseg.end())
            return {};
        current = it->second;
    }
    toolpex_assert(!!current);
    return current->m_callback;
}

router::callback_t router::remove(::std::string_view path)
{
    auto splitted = make_segments_vec(path);

    router_node* current = m_root_node.get();
    ::std::list<router_node*> back_trace;
    for (auto str : splitted)
    {
        auto it = current->m_subseg.find(str);
        if (it == current->m_subseg.end())
            return {};
        back_trace.push_front(::std::exchange(current, it->second));
    }

    auto result = ::std::exchange(current->m_callback, nullptr);
    if (current->m_subseg.empty())
    {
        decltype(back_trace)::iterator it;
        for (it = back_trace.begin(); it != back_trace.end(); ++it)
        {
            if ((**it).m_subseg.size() != 1)
                break;
        }
        if (it != back_trace.end())
        {
            delete *it;
        }
              
        delete current;
    }

    return result;
}

} // namespace koios::http
