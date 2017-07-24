#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <time.h>

#include "helpers.h"

#include <catalog/pg_control.h>
#include <common/controldata_utils.h>
#include <miscadmin.h>
#include <postgres.h>

#ifdef _WIN32
#include <windows.h>
#elif __APPLE__ || __OpenBSD__ || __FreeBSD__ || __NetBSD__
#include <sys/mount.h>
#include <sys/param.h>
#include <sys/sysctl.h>
#elif __linux__
#include <errno.h>
#include <linux/limits.h>
#include <linux/magic.h>
#include <sys/statfs.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#endif

#ifdef __linux__
#define num_elements( _a ) ( sizeof( _a ) / sizeof( _a[ 0 ] ) )

typedef struct Fstype_s
{
   long type;
   char* name;
} Fstype_s;

// FIXME: more in http://man7.org/linux/man-pages/man2/statfs.2.html
#define FSTYPE( x ) \
   {                \
      x, #x         \
   }

Fstype_s Fstype[] = {
   FSTYPE( ADFS_SUPER_MAGIC ),
   FSTYPE( AFFS_SUPER_MAGIC ),
   FSTYPE( AFS_SUPER_MAGIC ),
   FSTYPE( AUTOFS_SUPER_MAGIC ),
   FSTYPE( CODA_SUPER_MAGIC ),
   FSTYPE( CRAMFS_MAGIC ),
   FSTYPE( CRAMFS_MAGIC_WEND ),
   FSTYPE( DEBUGFS_MAGIC ),
   FSTYPE( SYSFS_MAGIC ),
   FSTYPE( SECURITYFS_MAGIC ),
   FSTYPE( SELINUX_MAGIC ),
   FSTYPE( RAMFS_MAGIC ),
   FSTYPE( TMPFS_MAGIC ),
   FSTYPE( HUGETLBFS_MAGIC ),
   FSTYPE( SQUASHFS_MAGIC ),
   FSTYPE( EFS_SUPER_MAGIC ),
   FSTYPE( EXT2_SUPER_MAGIC ),
   FSTYPE( EXT3_SUPER_MAGIC ),
   FSTYPE( XENFS_SUPER_MAGIC ),
   FSTYPE( EXT4_SUPER_MAGIC ),
   FSTYPE( BTRFS_SUPER_MAGIC ),
   FSTYPE( HPFS_SUPER_MAGIC ),
   FSTYPE( ISOFS_SUPER_MAGIC ),
   FSTYPE( JFFS2_SUPER_MAGIC ),
   FSTYPE( ANON_INODE_FS_MAGIC ),
   FSTYPE( MINIX_SUPER_MAGIC ),
   FSTYPE( MINIX_SUPER_MAGIC2 ),
   FSTYPE( MINIX2_SUPER_MAGIC ),
   FSTYPE( MINIX2_SUPER_MAGIC2 ),
   FSTYPE( MINIX3_SUPER_MAGIC ),
   FSTYPE( MSDOS_SUPER_MAGIC ),
   FSTYPE( NCP_SUPER_MAGIC ),
   FSTYPE( NFS_SUPER_MAGIC ),
   FSTYPE( OPENPROM_SUPER_MAGIC ),
   FSTYPE( PROC_SUPER_MAGIC ),
   FSTYPE( QNX4_SUPER_MAGIC ),
   FSTYPE( REISERFS_SUPER_MAGIC ),
   FSTYPE( SMB_SUPER_MAGIC ),
   FSTYPE( USBDEVICE_SUPER_MAGIC ),
   FSTYPE( CGROUP_SUPER_MAGIC ),
   FSTYPE( FUTEXFS_SUPER_MAGIC ),
   /*FSTYPE(INOTIFYFS_SUPER_MAGIC), not defined in earlier headers */
   FSTYPE( STACK_END_MAGIC ),
   FSTYPE( DEVPTS_SUPER_MAGIC ),
   FSTYPE( SOCKFS_MAGIC )
};

#endif

osinfo* get_os_info( void )
{
   osinfo* os = (osinfo*)malloc( sizeof( osinfo ) );
#if WIN32
   os.os_version = NULL;
   os.os_name = NULL;
   os.os_family = NULL;
   os.arch = NULL;
#else
   struct utsname u;
   if( uname( &u ) == -1 )
      err( 1, NULL );
   os->os_version = (char*)malloc( 256 );
   strcpy( os->os_version, u.version );
   os->os_name = (char*)malloc( 256 );
   strcpy( os->os_name, u.release );
   os->os_family = (char*)malloc( 256 );
   strcpy( os->os_family, u.sysname );
   os->os_arch = (char*)malloc( 256 );
   strcpy( os->os_arch, u.machine );
#endif

   return os;

   /*
	16.x.x  macOS 10.12.x Sierra
	15.x.x  OS X  10.11.x El Capitan
	14.x.x  OS X  10.10.x Yosemite
	13.x.x  OS X  10.9.x  Mavericks
	12.x.x  OS X  10.8.x  Mountain Lion
	11.x.x  OS X  10.7.x  Lion
	10.x.x  OS X  10.6.x  Snow Leopard
	 9.x.x  OS X  10.5.x  Leopard
	 8.x.x  OS X  10.4.x  Tiger
	 7.x.x  OS X  10.3.x  Panther
	 6.x.x  OS X  10.2.x  Jaguar
	 5.x    OS X  10.1.x  Puma
	*/
}

