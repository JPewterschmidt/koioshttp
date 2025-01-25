#ifndef _CANARY2_HTTP_REQUEST_H_
#define _CANARY2_HTTP_REQUEST_H_

#include <memory>

#include "koioshttp/message_details.h"

namespace koios::http
{

template<typename MessageType>
class basic_request : public MessageType
{
public:
    using message_type = MessageType;
    using string_type = typename message_type::string_type;

public:
    basic_request(message_type&& data)
        : m_data{ ::std::make_shared<message_type>(::std::move(data)) }
    {
    }

    basic_request(::std::shared_ptr<message_type> data_ptr) noexcept
        : m_data{ ::std::move(data_ptr) }
    {
    }

    version         get_version()       const noexcept { return this->m_version;      }
    method          get_method()        const noexcept { return this->m_method;       }        
    string_type     get_body()          const noexcept { return this->m_body;         }  
    string_type     get_url()           const noexcept { return this->m_url;          }
    string_type     get_path()          const noexcept { return this->m_path;         }
    string_type     get_query()         const noexcept { return this->m_query;        }
    string_type     get_fragment()      const noexcept { return this->m_fragment;     }  
    bool            get_keep_alive()    const noexcept { return this->m_keep_alive;   }
    bool            get_web_socket()    const noexcept { return this->m_web_socket;   }

    const auto& headers() const noexcept 
    { 
        toolpex_assert(!!m_data);
        return m_data->m_headers;   
    }
};

namespace server { using request = basic_request<details::message_from_net>; }
namespace client { using request = basic_request<details::message_to_net>; }

}

#endif
