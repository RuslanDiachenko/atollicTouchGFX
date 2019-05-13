#include "fs.h"
#include "lwip/def.h"
#include "fsdata.h"


#define file_NULL (struct fsdata_file *) NULL


static const unsigned int dummy_align__main_shtml = 0;
static const unsigned char data__main_shtml[] = {
/* /main.shtml (12 chars) */
0x2f,0x6d,0x61,0x69,0x6e,0x2e,0x73,0x68,0x74,0x6d,0x6c,0x00,

/* HTTP header */
/* "HTTP/1.0 200 OK
" (17 bytes) */
0x48,0x54,0x54,0x50,0x2f,0x31,0x2e,0x30,0x20,0x32,0x30,0x30,0x20,0x4f,0x4b,0x0d,
0x0a,
/* "Server: SageGlass
" (19 bytes) */
0x53,0x65,0x72,0x76,0x65,0x72,0x3a,0x20,0x53,0x61,0x67,0x65,0x47,0x6c,0x61,0x73,
0x73,0x0d,0x0a,
/* "Content-type: text/html
Expires: Fri, 10 Apr 2008 14:00:00 GMT
Pragma: no-cache

" (85 bytes) */
0x43,0x6f,0x6e,0x74,0x65,0x6e,0x74,0x2d,0x74,0x79,0x70,0x65,0x3a,0x20,0x74,0x65,
0x78,0x74,0x2f,0x68,0x74,0x6d,0x6c,0x0d,0x0a,0x45,0x78,0x70,0x69,0x72,0x65,0x73,
0x3a,0x20,0x46,0x72,0x69,0x2c,0x20,0x31,0x30,0x20,0x41,0x70,0x72,0x20,0x32,0x30,
0x30,0x38,0x20,0x31,0x34,0x3a,0x30,0x30,0x3a,0x30,0x30,0x20,0x47,0x4d,0x54,0x0d,
0x0a,0x50,0x72,0x61,0x67,0x6d,0x61,0x3a,0x20,0x6e,0x6f,0x2d,0x63,0x61,0x63,0x68,
0x65,0x0d,0x0a,0x0d,0x0a,
/* raw file data (3419 bytes) */
0x3c,0x21,0x64,0x6f,0x63,0x74,0x79,0x70,0x65,0x20,0x3e,0x0a,0x3c,0x68,0x74,0x6d,
0x6c,0x20,0x20,0x6c,0x61,0x6e,0x67,0x3d,0x22,0x65,0x6e,0x22,0x3e,0x0a,0x20,0x20,
0x20,0x20,0x3c,0x68,0x65,0x61,0x64,0x3e,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x3c,0x6d,0x65,0x74,0x61,0x20,0x63,0x68,0x61,0x72,0x73,0x65,0x74,0x3d,0x22,
0x75,0x74,0x66,0x2d,0x38,0x22,0x3e,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x3c,0x6d,0x65,0x74,0x61,0x20,0x68,0x74,0x74,0x70,0x2d,0x65,0x71,0x75,0x69,0x76,
0x3d,0x22,0x58,0x2d,0x55,0x41,0x2d,0x43,0x6f,0x6d,0x70,0x61,0x74,0x69,0x62,0x6c,
0x65,0x22,0x20,0x63,0x6f,0x6e,0x74,0x65,0x6e,0x74,0x3d,0x22,0x49,0x45,0x3d,0x65,
0x64,0x67,0x65,0x2c,0x63,0x68,0x72,0x6f,0x6d,0x65,0x3d,0x31,0x22,0x3e,0x0a,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x3c,0x74,0x69,0x74,0x6c,0x65,0x3e,0x3c,0x2f,
0x74,0x69,0x74,0x6c,0x65,0x3e,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x3c,
0x6d,0x65,0x74,0x61,0x20,0x6e,0x61,0x6d,0x65,0x3d,0x22,0x64,0x65,0x73,0x63,0x72,
0x69,0x70,0x74,0x69,0x6f,0x6e,0x22,0x20,0x63,0x6f,0x6e,0x74,0x65,0x6e,0x74,0x3d,
0x22,0x22,0x3e,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x3c,0x6d,0x65,0x74,
0x61,0x20,0x6e,0x61,0x6d,0x65,0x3d,0x22,0x76,0x69,0x65,0x77,0x70,0x6f,0x72,0x74,
0x22,0x20,0x63,0x6f,0x6e,0x74,0x65,0x6e,0x74,0x3d,0x22,0x77,0x69,0x64,0x74,0x68,
0x3d,0x64,0x65,0x76,0x69,0x63,0x65,0x2d,0x77,0x69,0x64,0x74,0x68,0x2c,0x20,0x69,
0x6e,0x69,0x74,0x69,0x61,0x6c,0x2d,0x73,0x63,0x61,0x6c,0x65,0x3d,0x31,0x2c,0x20,
0x73,0x68,0x72,0x69,0x6e,0x6b,0x2d,0x74,0x6f,0x2d,0x66,0x69,0x74,0x3d,0x6e,0x6f,
0x22,0x3e,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x3c,0x6c,0x69,0x6e,0x6b,
0x20,0x72,0x65,0x6c,0x3d,0x22,0x73,0x74,0x79,0x6c,0x65,0x73,0x68,0x65,0x65,0x74,
0x22,0x20,0x68,0x72,0x65,0x66,0x3d,0x22,0x2e,0x2f,0x73,0x74,0x79,0x6c,0x65,0x73,
0x2e,0x63,0x73,0x73,0x22,0x3e,0x0a,0x20,0x20,0x20,0x20,0x3c,0x2f,0x68,0x65,0x61,
0x64,0x3e,0x0a,0x20,0x20,0x20,0x20,0x3c,0x62,0x6f,0x64,0x79,0x3e,0x0a,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x3c,0x64,0x69,0x76,0x20,0x63,0x6c,0x61,0x73,0x73,
0x3d,0x22,0x77,0x72,0x61,0x70,0x22,0x3e,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x3c,0x66,0x6f,0x72,0x6d,0x20,0x61,0x63,0x74,0x69,0x6f,
0x6e,0x3d,0x22,0x2f,0x61,0x63,0x74,0x69,0x6f,0x6e,0x5f,0x70,0x61,0x67,0x65,0x20,
0x22,0x20,0x6d,0x65,0x74,0x68,0x6f,0x64,0x3d,0x22,0x50,0x4f,0x53,0x54,0x22,0x20,
0x63,0x6c,0x61,0x73,0x73,0x3d,0x22,0x66,0x6f,0x72,0x6d,0x2d,0x77,0x72,0x61,0x70,
0x22,0x3e,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x3c,0x64,0x69,0x76,0x20,0x63,0x6c,0x61,0x73,0x73,0x3d,0x22,0x77,
0x72,0x61,0x70,0x20,0x63,0x75,0x73,0x74,0x6f,0x6d,0x2d,0x63,0x6f,0x6e,0x66,0x69,
0x67,0x22,0x3e,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x3c,0x69,0x6e,
0x70,0x75,0x74,0x20,0x74,0x79,0x70,0x65,0x3d,0x22,0x72,0x61,0x64,0x69,0x6f,0x22,
0x20,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x6e,0x61,
0x6d,0x65,0x3d,0x22,0x6d,0x6f,0x64,0x65,0x22,0x20,0x0a,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x76,0x61,0x6c,0x75,0x65,0x3d,0x22,0x63,0x75,
0x73,0x74,0x6f,0x6d,0x22,0x20,0x20,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x69,0x64,0x3d,0x22,0x63,0x75,0x73,0x74,0x6f,0x6d,0x43,0x6f,
0x6e,0x66,0x69,0x67,0x22,0x3e,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x3c,0x6c,0x61,0x62,0x65,
0x6c,0x20,0x66,0x6f,0x72,0x3d,0x22,0x63,0x75,0x73,0x74,0x6f,0x6d,0x43,0x6f,0x6e,
0x66,0x69,0x67,0x22,0x20,0x63,0x6c,0x61,0x73,0x73,0x3d,0x22,0x72,0x61,0x64,0x69,
0x6f,0x73,0x74,0x79,0x6c,0x65,0x22,0x3e,0x43,0x75,0x73,0x74,0x74,0x6f,0x6d,0x20,
0x43,0x6f,0x6e,0x66,0x69,0x67,0x3c,0x2f,0x6c,0x61,0x62,0x65,0x6c,0x3e,0x0a,0x0a,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x3c,0x69,0x6e,0x70,0x75,0x74,0x20,0x74,0x79,0x70,0x65,0x3d,
0x22,0x72,0x61,0x64,0x69,0x6f,0x22,0x20,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x6e,0x61,0x6d,0x65,0x3d,0x22,0x6d,0x6f,0x64,0x65,0x22,
0x20,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x76,0x61,
0x6c,0x75,0x65,0x3d,0x22,0x64,0x68,0x63,0x70,0x22,0x20,0x0a,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x69,0x64,0x3d,0x22,0x64,0x68,0x63,0x70,
0x22,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x63,0x68,
0x65,0x63,0x6b,0x65,0x64,0x3e,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x3c,0x6c,0x61,0x62,0x65,
0x6c,0x20,0x66,0x6f,0x72,0x3d,0x22,0x64,0x68,0x63,0x70,0x22,0x20,0x63,0x6c,0x61,
0x73,0x73,0x3d,0x22,0x72,0x61,0x64,0x69,0x6f,0x73,0x74,0x79,0x6c,0x65,0x22,0x3e,
0x44,0x48,0x43,0x50,0x3c,0x2f,0x6c,0x61,0x62,0x65,0x6c,0x3e,0x0a,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x3c,0x64,0x69,0x76,0x20,0x69,0x64,0x3d,0x22,0x63,
0x75,0x73,0x74,0x6f,0x6d,0x57,0x72,0x61,0x70,0x22,0x3e,0x0a,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x3c,0x6c,0x61,0x62,0x65,0x6c,0x20,0x66,0x6f,0x72,0x3d,0x22,
0x66,0x6e,0x61,0x6d,0x65,0x22,0x3e,0x49,0x50,0x3c,0x2f,0x6c,0x61,0x62,0x65,0x6c,
0x3e,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x3c,0x69,0x6e,0x70,0x75,0x74,
0x20,0x74,0x79,0x70,0x65,0x3d,0x22,0x74,0x65,0x78,0x74,0x22,0x20,0x0a,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x69,0x64,
0x3d,0x22,0x69,0x70,0x22,0x20,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x6e,0x61,0x6d,0x65,0x3d,0x22,0x69,0x70,0x22,
0x20,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x70,0x6c,0x61,0x63,0x65,0x68,0x6f,0x6c,0x64,0x65,0x72,0x3d,0x22,0x49,
0x50,0x22,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x72,0x65,0x71,0x75,0x69,0x72,0x65,0x64,0x20,0x0a,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x70,0x61,0x74,
0x74,0x65,0x72,0x6e,0x3d,0x22,0x5c,0x64,0x7b,0x31,0x2c,0x33,0x7d,0x5c,0x2e,0x5c,
0x64,0x7b,0x31,0x2c,0x33,0x7d,0x5c,0x2e,0x5c,0x64,0x7b,0x31,0x2c,0x33,0x7d,0x5c,
0x2e,0x5c,0x64,0x7b,0x31,0x2c,0x33,0x7d,0x22,0x3e,0x0a,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x0a,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x3c,0x6c,0x61,0x62,0x65,0x6c,0x20,
0x66,0x6f,0x72,0x3d,0x22,0x6c,0x6e,0x61,0x6d,0x65,0x22,0x3e,0x4e,0x65,0x74,0x77,
0x6f,0x72,0x6b,0x20,0x4d,0x61,0x73,0x6b,0x3c,0x2f,0x6c,0x61,0x62,0x65,0x6c,0x3e,
0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x3c,0x69,0x6e,0x70,0x75,0x74,0x20,
0x74,0x79,0x70,0x65,0x3d,0x22,0x74,0x65,0x78,0x74,0x22,0x20,0x0a,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x69,0x64,0x3d,
0x22,0x6e,0x65,0x74,0x77,0x6f,0x72,0x6b,0x4d,0x61,0x73,0x6b,0x22,0x20,0x0a,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x6e,
0x61,0x6d,0x65,0x3d,0x22,0x6e,0x65,0x74,0x77,0x6f,0x72,0x6b,0x4d,0x61,0x73,0x6b,
0x22,0x20,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x70,0x6c,0x61,0x63,0x65,0x68,0x6f,0x6c,0x64,0x65,0x72,0x3d,0x22,
0x4e,0x65,0x74,0x77,0x6f,0x72,0x6b,0x20,0x4d,0x61,0x73,0x6b,0x22,0x0a,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x72,0x65,
0x71,0x75,0x69,0x72,0x65,0x64,0x20,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x70,0x61,0x74,0x74,0x65,0x72,0x6e,0x3d,
0x22,0x5c,0x64,0x7b,0x31,0x2c,0x33,0x7d,0x5c,0x2e,0x5c,0x64,0x7b,0x31,0x2c,0x33,
0x7d,0x5c,0x2e,0x5c,0x64,0x7b,0x31,0x2c,0x33,0x7d,0x5c,0x2e,0x5c,0x64,0x7b,0x31,
0x2c,0x33,0x7d,0x22,0x3e,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x0a,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x3c,0x6c,0x61,0x62,0x65,0x6c,0x20,0x66,0x6f,
0x72,0x3d,0x22,0x6c,0x6e,0x61,0x6d,0x65,0x22,0x3e,0x47,0x61,0x74,0x65,0x77,0x61,
0x79,0x3c,0x2f,0x6c,0x61,0x62,0x65,0x6c,0x3e,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x3c,0x69,0x6e,0x70,0x75,0x74,0x20,0x74,0x79,0x70,0x65,0x3d,0x22,0x74,
0x65,0x78,0x74,0x22,0x20,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x69,0x64,0x3d,0x22,0x67,0x61,0x74,0x65,0x77,0x61,
0x79,0x22,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x6e,0x61,0x6d,0x65,0x3d,0x22,0x67,0x61,0x74,0x65,0x77,0x61,0x79,
0x22,0x20,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x70,0x6c,0x61,0x63,0x65,0x68,0x6f,0x6c,0x64,0x65,0x72,0x3d,0x22,
0x47,0x61,0x74,0x65,0x77,0x61,0x79,0x22,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x72,0x65,0x71,0x75,0x69,0x72,0x65,
0x64,0x20,0x3e,0x20,0x20,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x3c,0x2f,0x64,0x69,0x76,0x3e,
0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x3c,0x64,0x69,0x76,0x20,0x69,0x64,0x3d,0x22,0x64,0x68,
0x63,0x70,0x57,0x72,0x61,0x70,0x22,0x3e,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x3c,0x68,0x33,0x3e,0x44,0x48,0x43,0x50,0x3c,0x2f,0x68,0x33,0x3e,0x0a,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x3c,0x2f,0x64,0x69,0x76,0x3e,0x20,0x0a,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x0a,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x3c,0x69,0x6e,0x70,0x75,0x74,0x20,0x74,0x79,0x70,0x65,0x3d,
0x22,0x73,0x75,0x62,0x6d,0x69,0x74,0x22,0x20,0x76,0x61,0x6c,0x75,0x65,0x3d,0x22,
0x53,0x75,0x62,0x6d,0x69,0x74,0x22,0x3e,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x3c,0x2f,0x64,0x69,0x76,0x3e,0x20,
0x20,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x0a,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x3c,0x2f,0x66,0x6f,0x72,
0x6d,0x3e,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x3c,0x2f,0x64,0x69,0x76,
0x3e,0x0a,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x3c,0x73,0x63,0x72,0x69,
0x70,0x74,0x3e,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x77,0x69,0x6e,0x64,0x6f,0x77,0x2e,0x6f,0x6e,0x6c,0x6f,0x61,0x64,0x20,0x3d,0x20,
0x28,0x29,0x20,0x3d,0x3e,0x20,0x7b,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x63,0x6f,0x6e,0x73,0x74,0x20,0x72,0x61,
0x64,0x69,0x6f,0x42,0x75,0x74,0x74,0x6f,0x6e,0x73,0x20,0x3d,0x20,0x64,0x6f,0x63,
0x75,0x6d,0x65,0x6e,0x74,0x2e,0x71,0x75,0x65,0x72,0x79,0x53,0x65,0x6c,0x65,0x63,
0x74,0x6f,0x72,0x41,0x6c,0x6c,0x28,0x27,0x69,0x6e,0x70,0x75,0x74,0x5b,0x74,0x79,
0x70,0x65,0x3d,0x22,0x72,0x61,0x64,0x69,0x6f,0x22,0x5d,0x27,0x29,0x3b,0x0a,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x63,
0x6f,0x6e,0x73,0x74,0x20,0x66,0x6f,0x72,0x6d,0x45,0x6c,0x20,0x3d,0x20,0x64,0x6f,
0x63,0x75,0x6d,0x65,0x6e,0x74,0x2e,0x71,0x75,0x65,0x72,0x79,0x53,0x65,0x6c,0x65,
0x63,0x74,0x6f,0x72,0x28,0x27,0x66,0x6f,0x72,0x6d,0x27,0x29,0x3b,0x0a,0x0a,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x72,
0x61,0x64,0x69,0x6f,0x42,0x75,0x74,0x74,0x6f,0x6e,0x73,0x2e,0x66,0x6f,0x72,0x45,
0x61,0x63,0x68,0x28,0x28,0x69,0x2c,0x20,0x69,0x6e,0x64,0x65,0x78,0x29,0x20,0x3d,
0x3e,0x20,0x7b,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x72,0x61,0x64,0x69,0x6f,0x42,0x75,0x74,
0x74,0x6f,0x6e,0x73,0x5b,0x69,0x6e,0x64,0x65,0x78,0x5d,0x2e,0x61,0x64,0x64,0x45,
0x76,0x65,0x6e,0x74,0x4c,0x69,0x73,0x74,0x65,0x6e,0x65,0x72,0x28,0x22,0x63,0x68,
0x61,0x6e,0x67,0x65,0x22,0x2c,0x20,0x28,0x65,0x29,0x20,0x3d,0x3e,0x20,0x73,0x65,
0x74,0x41,0x74,0x74,0x72,0x69,0x62,0x75,0x74,0x65,0x54,0x6f,0x46,0x6f,0x72,0x6d,
0x45,0x6c,0x28,0x65,0x2e,0x74,0x61,0x72,0x67,0x65,0x74,0x29,0x29,0x3b,0x0a,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x72,0x61,0x64,0x69,0x6f,0x42,0x75,0x74,0x74,0x6f,0x6e,0x73,0x5b,
0x69,0x6e,0x64,0x65,0x78,0x5d,0x2e,0x61,0x64,0x64,0x45,0x76,0x65,0x6e,0x74,0x4c,
0x69,0x73,0x74,0x65,0x6e,0x65,0x72,0x28,0x22,0x73,0x65,0x6c,0x65,0x63,0x74,0x22,
0x2c,0x20,0x28,0x65,0x29,0x20,0x3d,0x3e,0x20,0x73,0x65,0x74,0x41,0x74,0x74,0x72,
0x69,0x62,0x75,0x74,0x65,0x54,0x6f,0x46,0x6f,0x72,0x6d,0x45,0x6c,0x28,0x65,0x2e,
0x74,0x61,0x72,0x67,0x65,0x74,0x29,0x29,0x3b,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x7d,0x29,0x0a,0x0a,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x66,0x75,
0x6e,0x63,0x74,0x69,0x6f,0x6e,0x20,0x73,0x65,0x74,0x41,0x74,0x74,0x72,0x69,0x62,
0x75,0x74,0x65,0x54,0x6f,0x46,0x6f,0x72,0x6d,0x45,0x6c,0x20,0x28,0x65,0x29,0x20,
0x7b,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x69,0x66,0x20,0x28,0x65,0x2e,0x76,0x61,0x6c,0x75,
0x65,0x20,0x3d,0x3d,0x3d,0x20,0x22,0x64,0x68,0x63,0x70,0x22,0x29,0x20,0x7b,0x0a,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x66,0x6f,0x72,0x6d,0x45,0x6c,0x2e,0x73,
0x65,0x74,0x41,0x74,0x74,0x72,0x69,0x62,0x75,0x74,0x65,0x28,0x22,0x6e,0x6f,0x76,
0x61,0x6c,0x69,0x64,0x61,0x74,0x65,0x22,0x2c,0x20,0x74,0x72,0x75,0x65,0x29,0x0a,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x7d,0x20,0x65,0x6c,0x73,0x65,0x20,0x7b,0x0a,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x66,0x6f,0x72,0x6d,0x45,0x6c,0x2e,0x72,0x65,0x6d,0x6f,
0x76,0x65,0x41,0x74,0x74,0x72,0x69,0x62,0x75,0x74,0x65,0x28,0x22,0x6e,0x6f,0x76,
0x61,0x6c,0x69,0x64,0x61,0x74,0x65,0x22,0x29,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x7d,0x0a,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x7d,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x73,0x65,0x74,0x41,0x74,0x74,0x72,0x69,0x62,0x75,0x74,0x65,0x54,0x6f,
0x46,0x6f,0x72,0x6d,0x45,0x6c,0x28,0x72,0x61,0x64,0x69,0x6f,0x42,0x75,0x74,0x74,
0x6f,0x6e,0x73,0x5b,0x31,0x5d,0x29,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x7d,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x3c,0x2f,
0x73,0x63,0x72,0x69,0x70,0x74,0x3e,0x0a,0x20,0x20,0x20,0x20,0x3c,0x2f,0x62,0x6f,
0x64,0x79,0x3e,0x0a,0x3c,0x2f,0x68,0x74,0x6d,0x6c,0x3e,};

