#ifndef __HTTP_REQUEST_H
#define __HTTP_REQUEST_H

#include <string.h>
#include "main.h"
#include "httpd.h"
//#include "ip4_addr.h"
#include "lwip.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#include "tcp.h"

#define CUS_URL_TEST_OK 		"/ok"
#define CUS_URL_TEST_ERR 		"/err"

#define HTTP_OK_RESP 			"HTTP/1.1 200 OK\r\n\r\n"
#define HTTP_ERR_RESP 			"HTTP/1.1 404 Not Found\r\n\r\n"
#define HTTP_BAD_RESP           "HTTP/1.1 400 Bad Request\r\n\r\n"

/* Probably move this enums to http_request.c to eliminate conflicts */
typedef enum
{
  WEB_PAGE_LOGIN,
  WEB_PAGE_NETWORK,
  WEB_PAGE_UNKNOWN,
} rqst_t;

typedef enum
{
  MODE_CUSTOM,
  MODE_DHCP,
  MODE_UNKNOWN
} net_mode_t;


#endif /* __HTTP_REQUEST_H */
