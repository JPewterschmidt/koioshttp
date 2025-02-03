#include "koioshttp/response.h"

namespace koios::http
{
    template<> class basic_response<koios::http::details::message_from_net>;
    template<> class basic_response<koios::http::details::message_to_net>;
}