static const unsigned int dummy_align__index_shtml = 1;
static const unsigned char data__index_shtml[] = {
/* /index.shtml (13 chars) */
0x2f,0x69,0x6e,0x64,0x65,0x78,0x2e,0x73,0x68,0x74,0x6d,0x6c,0x00,0x00,0x00,0x00,

/* HTTP header */
/* "HTTP/1.0 200 OK
" (17 bytes) */
0x48,0x54,0x54,0x50,0x2f,0x31,0x2e,0x30,0x20,0x32,0x30,0x30,0x20,0x4f,0x4b,0x0d,
0x0a,
/* "Server: SageGlass
" (19 bytes) */
0x53,0x65,0x72,0x76,0x65,0x72,0x3a,0x20,0x53,0x61,0x67,0x65,0x47,0x6c,0x61,0x73,
0x73,0x0d,0x0a,
/* "Content-type: text/html
Expires: Fri, 10 Apr 2008 14:00:00 GMT
Pragma: no-cache

" (85 bytes) */
0x43,0x6f,0x6e,0x74,0x65,0x6e,0x74,0x2d,0x74,0x79,0x70,0x65,0x3a,0x20,0x74,0x65,
0x78,0x74,0x2f,0x68,0x74,0x6d,0x6c,0x0d,0x0a,0x45,0x78,0x70,0x69,0x72,0x65,0x73,
0x3a,0x20,0x46,0x72,0x69,0x2c,0x20,0x31,0x30,0x20,0x41,0x70,0x72,0x20,0x32,0x30,
0x30,0x38,0x20,0x31,0x34,0x3a,0x30,0x30,0x3a,0x30,0x30,0x20,0x47,0x4d,0x54,0x0d,
0x0a,0x50,0x72,0x61,0x67,0x6d,0x61,0x3a,0x20,0x6e,0x6f,0x2d,0x63,0x61,0x63,0x68,
0x65,0x0d,0x0a,0x0d,0x0a,
/* raw file data (958 bytes) */
0x3c,0x21,0x64,0x6f,0x63,0x74,0x79,0x70,0x65,0x20,0x3e,0x0a,0x3c,0x68,0x74,0x6d,
0x6c,0x20,0x20,0x6c,0x61,0x6e,0x67,0x3d,0x22,0x65,0x6e,0x22,0x3e,0x0a,0x20,0x20,
0x20,0x20,0x3c,0x68,0x65,0x61,0x64,0x3e,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x3c,0x6d,0x65,0x74,0x61,0x20,0x63,0x68,0x61,0x72,0x73,0x65,0x74,0x3d,0x22,
0x75,0x74,0x66,0x2d,0x38,0x22,0x3e,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x3c,0x6d,0x65,0x74,0x61,0x20,0x68,0x74,0x74,0x70,0x2d,0x65,0x71,0x75,0x69,0x76,
0x3d,0x22,0x58,0x2d,0x55,0x41,0x2d,0x43,0x6f,0x6d,0x70,0x61,0x74,0x69,0x62,0x6c,
0x65,0x22,0x20,0x63,0x6f,0x6e,0x74,0x65,0x6e,0x74,0x3d,0x22,0x49,0x45,0x3d,0x65,
0x64,0x67,0x65,0x2c,0x63,0x68,0x72,0x6f,0x6d,0x65,0x3d,0x31,0x22,0x3e,0x0a,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x3c,0x74,0x69,0x74,0x6c,0x65,0x3e,0x3c,0x2f,
0x74,0x69,0x74,0x6c,0x65,0x3e,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x3c,
0x6d,0x65,0x74,0x61,0x20,0x6e,0x61,0x6d,0x65,0x3d,0x22,0x64,0x65,0x73,0x63,0x72,
0x69,0x70,0x74,0x69,0x6f,0x6e,0x22,0x20,0x63,0x6f,0x6e,0x74,0x65,0x6e,0x74,0x3d,
0x22,0x22,0x3e,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x3c,0x6d,0x65,0x74,
0x61,0x20,0x6e,0x61,0x6d,0x65,0x3d,0x22,0x76,0x69,0x65,0x77,0x70,0x6f,0x72,0x74,
0x22,0x20,0x63,0x6f,0x6e,0x74,0x65,0x6e,0x74,0x3d,0x22,0x77,0x69,0x64,0x74,0x68,
0x3d,0x64,0x65,0x76,0x69,0x63,0x65,0x2d,0x77,0x69,0x64,0x74,0x68,0x2c,0x20,0x69,
0x6e,0x69,0x74,0x69,0x61,0x6c,0x2d,0x73,0x63,0x61,0x6c,0x65,0x3d,0x31,0x2c,0x20,
0x73,0x68,0x72,0x69,0x6e,0x6b,0x2d,0x74,0x6f,0x2d,0x66,0x69,0x74,0x3d,0x6e,0x6f,
0x22,0x3e,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x3c,0x6c,0x69,0x6e,0x6b,
0x20,0x72,0x65,0x6c,0x3d,0x22,0x73,0x74,0x79,0x6c,0x65,0x73,0x68,0x65,0x65,0x74,
0x22,0x20,0x68,0x72,0x65,0x66,0x3d,0x22,0x2e,0x2f,0x73,0x74,0x79,0x6c,0x65,0x73,
0x2e,0x63,0x73,0x73,0x22,0x3e,0x0a,0x20,0x20,0x20,0x20,0x3c,0x2f,0x68,0x65,0x61,
0x64,0x3e,0x0a,0x20,0x20,0x20,0x20,0x3c,0x62,0x6f,0x64,0x79,0x3e,0x0a,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x3c,0x64,0x69,0x76,0x20,0x63,0x6c,0x61,0x73,0x73,
0x3d,0x22,0x77,0x72,0x61,0x70,0x22,0x3e,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x3c,0x66,0x6f,0x72,0x6d,0x20,0x61,0x63,0x74,0x69,0x6f,
0x6e,0x3d,0x22,0x2f,0x6d,0x61,0x69,0x6e,0x20,0x22,0x20,0x6d,0x65,0x74,0x68,0x6f,
0x64,0x3d,0x22,0x50,0x4f,0x53,0x54,0x22,0x20,0x65,0x6e,0x63,0x74,0x79,0x70,0x65,
0x3d,0x22,0x74,0x65,0x78,0x74,0x2f,0x70,0x6c,0x61,0x69,0x6e,0x22,0x20,0x63,0x6c,
0x61,0x73,0x73,0x3d,0x22,0x6c,0x6f,0x67,0x69,0x6e,0x2d,0x66,0x6f,0x72,0x6d,0x22,
0x3e,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x3c,0x64,0x69,0x76,0x20,0x63,0x6c,0x61,0x73,0x73,0x3d,0x22,0x66,0x6f,
0x72,0x6d,0x2d,0x77,0x72,0x61,0x70,0x20,0x77,0x72,0x61,0x70,0x22,0x3e,0x0a,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x0a,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x3c,0x6c,0x61,0x62,0x65,0x6c,0x20,
0x66,0x6f,0x72,0x3d,0x22,0x66,0x6e,0x61,0x6d,0x65,0x22,0x3e,0x50,0x61,0x73,0x73,
0x77,0x6f,0x72,0x64,0x3c,0x2f,0x6c,0x61,0x62,0x65,0x6c,0x3e,0x0a,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x3c,0x69,0x6e,0x70,0x75,0x74,0x20,0x74,0x79,0x70,0x65,0x3d,0x22,0x70,0x61,
0x73,0x73,0x77,0x6f,0x72,0x64,0x22,0x20,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x69,0x64,0x3d,0x22,0x70,0x61,0x73,0x73,0x77,0x6f,0x72,0x64,
0x22,0x20,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x6e,0x61,
0x6d,0x65,0x3d,0x22,0x70,0x61,0x73,0x73,0x77,0x6f,0x72,0x64,0x22,0x20,0x0a,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x70,0x6c,0x61,0x63,0x65,0x68,
0x6f,0x6c,0x64,0x65,0x72,0x3d,0x22,0x50,0x61,0x73,0x73,0x77,0x6f,0x72,0x64,0x22,
0x20,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x72,0x65,0x71,
0x75,0x69,0x72,0x65,0x64,0x3e,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x3c,0x69,0x6e,0x70,0x75,0x74,0x20,0x74,0x79,0x70,0x65,0x3d,
0x22,0x73,0x75,0x62,0x6d,0x69,0x74,0x22,0x20,0x76,0x61,0x6c,0x75,0x65,0x3d,0x22,
0x4c,0x6f,0x67,0x69,0x6e,0x22,0x3e,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x3c,0x2f,0x66,0x6f,0x72,0x6d,0x3e,0x0a,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x3c,0x2f,0x64,0x69,0x76,0x3e,0x0a,0x20,0x20,0x20,0x20,0x3c,
0x2f,0x62,0x6f,0x64,0x79,0x3e,0x0a,0x3c,0x2f,0x68,0x74,0x6d,0x6c,0x3e,};

