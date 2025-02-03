#include "koioshttp/response_builder.h"

namespace koios::http
{
    template<> class basic_response_builder<details::message_from_net>;
    template<> class basic_response_builder<details::message_to_net>;
}
