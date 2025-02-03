#ifndef KOIOSHTTP_CONCEPTS_H 
#define KOIOSHTTP_CONCEPTS_H 

#include "toolpex/is_specialization_of.h"

#include "koioshttp/request.h"
#include "koioshttp/response.h"

namespace koios::http
{
    template<typename T>
    concept is_request = toolpex::is_specialization_of<T, basic_request>;
    
    template<typename T>
    concept is_response = toolpex::is_specialization_of<T, basic_response>;
}

#endif