static const unsigned int dummy_align__404_html = 2;
static const unsigned char data__404_html[] = {
/* /404.html (10 chars) */
0x2f,0x34,0x30,0x34,0x2e,0x68,0x74,0x6d,0x6c,0x00,0x00,0x00,

/* HTTP header */
/* "HTTP/1.0 200 OK
" (17 bytes) */
0x48,0x54,0x54,0x50,0x2f,0x31,0x2e,0x30,0x20,0x32,0x30,0x30,0x20,0x4f,0x4b,0x0d,
0x0a,
/* "Server: SageGlass
" (19 bytes) */
0x53,0x65,0x72,0x76,0x65,0x72,0x3a,0x20,0x53,0x61,0x67,0x65,0x47,0x6c,0x61,0x73,
0x73,0x0d,0x0a,
/* "Content-type: text/html

" (27 bytes) */
0x43,0x6f,0x6e,0x74,0x65,0x6e,0x74,0x2d,0x74,0x79,0x70,0x65,0x3a,0x20,0x74,0x65,
0x78,0x74,0x2f,0x68,0x74,0x6d,0x6c,0x0d,0x0a,0x0d,0x0a,
/* raw file data (718 bytes) */
0x3c,0x21,0x64,0x6f,0x63,0x74,0x79,0x70,0x65,0x20,0x3e,0x0a,0x3c,0x68,0x74,0x6d,
0x6c,0x20,0x20,0x6c,0x61,0x6e,0x67,0x3d,0x22,0x65,0x6e,0x22,0x3e,0x0a,0x20,0x20,
0x20,0x20,0x3c,0x68,0x65,0x61,0x64,0x3e,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x3c,0x6d,0x65,0x74,0x61,0x20,0x63,0x68,0x61,0x72,0x73,0x65,0x74,0x3d,0x22,
0x75,0x74,0x66,0x2d,0x38,0x22,0x3e,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x3c,0x6d,0x65,0x74,0x61,0x20,0x68,0x74,0x74,0x70,0x2d,0x65,0x71,0x75,0x69,0x76,
0x3d,0x22,0x58,0x2d,0x55,0x41,0x2d,0x43,0x6f,0x6d,0x70,0x61,0x74,0x69,0x62,0x6c,
0x65,0x22,0x20,0x63,0x6f,0x6e,0x74,0x65,0x6e,0x74,0x3d,0x22,0x49,0x45,0x3d,0x65,
0x64,0x67,0x65,0x2c,0x63,0x68,0x72,0x6f,0x6d,0x65,0x3d,0x31,0x22,0x3e,0x0a,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x3c,0x74,0x69,0x74,0x6c,0x65,0x3e,0x3c,0x2f,
0x74,0x69,0x74,0x6c,0x65,0x3e,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x3c,
0x6d,0x65,0x74,0x61,0x20,0x6e,0x61,0x6d,0x65,0x3d,0x22,0x64,0x65,0x73,0x63,0x72,
0x69,0x70,0x74,0x69,0x6f,0x6e,0x22,0x20,0x63,0x6f,0x6e,0x74,0x65,0x6e,0x74,0x3d,
0x22,0x22,0x3e,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x3c,0x6d,0x65,0x74,
0x61,0x20,0x6e,0x61,0x6d,0x65,0x3d,0x22,0x76,0x69,0x65,0x77,0x70,0x6f,0x72,0x74,
0x22,0x20,0x63,0x6f,0x6e,0x74,0x65,0x6e,0x74,0x3d,0x22,0x77,0x69,0x64,0x74,0x68,
0x3d,0x64,0x65,0x76,0x69,0x63,0x65,0x2d,0x77,0x69,0x64,0x74,0x68,0x2c,0x20,0x69,
0x6e,0x69,0x74,0x69,0x61,0x6c,0x2d,0x73,0x63,0x61,0x6c,0x65,0x3d,0x31,0x2c,0x20,
0x73,0x68,0x72,0x69,0x6e,0x6b,0x2d,0x74,0x6f,0x2d,0x66,0x69,0x74,0x3d,0x6e,0x6f,
0x22,0x3e,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x3c,0x6c,0x69,0x6e,0x6b,
0x20,0x72,0x65,0x6c,0x3d,0x22,0x73,0x74,0x79,0x6c,0x65,0x73,0x68,0x65,0x65,0x74,
0x22,0x20,0x68,0x72,0x65,0x66,0x3d,0x22,0x2e,0x2f,0x73,0x74,0x79,0x6c,0x65,0x73,
0x2e,0x63,0x73,0x73,0x22,0x3e,0x0a,0x20,0x20,0x20,0x20,0x3c,0x2f,0x68,0x65,0x61,
0x64,0x3e,0x0a,0x20,0x20,0x20,0x20,0x3c,0x62,0x6f,0x64,0x79,0x3e,0x0a,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x3c,0x64,0x69,0x76,0x20,0x63,0x6c,0x61,0x73,0x73,
0x3d,0x22,0x77,0x72,0x61,0x70,0x22,0x3e,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x3c,0x68,0x33,0x20,0x61,0x6c,0x69,0x67,0x6e,0x3d,0x22,
0x63,0x65,0x6e,0x74,0x65,0x72,0x22,0x3e,0x4f,0x6f,0x70,0x73,0x2c,0x20,0x73,0x6f,
0x6d,0x65,0x74,0x68,0x69,0x6e,0x67,0x20,0x77,0x65,0x6e,0x74,0x20,0x77,0x72,0x6f,
0x6e,0x67,0x21,0x3c,0x2f,0x68,0x33,0x3e,0x0a,0x09,0x20,0x20,0x20,0x20,0x3c,0x70,
0x20,0x61,0x6c,0x69,0x67,0x6e,0x3d,0x22,0x63,0x65,0x6e,0x74,0x65,0x72,0x22,0x3e,
0x3c,0x2f,0x70,0x3e,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x3c,0x61,0x20,0x68,0x72,0x65,0x66,0x3d,0x22,0x22,0x3e,0x47,0x6f,0x20,0x42,
0x61,0x63,0x6b,0x3c,0x2f,0x61,0x3e,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x3c,0x2f,0x64,0x69,0x76,0x3e,0x0a,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x3c,0x73,0x63,0x72,0x69,0x70,0x74,0x3e,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x64,0x6f,0x63,0x75,0x6d,0x65,0x6e,0x74,0x2e,0x67,0x65,
0x74,0x45,0x6c,0x65,0x6d,0x65,0x6e,0x74,0x73,0x42,0x79,0x54,0x61,0x67,0x4e,0x61,
0x6d,0x65,0x28,0x22,0x61,0x22,0x29,0x5b,0x30,0x5d,0x2e,0x68,0x72,0x65,0x66,0x3d,
0x77,0x69,0x6e,0x64,0x6f,0x77,0x2e,0x6c,0x6f,0x63,0x61,0x74,0x69,0x6f,0x6e,0x2e,
0x70,0x72,0x6f,0x74,0x6f,0x63,0x6f,0x6c,0x2b,0x22,0x2f,0x2f,0x22,0x2b,0x77,0x69,
0x6e,0x64,0x6f,0x77,0x2e,0x6c,0x6f,0x63,0x61,0x74,0x69,0x6f,0x6e,0x2e,0x68,0x6f,
0x73,0x74,0x2b,0x22,0x2f,0x22,0x3b,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x3c,0x2f,0x73,0x63,0x72,0x69,0x70,0x74,0x3e,0x0a,0x20,0x20,0x20,0x20,0x3c,0x2f,
0x62,0x6f,0x64,0x79,0x3e,0x0a,0x3c,0x2f,0x68,0x74,0x6d,0x6c,0x3e,0x0a,};

