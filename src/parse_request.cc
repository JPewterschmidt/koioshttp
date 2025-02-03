#include <string_view>

#include "llhttp.h"
#include "spdlog/spdlog.h"

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

    static inline server::request_builder& builder_ref(::llhttp_t* p)
    {
        toolpex_assert(!!p->data);
        return *reinterpret_cast<server::request_builder*>(p->data);
    }

    static int on_url(::llhttp_t* p, const char* at, size_t length)
    {
        auto& builder = builder_ref(p);
        builder.set_url(::std::string_view{ at, length });
        return HPE_OK;
    }

    static int on_method(::llhttp_t* p, const char* at, size_t length)
    {
        auto& builder = builder_ref(p);
        ::std::string_view method_str{ at, length };
        builder.set_method(to_method(method_str));
        return HPE_OK;
    }

    static int on_version(::llhttp_t* p, const char* at, size_t length)
    {
        builder_ref(p).set_version(to_version(::std::string_view{ at, length }));
        return HPE_OK;
    }

    static int on_header_field(::llhttp_t* p, const char* at, size_t length)
    {
        auto& builder = builder_ref(p);
        auto& header_item = builder.header_item_trampoline();
        toolpex_assert(header_item.first.empty());
        header_item.first = string_type{ at, length };

        return HPE_OK;
    }

    static int on_header_value(::llhttp_t* p, const char* at, size_t length)
    {
        auto& builder = builder_ref(p);
        auto& header_item = builder.header_item_trampoline();
        toolpex_assert(!header_item.first.empty());
        header_item.second = string_type{ at, length };

        builder.set_header(::std::move(header_item));
        header_item = {};

        return HPE_OK;
    }

    static int on_body(::llhttp_t* p, const char* at, size_t length)
    {
        auto& builder = builder_ref(p);
        builder.append_body(string_type{ at, length }); // TODO suppose to be append body
        return HPE_OK;
    }


    // ::llhttp_cb

    static int on_message_begin(::llhttp_t* p)
    {
        auto& builder = builder_ref(p);
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

    static void init_parser(::llhttp_t* parser, ::llhttp_settings_t* setting)
    {
        ::llhttp_settings_init(setting);

        setting->on_url                 = on_url;
        setting->on_method              = on_method;
        setting->on_version             = on_version;
        setting->on_header_field        = on_header_field;
        setting->on_header_value        = on_header_value;
        setting->on_body                = on_body;
        setting->on_chunk_header        = on_chunk_header;
        setting->on_chunk_complete      = on_chunk_complete;
        setting->on_url_complete        = on_url_complete;
        setting->on_message_begin       = on_message_begin;
        setting->on_message_complete    = on_message_complete;
    
        ::llhttp_init(parser, HTTP_REQUEST, setting);
    }

    task<bool> 
    fill_buffer(const toolpex::unique_posix_fd& fd, 
                toolpex::buffer& in_out_buffer,
                ::std::chrono::system_clock::time_point timeout_tp)
    {
        auto recv_ret = co_await uring::recv(fd, in_out_buffer.writable_span(), 0, timeout_tp);
        if (auto ec = recv_ret.error_code(); ec && koios::is_timeout_ec(ec))
        {
            co_return false;
        }
        else if (!ec)
        {
            // Success
        }
        else
        {
            spdlog::error("parse_request_from: pos2 err = {}", ec.message());
            co_return false;
        }
        in_out_buffer.commit_write(recv_ret.nbytes_delivered());
        co_return true;
    }

    // TODO: test
    size_t get_parsed_nbytes(const ::llhttp_t* parser, ::std::span<const char> sp)
    {
        const char* current_pos = reinterpret_cast<const char*>(parser->_span_pos0);
        const size_t parsed = current_pos - sp.data();
        return parsed;
    }

} // namspace { annyomous }

generator<::std::optional<server::request>> 
parse_request_from(
    const toolpex::unique_posix_fd& fd, 
    ::std::chrono::milliseconds timeout)
{
    ::std::span<char> parsing_bytes;
    ::llhttp_t parser{};
    ::llhttp_settings_t settings{};

    while (true)
    {
        server::request_builder builder;
        auto& buffer = builder.storage();
        
        init_parser(&parser, &settings);
        parser.data = &builder;
        ::std::chrono::system_clock::time_point timeout_tp = ::std::chrono::system_clock::now() + timeout;

        if (!co_await fill_buffer(fd, buffer, timeout_tp))
            co_yield ::std::nullopt;
        
        bool buffer_exhausted{};
        while (!buffer_exhausted)
        {
            toolpex::buffer_lens<char> lens(buffer);
            auto sp = lens.next_readable_span();

            // Successfully buffer_exhausted a request message.
            if (::llhttp_errno_t lle = ::llhttp_execute(&parser, sp.data(), sp.size()); lle == HPE_OK)
            {
                co_yield builder.generate_request();
            }

            // Need continue reading.
            else if (lle == HPE_PAUSED)
            {
                if (!co_await fill_buffer(fd, buffer, timeout_tp))
                {
                    buffer_exhausted = true;
                    co_yield ::std::nullopt;
                }
                continue;
            }

            // Other error.
            else 
            {
                spdlog::error("parse_request_from::llhttp_parsed: err = {}", ::llhttp_errno_name(lle));
                buffer_exhausted = true;
                co_yield ::std::nullopt;
            }

            const size_t parsed_nbytes = get_parsed_nbytes(&parser, sp);
            lens.commit_read(parsed_nbytes);
            buffer_exhausted = parsed_nbytes == sp.size();
        }
    }
}

} // namspace koios::http
