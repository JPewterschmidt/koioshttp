#include "llhttp.h"

#include "toolpex/assert.h"
#include "toolpex/buffer.h"
#include "koios/iouring_op_functions.h"

#include "koioshttp/parse.h"
#include "koioshttp/concepts.h"
#include "koioshttp/request_builder.h"

namespace koios::http
{

namespace 
{
    using string_type = server::request_builder::string_type;

    static int on_url(::llhttp_t* p, const char* at, size_t length)
    {
        auto& builder = *reinterpret_cast<server::request_builder*>(p->data);
        builder.set_url(string_type{ at, length });
        return HPE_OK;
    }

    static int on_status(::llhttp_t* p, const char* at, size_t length)
    {
        auto& builder = *reinterpret_cast<server::request_builder*>(p->data);
        string_type status_str{ at, length };
        builder.set_status(to_status(status_str));
        return HPE_OK;
    }

    static int on_method(::llhttp_t* p, const char* at, size_t length)
    {
        auto& builder = *reinterpret_cast<server::request_builder*>(p->data);
        string_type method_str{ at, length };
        builder.set_method(to_method(method_str));
        return HPE_OK;
    }

    static int on_header_field(::llhttp_t* p, const char* at, size_t length)
    {
        auto& builder = *reinterpret_cast<server::request_builder*>(p->data);
        auto& header_item = builder->header_item_trampoline();
        TOOLPEX_ASSERT(header_item.first.empty());
        header_item.first = string_type{ at, length };

        return HPE_OK;
    }

    static int on_header_value(::llhttp_t* p, const char* at, size_t length)
    {
        auto& builder = *reinterpret_cast<server::request_builder*>(p->data);
        auto& header_item = builder->header_item_trampoline();
        TOOLPEX_ASSERT(!header_item.first.empty());
        header_item.second = string_type{ at, length };

        builder.set_header(::std::move(header_item));
        header_item = {};

        return HPE_OK;
    }

    static int on_body(::llhttp_t* p, const char* at, size_t length)
    {
        auto& builder = reinterpret_cast<server::request_builder*>(p->data)->builder();
        builder.set_body(string_type{ at, length }); // TODO suppose to be append body
        return HPE_OK;
    }


    // ::llhttp_cb

    static int on_message_begin(::llhttp_t* p)
    {
        auto& builder = reinterpret_cast<server::request_builder*>(p->data)->builder();
        return HPE_OK;
    }

    static int on_headers_complete([[maybe_unused]] ::llhttp_t*)
    {
        /* After the header parsed completely,
         * information about this request 
         * such as `method` `version` etc.
         * are ready, interrupt the parsing execution and 
         * return to the position where `llhttp_execute` called,
         * parser will fill those information into message object
         * by request/response builder's `set_*` methods.
         *
         * After those settings done, parser will call
         * `llhttp_resume` back to the llhttp parsing execution.
         */
        return HPE_PAUSED;
    }

    static int on_message_complete(::llhttp_t* p)
    {
        return HPE_OK;
    }

    static int on_chunk_header(::llhttp_t* p)
    {
        return HPE_OK;
    }

    static int on_chunk_complete(::llhttp_t* p)
    {
        return HPE_OK;
    }

    static int on_url_complete(::llhttp_t* p)
    {
        return HPE_OK;
    }

    static int on_status_complete(::llhttp_t* p)
    {
        return HPE_OK;
    }

    static int on_header_field_complete(::llhttp_t* p)
    {
        return HPE_OK;
    }

    static int on_header_value_complete(::llhttp_t* p)
    {
        return HPE_OK;
    }

    

} // namspace { annyomous }

generator<::std::optional<server::request>> 
parse_request_from(
    const toolpex::unique_posix_fd& fd, 
    ::std::chrono::milliseconds timeout)
{
    while (true)
    {
        server::request_builder builder;
        auto buffer_ptr = builder.storage();

        ::llhttp_t parser{};
        ::llhttp_settings_t settings{};

        struct parser_guard 
        {
            constexpr ~parser_guard() noexcept 
            {
            }

            parser_guard(::llhttp_t* p, ::llhttp_settings_t* s) noexcept
                : m_p{ p }, m_s{ s }
            {
                ::llhttp_settings_init(m_s);

                m_s->on_message_begin       = on_message_begin;
                m_s->on_message_complete    = on_message_complete;

                m_s->on_url                 = on_url;
                m_s->on_status              = on_status;
                m_s->on_method              = on_method;
                m_s->on_version             = on_version;

                m_s->on_header_field        = on_header_field;
                m_s->on_header_value        = on_header_value;

                m_s->on_body                = on_body;

                m_s->on_chunk_header        = on_chunk_header;
                m_s->on_chunk_complete      = on_chunk_complete;
                m_s->on_url_complete        = on_url_complete;
                m_s->on_status_complete     = on_status_complete;

                ::llhttp_init(m_p, HTTP_REQUEST, m_s);
            }
            
            ::llhttp_t* m_p;
            ::llhttp_settings_t* m_s;
        } _(&parser, &settings);

        
        auto recv_ret = co_await uring::recv(fd, buffer_ptr->writable_span(), 0, timeout);
        if (recv_ret.error_code())
            co_return {};

        buffer_ptr->commit_write(recv_ret.nbytes_delivered());
        
        ::llhttp_execute(
    }
}

} // namspace koios::http
