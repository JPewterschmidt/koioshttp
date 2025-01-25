#include "koioshttp/response.h"

namespace wilson::http
{
    template<> class basic_response<details::message_from_net>;
    template<> class basic_response<details::message_to_net>;
}