static const unsigned int dummy_align__styles_css = 3;
static const unsigned char data__styles_css[] = {
/* /styles.css (12 chars) */
0x2f,0x73,0x74,0x79,0x6c,0x65,0x73,0x2e,0x63,0x73,0x73,0x00,

/* HTTP header */
/* "HTTP/1.0 200 OK
" (17 bytes) */
0x48,0x54,0x54,0x50,0x2f,0x31,0x2e,0x30,0x20,0x32,0x30,0x30,0x20,0x4f,0x4b,0x0d,
0x0a,
/* "Server: SageGlass
" (19 bytes) */
0x53,0x65,0x72,0x76,0x65,0x72,0x3a,0x20,0x53,0x61,0x67,0x65,0x47,0x6c,0x61,0x73,
0x73,0x0d,0x0a,
/* "Content-type: text/css

" (26 bytes) */
0x43,0x6f,0x6e,0x74,0x65,0x6e,0x74,0x2d,0x74,0x79,0x70,0x65,0x3a,0x20,0x74,0x65,
0x78,0x74,0x2f,0x63,0x73,0x73,0x0d,0x0a,0x0d,0x0a,
/* raw file data (1144 bytes) */
0x62,0x6f,0x64,0x79,0x20,0x7b,0x0a,0x20,0x20,0x20,0x20,0x6d,0x61,0x72,0x67,0x69,
0x6e,0x3a,0x20,0x30,0x3b,0x0a,0x7d,0x0a,0x69,0x6e,0x70,0x75,0x74,0x5b,0x74,0x79,
0x70,0x65,0x3d,0x74,0x65,0x78,0x74,0x5d,0x2c,0x20,0x69,0x6e,0x70,0x75,0x74,0x5b,
0x74,0x79,0x70,0x65,0x3d,0x70,0x61,0x73,0x73,0x77,0x6f,0x72,0x64,0x5d,0x20,0x7b,
0x0a,0x20,0x20,0x20,0x20,0x77,0x69,0x64,0x74,0x68,0x3a,0x20,0x31,0x30,0x30,0x25,
0x3b,0x0a,0x20,0x20,0x20,0x20,0x70,0x61,0x64,0x64,0x69,0x6e,0x67,0x3a,0x20,0x31,
0x32,0x70,0x78,0x20,0x32,0x30,0x70,0x78,0x3b,0x0a,0x20,0x20,0x20,0x20,0x6d,0x61,
0x72,0x67,0x69,0x6e,0x3a,0x20,0x38,0x70,0x78,0x20,0x30,0x3b,0x0a,0x20,0x20,0x20,
0x20,0x64,0x69,0x73,0x70,0x6c,0x61,0x79,0x3a,0x20,0x69,0x6e,0x6c,0x69,0x6e,0x65,
0x2d,0x62,0x6c,0x6f,0x63,0x6b,0x3b,0x0a,0x20,0x20,0x20,0x20,0x62,0x6f,0x72,0x64,
0x65,0x72,0x3a,0x20,0x31,0x70,0x78,0x20,0x73,0x6f,0x6c,0x69,0x64,0x20,0x23,0x63,
0x63,0x63,0x3b,0x0a,0x20,0x20,0x20,0x20,0x62,0x6f,0x72,0x64,0x65,0x72,0x2d,0x72,
0x61,0x64,0x69,0x75,0x73,0x3a,0x20,0x34,0x70,0x78,0x3b,0x0a,0x20,0x20,0x20,0x20,
0x62,0x6f,0x78,0x2d,0x73,0x69,0x7a,0x69,0x6e,0x67,0x3a,0x20,0x62,0x6f,0x72,0x64,
0x65,0x72,0x2d,0x62,0x6f,0x78,0x3b,0x0a,0x7d,0x0a,0x0a,0x69,0x6e,0x70,0x75,0x74,
0x5b,0x74,0x79,0x70,0x65,0x3d,0x73,0x75,0x62,0x6d,0x69,0x74,0x5d,0x2c,0x20,0x61,
0x20,0x7b,0x0a,0x20,0x20,0x20,0x20,0x77,0x69,0x64,0x74,0x68,0x3a,0x20,0x31,0x30,
0x30,0x25,0x3b,0x0a,0x20,0x20,0x20,0x20,0x62,0x61,0x63,0x6b,0x67,0x72,0x6f,0x75,
0x6e,0x64,0x2d,0x63,0x6f,0x6c,0x6f,0x72,0x3a,0x20,0x23,0x34,0x43,0x41,0x46,0x35,
0x30,0x3b,0x0a,0x20,0x20,0x20,0x20,0x63,0x6f,0x6c,0x6f,0x72,0x3a,0x20,0x77,0x68,
0x69,0x74,0x65,0x3b,0x0a,0x20,0x20,0x20,0x20,0x70,0x61,0x64,0x64,0x69,0x6e,0x67,
0x3a,0x20,0x31,0x34,0x70,0x78,0x20,0x32,0x30,0x70,0x78,0x3b,0x0a,0x20,0x20,0x20,
0x20,0x6d,0x61,0x72,0x67,0x69,0x6e,0x3a,0x20,0x38,0x70,0x78,0x20,0x30,0x3b,0x0a,
0x20,0x20,0x20,0x20,0x62,0x6f,0x72,0x64,0x65,0x72,0x3a,0x20,0x6e,0x6f,0x6e,0x65,
0x3b,0x0a,0x20,0x20,0x20,0x20,0x62,0x6f,0x72,0x64,0x65,0x72,0x2d,0x72,0x61,0x64,
0x69,0x75,0x73,0x3a,0x20,0x34,0x70,0x78,0x3b,0x0a,0x20,0x20,0x20,0x20,0x63,0x75,
0x72,0x73,0x6f,0x72,0x3a,0x20,0x70,0x6f,0x69,0x6e,0x74,0x65,0x72,0x3b,0x0a,0x7d,
0x0a,0x0a,0x69,0x6e,0x70,0x75,0x74,0x5b,0x74,0x79,0x70,0x65,0x3d,0x73,0x75,0x62,
0x6d,0x69,0x74,0x5d,0x2c,0x20,0x61,0x3a,0x68,0x6f,0x76,0x65,0x72,0x20,0x7b,0x0a,
0x20,0x20,0x20,0x20,0x62,0x61,0x63,0x6b,0x67,0x72,0x6f,0x75,0x6e,0x64,0x2d,0x63,
0x6f,0x6c,0x6f,0x72,0x3a,0x20,0x23,0x34,0x35,0x61,0x30,0x34,0x39,0x3b,0x0a,0x7d,
0x0a,0x0a,0x2e,0x77,0x72,0x61,0x70,0x20,0x7b,0x0a,0x20,0x20,0x20,0x20,0x62,0x6f,
0x72,0x64,0x65,0x72,0x2d,0x72,0x61,0x64,0x69,0x75,0x73,0x3a,0x20,0x35,0x70,0x78,
0x3b,0x0a,0x20,0x20,0x20,0x20,0x70,0x61,0x64,0x64,0x69,0x6e,0x67,0x3a,0x20,0x32,
0x30,0x70,0x78,0x3b,0x0a,0x7d,0x0a,0x0a,0x2e,0x66,0x6f,0x72,0x6d,0x2d,0x77,0x72,
0x61,0x70,0x20,0x7b,0x0a,0x20,0x20,0x20,0x20,0x77,0x69,0x64,0x74,0x68,0x3a,0x20,
0x37,0x30,0x25,0x3b,0x0a,0x20,0x20,0x20,0x20,0x6d,0x61,0x72,0x67,0x69,0x6e,0x3a,
0x20,0x30,0x20,0x61,0x75,0x74,0x6f,0x3b,0x0a,0x7d,0x0a,0x0a,0x2e,0x6d,0x61,0x69,
0x6e,0x2d,0x66,0x6f,0x72,0x6d,0x20,0x7b,0x0a,0x20,0x20,0x20,0x20,0x70,0x6f,0x73,
0x69,0x74,0x69,0x6f,0x6e,0x3a,0x20,0x61,0x62,0x73,0x6f,0x6c,0x75,0x74,0x65,0x3b,
0x0a,0x20,0x20,0x20,0x20,0x6c,0x65,0x66,0x74,0x3a,0x20,0x35,0x30,0x25,0x3b,0x0a,
0x20,0x20,0x20,0x20,0x74,0x6f,0x70,0x3a,0x20,0x35,0x30,0x25,0x3b,0x0a,0x20,0x20,
0x20,0x20,0x74,0x72,0x61,0x6e,0x73,0x66,0x6f,0x72,0x6d,0x3a,0x20,0x74,0x72,0x61,
0x6e,0x73,0x6c,0x61,0x74,0x65,0x28,0x2d,0x35,0x30,0x25,0x2c,0x20,0x2d,0x38,0x30,
0x25,0x29,0x3b,0x0a,0x7d,0x0a,0x0a,0x2e,0x6c,0x6f,0x67,0x69,0x6e,0x2d,0x66,0x6f,
0x72,0x6d,0x20,0x7b,0x0a,0x20,0x20,0x20,0x20,0x70,0x6f,0x73,0x69,0x74,0x69,0x6f,
0x6e,0x3a,0x20,0x61,0x62,0x73,0x6f,0x6c,0x75,0x74,0x65,0x3b,0x0a,0x20,0x20,0x20,
0x20,0x77,0x69,0x64,0x74,0x68,0x3a,0x20,0x35,0x30,0x25,0x3b,0x0a,0x20,0x20,0x20,
0x20,0x74,0x6f,0x70,0x3a,0x20,0x35,0x30,0x25,0x3b,0x0a,0x20,0x20,0x20,0x20,0x6c,
0x65,0x66,0x74,0x3a,0x20,0x32,0x35,0x25,0x3b,0x0a,0x20,0x20,0x20,0x20,0x74,0x72,
0x61,0x6e,0x73,0x66,0x6f,0x72,0x6d,0x3a,0x20,0x74,0x72,0x61,0x6e,0x73,0x6c,0x61,
0x74,0x65,0x28,0x30,0x2c,0x20,0x2d,0x35,0x30,0x25,0x29,0x3b,0x0a,0x7d,0x0a,0x0a,
0x61,0x20,0x7b,0x0a,0x20,0x20,0x20,0x20,0x74,0x65,0x78,0x74,0x2d,0x64,0x65,0x63,
0x6f,0x72,0x61,0x74,0x69,0x6f,0x6e,0x3a,0x20,0x6e,0x6f,0x6e,0x65,0x3b,0x0a,0x20,
0x20,0x20,0x20,0x6d,0x61,0x72,0x67,0x69,0x6e,0x3a,0x20,0x33,0x36,0x70,0x78,0x20,
0x61,0x75,0x74,0x6f,0x3b,0x0a,0x20,0x20,0x20,0x20,0x74,0x65,0x78,0x74,0x2d,0x61,
0x6c,0x69,0x67,0x6e,0x3a,0x20,0x63,0x65,0x6e,0x74,0x65,0x72,0x3b,0x0a,0x20,0x20,
0x20,0x20,0x77,0x69,0x64,0x74,0x68,0x3a,0x20,0x31,0x30,0x25,0x3b,0x0a,0x20,0x20,
0x20,0x20,0x70,0x6f,0x73,0x69,0x74,0x69,0x6f,0x6e,0x3a,0x20,0x61,0x62,0x73,0x6f,
0x6c,0x75,0x74,0x65,0x3b,0x0a,0x20,0x20,0x20,0x20,0x6c,0x65,0x66,0x74,0x3a,0x20,
0x35,0x30,0x25,0x3b,0x0a,0x20,0x20,0x20,0x20,0x74,0x72,0x61,0x6e,0x73,0x66,0x6f,
0x72,0x6d,0x3a,0x20,0x74,0x72,0x61,0x6e,0x73,0x6c,0x61,0x74,0x65,0x28,0x2d,0x35,
0x30,0x25,0x29,0x3b,0x0a,0x7d,0x0a,0x69,0x6e,0x70,0x75,0x74,0x3a,0x63,0x68,0x65,
0x63,0x6b,0x65,0x64,0x20,0x69,0x6e,0x70,0x75,0x74,0x20,0x7b,0x63,0x6f,0x6c,0x6f,
0x72,0x3a,0x20,0x72,0x65,0x64,0x7d,0x0a,0x0a,0x23,0x64,0x68,0x63,0x70,0x3a,0x63,
0x68,0x65,0x63,0x6b,0x65,0x64,0x7e,0x64,0x69,0x76,0x23,0x63,0x75,0x73,0x74,0x6f,
0x6d,0x57,0x72,0x61,0x70,0x20,0x7b,0x0a,0x20,0x20,0x20,0x20,0x64,0x69,0x73,0x70,
0x6c,0x61,0x79,0x3a,0x20,0x6e,0x6f,0x6e,0x65,0x3b,0x0a,0x7d,0x20,0x0a,0x23,0x63,
0x75,0x73,0x74,0x6f,0x6d,0x43,0x6f,0x6e,0x66,0x69,0x67,0x3a,0x63,0x68,0x65,0x63,
0x6b,0x65,0x64,0x7e,0x64,0x69,0x76,0x23,0x64,0x68,0x63,0x70,0x57,0x72,0x61,0x70,
0x20,0x7b,0x0a,0x20,0x20,0x20,0x20,0x64,0x69,0x73,0x70,0x6c,0x61,0x79,0x3a,0x20,
0x6e,0x6f,0x6e,0x65,0x3b,0x0a,0x7d,0x20,};



const struct fsdata_file file__main_shtml[] = { {
file_NULL,
data__main_shtml,
data__main_shtml + 12,
sizeof(data__main_shtml) - 12,
1,
}};

const struct fsdata_file file__index_shtml[] = { {
file__main_shtml,
data__index_shtml,
data__index_shtml + 16,
sizeof(data__index_shtml) - 16,
1,
}};

const struct fsdata_file file__404_html[] = { {
file__index_shtml,
data__404_html,
data__404_html + 12,
sizeof(data__404_html) - 12,
1,
}};

const struct fsdata_file file__styles_css[] = { {
file__404_html,
data__styles_css,
data__styles_css + 12,
sizeof(data__styles_css) - 12,
1,
}};

#define FS_ROOT file__styles_css
#define FS_NUMFILES 4

