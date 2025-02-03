#ifndef _CANARY2_HTTP_REQUEST_H_
#define _CANARY2_HTTP_REQUEST_H_

#include <memory>

#include "koioshttp/message_details.h"

namespace koios::http
{

template<typename MessageType>
class basic_request
{
public:
    using message_type = MessageType;
    using string_type = typename message_type::string_type;

public:
    basic_request(::std::shared_ptr<message_type> data) noexcept
        : m_data{ ::std::move(data) }
    {
    }

    version                             get_version()       const noexcept { return m_data->m_version;      }
    method                              get_method()        const noexcept { return m_data->m_method;       }        
    const ::std::vector<string_type>&   get_body()          const noexcept { return m_data->m_body_parts;   }  
    string_type                         get_url()           const noexcept { return m_data->m_url;          }
    string_type                         get_path()          const noexcept { return m_data->m_path;         }
    string_type                         get_query()         const noexcept { return m_data->m_query;        }
    string_type                         get_fragment()      const noexcept { return m_data->m_fragment;     }  
    bool                                get_keep_alive()    const noexcept { return m_data->m_keep_alive;   }
    bool                                get_web_socket()    const noexcept { return m_data->m_web_socket;   }

    const auto& headers() const noexcept { return this->m_headers; }

private:
    ::std::shared_ptr<message_type> m_data;
};

namespace server { using request = basic_request<details::message_from_net>; }
namespace client { using request = basic_request<details::message_to_net>; }

}

#endif