hwinfo* get_hw_info( void )
{
   hwinfo* hw = (hwinfo*)malloc( sizeof( hwinfo ) );
#ifdef __APPLE__
   uint32_t count;
   int64_t physmem;
   size_t siz;
   int mib[ 2 ];
   struct timeval ts;

   siz = sizeof( count );
   mib[ 0 ] = CTL_HW;
   mib[ 1 ] = HW_AVAILCPU;
   sysctl( mib, 2, &count, &siz, NULL, 0 );
   if( count < 1 ) {
      mib[ 1 ] = HW_NCPU;
      sysctl( mib, 2, &count, &siz, NULL, 0 );
      if( count < 1 ) {
         count = 1;
      }
   }
   hw->ncpu = count;

   mib[ 0 ] = CTL_HW;
   mib[ 1 ] = HW_MEMSIZE;
   siz = sizeof( int64_t );
   if( sysctl( mib, 2, &physmem, &siz, NULL, 0 ) == -1 )
      err( 1, "sysctl" );
   hw->memsize = physmem;

   siz = sizeof( ts );
   mib[ 0 ] = CTL_KERN;
   mib[ 1 ] = KERN_BOOTTIME;
   if( sysctl( mib, 2, &ts, &siz, NULL, 0 ) != 0 )
      err( 1, "sysctl" );
   time_t bsec = ts.tv_sec, csec = time( NULL );
   hw->uptime = difftime( csec, bsec );
#elif __OpenBSD__ || __FreeBSD__ || __NetBSD__
   uint32_t count;
   int64_t physmem;
   size_t siz;
   int mib[ 2 ];
   struct timeval ts;
   time_t bsec, csec;

   mib[ 0 ] = CTL_HW;
   mib[ 1 ] = HW_NCPUFOUND;
   siz = sizeof( count );
   if( sysctl( mib, 2, &count, &siz, NULL, 0 ) == -1 )
      err( 1, "sysctl" );

   mib[ 0 ] = CTL_HW;
   mib[ 1 ] = HW_PHYSMEM64;
   if( sysctl( mib, 2, &physmem, &siz, NULL, 0 ) < 0 ) {
      warnx( "physmem: failed to get hw.physmem" );
   }

   siz = sizeof( ts );
   mib[ 0 ] = CTL_KERN;
   mib[ 1 ] = KERN_BOOTTIME;
   if( sysctl( mib, 2, &ts, &siz, NULL, 0 ) != 0 )
      err( 1, "sysctl" );
   bsec = ts.tv_sec;
   csec = time( NULL );
   hw->uptime = difftime( csec, bsec );
#elif __linux__
   struct sysinfo sys_info;
   int32_t total_ram = 0;
   if( sysinfo( &sys_info ) != 0 )
      err( 1, "sysinfo" );
   total_ram = ( (uint64_t)sys_info.totalram * sys_info.mem_unit ) / 1024;
   hw->ncpu = sysconf( _SC_NPROCESSORS_ONLN );
   hw->memsize = sys_info.uptime;
   hw->uptime = total_ram;
#elif WIN32
   SYSTEM_INFO sysinfo;
   GetSystemInfo( &sysinfo );
   hw->ncpu = sysinfo.dwNumberOfProcessors;
   hw->memsize = NULL;  // FIXME
   hw->uptime = NULL;  // FIXME
#else
   hw->ncpu = NULL;
   hw->memsize = NULL;
   hw->uptime = NULL;
#endif

   return hw;
}

long long int get_proc_uptime( pid_t pid )
{
   unsigned long long proc_uptime;
#ifdef __linux__
   /*
	  see proc(5) -> /proc/[pid]/stat
    */

   char filename[ 256 ];
   FILE* f;

   sprintf( filename, "/proc/%u/stat", (unsigned)pid );
   if( -1 == access( filename, R_OK ) ) {
      return -1;
   }

   if( ( f = fopen( filename, "r" ) ) == NULL ) {
      return -1;
   }

   if( fscanf( f, "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %*u"
                  "%*u %*d %*d %*d %*d %*d %*d %*d %llu %*u %*d",
               &proc_uptime )
       == EOF ) {
      fclose( f );
      return -1;
   }

   fclose( f );
#elif __APPLE_ || __OpenBSD__ || __FreeBSD__ || __NetBSD__
   struct kinfo_proc proc;
   size_t siz = sizeof( proc );
   const u_int len_path = 4;
   int ret;
   int path[ len_path ];

   path[ 0 ] = CTL_KERN;
   path[ 1 ] = KERN_PROC;
   path[ 2 ] = KERN_PROC_PID;
   path[ 3 ] = pid;
   ret = sysctl( path, len_path, &proc, &siz, NULL, 0 );

   proc_uptime = 0;
#else
   proc_uptime = 0;
#endif
   return proc_uptime;
}

#ifdef __linux__
char* lookup_fstype( long fstype )
{
   int i;
   for( i = 0; i < num_elements( Fstype ); i++ ) {
      if( fstype == Fstype[ i ].type )
         return Fstype[ i ].name;
   }

   return "<fs type unknown>";
}
#endif

char* dirfs( char* path )
{
   struct statfs st;
   char* fs;

   fs = "";
   if( statfs( path, &st ) == -1 )
      err( 1, NULL );
#ifdef __linux__
   fs = lookup_fstype( st.f_type );
#elif __APPLE__
   fs = st.f_fstypename;
#endif
   return fs;
}

char* system_db_id()
{
   ControlFileData* ControlFile;
   bool crc_ok = 0;
   char* sysident_str;
   sysident_str = (char*)malloc( 32 );

   ControlFile = get_controlfile( DataDir, &crc_ok );
   if( !crc_ok )
      printf( _( "WARNING: Calculated CRC checksum does not match value stored in file.\n"
                 "Either the file is corrupt, or it has a different layout than this program\n"
                 "is expecting. The results below are untrustworthy.\n\n" ) );
   snprintf( sysident_str, sizeof( sysident_str ), UINT64_FORMAT, ControlFile->system_identifier );
   return sysident_str;
}
