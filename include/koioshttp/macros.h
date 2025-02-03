#ifndef KOIOSHTTP_MACROS_H 
#define KOIOSHTTP_MACROS_H 

/* Request Methods */
#define KH_HTTP_METHOD_MAP(XX)         \
  XX(0U,  DELETE,      "DELETE")       \
  XX(1U,  GET,         "GET")          \
  XX(2U,  HEAD,        "HEAD")         \
  XX(3U,  POST,        "POST")         \
  XX(4U,  PUT,         "PUT")          \
  /* pathological */                   \
  XX(5U,  CONNECT,     "CONNECT")      \
  XX(6U,  OPTIONS,     "OPTIONS")      \
  XX(7U,  TRACE,       "TRACE")        \
  /* WebDAV */                         \
  XX(8U,  COPY,        "COPY")         \
  XX(9U,  LOCK,        "LOCK")         \
  XX(10U, MKCOL,       "MKCOL")        \
  XX(11U, MOVE,        "MOVE")         \
  XX(12U, PROPFIND,    "PROPFIND")     \
  XX(13U, PROPPATCH,   "PROPPATCH")    \
  XX(14U, SEARCH,      "SEARCH")       \
  XX(15U, UNLOCK,      "UNLOCK")       \
  XX(16U, BIND,        "BIND")         \
  XX(17U, REBIND,      "REBIND")       \
  XX(18U, UNBIND,      "UNBIND")       \
  XX(19U, ACL,         "ACL")          \
  /* subversion */                     \
  XX(20U, REPORT,      "REPORT")       \
  XX(21U, MKACTIVITY,  "MKACTIVITY")   \
  XX(22U, CHECKOUT,    "CHECKOUT")     \
  XX(23U, MERGE,       "MERGE")        \
  /* upnp */                           \
  XX(24U, MSEARCH,     "M-SEARCH")     \
  XX(25U, NOTIFY,      "NOTIFY")       \
  XX(26U, SUBSCRIBE,   "SUBSCRIBE")    \
  XX(27U, UNSUBSCRIBE, "UNSUBSCRIBE")  \
  /* RFC-5789 */                       \
  XX(28U, PATCH,       "PATCH")        \
  XX(29U, PURGE,       "PURGE")        \
  /* CalDAV */                         \
  XX(30U, MKCALENDAR,  "MKCALENDAR")   \
  /* RFC-2068, section" 19.6.1.2 */    \
  XX(31U, LINK,        "LINK")         \
  XX(32U, UNLINK,      "UNLINK")       \
  /* icecast */                        \
  XX(33U, SOURCE,      "SOURCE")       \

/* Status Codes */
#define KH_HTTP_STATUS_MAP(XX)                                                 \
  XX(100U, CONTINUE,                        "Continue")                        \
  XX(101U, SWITCHING_PROTOCOLS,             "Switching Protocols")             \
  XX(102U, PROCESSING,                      "Processing")                      \
  XX(200U, OK,                              "OK")                              \
  XX(201U, CREATED,                         "Created")                         \
  XX(202U, ACCEPTED,                        "Accepted")                        \
  XX(203U, NON_AUTHORITATIVE_INFORMATION,   "Non-Authoritative Information")   \
  XX(204U, NO_CONTENT,                      "No Content")                      \
  XX(205U, RESET_CONTENT,                   "Reset Content")                   \
  XX(206U, PARTIAL_CONTENT,                 "Partial Content")                 \
  XX(207U, MULTI_STATUS,                    "Multi-Status")                    \
  XX(208U, ALREADY_REPORTED,                "Already Reported")                \
  XX(226U, IM_USED,                         "IM Used")                         \
  XX(300U, MULTIPLE_CHOICES,                "Multiple Choices")                \
  XX(301U, MOVED_PERMANENTLY,               "Moved Permanently")               \
  XX(302U, FOUND,                           "Found")                           \
  XX(303U, SEE_OTHER,                       "See Other")                       \
  XX(304U, NOT_MODIFIED,                    "Not Modified")                    \
  XX(305U, USE_PROXY,                       "Use Proxy")                       \
  XX(307U, TEMPORARY_REDIRECT,              "Temporary Redirect")              \
  XX(308U, PERMANENT_REDIRECT,              "Permanent Redirect")              \
  XX(400U, BAD_REQUEST,                     "Bad Request")                     \
  XX(401U, UNAUTHORIZED,                    "Unauthorized")                    \
  XX(402U, PAYMENT_REQUIRED,                "Payment Required")                \
  XX(403U, FORBIDDEN,                       "Forbidden")                       \
  XX(404U, NOT_FOUND,                       "Not Found")                       \
  XX(405U, METHOD_NOT_ALLOWED,              "Method Not Allowed")              \
  XX(406U, NOT_ACCEPTABLE,                  "Not Acceptable")                  \
  XX(407U, PROXY_AUTHENTICATION_REQUIRED,   "Proxy Authentication Required")   \
  XX(408U, REQUEST_TIMEOUT,                 "Request Timeout")                 \
  XX(409U, CONFLICT,                        "Conflict")                        \
  XX(410U, GONE,                            "Gone")                            \
  XX(411U, LENGTH_REQUIRED,                 "Length Required")                 \
  XX(412U, PRECONDITION_FAILED,             "Precondition Failed")             \
  XX(413U, PAYLOAD_TOO_LARGE,               "Payload Too Large")               \
  XX(414U, URI_TOO_LONG,                    "URI Too Long")                    \
  XX(415U, UNSUPPORTED_MEDIA_TYPE,          "Unsupported Media Type")          \
  XX(416U, RANGE_NOT_SATISFIABLE,           "Range Not Satisfiable")           \
  XX(417U, EXPECTATION_FAILED,              "Expectation Failed")              \
  XX(421U, MISDIRECTED_REQUEST,             "Misdirected Request")             \
  XX(422U, UNPROCESSABLE_ENTITY,            "Unprocessable Entity")            \
  XX(423U, LOCKED,                          "Locked")                          \
  XX(424U, FAILED_DEPENDENCY,               "Failed Dependency")               \
  XX(426U, UPGRADE_REQUIRED,                "Upgrade Required")                \
  XX(428U, PRECONDITION_REQUIRED,           "Precondition Required")           \
  XX(429U, TOO_MANY_REQUESTS,               "Too Many Requests")               \
  XX(431U, REQUEST_HEADER_FIELDS_TOO_LARGE, "Request Header Fields Too Large") \
  XX(451U, UNAVAILABLE_FOR_LEGAL_REASONS,   "Unavailable For Legal Reasons")   \
  XX(500U, INTERNAL_SERVER_ERROR,           "Internal Server Error")           \
  XX(501U, NOT_IMPLEMENTED,                 "Not Implemented")                 \
  XX(502U, BAD_GATEWAY,                     "Bad Gateway")                     \
  XX(503U, SERVICE_UNAVAILABLE,             "Service Unavailable")             \
  XX(504U, GATEWAY_TIMEOUT,                 "Gateway Timeout")                 \
  XX(505U, HTTP_VERSION_NOT_SUPPORTED,      "HTTP Version Not Supported")      \
  XX(506U, VARIANT_ALSO_NEGOTIATES,         "Variant Also Negotiates")         \
  XX(507U, INSUFFICIENT_STORAGE,            "Insufficient Storage")            \
  XX(508U, LOOP_DETECTED,                   "Loop Detected")                   \
  XX(510U, NOT_EXTENDED,                    "Not Extended")                    \
  XX(511U, NETWORK_AUTHENTICATION_REQUIRED, "Network Authentication Required") \


#endif
