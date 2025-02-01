#include "koioshttp/request_builder.h"

namespace koios::http
{
    template<> class basic_request_builder<details::message_from_net>;
    template<> class basic_request_builder<details::message_to_net>;
}
