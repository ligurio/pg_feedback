#ifndef HELPERS_H_INCLUDED
#define HELPERS_H_INCLUDED

#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>

#ifdef __linux__
#include <linux/magic.h>
#endif

typedef struct hw_info
{
   int ncpu;
   int64_t memsize;
   unsigned long long uptime;
} hwinfo;

typedef struct os_info
{
   char* os_version;
   char* os_family;
   char* os_name;
   char* os_arch;
} osinfo;

osinfo* get_os_info( void );
hwinfo* get_hw_info( void );
char* dirfs( char* path );
long long int get_proc_uptime( pid_t pid );
char* lookup_fstype( long fstype );

#endif /* HELPERS_H_INCLUDED */
