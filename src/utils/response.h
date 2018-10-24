#ifndef PROJECT_RESPONSE_H
#define PROJECT_RESPONSE_H

#include <stdint.h>
#include "buffer.h"
#include "request.h"

enum response_state {
    response_detect_status,
    response_new_line,
    response_dot,
    response_dot_cr,
    response_byte,
    response_cr,
    response_done,
    response_error,
};

struct response_parser {
    enum response_state response_state;
    bool pop3_response_success;
    struct request* request;
};

void
response_parser_init(struct response_parser *p, struct request *request);

bool
response_is_done(enum response_state st, bool *errored);

enum response_state
response_consume(buffer *rb, buffer *wb, struct response_parser *p, bool *errored);

void
response_close(struct response_parser *p);

#endif //PROJECT_RESPONSE_H
