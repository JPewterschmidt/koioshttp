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
    constexpr basic_response() noexcept = default;
    basic_response(message_type&& data)
        : m_data{ ::std::make_shared<message_type>(::std::move(data)) }
    {
    }

    version         get_version()       const noexcept { return m_data->m_version;   }
    status          get_status()        const noexcept { return m_data->m_status;    }        
    string_type     get_body()          const noexcept { return m_data->m_body;      }  
    string_type     get_url()           const noexcept { return m_data->m_url;       }
    string_type     get_path()          const noexcept { return m_data->m_path;      }
    string_type     get_query()         const noexcept { return m_data->m_query;     }
    string_type     get_fragment()      const noexcept { return m_data->m_fragment;  }  
    bool            get_keep_alive()    const noexcept { return m_data->m_keep_alive;}
    bool            get_web_socket()    const noexcept { return m_data->m_web_socket;}
    const auto&     headers()           const noexcept { return m_data->m_headers;   }

private:
    ::std::shared_ptr<message_type> m_data;
};

namespace client { using response = basic_response<details::message_from_net>; }
namespace server { using response = basic_response<details::message_to_net>; }

}

#endif
