#ifndef PROJECT_REQUEST_H
#define PROJECT_REQUEST_H

#include <glob.h>
#include <stdint.h>
#include "../utils/buffer.h"

/**  The SPCP request is formed as follows:
 *
 *      +-----+-------+----------+----------+...+----------+----------+
 *      | CMD | NARGS | ARGLEN 1 |   ARG 1  |   | ARGLEN N |   ARG N  |
 *      +-----+-------+----------+----------+...+----------+----------+
 *      |  1  |   1   |    1    | Variable |   |    1     | Variable |
 *      +-----+-------+----------+----------+...+----------+----------+
 *
 *   Where:
 *
 *        o  CMD
 *           o  USER                        X'00'
 *           o  PASS                        X'01'
 *           o  GET CONCURRENT CONNECTIONS  X'02'
 *           o  GET TRANSFERRED BYTES       X'03'
 *           o  GET HISTORICAL ACCESSES     X'04'
 *           o  GET ACTIVE TRANSFORMATION   X'05'
 *           o  QUIT                        X'06'
 *           o  SET BUFFER SIZE             X'07'
 *           o  CHANGE TRANSFORMATION       X'08'
 *           o  SET TIMEOUTS                X'09'
 *
 *
 *        o  NARGS      number of arguments sent
 *        o  ARGLEN N   length of the nth argument
 *        o  ARG N      the nth argument
 *
 */

enum spcp_request_state {
    request_cmd,
    request_nargs,
    request_arg_size,
    request_arg,

    request_done,

    request_error_invalid_command,
    request_error_invalid_arguments,
    request_error,
};

enum spcp_response_status {
    success             = 0x00,
    auth_err            = 0x01,
    invalid_command     = 0x02,
    invalid_arguments   = 0x03,
    err                 = 0x04,
};

enum spcp_request_cmd {
    user,
    pass,
    concurrent_connections,
    transfered_bytes,
    historical_accesses,
    active_transformation,
    set_buffer_size,
    change_transformation,
    set_timeouts,
    quit,
};

struct spcp_request {
    enum spcp_request_cmd cmd;
    uint8_t *arg0;
    uint8_t *arg1;
    size_t arg0_size;
    size_t arg1_size;
};

struct spcp_request_parser {
    struct spcp_request         *request;
    enum   spcp_request_state   state;
    /** cantidad de argumentos */
    uint8_t nargs;
    /** argumentos ya leidos */
    uint8_t nargs_i;
    /** ultimo narg parseado */
    uint8_t arg_size;
    /** bytes del argumento ya leidos*/
    uint8_t arg_size_i;


};

extern enum spcp_request_state
spcp_request_consume(buffer *b, struct spcp_request_parser *p, bool *errored);

extern void
spcp_request_parser_init(struct spcp_request_parser *p);

extern int
spcp_no_data_request_marshall(buffer *b, uint8_t status);

extern int
spcp_data_request_marshall(buffer *b, uint8_t status, char *data);

#endif