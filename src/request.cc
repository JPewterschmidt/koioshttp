#include "koioshttp/request.h"

namespace koios::http
{
    template<> class basic_request<details::message_from_net>;
    template<> class basic_request<details::message_to_net>;
}
