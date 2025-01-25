#ifndef KOIOSHTTP_RESPONSE_H
#define KOIOSHTTP_RESPONSE_H

#include <memory>

#include "koioshttp/message_details.h"

namespace koios::http
{

template<typename MessageType>
class basic_response : public MessageType
{
public:
    using message_type = MessageType;
    using string_type = typename message_type::string_type;

public:
    basic_response(message_type&& data)
        : message_type{ ::std::move(data) }
    {
    }

    version         get_version()       const noexcept { return this->m_version;   }
    status          get_status()        const noexcept { return this->m_status;    }        
    string_type     get_body()          const noexcept { return this->m_body;      }  
    string_type     get_url()           const noexcept { return this->m_url;       }
    string_type     get_path()          const noexcept { return this->m_path;      }
    string_type     get_query()         const noexcept { return this->m_query;     }
    string_type     get_fragment()      const noexcept { return this->m_fragment;  }  
    bool            get_keep_alive()    const noexcept { return this->m_keep_alive;}
    bool            get_web_socket()    const noexcept { return this->m_web_socket;}
    const auto&     headers()           const noexcept { return this->m_headers;   }
};

namespace client { using response = basic_response<details::message_from_net>; }
namespace server { using response = basic_response<details::message_to_net>; }

}

#endif
