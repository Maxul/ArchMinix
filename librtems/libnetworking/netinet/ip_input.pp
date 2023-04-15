





























 



 

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 


#pragma diag_push
#pragma CHECK_MISRA("-19.7")  
#pragma CHECK_MISRA("-20.1")  
#pragma CHECK_MISRA("-20.2")  



typedef int ptrdiff_t;

typedef unsigned size_t;


typedef unsigned short wchar_t;


#pragma diag_push
#pragma CHECK_MISRA("-19.10")  



#pragma diag_pop



#pragma diag_push
#pragma CHECK_MISRA("-19.15")  

 
 
 
 
 
 
 

#pragma diag_pop








































 


 


















































 





















 






























 

 










 









 

 








 








 




 








 


 
 


 
 

 

 

 
 

 
 

 
 

 

 
 

 
 


 
 


 

 


 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 



 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 



 

 



 


 





 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 


 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 


#pragma diag_push
#pragma CHECK_MISRA("-19.4")  

 
 
 

 
 
 
 
 
 

 
 
 

 
 
 
 

#pragma diag_pop
































 







 


 





 
 






























 


 

 






































 





 





















 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 

 
 
 













































 






typedef signed char __int8_t ;
typedef unsigned char __uint8_t ;

typedef signed short __int16_t;
typedef unsigned short __uint16_t;

typedef __int16_t __int_least16_t;
typedef __uint16_t __uint_least16_t;


typedef signed int __int32_t;
typedef unsigned int __uint32_t;

typedef __int32_t __int_least32_t;
typedef __uint32_t __uint_least32_t;


typedef signed long long __int64_t;
typedef unsigned long long __uint64_t;







typedef unsigned int clock_t;

typedef unsigned int time_t;

typedef unsigned int  clockid_t;

typedef unsigned int timer_t;






 
typedef unsigned long useconds_t;
typedef long suseconds_t;
typedef	__int64_t	sbintime_t;

typedef long int __off_t;
typedef int __pid_t;
typedef long int __loff_t;




struct timespec {
	time_t	tv_sec;		 
	long	tv_nsec;	 
};






typedef unsigned int clock_t;
typedef unsigned int __time32_t;

typedef long long __time64_t;





struct tm 
{
    int tm_sec;       
    int tm_min;       
    int tm_hour;      
    int tm_mday;      
    int tm_mon;       
    int tm_year;      
    int tm_wday;      
    int tm_yday;      
    int tm_isdst;     

};

 
 
 
typedef struct 
{
    short daylight;
    long  timezone;
    char  tzname[4];
    char  dstname[4];
} TZ;

extern __far TZ _tz;

 
 
 
 clock_t    clock(void);             

 __time32_t __time32(__time32_t *_timer);   
 __time32_t __mktime32(struct tm *_tptr);
 double     __difftime32(__time32_t _time1, __time32_t _time0);
      extern  char      *__ctime32(const __time32_t *_timer);
 struct tm *__gmtime32(const __time32_t *_timer);
 struct tm *__localtime32(const __time32_t *_timer);

 __time64_t __time64(__time64_t *_timer);   
 __time64_t __mktime64(struct tm *_tptr);
 double     __difftime64(__time64_t _time1, __time64_t _time0);
      extern  char      *__ctime64(const __time64_t *_timer);
 struct tm *__gmtime64(const __time64_t *_timer);
 struct tm *__localtime64(const __time64_t *_timer);

 char      *asctime(const struct tm *_timeptr);
 size_t     strftime(char *_out, size_t _maxsize, 
				 const char *_format,
				 const struct tm *_timeptr);



 
 
 
 
 time_t     time(time_t *_timer);   
 time_t     mktime(struct tm *_tptr);
 double     difftime(time_t _time1, time_t _time0);
      extern  char      *ctime(const time_t *_timer);
 struct tm *gmtime(const time_t *_timer);
 struct tm *localtime(const time_t *_timer);










 

 
typedef __uint8_t	u_int8_t;
typedef __uint16_t	u_int16_t;
typedef __uint32_t	u_int32_t;

typedef __uint64_t	u_int64_t;

 
typedef	__uint64_t	u_quad_t;
typedef	__int64_t	quad_t;
typedef	quad_t 	*qaddr_t;




 





 



 

typedef int _LOCK_T;
typedef int _LOCK_RECURSIVE_T;



typedef long _off_t;
  typedef long long _off64_t;

typedef int _ssize_t;

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 



#pragma diag_push
#pragma CHECK_MISRA("-19.15")  

 
 
 
 
 
 
 


typedef unsigned int wint_t;




#pragma diag_pop


 
typedef struct
{
    int __count;
    union
    {
        wint_t __wch;
        unsigned char __wchb[4];
    } __value;		 
} _mbstate_t;

typedef _LOCK_RECURSIVE_T _flock_t;

 
typedef void *_iconv_t;

typedef long *__intptr_t;
typedef unsigned long *__uintptr_t;



 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 



#pragma diag_push
#pragma CHECK_MISRA("-19.15")  

 
 
 
 
 
 
 

#pragma diag_pop

















 





















 



 



 

 






 




 

struct sched_param
{
    int sched_priority;            

    
     
    
     
    

    int sched_ss_low_priority;
     
    struct timespec sched_ss_repl_period;
     
    struct timespec sched_ss_init_budget;
};


 








 



 
typedef	unsigned char	u_char;
typedef	unsigned short	u_short;
typedef	unsigned int	u_int;
typedef	unsigned long	u_long;

typedef	unsigned short	ushort;		 
typedef	unsigned int	uint;		 

typedef	long	daddr_t;
typedef	char 	*caddr_t;

typedef	unsigned long	ino_t;







 

 
typedef unsigned long long dev_t;

typedef long off_t;

typedef	unsigned short	uid_t;
typedef	unsigned short	gid_t;
typedef int pid_t;

typedef	long key_t;
typedef _ssize_t ssize_t;


typedef	char 	*addr_t;
typedef int mode_t;

typedef unsigned long nlink_t;






 





 

typedef	long	fd_mask;


 
typedef	struct xx_types_fd_set
{
    fd_mask	fds_bits[(((64)+(((sizeof (fd_mask) * 8))-1))/((sizeof (fd_mask) * 8)))];
} xx_types_fd_set;













 



 

typedef __uint32_t pthread_t;             

 


 

 
 


 



typedef struct
{
    int is_initialized;
    void *stackaddr;
    int stacksize;
    int contentionscope;
    int inheritsched;
    int schedpolicy;
    struct sched_param schedparam;

     
    int  cputime_clock_allowed;   
    int  detachstate;

} pthread_attr_t;

 

 
 

 

 


typedef __uint32_t pthread_mutex_t;       

enum pthread_mutextype {
	PTHREAD_MUTEX_ERRORCHECK	= 1,	 
	PTHREAD_MUTEX_RECURSIVE		= 2,	 
	PTHREAD_MUTEX_NORMAL		= 3,	 
	PTHREAD_MUTEX_ADAPTIVE_NP	= 4,	 
	PTHREAD_MUTEX_TYPE_MAX
};

typedef struct
{
    int   is_initialized;
    int type;
    int   process_shared;   
    int   prio_ceiling;
    int   protocol;
    int   recursive;
} pthread_mutexattr_t;

 

typedef __uint32_t pthread_cond_t;        

typedef struct
{
    int   is_initialized;
    int   clock;
    int   process_shared;        
} pthread_condattr_t;          

 

typedef __uint32_t pthread_key_t;         

typedef struct
{
    int   is_initialized;   
    int   init_executed;    
} pthread_once_t;        

 

typedef __uint32_t pthread_barrier_t;         
typedef struct
{
    int   is_initialized;   
    int   process_shared;        
} pthread_barrierattr_t;

 

typedef __uint32_t pthread_spinlock_t;         

 

typedef __uint32_t pthread_rwlock_t;          
typedef struct
{
    int   is_initialized;        
    int   process_shared;        
} pthread_rwlockattr_t;



 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 



#pragma diag_push
#pragma CHECK_MISRA("-19.15")  

 
 
 
 
 
 
 

#pragma diag_pop

extern char **environ;

void	_exit (int __status );

int	access (const char *__path, int __amode );
unsigned  alarm (unsigned __secs );
int     chdir (const char *__path );
int     chmod (const char *__path, mode_t __mode );
int     chown (const char *__path, uid_t __owner, gid_t __group );
int     chroot (const char *__path );
int     close (int __fildes );
char    *ctermid (char *__s );
char    *cuserid (char *__s );
int     dup (int __fildes );
int     dup2 (int __fildes, int __fildes2 );
int     execl (const char *__path, const char *, ... );
int     execle (const char *__path, const char *, ... );
int     execlp (const char *__file, const char *, ... );
int     execv (const char *__path, char *const __argv[] );
int     execve (const char *__path, char *const __argv[], char *const __envp[] );
int     execvp (const char *__file, char *const __argv[] );
int     fchdir (int __fildes);
int     fchmod (int __fildes, mode_t __mode );
int     fchown (int __fildes, uid_t __owner, gid_t __group );
pid_t   fork (void );
long    fpathconf (int __fd, int __name );
int     fsync (int __fd);
int     fdatasync (int __fd);
char    *getcwd (char *__buf, size_t __size );
gid_t   getegid (void );
uid_t   geteuid (void );
gid_t   getgid (void );
int     getgroups (int __gidsetsize, gid_t __grouplist[] );
char    *getlogin (void );
int getlogin_r (char *name, size_t namesize);
char 	*getpass (const char *__prompt);
size_t  getpagesize (void);
pid_t   getpgid (pid_t);
pid_t   getpgrp (void );
pid_t   getpid (void );
pid_t   getppid (void );
uid_t   getuid (void );
int     isatty (int __fildes );
int     lchown (const char *__path, uid_t __owner, gid_t __group );
int     link (const char *__path1, const char *__path2 );
int	nice (int __nice_value );
off_t   lseek (int __fildes, off_t __offset, int __whence );
long    pathconf (const char *__path, int __name );
int     pause (void );
int     pipe (int __fildes[2] );
ssize_t pread (int __fd, void *__buf, size_t __nbytes, off_t __offset);
ssize_t pwrite (int __fd, const void *__buf, size_t __nbytes, off_t __offset);

_ssize_t read (int __fd, void *__buf, size_t __nbyte );

int     rmdir (const char *__path );
void   *sbrk (ptrdiff_t __incr);
int     setgid (gid_t __gid );
int     setpgid (pid_t __pid, pid_t __pgid );
int     setpgrp (void );
pid_t   setsid (void );
int     setuid (uid_t __uid );
unsigned sleep (unsigned int __seconds );
void    swab (const void *, void *, ssize_t);
long    sysconf (int __name );
pid_t   tcgetpgrp (int __fildes );
int     tcsetpgrp (int __fildes, pid_t __pgrp_id );
char    *ttyname (int __fildes );
int     unlink (const char *__path );
int     vhangup (void );

_ssize_t write (int __fd, const void *__buf, size_t __nbyte );

extern char *optarg;			 
extern int optind, opterr, optopt;
int	 getopt(int, char *const [], const char *);
extern int optreset;			 

pid_t   vfork (void );

extern char *suboptarg;			 
int	 getsubopt(char **, char *const *, char **);


int     ftruncate (int __fd, off_t __length);
int     truncate (const char *, off_t __length);
int	getdtablesize (void);
int	setdtablesize (int);
useconds_t ualarm (useconds_t __useconds, useconds_t __interval);
int usleep (useconds_t __useconds);
 
int	gethostname (char *__name, size_t __len);
char 	*mktemp (char *);
int     sync (void);
ssize_t readlink (const char *__path, char *__buf, size_t __buflen);
int     symlink (const char *__name1, const char *__name2);









 


 
 
 


 












 


 

 
 


 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 


#pragma diag_push
#pragma CHECK_MISRA("-20.1")  
#pragma CHECK_MISRA("-20.2")  
#pragma CHECK_MISRA("-19.7")  
#pragma CHECK_MISRA("-19.10")  


    typedef char *va_list;


 
 
 

 
 
 
 
 

 
 
 


 
 
 
 



 
 
 
 
 
 
 
 
 
 
 

 
 
 
 
 
 
 


 
 
 
 
 







 
 
 
 
 
 






#pragma diag_pop


#pragma diag_push


 
 
#pragma CHECK_MISRA("-19.15")


#pragma diag_pop





















































 


 





 



 

typedef long _Int32t;
typedef unsigned long _Uint32t;

typedef  int  _Ptrdifft;

typedef  unsigned _Sizet;







 


 
 
 






 


 





 







 


 

 




 

 



typedef long long _Longlong;
typedef unsigned long long _ULonglong;


 



typedef unsigned short _Wchart;
typedef unsigned short _Wintt;

 

 


 
typedef  va_list _Va_list;

 


void _Atexit(void (*)(void));


 

typedef char _Sysch_t;

 

 


 

 



 






 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 




#pragma diag_push



 
#pragma CHECK_MISRA("-16.5")

 void _nop();

extern __far void (  *_lock)();
extern __far void (*_unlock)();

 void _register_lock  (void (  *lock)());
 void _register_unlock(void (*unlock)());

#pragma diag_pop








 





 


 


 


 

typedef int sig_atomic_t;


typedef void _Sigfun(int);

 
_Sigfun *signal(int, _Sigfun *);


		 
int raise(int);







 

 


typedef unsigned int  sigset_t;




 

 
 
 
 



 

union sigval
{
    int    sival_int;     
    void  *sival_ptr;     
};

struct sigevent
{
    int              sigev_notify;                
    int              sigev_signo;                 
    union sigval     sigev_value;                 

    void           (*sigev_notify_function)( union sigval );
     
    pthread_attr_t  *sigev_notify_attributes;     
};

 
 


typedef struct
{
    int          si_signo;     
    int          si_code;      
    union sigval si_value;     
} siginfo_t;

 

 







 

typedef void (*_sig_func_ptr)();

struct sigaction
{
    int         sa_flags;        
    sigset_t    sa_mask;         
     
     
    union
    {
        _sig_func_ptr _handler;   
        void      (*_sigaction)( int, siginfo_t *, void * );
    } _signal_handlers;
};






 

int sigprocmask (int how, const sigset_t *set, sigset_t *oset);

int pthread_sigmask (int how, const sigset_t *set, sigset_t *oset);

 



 
int kill (int, int);
int killpg (pid_t, int);
int sigaction (int, const struct sigaction *, struct sigaction *);
int sigaddset (sigset_t *, const int);
int sigdelset (sigset_t *, const int);
int sigismember (const sigset_t *, int);
int sigfillset (sigset_t *);
int sigemptyset (sigset_t *);
int sigpending (sigset_t *);
int sigsuspend (const sigset_t *);
int sigpause (int);

int pthread_kill (pthread_t thread, int sig);



 

int sigwaitinfo (const sigset_t *set, siginfo_t *info);
int sigtimedwait (const sigset_t *set, siginfo_t *info, const struct timespec *timeout);
int sigwait (const sigset_t *set, int *sig);

 
int sigqueue (pid_t pid, int signo, const union sigval value);



 






 





 


 


 




 




 





 












 




 

 

 






 




 




 

 





 









 

 














 









 

 

 

 















 











 




































 










 










 







 






 
 









 








 









 
















 








 










 








 








 










 






 
 

 
 


 
 

 

 

 

 
 

 

 


 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 



#pragma diag_push
#pragma CHECK_MISRA("-19.15")  

 
 
 
 
 
 
 

#pragma diag_pop




























 





 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 

 

    typedef   signed char   int8_t;
    typedef unsigned char  uint8_t;
    typedef          short  int16_t;
    typedef unsigned short uint16_t;
    typedef          int    int32_t;
    typedef unsigned int   uint32_t;

    typedef          __int40_t  int40_t;
    typedef unsigned __int40_t uint40_t;

    typedef          long long  int64_t;
    typedef unsigned long long uint64_t;

 

    typedef  int8_t   int_least8_t;
    typedef uint8_t  uint_least8_t;

    typedef  int16_t  int_least16_t;
    typedef uint16_t uint_least16_t;
    typedef  int32_t  int_least32_t;
    typedef uint32_t uint_least32_t;

    typedef  int40_t  int_least40_t;
    typedef uint40_t uint_least40_t;

    typedef  int64_t  int_least64_t;
    typedef uint64_t uint_least64_t;

 

    typedef  int32_t  int_fast8_t;
    typedef uint32_t uint_fast8_t;
    typedef  int32_t  int_fast16_t;
    typedef uint32_t uint_fast16_t;

    typedef  int32_t  int_fast32_t;
    typedef uint32_t uint_fast32_t;

    typedef  int40_t  int_fast40_t;
    typedef uint40_t uint_fast40_t;

    typedef  int64_t  int_fast64_t;
    typedef uint64_t uint_fast64_t;

 
    typedef          int intptr_t;
    typedef unsigned int uintptr_t;

 
    typedef          long long intmax_t;
    typedef unsigned long long uintmax_t;





 

 
















 







 







 




 




  



 

  



 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 



#pragma diag_push
#pragma CHECK_MISRA("-6.3")  
#pragma CHECK_MISRA("-19.1")  
#pragma CHECK_MISRA("-20.1")  
#pragma CHECK_MISRA("-20.2")  

 



#pragma diag_push
#pragma CHECK_MISRA("-19.4")  


#pragma diag_pop

extern  size_t  strlen(const char *string);

extern  char *strcpy(char *dest, const char *src);
extern  char *strncpy(char *dest, const char *src, size_t n);
extern  char *strcat(char *string1, const char *string2);
extern  char *strncat(char *dest, const char *src, size_t n);
extern  char *strchr(const char *string, int c);
extern  char *strrchr(const char *string, int c);

extern  int  strcmp(const char *string1, const char *string2);
extern  int  strncmp(const char *string1, const char *string2, size_t n);

 int     strcoll(const char *string1, const char *_string2);
 size_t  strxfrm(char *to, const char *from, size_t n);
 char   *strpbrk(const char *string, const char *chs);
 size_t  strspn(const char *string, const char *chs);
 size_t  strcspn(const char *string, const char *chs);
 char   *strstr(const char *string1, const char *string2);
 char   *strtok(char *str1, const char *str2);
 char   *strerror(int _errno);
 char   *strdup(const char *string);


 void   *memmove(void *s1, const void *s2, size_t n);
#pragma diag_push
#pragma CHECK_MISRA("-16.4")  
 void   *memcpy(void *s1, const void *s2, size_t n);
#pragma diag_pop

extern  int     memcmp(const void *cs, const void *ct, size_t n);
extern  void   *memchr(const void *cs, int c, size_t n);

 void   *memset(void *mem, int ch, size_t length);




#pragma diag_pop


#pragma diag_push


 
 
#pragma CHECK_MISRA("-19.15")


#pragma diag_pop

  

 

  

 







 






 






 






 






 





 







 






 




 





 




 







 







 


 







 







 







 



  

 
  typedef void * proc_ptr;

 






 

 
typedef uintptr_t CPU_Uint32ptr;

 
typedef uint16_t Priority_bit_map_Word;












 





 











 




 






 



extern unsigned int GetCSR(void);    
extern void SetCSRBit(int bit_value);
extern void CleanCSRBit(int bit_value);
extern cregister volatile unsigned int ISTP; 	 









 




static inline unsigned int cpu_st_get(void)
{
    register unsigned int st_value;

    st_value = GetCSR();
    return st_value;
}










 







 






 

static inline void *c6x_get_ittp(void)
{
    

 
    return (void *)ISTP;
}

static inline void c6x_set_ittp(void *_ittp_value)
{
	ISTP = (unsigned int)_ittp_value;
}



 

 





















 



























 











 


















 












 









 

















 

















 
























 















 





























 
























 











 





 











 






 
 













 













 

 











 






 




 







 

 






 
typedef struct {
	 
	int reg1;
} CPU_Per_CPU_control;










































 
 






 
typedef struct {
	uint64_t REG_A0;
	


 
	uint32_t REG_B10;
	uint32_t REG_B11;

	uint32_t REG_A12;
	uint32_t REG_A13;

	uint32_t REG_B12;
	uint32_t REG_B13;

	uint32_t REG_A14;
	uint32_t REG_A15;

	uint32_t REG_B14;
	uint32_t REG_B15;

	uint32_t AMR;
	uint32_t CSR;
	uint32_t IER;
	uint32_t IRP;
	uint32_t REG_B3;

} Context_Control;









 






 
typedef struct {
	 
	double some_float_register;
} Context_Control_fp;







 
typedef struct {
	



 
	uint64_t REG_A0;
	
	uint32_t REG_B0;
	uint32_t REG_B1;
	uint32_t REG_A2;
	uint32_t REG_A3;
	uint32_t REG_B2;
	uint32_t REG_B3;
	uint32_t REG_A4;
	uint32_t REG_A5;
	uint32_t REG_B4;
	uint32_t REG_B5;
	uint32_t REG_A6;
	uint32_t REG_A7;
	uint32_t REG_B6;
	uint32_t REG_B7;
	uint32_t REG_A8;
	uint32_t REG_A9;
	uint32_t REG_B8;
	uint32_t REG_B9;
	uint32_t REG_A10;
	uint32_t REG_A11;
	uint32_t REG_B10;
	uint32_t REG_B11;
	uint32_t REG_A12;
	uint32_t REG_A13;
	uint32_t REG_B12;
	uint32_t REG_B13;
	uint32_t REG_A14;
	uint32_t REG_A15;
	uint32_t REG_B14;
	uint32_t REG_B15;
	uint32_t REG_A16;
	uint32_t REG_A17;
	uint32_t REG_B16;
	uint32_t REG_B17;
	uint32_t REG_A18;
	uint32_t REG_A19;
	uint32_t REG_B18;
	uint32_t REG_B19;
	uint32_t REG_A20;
	uint32_t REG_A21;
	uint32_t REG_B20;
	uint32_t REG_B21;
	uint32_t REG_A22;
	uint32_t REG_A23;
	uint32_t REG_B22;
	uint32_t REG_B23;
	uint32_t REG_A24;
	uint32_t REG_A25;
	uint32_t REG_B24;
	uint32_t REG_B25;
	uint32_t REG_A26;
	uint32_t REG_A27;
	uint32_t REG_B26;
	uint32_t REG_B27;
	uint32_t REG_A28;
	uint32_t REG_A29;
	uint32_t REG_B28;
	uint32_t REG_B29;
	uint32_t REG_A30;
	uint32_t REG_A31;
	uint32_t REG_B30;
	uint32_t REG_B31;

	uint32_t NRP;
	uint32_t CSR;
	uint32_t ITSR;
	uint32_t RILC;
	uint32_t ILC;
	uint32_t IRP;
} CPU_Interrupt_frame;










 
extern Context_Control_fp _CPU_Null_fp_context;

 


















 







 

 












 









 










 










 






 










 







 











 






















 
















 













 



 









 












 













 














 

















 












 

 

 

































 
void _CPU_Context_Initialize(Context_Control *_the_context, void *_stack_base,
		uint32_t _size, uint32_t _isr, void (*_entry_point)(void), int _is_fp,
		void * _tls_area);














 























 


















 

 

 









 

 

 







 
 




 

 






 































































 


 

 









 














 

 

 







 
void _CPU_Initialize(void);














 
void _CPU_ISR_install_raw_handler(uint32_t vector, proc_ptr new_handler,
		proc_ptr *old_handler);













 
void _CPU_ISR_install_vector(uint32_t vector, proc_ptr new_handler,
		proc_ptr *old_handler);











 
void _CPU_Install_interrupt_stack(void);










 
void *_CPU_Thread_Idle_body(uintptr_t ignored);












 
void _CPU_Context_switch(Context_Control *run, Context_Control *heir);














 
void _CPU_Context_restore(Context_Control *new_context);















 
void _CPU_Context_save_fp(Context_Control_fp **fp_context_ptr);















 
void _CPU_Context_restore_fp(Context_Control_fp **fp_context_ptr);












 
void _CPU_Context_volatile_clobber(uintptr_t pattern);















 
void _CPU_Context_validate(uintptr_t pattern);









 
typedef struct {
	uint32_t processor_state_register;
	uint32_t integer_registers[1];
	double float_registers[1];
} CPU_Exception_frame;





 
void _CPU_Exception_frame_print(const CPU_Exception_frame *frame);





























 
static inline uint32_t CPU_swap_u32(uint32_t value) {
	uint32_t byte1, byte2, byte3, byte4, swapped;

	byte4 = (value >> 24) & 0xff;
	byte3 = (value >> 16) & 0xff;
	byte2 = (value >> 8) & 0xff;
	byte1 = value & 0xff;

	swapped = (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
	return swapped;
}








 



 
typedef uint32_t CPU_Counter_ticks;










 
CPU_Counter_ticks _CPU_Counter_read(void);












 
CPU_Counter_ticks _CPU_Counter_difference(CPU_Counter_ticks second,
		CPU_Counter_ticks first);







 







 








 
 



 
int POSIX_NOT_IMPLEMENTED( void );





 
extern const char _RTEMS_version[];



 
extern const char _Copyright_Notice[];

 



 
 










 







 











 
 






 
typedef enum {
  

 
  RTEMS_SUCCESSFUL               =  0,
  

 
  RTEMS_TASK_EXITTED             =  1,
  

 
  RTEMS_MP_NOT_CONFIGURED        =  2,
  

 
  RTEMS_INVALID_NAME             =  3,
  

 
  RTEMS_INVALID_ID               =  4,
  


 
  RTEMS_TOO_MANY                 =  5,
  

 
  RTEMS_TIMEOUT                  =  6,
  


 
  RTEMS_OBJECT_WAS_DELETED       =  7,
  

 
  RTEMS_INVALID_SIZE             =  8,
  

 
  RTEMS_INVALID_ADDRESS          =  9,
  

 
  RTEMS_INVALID_NUMBER           = 10,
  

 
  RTEMS_NOT_DEFINED              = 11,
  


 
  RTEMS_RESOURCE_IN_USE          = 12,
  

 
  RTEMS_UNSATISFIED              = 13,
  


 
  RTEMS_INCORRECT_STATE          = 14,
  

 
  RTEMS_ALREADY_SUSPENDED        = 15,
  


 
  RTEMS_ILLEGAL_ON_SELF          = 16,
  

 
  RTEMS_ILLEGAL_ON_REMOTE_OBJECT = 17,
  


 
  RTEMS_CALLED_FROM_ISR          = 18,
  


 
  RTEMS_INVALID_PRIORITY         = 19,
  

 
  RTEMS_INVALID_CLOCK            = 20,
  

 
  RTEMS_INVALID_NODE             = 21,
  

 
  RTEMS_NOT_CONFIGURED           = 22,
  


 
  RTEMS_NOT_OWNER_OF_RESOURCE    = 23,
  




 
  RTEMS_NOT_IMPLEMENTED          = 24,
  


 
  RTEMS_INTERNAL_ERROR           = 25,
  


 
  RTEMS_NO_MEMORY                = 26,
  

 
  RTEMS_IO_ERROR                 = 27,
  






 
  RTEMS_PROXY_BLOCKING           = 28
} rtems_status_code;



 



 






 
static inline _Bool rtems_is_status_successful(
  rtems_status_code code
)
{
  return (code == RTEMS_SUCCESSFUL);
}






 
static inline _Bool rtems_are_statuses_equal(
  rtems_status_code code1,
  rtems_status_code code2
)
{
   return (code1 == code2);
}






















 
int rtems_status_code_to_errno(rtems_status_code sc);










 
const char *rtems_status_text( rtems_status_code code );

 


 







 







 




 







 








 









 










 








 







 








 






 








 










 








 














 








 



 






 



 




 



 










 
 




 
typedef uint32_t   ISR_Level;














 











 



















 










 









 



 



typedef unsigned int CPU_atomic_Uint;

typedef unsigned long CPU_atomic_Ulong;

typedef uintptr_t CPU_atomic_Pointer;

typedef _Bool CPU_atomic_Flag;

typedef int CPU_atomic_Order;











static inline void _CPU_atomic_Fence( CPU_atomic_Order order )
{
  (void) order;
  ;
}

static inline void _CPU_atomic_Init_uint( CPU_atomic_Uint *obj, unsigned int desired )
{
  *obj = desired;
}

static inline void _CPU_atomic_Init_ulong( CPU_atomic_Ulong *obj, unsigned long desired )
{
  *obj = desired;
}

static inline void _CPU_atomic_Init_ptr( CPU_atomic_Pointer *obj, void *desired )
{
  *obj = (uintptr_t) desired;
}

static inline unsigned int _CPU_atomic_Load_uint( const CPU_atomic_Uint *obj, CPU_atomic_Order order )
{
  unsigned int val;

  (void) order;
  val = *obj;
  ;

  return val;
}

static inline unsigned long _CPU_atomic_Load_ulong( const CPU_atomic_Ulong *obj, CPU_atomic_Order order )
{
  unsigned long val;

  (void) order;
  val = *obj;
  ;

  return val;
}

static inline void *_CPU_atomic_Load_ptr( const CPU_atomic_Pointer *obj, CPU_atomic_Order order )
{
  uintptr_t val;

  (void) order;
  val = *obj;
  ;

  return (void *) val;
}

static inline void _CPU_atomic_Store_uint( CPU_atomic_Uint *obj, unsigned int desired, CPU_atomic_Order order )
{
  (void) order;
  ;
  *obj = desired;
}

static inline void _CPU_atomic_Store_ulong( CPU_atomic_Ulong *obj, unsigned long desired, CPU_atomic_Order order )
{
  (void) order;
  ;
  *obj = desired;
}

static inline void _CPU_atomic_Store_ptr( CPU_atomic_Pointer *obj, void *desired, CPU_atomic_Order order )
{
  (void) order;
  ;
  *obj = (uintptr_t) desired;
}

static inline unsigned int _CPU_atomic_Fetch_add_uint( CPU_atomic_Uint *obj, unsigned int arg, CPU_atomic_Order order )
{
  unsigned int val;
  ISR_Level level;

  (void) order;
  do { do { (level) = (cpu_st_get() & 0x00000001); __asm("   DINT"); } while (0); ( ( void ) 0 ); ; } while (0);
  val = *obj;
  *obj = val + arg;
  do { ; ( ( void ) 0 ); do { SetCSRBit(level); { __asm(" nop");; __asm(" nop");; }; } while (0); } while (0);

  return val;
}

static inline unsigned long _CPU_atomic_Fetch_add_ulong( CPU_atomic_Ulong *obj, unsigned long arg, CPU_atomic_Order order )
{
  unsigned long val;
  ISR_Level level;

  (void) order;
  do { do { (level) = (cpu_st_get() & 0x00000001); __asm("   DINT"); } while (0); ( ( void ) 0 ); ; } while (0);
  val = *obj;
  *obj = val + arg;
  do { ; ( ( void ) 0 ); do { SetCSRBit(level); { __asm(" nop");; __asm(" nop");; }; } while (0); } while (0);

  return val;
}

static inline void *_CPU_atomic_Fetch_add_ptr( CPU_atomic_Pointer *obj, void *arg, CPU_atomic_Order order )
{
  uintptr_t val;
  ISR_Level level;

  (void) order;
  do { do { (level) = (cpu_st_get() & 0x00000001); __asm("   DINT"); } while (0); ( ( void ) 0 ); ; } while (0);
  val = *obj;
  *obj = val + (uintptr_t) arg;
  do { ; ( ( void ) 0 ); do { SetCSRBit(level); { __asm(" nop");; __asm(" nop");; }; } while (0); } while (0);

  return (void *) val;
}

static inline unsigned int _CPU_atomic_Fetch_sub_uint( CPU_atomic_Uint *obj, unsigned int arg, CPU_atomic_Order order )
{
  unsigned int val;
  ISR_Level level;

  (void) order;
  do { do { (level) = (cpu_st_get() & 0x00000001); __asm("   DINT"); } while (0); ( ( void ) 0 ); ; } while (0);
  val = *obj;
  *obj = val - arg;
  do { ; ( ( void ) 0 ); do { SetCSRBit(level); { __asm(" nop");; __asm(" nop");; }; } while (0); } while (0);

  return val;
}

static inline unsigned long _CPU_atomic_Fetch_sub_ulong( CPU_atomic_Ulong *obj, unsigned long arg, CPU_atomic_Order order )
{
  unsigned long val;
  ISR_Level level;

  (void) order;
  do { do { (level) = (cpu_st_get() & 0x00000001); __asm("   DINT"); } while (0); ( ( void ) 0 ); ; } while (0);
  val = *obj;
  *obj = val - arg;
  do { ; ( ( void ) 0 ); do { SetCSRBit(level); { __asm(" nop");; __asm(" nop");; }; } while (0); } while (0);

  return val;
}

static inline void *_CPU_atomic_Fetch_sub_ptr( CPU_atomic_Pointer *obj, void *arg, CPU_atomic_Order order )
{
  uintptr_t val;
  ISR_Level level;

  (void) order;
  do { do { (level) = (cpu_st_get() & 0x00000001); __asm("   DINT"); } while (0); ( ( void ) 0 ); ; } while (0);
  val = *obj;
  *obj = val - (uintptr_t) arg;
  do { ; ( ( void ) 0 ); do { SetCSRBit(level); { __asm(" nop");; __asm(" nop");; }; } while (0); } while (0);

  return (void *) val;
}

static inline unsigned int _CPU_atomic_Fetch_or_uint( CPU_atomic_Uint *obj, unsigned int arg, CPU_atomic_Order order )
{
  unsigned int val;
  ISR_Level level;

  (void) order;
  do { do { (level) = (cpu_st_get() & 0x00000001); __asm("   DINT"); } while (0); ( ( void ) 0 ); ; } while (0);
  val = *obj;
  *obj = val | arg;
  do { ; ( ( void ) 0 ); do { SetCSRBit(level); { __asm(" nop");; __asm(" nop");; }; } while (0); } while (0);

  return val;
}

static inline unsigned long _CPU_atomic_Fetch_or_ulong( CPU_atomic_Ulong *obj, unsigned long arg, CPU_atomic_Order order )
{
  unsigned long val;
  ISR_Level level;

  (void) order;
  do { do { (level) = (cpu_st_get() & 0x00000001); __asm("   DINT"); } while (0); ( ( void ) 0 ); ; } while (0);
  val = *obj;
  *obj = val | arg;
  do { ; ( ( void ) 0 ); do { SetCSRBit(level); { __asm(" nop");; __asm(" nop");; }; } while (0); } while (0);

  return val;
}

static inline void *_CPU_atomic_Fetch_or_ptr( CPU_atomic_Pointer *obj, void *arg, CPU_atomic_Order order )
{
  uintptr_t val;
  ISR_Level level;

  (void) order;
  do { do { (level) = (cpu_st_get() & 0x00000001); __asm("   DINT"); } while (0); ( ( void ) 0 ); ; } while (0);
  val = *obj;
  *obj = val | (uintptr_t) arg;
  do { ; ( ( void ) 0 ); do { SetCSRBit(level); { __asm(" nop");; __asm(" nop");; }; } while (0); } while (0);

  return (void *) val;
}

static inline unsigned int _CPU_atomic_Fetch_and_uint( CPU_atomic_Uint *obj, unsigned int arg, CPU_atomic_Order order )
{
  unsigned int val;
  ISR_Level level;

  (void) order;
  do { do { (level) = (cpu_st_get() & 0x00000001); __asm("   DINT"); } while (0); ( ( void ) 0 ); ; } while (0);
  val = *obj;
  *obj = val & arg;
  do { ; ( ( void ) 0 ); do { SetCSRBit(level); { __asm(" nop");; __asm(" nop");; }; } while (0); } while (0);

  return val;
}

static inline unsigned long _CPU_atomic_Fetch_and_ulong( CPU_atomic_Ulong *obj, unsigned long arg, CPU_atomic_Order order )
{
  unsigned long val;
  ISR_Level level;

  (void) order;
  do { do { (level) = (cpu_st_get() & 0x00000001); __asm("   DINT"); } while (0); ( ( void ) 0 ); ; } while (0);
  val = *obj;
  *obj = val & arg;
  do { ; ( ( void ) 0 ); do { SetCSRBit(level); { __asm(" nop");; __asm(" nop");; }; } while (0); } while (0);

  return val;
}

static inline void *_CPU_atomic_Fetch_and_ptr( CPU_atomic_Pointer *obj, void *arg, CPU_atomic_Order order )
{
  uintptr_t val;
  ISR_Level level;

  (void) order;
  do { do { (level) = (cpu_st_get() & 0x00000001); __asm("   DINT"); } while (0); ( ( void ) 0 ); ; } while (0);
  val = *obj;
  *obj = val & (uintptr_t) arg;
  do { ; ( ( void ) 0 ); do { SetCSRBit(level); { __asm(" nop");; __asm(" nop");; }; } while (0); } while (0);

  return (void *) val;
}

static inline unsigned int _CPU_atomic_Exchange_uint( CPU_atomic_Uint *obj, unsigned int desired, CPU_atomic_Order order )
{
  unsigned int val;
  ISR_Level level;

  (void) order;
  do { do { (level) = (cpu_st_get() & 0x00000001); __asm("   DINT"); } while (0); ( ( void ) 0 ); ; } while (0);
  val = *obj;
  *obj = desired;
  do { ; ( ( void ) 0 ); do { SetCSRBit(level); { __asm(" nop");; __asm(" nop");; }; } while (0); } while (0);

  return val;
}

static inline unsigned long _CPU_atomic_Exchange_ulong( CPU_atomic_Ulong *obj, unsigned long desired, CPU_atomic_Order order )
{
  unsigned long val;
  ISR_Level level;

  (void) order;
  do { do { (level) = (cpu_st_get() & 0x00000001); __asm("   DINT"); } while (0); ( ( void ) 0 ); ; } while (0);
  val = *obj;
  *obj = desired;
  do { ; ( ( void ) 0 ); do { SetCSRBit(level); { __asm(" nop");; __asm(" nop");; }; } while (0); } while (0);

  return val;
}

static inline void *_CPU_atomic_Exchange_ptr( CPU_atomic_Pointer *obj, void *desired, CPU_atomic_Order order )
{
  uintptr_t val;
  ISR_Level level;

  (void) order;
  do { do { (level) = (cpu_st_get() & 0x00000001); __asm("   DINT"); } while (0); ( ( void ) 0 ); ; } while (0);
  val = *obj;
  *obj = (uintptr_t) desired;
  do { ; ( ( void ) 0 ); do { SetCSRBit(level); { __asm(" nop");; __asm(" nop");; }; } while (0); } while (0);

  return (void *) val;
}

static inline _Bool _CPU_atomic_Compare_exchange_uint( CPU_atomic_Uint *obj, unsigned int *expected, unsigned int desired, CPU_atomic_Order succ, CPU_atomic_Order fail )
{
  _Bool success;
  ISR_Level level;

  (void) succ;
  (void) fail;
  do { do { (level) = (cpu_st_get() & 0x00000001); __asm("   DINT"); } while (0); ( ( void ) 0 ); ; } while (0);
  success = *obj == *expected;
  if ( success ) {
    *obj = desired;
  }
  do { ; ( ( void ) 0 ); do { SetCSRBit(level); { __asm(" nop");; __asm(" nop");; }; } while (0); } while (0);

  return success;
}

static inline _Bool _CPU_atomic_Compare_exchange_ulong( CPU_atomic_Ulong *obj, unsigned long *expected, unsigned long desired, CPU_atomic_Order succ, CPU_atomic_Order fail )
{
  _Bool success;
  ISR_Level level;

  (void) succ;
  (void) fail;
  do { do { (level) = (cpu_st_get() & 0x00000001); __asm("   DINT"); } while (0); ( ( void ) 0 ); ; } while (0);
  success = *obj == *expected;
  if ( success ) {
    *obj = desired;
  }
  do { ; ( ( void ) 0 ); do { SetCSRBit(level); { __asm(" nop");; __asm(" nop");; }; } while (0); } while (0);

  return success;
}

static inline _Bool _CPU_atomic_Compare_exchange_ptr( CPU_atomic_Pointer *obj, void **expected, void *desired, CPU_atomic_Order succ, CPU_atomic_Order fail )
{
  _Bool success;
  ISR_Level level;

  (void) succ;
  (void) fail;
  do { do { (level) = (cpu_st_get() & 0x00000001); __asm("   DINT"); } while (0); ( ( void ) 0 ); ; } while (0);
  success = *obj == (uintptr_t) *expected;
  if ( success ) {
    *obj = (uintptr_t) desired;
  }
  do { ; ( ( void ) 0 ); do { SetCSRBit(level); { __asm(" nop");; __asm(" nop");; }; } while (0); } while (0);

  return success;
}

static inline void _CPU_atomic_Flag_clear( CPU_atomic_Flag *obj, CPU_atomic_Order order )
{
  (void) order;
  *obj = 0;
}

static inline _Bool _CPU_atomic_Flag_test_and_set( CPU_atomic_Flag *obj, CPU_atomic_Order order )
{
  _Bool flag;
  ISR_Level level;

  (void) order;
  do { do { (level) = (cpu_st_get() & 0x00000001); __asm("   DINT"); } while (0); ( ( void ) 0 ); ; } while (0);
  flag = *obj;
  *obj = 1;
  do { ; ( ( void ) 0 ); do { SetCSRBit(level); { __asm(" nop");; __asm(" nop");; }; } while (0); } while (0);

  return flag;
}
















 

typedef CPU_atomic_Uint Atomic_Uint;

typedef CPU_atomic_Ulong Atomic_Ulong;

typedef CPU_atomic_Pointer Atomic_Pointer;

typedef CPU_atomic_Flag Atomic_Flag;

typedef CPU_atomic_Order Atomic_Order;








































 







 








 












 
 








 

















 








 










 








 













 









 










 








 


 

 










 








 










 








 








 










 













 
 








 
typedef struct Chain_Node_struct Chain_Node;















 
struct Chain_Node_struct {
   
  Chain_Node *next;
   
  Chain_Node *previous;
};











 
typedef union {
  struct {
    Chain_Node Node;
    Chain_Node *fill;
  } Head;

  struct {
    Chain_Node *fill;
    Chain_Node Node;
  } Tail;
} Chain_Control;

 


 






 
 

  




 







 
 





 
 




 
typedef union {
     
    const char *name_p;
   
  uint32_t    name_u32;
} Objects_Name;









 
typedef uint32_t   Objects_Id;




 
typedef uint16_t   Objects_Maximum;




 



 




 




 



 



 



 



 




 




 




 




 




 



 



 



 
typedef enum {
  OBJECTS_NO_API       = 0,
  OBJECTS_INTERNAL_API = 1,
  OBJECTS_CLASSIC_API  = 2,
  OBJECTS_POSIX_API    = 3,
  OBJECTS_FAKE_OBJECTS_API = 7
} Objects_APIs;

 




 
typedef struct {
   
  Chain_Node     Node;
   
  Objects_Id     id;
   
  Objects_Name   name;
} Objects_Control;




 
typedef struct {
   
  Objects_Control Object;
  


 
  uint32_t        name;
}   Objects_MP_Control;



 




 




 




 




 




 




 



 










 







 
static inline Objects_APIs _Objects_Get_API(
  Objects_Id id
)
{
  return (Objects_APIs) ((id >> 24U) & (Objects_Id)0x00000007U);
}





 
static inline uint32_t _Objects_Get_class(
  Objects_Id id
)
{
  return (uint32_t)
    ((id >> 27U) & (Objects_Id)0x0000001fU);
}







 
static inline uint32_t _Objects_Get_node(
  Objects_Id id
)
{
  


 
    return (id >> 16U) & (Objects_Id)0x000000ffU;
}







 
static inline Objects_Maximum _Objects_Get_index(
  Objects_Id id
)
{
  return
    (Objects_Maximum)((id >> 0U) &
                                          (Objects_Id)0x0000ffffU);
}












 
static inline Objects_Id _Objects_Build_id(
  Objects_APIs     the_api,
  uint16_t         the_class,
  uint8_t          node,
  uint16_t         index
)
{
  return (( (Objects_Id) the_api )   << 24U)   |
         (( (Objects_Id) the_class ) << 27U) |
           (( (Objects_Id) node )    << 16U)  |
         (( (Objects_Id) index )     << 0U);
}








 
static inline _Bool _Objects_Is_unlimited( uint32_t maximum )
{
  return (maximum & 0x80000000U) != 0;
}




 

 
 
 


 








 








 

















 
 



 







 
typedef uint32_t   Priority_Control;

 





 

 

 



 
extern uint8_t rtems_maximum_priority;


 

 








 








 














 
 





 
typedef uint32_t   Watchdog_Interval;





 
typedef void Watchdog_Service_routine;





 
typedef Watchdog_Service_routine ( *Watchdog_Service_routine_entry )(
                 Objects_Id,
                 void *
             );






 






 

typedef enum {
   
  WATCHDOG_INACTIVE,
  

 
  WATCHDOG_BEING_INSERTED,
   
  WATCHDOG_ACTIVE
} Watchdog_States;






 
typedef struct {
  

 
  Chain_Node                      Node;
   
  Watchdog_States                 state;
   
  Watchdog_Interval               initial;
   
  Watchdog_Interval               delta_interval;
   
  Watchdog_Interval               start_time;
   
  Watchdog_Interval               stop_time;
   
  Watchdog_Service_routine_entry  routine;
   
  Objects_Id                      id;
  

 
  void                           *user_data;
}   Watchdog_Control;





 
extern volatile Watchdog_Interval _Watchdog_Ticks_since_boot;

 


 










 
 







 
typedef enum {
  MP_PACKET_MPCI_INTERNAL    = 0,
  MP_PACKET_TASKS            = 1,
  MP_PACKET_MESSAGE_QUEUE    = 2,
  MP_PACKET_SEMAPHORE        = 3,
  MP_PACKET_PARTITION        = 4,
  MP_PACKET_REGION           = 5,
  MP_PACKET_EVENT            = 6,
  MP_PACKET_SIGNAL           = 7
}   MP_packet_Classes;



 



 








 
typedef struct {
   
  MP_packet_Classes       the_class;
   
  Objects_Id              id;
   
  Objects_Id              source_tid;
   
  Priority_Control        source_priority;
   
  uint32_t                return_code;
   
  uint32_t                length;
   
  uint32_t                to_convert;
   
  Watchdog_Interval       timeout;
}   MP_packet_Prefix;



 







 

 


 






 













 








 










 





















 






 
typedef struct {
} ISR_lock_Control;



 
typedef struct {
  ISR_Level isr_level;
} ISR_lock_Context;







 








 










 








 






 










 







 














 














 
















 












 
















 











 










 

 














 






















































































































 

typedef struct Resource_Node Resource_Node;

typedef struct Resource_Control Resource_Control;




 
struct Resource_Node {
  



 
  Chain_Node Node;

  



 
  Chain_Control Resources;

  




 
  Resource_Control *dependency;

  



 
  Resource_Node *root;
};




 
struct Resource_Control {
  



 
  Chain_Node Node;

  



 
  Chain_Control Rivals;

  

 
  Resource_Node *owner;
};

 










 








 











 
 




 



 
typedef struct {
   
  size_t      size;
   
  void       *area;
}   Stack_Control;





 
extern uint32_t rtems_minimum_stack_size;

 


 






 








 











 
 




 
typedef uint32_t   States_Control;

 


 







 








 









 







 


 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 



#pragma diag_push
#pragma CHECK_MISRA("-19.15")  

 
 
 
 
 
 
 

#pragma diag_pop








 








 











 
 












 
static inline void *_Addresses_Add_offset (
  const void *base,
  uintptr_t   offset
)
{
  return (void *)((uintptr_t)base + offset);
}












 

static inline void *_Addresses_Subtract_offset (
  const void *base,
  uintptr_t   offset
)
{
  return (void *)((uintptr_t)base - offset);
}














 
static inline int32_t _Addresses_Subtract (
  const void *left,
  const void *right
)
{
  return (int32_t) ((const char *) left - (const char *) right);
}












 
static inline _Bool _Addresses_Is_aligned (
  const void *address
)
{
    return (((uintptr_t)address % 8) == 0);
}
















 
static inline _Bool _Addresses_Is_in_range (
  const void *address,
  const void *base,
  const void *limit
)
{
  return (address >= base && address <= limit);
}













 
static inline void *_Addresses_Align_up(
  void *address,
  size_t alignment
)
{
  uintptr_t mask = alignment - (uintptr_t)1;
  return (void*)(((uintptr_t)address + mask) & ~mask);
}













 
static inline void *_Addresses_Align_down(
  void *address,
  size_t alignment
)
{
  uintptr_t mask = alignment - (uintptr_t)1;
  return (void*)((uintptr_t)address & ~mask);
}

 


 












 
 








 
typedef struct RBTree_Node_struct RBTree_Node;



 
typedef enum {
  RBT_BLACK,
  RBT_RED
} RBTree_Color;















 
struct RBTree_Node_struct {
   
  RBTree_Node *parent;
   
  RBTree_Node *child[2];
   
  RBTree_Color color;
};



 
typedef enum {
  RBT_LEFT=0,
  RBT_RIGHT=1
} RBTree_Direction;







 
typedef long RBTree_Compare_result;













 
typedef RBTree_Compare_result ( *RBTree_Compare )(
  const RBTree_Node *first,
  const RBTree_Node *second
);












 






 
typedef struct {
   
  RBTree_Node *permanent_null;
   
  RBTree_Node *root;
   
  RBTree_Node *first[2];
} RBTree_Control;



 



 



 



 















 
void _RBTree_Initialize(
  RBTree_Control *the_rbtree,
  RBTree_Compare  compare,
  void           *starting_address,
  size_t          number_nodes,
  size_t          node_size,
  _Bool            is_unique
);














 
RBTree_Node *_RBTree_Find(
  const RBTree_Control *the_rbtree,
  const RBTree_Node    *the_node,
  RBTree_Compare        compare,
  _Bool                  is_unique
);
















 
RBTree_Node *_RBTree_Insert(
  RBTree_Control *the_rbtree,
  RBTree_Node    *the_node,
  RBTree_Compare  compare,
  _Bool            is_unique
);












 
void _RBTree_Extract(
  RBTree_Control *the_rbtree,
  RBTree_Node *the_node
);









 
RBTree_Node *_RBTree_Next(
  const RBTree_Node *node,
  RBTree_Direction dir
);









 
static inline void _RBTree_Set_off_tree( RBTree_Node *the_node )
{
  the_node->parent = 0;
}











 
static inline _Bool _RBTree_Is_node_off_tree(
  const RBTree_Node *the_node
)
{
  return the_node->parent == 0;
}












 
static inline RBTree_Node *_RBTree_Root(
  const RBTree_Control *the_rbtree
)
{
  return the_rbtree->root;
}






 
static inline RBTree_Node *_RBTree_First(
  const RBTree_Control *the_rbtree,
  RBTree_Direction dir
)
{
  return the_rbtree->first[dir];
}












 
static inline RBTree_Node *_RBTree_Parent(
  const RBTree_Node *the_node
)
{
  return the_node->parent;
}









 
static inline RBTree_Node *_RBTree_Left(
  const RBTree_Node *the_node
)
{
  return the_node->child[RBT_LEFT];
}









 
static inline RBTree_Node *_RBTree_Right(
  const RBTree_Node *the_node
)
{
  return the_node->child[RBT_RIGHT];
}











 
static inline _Bool _RBTree_Is_empty(
  const RBTree_Control *the_rbtree
)
{
  return (the_rbtree->root == 0);
}











 
static inline _Bool _RBTree_Is_first(
  const RBTree_Control *the_rbtree,
  const RBTree_Node *the_node,
  RBTree_Direction dir
)
{
  return (the_node == _RBTree_First(the_rbtree, dir));
}















 
static inline _Bool _RBTree_Is_root(
  const RBTree_Node *the_node
)
{
  return _RBTree_Parent( _RBTree_Parent( the_node ) ) == 0;
}





 
static inline void _RBTree_Initialize_empty(
  RBTree_Control *the_rbtree
)
{
  the_rbtree->permanent_null   = 0;
  the_rbtree->root             = 0;
  the_rbtree->first[RBT_LEFT]  = 0;
  the_rbtree->first[RBT_RIGHT] = 0;
}








 
static inline RBTree_Node *_RBTree_Predecessor(
  const RBTree_Node *node
)
{
  return _RBTree_Next( node, RBT_LEFT );
}







 
static inline RBTree_Node *_RBTree_Successor(
  const RBTree_Node *node
)
{
  return _RBTree_Next( node, RBT_RIGHT );
}

















 
static inline RBTree_Node *_RBTree_Get(
  RBTree_Control *the_rbtree,
  RBTree_Direction dir
)
{
  RBTree_Node *the_node = the_rbtree->first[ dir ];

  if ( the_node != 0 ) {
    _RBTree_Extract( the_rbtree, the_node );
  }

  return the_node;
}

 


 










 
 

typedef struct Thread_Control Thread_Control;

typedef struct Thread_queue_Control Thread_queue_Control;









 
typedef void ( *Thread_queue_Priority_change_operation )(
  Thread_Control       *the_thread,
  Priority_Control      new_priority,
  Thread_queue_Control *the_thread_queue
);







 
typedef void ( *Thread_queue_Initialize_operation )(
  Thread_queue_Control *the_thread_queue
);








 
typedef void ( *Thread_queue_Enqueue_operation )(
  Thread_queue_Control *the_thread_queue,
  Thread_Control       *the_thread
);








 
typedef void ( *Thread_queue_Extract_operation )(
  Thread_queue_Control *the_thread_queue,
  Thread_Control       *the_thread
);











 
typedef Thread_Control *( *Thread_queue_First_operation )(
  Thread_queue_Control *the_thread_queue
);





 
typedef struct {
  








 
  Thread_queue_Priority_change_operation priority_change;

  



 
  Thread_queue_Initialize_operation initialize;

  



 
  Thread_queue_Enqueue_operation enqueue;

  



 
  Thread_queue_Extract_operation extract;

  

 
  Thread_queue_First_operation first;
} Thread_queue_Operations;




 
typedef enum {
  THREAD_QUEUE_DISCIPLINE_FIFO,      
  THREAD_QUEUE_DISCIPLINE_PRIORITY   
}   Thread_queue_Disciplines;




 
struct Thread_queue_Control {
  

 
  union {
     
    Chain_Control Fifo;
     
    RBTree_Control Priority;
  } Queues;

  

 
  const Thread_queue_Operations *operations;

  







 
  
};

 


 


struct Per_CPU_Control;

struct Scheduler_Control;

struct Scheduler_Node;
















 
 





 





 






 








 





















 
 































 

































 








 
struct itimerspec {
	struct timespec it_interval;
	struct timespec it_value;
};



























 




 
struct timeval {
	time_t		tv_sec;		 
	uint64_t	tv_usec;	 
};



struct timezone {
	int	tz_minuteswest;	 
	int	tz_dsttime;	 
};

struct bintime {
	time_t	sec;
	uint64_t frac;
};

static __inline void
bintime_addx(struct bintime *_bt, uint64_t _x)
{
	uint64_t _u;

	_u = _bt->frac;
	_bt->frac += _x;
	if (_u > _bt->frac)
		_bt->sec++;
}

static __inline void
bintime_add(struct bintime *_bt, const struct bintime *_bt2)
{
	uint64_t _u;

	_u = _bt->frac;
	_bt->frac += _bt2->frac;
	if (_u > _bt->frac)
		_bt->sec++;
	_bt->sec += _bt2->sec;
}

static __inline void
bintime_sub(struct bintime *_bt, const struct bintime *_bt2)
{
	uint64_t _u;

	_u = _bt->frac;
	_bt->frac -= _bt2->frac;
	if (_u < _bt->frac)
		_bt->sec--;
	_bt->sec -= _bt2->sec;
}

static __inline void
bintime_mul(struct bintime *_bt, u_int _x)
{
	uint64_t _p1, _p2;

	_p1 = (_bt->frac & 0xffffffffull) * _x;
	_p2 = (_bt->frac >> 32) * _x + (_p1 >> 32);
	_bt->sec *= _x;
	_bt->sec += (_p2 >> 32);
	_bt->frac = (_p2 << 32) | (_p1 & 0xffffffffull);
}

static __inline void
bintime_shift(struct bintime *_bt, int _exp)
{

	if (_exp > 0) {
		_bt->sec <<= _exp;
		_bt->sec |= _bt->frac >> (64 - _exp);
		_bt->frac <<= _exp;
	} else if (_exp < 0) {
		_bt->frac >>= -_exp;
		_bt->frac |= (uint64_t)_bt->sec << (64 + _exp);
		_bt->sec >>= -_exp;
	}
}



static __inline int
sbintime_getsec(sbintime_t _sbt)
{

	return (_sbt >> 32);
}

static __inline sbintime_t
bttosbt(const struct bintime _bt)
{

	return (((sbintime_t)_bt.sec << 32) + (_bt.frac >> 32));
}

static __inline struct bintime
sbttobt(sbintime_t _sbt)
{
	struct bintime _bt;

	_bt.sec = _sbt >> 32;
	_bt.frac = _sbt << 32;
	return (_bt);
}













 

static __inline void
bintime2timespec(const struct bintime *_bt, struct timespec *_ts)
{

	_ts->tv_sec = _bt->sec;
	_ts->tv_nsec = ((uint64_t)1000000000 *
	    (uint32_t)(_bt->frac >> 32)) >> 32;
}

static __inline void
timespec2bintime(const struct timespec *_ts, struct bintime *_bt)
{

	_bt->sec = _ts->tv_sec;
	 
	_bt->frac = _ts->tv_nsec * (uint64_t)18446744073LL;
}

static __inline void
bintime2timeval(const struct bintime *_bt, struct timeval *_tv)
{

	_tv->tv_sec = _bt->sec;
	_tv->tv_usec = ((uint64_t)1000000 * (uint32_t)(_bt->frac >> 32)) >> 32;
}

static __inline void
timeval2bintime(const struct timeval *_tv, struct bintime *_bt)
{

	_bt->sec = _tv->tv_sec;
	 
	_bt->frac = _tv->tv_usec * (uint64_t)18446744073709LL;
}

static __inline struct timespec
sbttots(sbintime_t _sbt)
{
	struct timespec _ts;

	_ts.tv_sec = _sbt >> 32;
	_ts.tv_nsec = ((uint64_t)1000000000 * (uint32_t)_sbt) >> 32;
	return (_ts);
}

static __inline sbintime_t
tstosbt(struct timespec _ts)
{

	return (((sbintime_t)_ts.tv_sec << 32) +
	    (_ts.tv_nsec * (((uint64_t)1 << 63) / 500000000) >> 32));
}

static __inline struct timeval
sbttotv(sbintime_t _sbt)
{
	struct timeval _tv;

	_tv.tv_sec = _sbt >> 32;
	_tv.tv_usec = ((uint64_t)1000000 * (uint32_t)_sbt) >> 32;
	return (_tv);
}

static __inline sbintime_t
tvtosbt(struct timeval _tv)
{

	return (((sbintime_t)_tv.tv_sec << 32) +
	    (_tv.tv_usec * (((uint64_t)1 << 63) / 500000) >> 32));
}


 

 


 






 

struct itimerval {
	struct	timeval it_interval;	 
	struct	timeval it_value;	 
};



 
struct clockinfo {
	int	hz;		 
	int	tick;		 
	int	spare;
	int	stathz;		 
	int	profhz;		 
};

 






 
void	inittodr(time_t base);
void	resettodr(void);

extern volatile time_t	time_second;
extern volatile time_t	time_uptime;
extern struct bintime boottimebin;
extern struct timeval boottime;
extern struct bintime tc_tick_bt;
extern sbintime_t tc_tick_sbt;
extern struct bintime tick_bt;
extern sbintime_t tick_sbt;
extern int tc_precexp;
extern int tc_timepercentage;
extern struct bintime bt_timethreshold;
extern struct bintime bt_tickthreshold;
extern sbintime_t sbt_timethreshold;
extern sbintime_t sbt_tickthreshold;




















 

void	binuptime(struct bintime *bt);
void	nanouptime(struct timespec *tsp);
void	microuptime(struct timeval *tvp);

static __inline sbintime_t
sbinuptime(void)
{
	struct bintime _bt;

	binuptime(&_bt);
	return (bttosbt(_bt));
}

void	bintime(struct bintime *bt);
void	nanotime(struct timespec *tsp);
void	microtime(struct timeval *tvp);

void	getbinuptime(struct bintime *bt);
void	getnanouptime(struct timespec *tsp);
void	getmicrouptime(struct timeval *tvp);

static __inline sbintime_t
getsbinuptime(void)
{
	struct bintime _bt;

	getbinuptime(&_bt);
	return (bttosbt(_bt));
}

void	getbintime(struct bintime *bt);
void	getnanotime(struct timespec *tsp);
void	getmicrotime(struct timeval *tvp);

 
int	itimerdecr(struct itimerval *itp, int usec);
int	itimerfix(struct timeval *tv);
int	ppsratecheck(struct timeval *, int *, int);
int	ratecheck(struct timeval *, const struct timeval *);
void	timevaladd(struct timeval *t1, const struct timeval *t2);
void	timevalsub(struct timeval *t1, const struct timeval *t2);
int	tvtohz(struct timeval *tv);













 








 









 
 

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 













 








 









 









 









 
uint64_t _Timespec_Get_as_nanoseconds(
  const struct timespec *time
);










 
_Bool _Timespec_Is_valid(
  const struct timespec *time
);











 
_Bool _Timespec_Less_than(
  const struct timespec *lhs,
  const struct timespec *rhs
);











 











 











 
uint32_t _Timespec_Add_to(
  struct timespec       *time,
  const struct timespec *add
);










 
uint32_t _Timespec_To_ticks(
  const struct timespec *time
);









 
void _Timespec_From_ticks(
  uint32_t         ticks,
  struct timespec *time
);












 
void _Timespec_Subtract(
  const struct timespec *start,
  const struct timespec *end,
  struct timespec       *result
);













 
void _Timespec_Divide_by_integer(
  const struct timespec *time,
  uint32_t               iterations,
  struct timespec       *result
);













 
void _Timespec_Divide(
  const struct timespec *lhs,
  const struct timespec *rhs,
  uint32_t              *ival_percentage,
  uint32_t              *fval_percentage
);


 

 




 
typedef struct bintime Timestamp_Control;










 
static inline void _Timestamp_Set(
  Timestamp_Control *_time,
  time_t             _seconds,
  long               _nanoseconds
)
{
  struct timespec _ts;

  _ts.tv_sec = _seconds;
  _ts.tv_nsec = _nanoseconds;

  timespec2bintime( &_ts, _time );
}








 

static inline void _Timestamp_Set_to_zero(
  Timestamp_Control *_time
)
{
  _time->sec = 0;
  _time->frac = 0;
}











 

static inline _Bool _Timestamp_Less_than(
  const Timestamp_Control *_lhs,
  const Timestamp_Control *_rhs
)
{
  if ( _lhs->sec < _rhs->sec )
    return 1;

  if ( _lhs->sec > _rhs->sec )
    return 0;

  return _lhs->frac < _rhs->frac;
}











 

static inline _Bool _Timestamp_Greater_than(
  const Timestamp_Control *_lhs,
  const Timestamp_Control *_rhs
)
{
  if ( _lhs->sec > _rhs->sec )
    return 1;

  if ( _lhs->sec < _rhs->sec )
    return 0;

  return _lhs->frac > _rhs->frac;
}











 

static inline _Bool _Timestamp_Equal_to(
  const Timestamp_Control *_lhs,
  const Timestamp_Control *_rhs
)
{
  return _lhs->sec == _rhs->sec && _lhs->frac == _rhs->frac;
}











 
static inline time_t _Timestamp_Add_to(
  Timestamp_Control *_time,
  const Timestamp_Control *_add
)
{
  time_t seconds = _time->sec;

  bintime_add( _time, _add );

  return _time->sec - seconds;
}













 
static inline void _Timestamp_Subtract(
  const Timestamp_Control *_start,
  const Timestamp_Control *_end,
  Timestamp_Control       *_result
)
{
  _result->sec = _end->sec;
  _result->frac = _end->frac;

  bintime_sub( _result, _start );
}













 
static inline void _Timestamp_Divide(
  const Timestamp_Control *_lhs,
  const Timestamp_Control *_rhs,
  uint32_t                *_ival_percentage,
  uint32_t                *_fval_percentage
)
{
  struct timespec _ts_left;
  struct timespec _ts_right;

  bintime2timespec( _lhs, &_ts_left );
  bintime2timespec( _rhs, &_ts_right );

  _Timespec_Divide(
    &_ts_left,
    &_ts_right,
    _ival_percentage,
    _fval_percentage
  );
}









 
static inline time_t _Timestamp_Get_seconds(
  const Timestamp_Control *_time
)
{
  return _time->sec;
}









 
static inline uint32_t _Timestamp_Get_nanoseconds(
  const Timestamp_Control *_time
)
{
  struct timespec _ts;

  bintime2timespec( _time, &_ts );

  return _ts.tv_nsec;
}









 
static inline uint64_t _Timestamp_Get_as_nanoseconds(
  const Timestamp_Control *_time
)
{
  struct timespec _ts;

  bintime2timespec( _time, &_ts );

  return _Timespec_Get_as_nanoseconds( &_ts );
}








 
static inline void _Timestamp_To_timespec(
  const Timestamp_Control *_timestamp,
  struct timespec         *_timespec
)
{
  bintime2timespec( _timestamp, _timespec );
}






 
static inline void _Timestamp_To_timeval(
  const Timestamp_Control *_timestamp,
  struct timeval          *_timeval
)
{
  bintime2timeval( _timestamp, _timeval );
}


 

 

typedef Timestamp_Control Thread_CPU_usage_t;








 
typedef void *Thread;









 
typedef CPU_Uint32ptr Thread_Entry_numeric_type;







 
typedef enum {
  THREAD_START_NUMERIC,
  THREAD_START_POINTER,
} Thread_Start_types;

 
typedef Thread ( *Thread_Entry )( void );    



 
typedef Thread ( *Thread_Entry_numeric )( Thread_Entry_numeric_type );



 
typedef Thread ( *Thread_Entry_pointer )( void * );



 
typedef Thread ( *Thread_Entry_both_pointer_first )( void *, Thread_Entry_numeric_type );




 
typedef Thread ( *Thread_Entry_both_numeric_first )( Thread_Entry_numeric_type, void * );







 
typedef enum {
  THREAD_CPU_BUDGET_ALGORITHM_NONE,
  THREAD_CPU_BUDGET_ALGORITHM_RESET_TIMESLICE,
    THREAD_CPU_BUDGET_ALGORITHM_EXHAUST_TIMESLICE,
    THREAD_CPU_BUDGET_ALGORITHM_CALLOUT
}  Thread_CPU_budget_algorithms;



 
typedef void (*Thread_CPU_budget_algorithm_callout )( Thread_Control * );





 
struct rtems_task_variable_tt;





 
typedef struct {
   
  struct rtems_task_variable_tt  *next;
  

 
  void                          **ptr;
   
  void                           *gval;
   
  void                           *tval;
   
  void                          (*dtor)(void *);
} rtems_task_variable_t;




 
typedef struct {
   
  Thread_Entry                         entry_point;
   
  Thread_Start_types                   prototype;
   
  void                                *pointer_argument;
   
  Thread_Entry_numeric_type            numeric_argument;
   
  

 
  _Bool                                 is_preemptible;
   
  Thread_CPU_budget_algorithms         budget_algorithm;
  

 
  Thread_CPU_budget_algorithm_callout  budget_callout;
   
  uint32_t                             isr_level;
   
  Priority_Control                     initial_priority;
     
    _Bool                                 core_allocated_stack;
   
  Stack_Control                        Initial_stack;
   
  void                                *stack;
   
  void                                *tls_area;
} Thread_Start_information;










 
typedef union {
  void       *mutable_object;
  const void *immutable_object;
} Thread_Wait_information_Object_argument_type;














 
typedef unsigned int Thread_Wait_flags;






 
typedef struct {
  

 
  union {
    

 
    Chain_Node Chain;

    

 
    RBTree_Node RBTree;
  } Node;

   
  Objects_Id            id;
   
  uint32_t              count;
   
  void                 *return_argument;
   
  Thread_Wait_information_Object_argument_type
                        return_argument_second;
   
  uint32_t              option;
  



 
  uint32_t              return_code;

  

 
  uint32_t timeout_code;

  






 
  Thread_queue_Control *queue;

  


 
  Thread_Wait_flags     flags;

  





 
  const Thread_queue_Operations *operations;
}   Thread_Wait_information;







 
typedef struct {
   
  Objects_Control          Object;
   
  States_Control           current_state;

  




 
  Priority_Control         current_priority;

  




 
  Priority_Control         real_priority;

  




 
  uint32_t                 priority_generation;

  







 
  _Bool                     priority_restore_hint;

   
  uint32_t                 resource_count;

   
  Thread_Wait_information  Wait;
   
  Watchdog_Control         Timer;
   
  MP_packet_Prefix        *receive_packet;
      
   
  Chain_Node               Active;
}   Thread_Proxy_control;







 
typedef enum {
   
  THREAD_API_RTEMS,
   
  THREAD_API_POSIX
}  Thread_APIs;

 

 

typedef struct Thread_Action Thread_Action;
















 
typedef void ( *Thread_Action_handler )(
  Thread_Control         *thread,
  Thread_Action          *action,
  struct Per_CPU_Control *cpu,
  ISR_Level               level
);

















 
struct Thread_Action {
  Chain_Node            Node;
  Thread_Action_handler handler;
};





 
typedef struct {
  Chain_Control Chain;
} Thread_Action_control;







 
typedef enum {
  THREAD_LIFE_NORMAL = 0x0,
  THREAD_LIFE_PROTECTED = 0x1,
  THREAD_LIFE_RESTARTING = 0x2,
  THREAD_LIFE_PROTECTED_RESTARTING = 0x3,
  THREAD_LIFE_TERMINATING = 0x4,
  THREAD_LIFE_PROTECTED_TERMINATING = 0x5,
  THREAD_LIFE_RESTARTING_TERMINATING = 0x6,
  THREAD_LIFE_PROTECTED_RESTARTING_TERMINATING = 0x7
} Thread_Life_state;



 
typedef struct {
  


 
  Thread_Action      Action;

  

 
  Thread_Life_state  state;

  





 
  Thread_Control    *terminator;
} Thread_Life_control;




 
typedef struct {

  







 
  struct Scheduler_Node *node;

} Thread_Scheduler_control;

typedef struct  {
  uint32_t      flags;
  void *        control;
}Thread_Capture_control;




 
struct Thread_Control {
   
  Objects_Control          Object;
   
  States_Control           current_state;

  




 
  Priority_Control         current_priority;

  




 
  Priority_Control         real_priority;

  




 
  uint32_t                 priority_generation;

  







 
  _Bool                     priority_restore_hint;

   
  uint32_t                 resource_count;
   
  Thread_Wait_information  Wait;
   
  Watchdog_Control         Timer;
   
  MP_packet_Prefix        *receive_packet;
      


   
  _Bool                                  is_global;
   
  _Bool                                  is_preemptible;
   
  _Bool                                  is_fp;

  

 
  Thread_Scheduler_control              Scheduler;

  


 
  uint32_t                              cpu_time_budget;
  


 
  Thread_CPU_budget_algorithms          budget_algorithm;
   
  Thread_CPU_budget_algorithm_callout   budget_callout;
  

 
  Thread_CPU_usage_t                    cpu_time_used;

  

 
  Thread_Start_information              Start;

  Thread_Action_control                 Post_switch_actions;

   
  Context_Control                       Registers;
   
  struct _reent                        *libc_reent;
   
  void                                 *API_Extensions[ THREAD_API_POSIX + 1 ];

   
  rtems_task_variable_t                *task_variables;

  





 
  Chain_Control           Key_Chain;

  



 
  Thread_Life_control                   Life;

  Thread_Capture_control                Capture;

  



 
  void                                 *extensions[ 0 ];
};



 
typedef void (*rtems_per_thread_routine)( Thread_Control * );





 
void rtems_iterate_over_all_threads(
  rtems_per_thread_routine routine
);



 
typedef struct {
  



 
  size_t destination_offset;

  


 
  size_t source_offset;
} Thread_Control_add_on;













 
extern const Thread_Control_add_on _Thread_Control_add_ons[];







 
extern const size_t _Thread_Control_add_on_count;







 
extern const size_t _Thread_Control_size;

 


 




































































































 
 

typedef struct Heap_Control Heap_Control;

typedef struct Heap_Block Heap_Block;





 



 
struct Heap_Block {
  











 
  uintptr_t prev_size;


  











 
  uintptr_t size_and_flag;


  







 
  Heap_Block *next;

  




 
  Heap_Block *prev;
};







 
typedef struct {
  



 
  uint64_t lifetime_allocated;

  



 
  uint64_t lifetime_freed;

  



 
  uintptr_t size;

  



 
  uintptr_t free_size;

  



 
  uintptr_t min_free_size;

  

 
  uint32_t free_blocks;

  

 
  uint32_t max_free_blocks;

  

 
  uint32_t used_blocks;

  

 
  uint32_t max_search;

  

 
  uint32_t searches;

  

 
  uint32_t allocs;

  

 
  uint32_t failed_allocs;

  

 
  uint32_t frees;

  

 
  uint32_t resizes;
} Heap_Statistics;



 
struct Heap_Control {
  Heap_Block free_list;
  uintptr_t page_size;
  uintptr_t min_block_size;
  uintptr_t area_begin;
  uintptr_t area_end;
  Heap_Block *first_block;
  Heap_Block *last_block;
  Heap_Statistics stats;
};



 
typedef struct {
  

 
  uint32_t number;

  

 
  uint32_t largest;

  

 
  uint32_t total;
} Heap_Information;



 
typedef struct {
  Heap_Information Free;
  Heap_Information Used;
  Heap_Statistics Stats;
} Heap_Information_block;






 
typedef struct {
  void *begin;
  uintptr_t size;
} Heap_Area;









 
typedef uintptr_t (*Heap_Initialization_or_extend_handler)(
  Heap_Control *heap,
  void *area_begin,
  uintptr_t area_size,
  uintptr_t page_size_or_unused
);

















 
uintptr_t _Heap_Extend(
  Heap_Control *heap,
  void *area_begin,
  uintptr_t area_size,
  uintptr_t unused
);









 
uintptr_t _Heap_No_extend(
  Heap_Control *unused_0,
  void *unused_1,
  uintptr_t unused_2,
  uintptr_t unused_3
);

static inline uintptr_t _Heap_Align_up(
  uintptr_t value,
  uintptr_t alignment
)
{
  uintptr_t remainder = value % alignment;

  if ( remainder != 0 ) {
    return value - remainder + alignment;
  } else {
    return value;
  }
}

static inline uintptr_t _Heap_Min_block_size( uintptr_t page_size )
{
  return _Heap_Align_up( sizeof( Heap_Block ), page_size );
}



 
static inline uintptr_t _Heap_Area_overhead(
  uintptr_t page_size
)
{
  if ( page_size != 0 ) {
    page_size = _Heap_Align_up( page_size, 8 );
  } else {
    page_size = 8;
  }

  return 2 * (page_size - 1) + (2 * sizeof(uintptr_t) + 0);
}




 
static inline uintptr_t _Heap_Size_with_overhead(
  uintptr_t page_size,
  uintptr_t size,
  uintptr_t alignment
)
{
  if ( page_size != 0 ) {
    page_size = _Heap_Align_up( page_size, 8 );
  } else {
    page_size = 8;
  }

  if ( page_size < alignment ) {
    page_size = alignment;
  }

  return (2 * sizeof(uintptr_t) + 0) + page_size - 1 + size;
}

 


 










 







 











 
 




 
typedef uint32_t   Modes_Control;




 



 




 

 

 

 

 

 
 

 
 

 
 










 





 
extern const uint32_t rtems_interrupt_mask;











 
Modes_Control rtems_interrupt_level_body(
  uint32_t   level
);

 


 






 








 














 
 



 



 
typedef void MPCI_Entry;




 
typedef MPCI_Entry ( *MPCI_initialization_entry )( void );





 
typedef MPCI_Entry ( *MPCI_get_packet_entry )(
                     MP_packet_Prefix **
                   );






 
typedef MPCI_Entry ( *MPCI_return_packet_entry )(
                     MP_packet_Prefix *
                   );






 
typedef MPCI_Entry ( *MPCI_send_entry )(
                     uint32_t,
                     MP_packet_Prefix *
                   );







 
typedef MPCI_Entry ( *MPCI_receive_entry )(
                     MP_packet_Prefix **
                   );








 
typedef struct {
   
  uint32_t                   default_timeout;
  


 
  size_t                     maximum_packet_size;
   
  MPCI_initialization_entry  initialization;
   
  MPCI_get_packet_entry      get_packet;
   
  MPCI_return_packet_entry   return_packet;
   
  MPCI_send_entry            send_packet;
   
  MPCI_receive_entry         receive_packet;
} MPCI_Control;

 


 




 
 








 
typedef uint32_t         rtems_name;




 
typedef Objects_Id       rtems_id;





 



 
typedef Context_Control            rtems_context;







 
typedef CPU_Interrupt_frame        rtems_interrupt_frame;




 
typedef Heap_Information_block region_information_block;




 
typedef Watchdog_Interval rtems_interval;






 
typedef struct timespec rtems_thread_cpu_usage_t;




 
typedef struct {
  

 
  uint32_t   year;
  

 
  uint32_t   month;
  

 
  uint32_t   day;
  

 
  uint32_t   hour;
  

 
  uint32_t   minute;
  

 
  uint32_t   second;
  

 
  uint32_t   ticks;
}   rtems_time_of_day;



 
typedef Modes_Control rtems_mode;



 



 
typedef MP_packet_Classes          rtems_mp_packet_classes;



 
typedef MP_packet_Prefix           rtems_packet_prefix;




 
typedef MPCI_initialization_entry  rtems_mpci_initialization_entry;



 
typedef MPCI_get_packet_entry      rtems_mpci_get_packet_entry;




 
typedef MPCI_return_packet_entry   rtems_mpci_return_packet_entry;



 
typedef MPCI_send_entry            rtems_mpci_send_packet_entry;



 
typedef MPCI_receive_entry         rtems_mpci_receive_packet_entry;



 
typedef MPCI_Entry                 rtems_mpci_entry;



 
typedef MPCI_Control               rtems_mpci_table;


 


 








 








 









 








 








 








 











 
 




 
typedef struct {
   
  void       *(*thread_entry)(void *);
   
  int       stack_size;
} posix_initialization_threads_table;









 
typedef struct {
  


 
  uint32_t                            maximum_threads;

  


 
  uint32_t                            maximum_mutexes;

  


 
  uint32_t                            maximum_condition_variables;

  


 
  uint32_t                            maximum_timers;

  


 
  uint32_t                            maximum_queued_signals;

  


 
  uint32_t                            maximum_message_queues;

  







 
  uint32_t                            maximum_message_queue_descriptors;

  


 
  uint32_t                            maximum_semaphores;

  


 
  uint32_t                            maximum_barriers;

  


 
  uint32_t                            maximum_rwlocks;

  


 
  uint32_t                            maximum_spinlocks;

  


 
  uint32_t                            number_of_initialization_threads;

  

 
  posix_initialization_threads_table *User_initialization_threads_table;
} posix_api_configuration_table;






 
extern posix_api_configuration_table Configuration_POSIX_API;

 

 











 







 





























 








 



















 







 












 







 












 
 




 
typedef uint32_t   rtems_option;




 



 




 




 




 

 


 








 




 
typedef uint32_t   rtems_event_set;



 

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 

 


















































 




 


































 
rtems_status_code rtems_event_send (
  rtems_id        id,
  rtems_event_set event_in
);





























































 
rtems_status_code rtems_event_receive (
  rtems_event_set  event_in,
  rtems_option     option_set,
  rtems_interval   ticks,
  rtems_event_set *event_out
);

 













 
 



 



 



 



 



 



 
rtems_status_code rtems_event_system_send(
  rtems_id id,
  rtems_event_set event_in
);



 
rtems_status_code rtems_event_system_receive(
  rtems_event_set event_in,
  rtems_option option_set,
  rtems_interval ticks,
  rtems_event_set *event_out
);

 










































































































 
 





 
static inline rtems_status_code rtems_event_transient_send(
  rtems_id id
)
{
  return rtems_event_system_send( id, 0x80000000 );
}





 
static inline rtems_status_code rtems_event_transient_receive(
  rtems_option option_set,
  rtems_interval ticks
)
{
  rtems_event_set event_out;

  return rtems_event_system_receive(
    0x80000000,
    0x00000000 | option_set,
    ticks,
    &event_out
  );
}





 
static inline void rtems_event_transient_clear( void )
{
  rtems_event_set event_out;

  rtems_event_system_receive(
    0x80000000,
    0x00000000 | 0x00000001,
    0,
    &event_out
  );
}

 

typedef struct {
  rtems_event_set pending_events;
} Event_Control;


 











 







 











 
 




 
typedef uint32_t   rtems_signal_set;



 
typedef void rtems_asr;




 
typedef rtems_asr ( *rtems_asr_entry )(
                 rtems_signal_set
             );




 
typedef struct {
   
  _Bool              is_enabled;
   
  rtems_asr_entry   handler;
   
  Modes_Control     mode_set;
   
  rtems_signal_set  signals_posted;
   
  rtems_signal_set  signals_pending;
   
  uint32_t          nest_level;
   
  ISR_lock_Control  Lock;
}   ASR_Information;




 

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 

 


 










 







 











 
 




 
typedef uint32_t   rtems_attribute;

 




 



 




 




 

 





 





 

 




 




 




 




 




 






 




 






 




 






 

 




 




 

 




 




 

 


 










 
 



 




 



 
typedef Priority_Control rtems_task_priority;






 




 










 




 

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 

 



 
typedef Thread_Control rtems_tcb;



 
typedef void rtems_task;



 
typedef Thread_Entry_numeric_type rtems_task_argument;



 
typedef rtems_task ( *rtems_task_entry )(
                      rtems_task_argument
                   );






 
typedef struct {
   
  rtems_name            name;
   
  size_t                stack_size;
   
  rtems_task_priority   initial_priority;
   
  rtems_attribute       attribute_set;
   
  rtems_task_entry      entry_point;
   
  rtems_mode            mode_set;
   
  rtems_task_argument	argument;
} rtems_initialization_tasks_table;




















 
rtems_status_code rtems_task_create(
  rtems_name           name,
  rtems_task_priority  initial_priority,
  size_t               stack_size,
  rtems_mode           initial_modes,
  rtems_attribute      attribute_set,
  rtems_id            *id
);




















 
rtems_status_code rtems_task_ident(
  rtems_name    name,
  uint32_t      node,
  rtems_id     *id
);














 
rtems_status_code rtems_task_delete(
  rtems_id   id
);















 
rtems_status_code rtems_task_get_note(
  rtems_id    id,
  uint32_t    notepad,
  uint32_t   *note
) ;

















 
rtems_status_code rtems_task_set_note(
  rtems_id   id,
  uint32_t   notepad,
  uint32_t   note
) ;















 
rtems_status_code rtems_task_mode(
  rtems_mode  mode_set,
  rtems_mode  mask,
  rtems_mode *previous_mode_set
);












 
rtems_status_code rtems_task_restart(
  rtems_id   id,
  uint32_t   arg
);













 
rtems_status_code rtems_task_suspend(
  rtems_id   id
);












 
rtems_status_code rtems_task_resume(
  rtems_id   id
);















 
rtems_status_code rtems_task_set_priority(
  rtems_id             id,
  rtems_task_priority  new_priority,
  rtems_task_priority *old_priority
);









 
rtems_status_code rtems_task_start(
  rtems_id             id,
  rtems_task_entry     entry_point,
  rtems_task_argument  argument
);











 
rtems_status_code rtems_task_wake_when(
  rtems_time_of_day *time_buffer
);










 
rtems_status_code rtems_task_wake_after(
  rtems_interval  ticks
);






 
rtems_status_code rtems_task_is_suspended(
  rtems_id   id
);









 
rtems_status_code rtems_task_variable_add(
  rtems_id  tid,
  void    **ptr,
  void    (*dtor)(void *)
) ;









 
rtems_status_code rtems_task_variable_get(
  rtems_id tid,
  void **ptr,
  void **result
) ;









 
rtems_status_code rtems_task_variable_delete(
  rtems_id  tid,
  void    **ptr
) ;












 
rtems_status_code rtems_task_get_scheduler(
  rtems_id  task_id,
  rtems_id *scheduler_id
);















 
rtems_status_code rtems_task_set_scheduler(
  rtems_id task_id,
  rtems_id scheduler_id
);





 
rtems_id rtems_task_self(void);














 
rtems_status_code rtems_scheduler_ident(
  rtems_name  name,
  rtems_id   *id
);


 








 
typedef struct {
   
  Event_Control            Event;
   
  Event_Control            System_event;
   
  ASR_Information          Signal;

  

 
  Thread_Action            Signal_action;

  





 
  uint32_t Notepads[ 0 ] ;
}  RTEMS_API_Control;







 
extern void (*_RTEMS_tasks_Initialize_user_tasks_p)(void);












 

extern void _RTEMS_tasks_Initialize_user_tasks_body( void );


 









 
 








 
typedef struct {
  


 
  uint32_t                    maximum_tasks;

  


 
  _Bool                        notepads_enabled;

  


 
  uint32_t                    maximum_timers;

  


 
  uint32_t                    maximum_semaphores;

  


 
  uint32_t                    maximum_message_queues;

  


 
  uint32_t                    maximum_partitions;

  


 
  uint32_t                    maximum_regions;

  



 
  uint32_t                    maximum_ports;

  



 
  uint32_t                    maximum_periods;

  


 
  uint32_t                    maximum_barriers;

  


 
  uint32_t                    number_of_initialization_tasks;

  


 
  rtems_initialization_tasks_table *User_initialization_tasks_table;
} rtems_api_configuration_table;






 
extern rtems_api_configuration_table Configuration_RTEMS_API;

 




 



 


 

 





 








 









 








 









 








 











 
 





 
typedef enum {
  



 
  INTERNAL_ERROR_CORE,

  

 
  INTERNAL_ERROR_RTEMS_API,

  

 
  INTERNAL_ERROR_POSIX_API,

  



 
  RTEMS_FATAL_SOURCE_BDBUF,

  



 
  RTEMS_FATAL_SOURCE_APPLICATION,

  



 
  RTEMS_FATAL_SOURCE_EXIT,

  






 
  RTEMS_FATAL_SOURCE_BSP,

  





 
  RTEMS_FATAL_SOURCE_ASSERT,

  



 
  RTEMS_FATAL_SOURCE_STACK_CHECKER,

  





 
  RTEMS_FATAL_SOURCE_EXCEPTION,

  



 
  RTEMS_FATAL_SOURCE_SMP,

  




 
  RTEMS_FATAL_SOURCE_LAST = 0xffffffff
} Internal_errors_Source;



 
typedef enum {
  INTERNAL_ERROR_NO_CONFIGURATION_TABLE,
  INTERNAL_ERROR_NO_CPU_TABLE,
  INTERNAL_ERROR_TOO_LITTLE_WORKSPACE,
  INTERNAL_ERROR_WORKSPACE_ALLOCATION,
  INTERNAL_ERROR_INTERRUPT_STACK_TOO_SMALL,
  INTERNAL_ERROR_THREAD_EXITTED,
  INTERNAL_ERROR_INCONSISTENT_MP_INFORMATION,
  INTERNAL_ERROR_INVALID_NODE,
  INTERNAL_ERROR_NO_MPCI,
  INTERNAL_ERROR_BAD_PACKET,
  INTERNAL_ERROR_OUT_OF_PACKETS,
  INTERNAL_ERROR_OUT_OF_GLOBAL_OBJECTS,
  INTERNAL_ERROR_OUT_OF_PROXIES,
  INTERNAL_ERROR_INVALID_GLOBAL_ID,
  INTERNAL_ERROR_BAD_STACK_HOOK,
  INTERNAL_ERROR_BAD_ATTRIBUTES,
  INTERNAL_ERROR_IMPLEMENTATION_KEY_CREATE_INCONSISTENCY,
  OBSOLETE_INTERNAL_ERROR_IMPLEMENTATION_BLOCKING_OPERATION_CANCEL,
  INTERNAL_ERROR_MUTEX_OBTAIN_FROM_BAD_STATE,
  INTERNAL_ERROR_UNLIMITED_AND_MAXIMUM_IS_0,
  OBSOLETE_INTERNAL_ERROR_SHUTDOWN_WHEN_NOT_UP,
  INTERNAL_ERROR_GXX_KEY_ADD_FAILED,
  INTERNAL_ERROR_GXX_MUTEX_INIT_FAILED,
  INTERNAL_ERROR_NO_MEMORY_FOR_HEAP,
  INTERNAL_ERROR_CPU_ISR_INSTALL_VECTOR,
  INTERNAL_ERROR_RESOURCE_IN_USE
} Internal_errors_Core_list;

typedef CPU_Uint32ptr Internal_errors_t;



 
typedef struct {
   
  Internal_errors_Source  the_source;
   
  _Bool                    is_internal;
   
  Internal_errors_t       the_error;
} Internal_errors_Information;



 
extern Internal_errors_Information _Internal_errors_What_happened;









































 
void _Terminate(
  Internal_errors_Source  the_source,
  _Bool                    is_internal,
  Internal_errors_t       the_error
) ;


 

 


typedef void User_extensions_routine ;









 
 





























 
typedef _Bool ( *User_extensions_thread_create_extension )(
  Thread_Control *executing,
  Thread_Control *created
);















 
typedef void( *User_extensions_thread_delete_extension )(
  Thread_Control *executing,
  Thread_Control *deleted
);















 
typedef void( *User_extensions_thread_start_extension )(
  Thread_Control *executing,
  Thread_Control *started
);

















 
typedef void( *User_extensions_thread_restart_extension )(
  Thread_Control *executing,
  Thread_Control *restarted
);


















 
typedef void( *User_extensions_thread_switch_extension )(
  Thread_Control *executing,
  Thread_Control *heir
);










 
typedef void( *User_extensions_thread_begin_extension )(
  Thread_Control *executing
);










 
typedef void( *User_extensions_thread_exitted_extension )(
  Thread_Control *executing
);














 
typedef void( *User_extensions_fatal_extension )(
  Internal_errors_Source source,
  _Bool                   is_internal,
  Internal_errors_t      code
);
















 
typedef void( *User_extensions_thread_terminate_extension )(
  Thread_Control *terminated
);



 
typedef struct {
  User_extensions_thread_create_extension  thread_create;
  User_extensions_thread_start_extension   thread_start;
  User_extensions_thread_restart_extension thread_restart;
  User_extensions_thread_delete_extension  thread_delete;
  User_extensions_thread_switch_extension  thread_switch;
  User_extensions_thread_begin_extension   thread_begin;
  User_extensions_thread_exitted_extension thread_exitted;
  User_extensions_fatal_extension          fatal;
  User_extensions_thread_terminate_extension thread_terminate;
}   User_extensions_Table;





 
typedef struct {
  Chain_Node                              Node;
  User_extensions_thread_switch_extension thread_switch;
}   User_extensions_Switch_control;






 
typedef struct {
  Chain_Node                     Node;
  User_extensions_Switch_control Switch;
  User_extensions_Table          Callouts;
}   User_extensions_Control;

 


 

typedef struct {
  Objects_Control          Object;
  User_extensions_Control  Extension;
}   Extension_Control;

typedef User_extensions_routine
  rtems_extension ;





























































































































 
 

typedef User_extensions_thread_create_extension   rtems_task_create_extension;
typedef User_extensions_thread_delete_extension   rtems_task_delete_extension;
typedef User_extensions_thread_start_extension    rtems_task_start_extension;
typedef User_extensions_thread_restart_extension  rtems_task_restart_extension;
typedef User_extensions_thread_switch_extension   rtems_task_switch_extension;
typedef User_extensions_thread_begin_extension    rtems_task_begin_extension;
typedef User_extensions_thread_exitted_extension  rtems_task_exitted_extension;
typedef User_extensions_fatal_extension           rtems_fatal_extension;

typedef User_extensions_Table                     rtems_extensions_table;

typedef Internal_errors_Source rtems_fatal_source;

typedef Internal_errors_t rtems_fatal_code;



















 
rtems_status_code rtems_extension_create(
  rtems_name                    name,
  const rtems_extensions_table *extension_table,
  rtems_id                     *id
);
















 
rtems_status_code rtems_extension_ident(
  rtems_name  name,
  rtems_id   *id
);











 
rtems_status_code rtems_extension_delete(
  rtems_id id
);

 


 






 
typedef struct {
   
  uint32_t            node;
   
  uint32_t            maximum_nodes;
   
  uint32_t            maximum_global_objects;
   
  uint32_t            maximum_proxies;

  



 
  uint32_t            extra_mpci_receive_server_stack;

   
  rtems_mpci_table   *User_mpci_table;
} rtems_multiprocessing_table;





 
typedef void (*rtems_stack_allocate_init_hook)( size_t stack_space_size );








 
typedef void *(*rtems_stack_allocate_hook)( size_t stack_size );





 
typedef void (*rtems_stack_free_hook)( void *addr );











 
typedef struct {
  

 
  uintptr_t                      work_space_size;

  

 
  uintptr_t                      stack_space_size;

  


 
  uint32_t                       maximum_extensions;

  


 
  uint32_t                       maximum_keys;

  






 
  uint32_t                            maximum_key_value_pairs;

  


 
  uint32_t                       microseconds_per_tick;

  




 
  uint32_t                       nanoseconds_per_tick;

  

 
  uint32_t                       ticks_per_timeslice;

  


 
  Thread                       (*idle_task)( uintptr_t );

  



 
  uint32_t                       idle_task_stack_size;

  



 
  uint32_t                       interrupt_stack_size;

  

 
  rtems_stack_allocate_init_hook stack_allocate_init_hook;

  

 
  rtems_stack_allocate_hook      stack_allocate_hook;

  

 
  rtems_stack_free_hook          stack_free_hook;

  




 
  _Bool                           do_zero_of_workspace;

  




 
  _Bool                           unified_work_area;

  





 
  _Bool                           stack_allocator_avoids_work_space;


  uint32_t                       number_of_initial_extensions;
  const rtems_extensions_table  *User_extension_table;
    rtems_multiprocessing_table   *User_multiprocessing_table;
} rtems_configuration_table;



 
extern const rtems_configuration_table Configuration;

  

 
  extern rtems_multiprocessing_table  *_Configuration_MP_table;

  




 
  extern rtems_multiprocessing_table  Multiprocessing_configuration;





 














 


 













 











 




 








 








 







 
 




 
typedef uint32_t   rtems_debug_control;



 

 

 



 
extern rtems_debug_control _Debug_Level;



 
void _Debug_Manager_initialization( void );



 
void rtems_debug_enable(
  rtems_debug_control  to_be_enabled
);



 
void rtems_debug_disable(
  rtems_debug_control  to_be_disabled
);






 
_Bool rtems_debug_is_enabled(
  rtems_debug_control  level
);


 
 
















 








 













 







 












 








 













 
 




 
typedef uint32_t   ISR_Vector_number;



 
typedef void ISR_Handler;



 
typedef ISR_Handler ( *ISR_Handler_entry )(
                 ISR_Vector_number,
                 CPU_Interrupt_frame *
             );




 
extern ISR_Handler_entry _ISR_Vector_table[ 16 ];





 
void _ISR_Handler_initialization ( void );

















 














 
void _ISR_Handler( void );













 
void _ISR_Dispatch( void );









 
_Bool _ISR_Is_in_progress( void );


 

 








 
 



 
typedef ISR_Level rtems_interrupt_level;



 
typedef ISR_Vector_number rtems_vector_number;



 
typedef ISR_Handler rtems_isr;





 
typedef rtems_isr ( *rtems_isr_entry )(
                 rtems_vector_number
             );














 
rtems_status_code rtems_interrupt_catch(
  rtems_isr_entry      new_isr_handler,
  rtems_vector_number  vector,
  rtems_isr_entry     *old_isr_handler
);









 








 









 












 







 







 





 





 
















 



 
typedef ISR_lock_Control rtems_interrupt_lock;



 
typedef ISR_lock_Context rtems_interrupt_lock_context;







 








 










 








 






 










 







 














 














 

















 













 

 


 

 




 
extern const rtems_multiprocessing_table
  _Initialization_Default_multiprocessing_table;







 
void rtems_initialize_data_structures(void);






 
void rtems_initialize_before_drivers(void);






 
void rtems_initialize_device_drivers(void);















 
void rtems_initialize_start_multitasking(void)
  ;









 
void rtems_shutdown_executive(
  uint32_t   result
) ;


 


















 







 









 








 











 
 



 
typedef enum {
  

 
  CORE_BARRIER_AUTOMATIC_RELEASE,
  

 
  CORE_BARRIER_MANUAL_RELEASE
}   CORE_barrier_Disciplines;




 
typedef struct {
  
 
  CORE_barrier_Disciplines  discipline;
  

 
  uint32_t                  maximum_count;
}   CORE_barrier_Attributes;




 
typedef struct {
  

 
  Thread_queue_Control     Wait_queue;
  

 
  CORE_barrier_Attributes  Attributes;
  
 
  uint32_t                 number_of_waiting_threads;
}   CORE_barrier_Control;

 


 









 
 



 
typedef struct {
   
  Objects_Control          Object;
   
  rtems_attribute          attribute_set;
   
  CORE_barrier_Control     Barrier;
}   Barrier_Control;



















 
rtems_status_code rtems_barrier_create(
  rtems_name           name,
  rtems_attribute      attribute_set,
  uint32_t             maximum_waiters,
  rtems_id            *id
);
















 
rtems_status_code rtems_barrier_ident(
  rtems_name    name,
  rtems_id     *id
);












 
rtems_status_code rtems_barrier_delete(
  rtems_id   id
);














 
rtems_status_code rtems_barrier_wait(
  rtems_id       id,
  rtems_interval timeout
);















 
rtems_status_code rtems_barrier_release(
  rtems_id  id,
  uint32_t *released
);

 


 




 







 


















 









 
size_t rtems_cache_get_data_line_size( void );









 
size_t rtems_cache_get_instruction_line_size( void );









 
size_t rtems_cache_get_maximal_line_size( void );








 
size_t rtems_cache_get_data_cache_size( uint32_t level );








 
size_t rtems_cache_get_instruction_cache_size( uint32_t level );









 
void rtems_cache_flush_multiple_data_lines( const void *addr, size_t size );












 
void rtems_cache_invalidate_multiple_data_lines(
  const void *addr,
  size_t size
);












 
void rtems_cache_invalidate_multiple_instruction_lines(
  const void *addr,
  size_t size
);













 
void rtems_cache_instruction_sync_after_code_change(
  const void * code_addr,
  size_t n_bytes
);





 
void rtems_cache_flush_entire_data( void );





 
void rtems_cache_invalidate_entire_instruction( void );




 
void rtems_cache_invalidate_entire_data( void );



 
void rtems_cache_freeze_data( void );



 
void rtems_cache_unfreeze_data( void );



 
void rtems_cache_enable_data( void );



 
void rtems_cache_disable_data( void );



 
void rtems_cache_freeze_instruction( void );



 
void rtems_cache_unfreeze_instruction( void );



 
void rtems_cache_enable_instruction( void );



 
void rtems_cache_disable_instruction( void );




 
void *rtems_cache_aligned_malloc ( size_t nbytes );





























 
void *rtems_cache_coherent_allocate(
  size_t size,
  uintptr_t alignment,
  uintptr_t boundary
);








 
void rtems_cache_coherent_free( void *ptr );











 
void rtems_cache_coherent_add_area(
  void *area_begin,
  uintptr_t area_size
);


 


 


















 







 








 








 





 






 













 








 













 











 












 

struct timecounter;
typedef uint32_t timecounter_get_t(struct timecounter *);
typedef void timecounter_pps_t(struct timecounter *);

struct timecounter {
	timecounter_get_t	*tc_get_timecount;
		



 
	timecounter_pps_t	*tc_poll_pps;
		




 
	uint32_t 		tc_counter_mask;
		 
	uint64_t		tc_frequency;
		 
	char			*tc_name;
		 
	int			tc_quality;
		



 
	u_int			tc_flags;

	void			*tc_priv;
		 
	struct timecounter	*tc_next;
		 
};

extern struct timecounter *timecounter;
extern int tc_min_ticktock_freq; 


 

u_int64_t tc_getfrequency(void);
void	tc_init(struct timecounter *tc);
void	tc_setclock(struct timespec *ts);
void	tc_ticktock(int cnt);
void	cpu_tick_calibration(void);











 





 
void _Timecounter_Bintime( struct bintime *bt );





 
void _Timecounter_Nanotime( struct timespec *ts );





 
void _Timecounter_Microtime( struct timeval *tv );





 
void _Timecounter_Binuptime( struct bintime *bt );





 
void _Timecounter_Nanouptime( struct timespec *ts );





 
void _Timecounter_Microuptime( struct timeval *tv );








 
void _Timecounter_Getbintime( struct bintime *bt );










 
void _Timecounter_Getnanotime( struct timespec *ts );










 
void _Timecounter_Getmicrotime( struct timeval *tv );








 
void _Timecounter_Getbinuptime( struct bintime *bt );








 
void _Timecounter_Getnanouptime( struct timespec *ts );








 
void _Timecounter_Getmicrouptime( struct timeval *tv );









 
void _Timecounter_Install( struct timecounter *tc );



 
void _Timecounter_Tick( void );



 








 












 
void _Timecounter_Tick_simple(
  uint32_t delta,
  uint32_t offset,
  ISR_lock_Context *lock_context
);



 
extern volatile time_t _Timecounter_Time_second;






 
extern volatile time_t _Timecounter_Time_uptime;



 
extern struct timecounter *_Timecounter;

 








 

void _Timecounter_Initialize( void );

void _Timecounter_Set_clock( const struct timespec *ts );

 











 
 




 
typedef enum {
   
  RTEMS_CLOCK_GET_TOD,
   
  RTEMS_CLOCK_GET_SECONDS_SINCE_EPOCH,
   
  RTEMS_CLOCK_GET_TICKS_SINCE_BOOT,
   
  RTEMS_CLOCK_GET_TICKS_PER_SECOND,
   
  RTEMS_CLOCK_GET_TIME_VALUE
} rtems_clock_get_options;




















 
rtems_status_code rtems_clock_get(
  rtems_clock_get_options  option,
  void                    *time_buffer
) ;















 
rtems_status_code rtems_clock_get_tod(
  rtems_time_of_day *time_buffer
);













 
rtems_status_code rtems_clock_get_tod_timeval(
  struct timeval *time
);













 
rtems_status_code rtems_clock_get_seconds_since_epoch(
  rtems_interval *the_interval
);






 
static inline rtems_interval rtems_clock_get_ticks_since_boot(void)
{
  return _Watchdog_Ticks_since_boot;
}







 
static inline rtems_interval rtems_clock_tick_later(
  rtems_interval delta
)
{
  return _Watchdog_Ticks_since_boot + delta;
}








 
static inline rtems_interval rtems_clock_tick_later_usec(
  rtems_interval delta_in_usec
)
{
  rtems_interval us_per_tick = (Configuration . microseconds_per_tick);

  


 
  return _Watchdog_Ticks_since_boot
    + (delta_in_usec + us_per_tick - 1) / us_per_tick + 1;
}



























 
static inline _Bool rtems_clock_tick_before(
  rtems_interval tick
)
{
  return (int32_t) ( tick - _Watchdog_Ticks_since_boot ) > 0;
}










 
rtems_interval rtems_clock_get_ticks_per_second(void);















 
rtems_status_code rtems_clock_set(
  const rtems_time_of_day *time_buffer
);












 
rtems_status_code rtems_clock_tick( void );












 
rtems_status_code rtems_clock_get_uptime(
  struct timespec *uptime
);









 
void rtems_clock_get_uptime_timeval( struct timeval *uptime );





 
static inline time_t rtems_clock_get_uptime_seconds( void )
{
  return _Timecounter_Time_uptime - 1;
}





 
uint64_t rtems_clock_get_uptime_nanoseconds( void );












 
_Bool _TOD_Validate(
  const rtems_time_of_day *the_tod
);










 
Watchdog_Interval _TOD_To_seconds(
  const rtems_time_of_day *the_tod
);


 

 



























 










 















 
 




 
typedef enum {
  


 
  TIMER_INTERVAL,

  


 
  TIMER_INTERVAL_ON_TASK,

  


 
  TIMER_TIME_OF_DAY,

  


 
  TIMER_TIME_OF_DAY_ON_TASK,

  

 
  TIMER_DORMANT
} Timer_Classes;



 
typedef void rtems_timer_service_routine;





 
typedef rtems_timer_service_routine ( *rtems_timer_service_routine_entry )(
                 rtems_id,
                 void *
             );




 
typedef struct {
   
  Objects_Control  Object;
   
  Watchdog_Control Ticker;
   
  Timer_Classes    the_class;
}   Timer_Control;












 
rtems_status_code rtems_timer_create(
  rtems_name    name,
  rtems_id     *id
);














 
rtems_status_code rtems_timer_ident(
  rtems_name    name,
  rtems_id     *id
);






 
rtems_status_code rtems_timer_cancel(
  rtems_id   id
);












 
rtems_status_code rtems_timer_delete(
  rtems_id   id
);


















 
rtems_status_code rtems_timer_fire_after(
  rtems_id                           id,
  rtems_interval                     ticks,
  rtems_timer_service_routine_entry  routine,
  void                              *user_data
);


















 
rtems_status_code rtems_timer_server_fire_after(
  rtems_id                           id,
  rtems_interval                     ticks,
  rtems_timer_service_routine_entry  routine,
  void                              *user_data
);


















 
rtems_status_code rtems_timer_fire_when(
  rtems_id                            id,
  rtems_time_of_day                  *wall_time,
  rtems_timer_service_routine_entry   routine,
  void                               *user_data
);











 
rtems_status_code rtems_timer_server_fire_when(
  rtems_id                            id,
  rtems_time_of_day                  *wall_time,
  rtems_timer_service_routine_entry   routine,
  void                               *user_data
);










 
rtems_status_code rtems_timer_reset(
  rtems_id   id
);







 
rtems_status_code rtems_timer_initiate_server(
  uint32_t             priority,
  uint32_t             stack_size,
  rtems_attribute      attribute_set
);





 




 
typedef struct {
   
  Timer_Classes      the_class;
   
  Watchdog_Interval  initial;
   
  Watchdog_Interval  start_time;
   
  Watchdog_Interval  stop_time;
} rtems_timer_information;












 
rtems_status_code rtems_timer_get_information(
  rtems_id                 id,
  rtems_timer_information *the_info
);

 


 





















 








 









 







 






 
 



 



 



 



 
static inline _Bool rtems_is_name_valid (
  rtems_name name
)
{
  return ( name != 0 );
}




 
static inline void rtems_name_to_characters(
  rtems_name    name,
  char         *c1,
  char         *c2,
  char         *c3,
  char         *c4
)
{
  *c1 = (char) ((name >> 24) & 0xff);
  *c2 = (char) ((name >> 16) & 0xff);
  *c3 = (char) ((name >> 8) & 0xff);
  *c4 = (char) ( name & 0xff);
}

 







 
 








 
_Bool rtems_workspace_get_information(
  Heap_Information_block  *the_info
);








 
_Bool rtems_workspace_allocate(
  size_t   bytes,
  void   **pointer
);








 
_Bool rtems_workspace_free(
  void *pointer
);









 
void *rtems_workspace_greedy_allocate(
  const uintptr_t *block_sizes,
  size_t block_count
);









 
void *rtems_workspace_greedy_allocate_all_except_largest(
  uintptr_t *allocatable_size
);







 
void rtems_workspace_greedy_free( void *opaque );

 


 









 








 











 
 





 
typedef enum {
   
  CORE_MUTEX_DISCIPLINES_FIFO,
   
  CORE_MUTEX_DISCIPLINES_PRIORITY,
  

 
  CORE_MUTEX_DISCIPLINES_PRIORITY_INHERIT,
  

 
  CORE_MUTEX_DISCIPLINES_PRIORITY_CEILING
}   CORE_mutex_Disciplines;






 
typedef enum {
  






 
  CORE_MUTEX_NESTING_ACQUIRES,
  





 
  CORE_MUTEX_NESTING_IS_ERROR,
  




 
  CORE_MUTEX_NESTING_BLOCKS
}  CORE_mutex_Nesting_behaviors;






 
typedef struct {
  

 
  CORE_mutex_Nesting_behaviors lock_nesting_behavior;
  

 
  _Bool                         only_owner_release;
  

 
  CORE_mutex_Disciplines       discipline;
  

 
  Priority_Control             priority_ceiling;
}   CORE_mutex_Attributes;






 
typedef struct {
  

 
  Thread_queue_Control    Wait_queue;
  

 
  CORE_mutex_Attributes   Attributes;
  

 
  uint32_t                nest_count;
  


 
  Thread_Control         *holder;

}   CORE_mutex_Control;

 


 









 








 











 
 



 
typedef enum {
   
  CORE_SEMAPHORE_DISCIPLINES_FIFO,
  

 
  CORE_SEMAPHORE_DISCIPLINES_PRIORITY
}   CORE_semaphore_Disciplines;




 
typedef struct {
   
  uint32_t                    maximum_count;
  

 
  CORE_semaphore_Disciplines  discipline;
}   CORE_semaphore_Attributes;




 
typedef struct {
  

 
  Thread_queue_Control        Wait_queue;
  

 
  CORE_semaphore_Attributes   Attributes;
   
  uint32_t                    count;
}   CORE_semaphore_Control;

 


 












 













 
 



 
typedef struct {
   
  Objects_Control          Object;

  



 
  rtems_attribute          attribute_set;

  







 
  union {
    


 
    CORE_mutex_Control     mutex;

    


 
    CORE_semaphore_Control semaphore;

  } Core_control;
}   Semaphore_Control;











 
rtems_status_code rtems_semaphore_create(
  rtems_name           name,
  uint32_t             count,
  rtems_attribute      attribute_set,
  rtems_task_priority  priority_ceiling,
  rtems_id            *id
);


















 
rtems_status_code rtems_semaphore_ident(
  rtems_name    name,
  uint32_t      node,
  rtems_id     *id
);












 
rtems_status_code rtems_semaphore_delete(
  rtems_id   id
);



















 
rtems_status_code rtems_semaphore_obtain(
  rtems_id       id,
  rtems_option   option_set,
  rtems_interval timeout
);











 
rtems_status_code rtems_semaphore_release(
  rtems_id   id
);













 
rtems_status_code rtems_semaphore_flush(
  rtems_id	   id
);


































 
rtems_status_code rtems_semaphore_set_priority(
  rtems_id             semaphore_id,
  rtems_id             scheduler_id,
  rtems_task_priority  new_priority,
  rtems_task_priority *old_priority
);

 


 






















 







 









 








 











 
 

  



 

  



 

  


 









 
typedef struct {
   
  size_t      size;
   
  uint32_t    buffer[1];
} CORE_message_queue_Buffer;






 
typedef struct {
   
  Chain_Node                 Node;
     
    int                        priority;
   
  CORE_message_queue_Buffer  Contents;
}   CORE_message_queue_Buffer_control;






 
typedef enum {
   
  CORE_MESSAGE_QUEUE_DISCIPLINES_FIFO,
   
  CORE_MESSAGE_QUEUE_DISCIPLINES_PRIORITY
}   CORE_message_queue_Disciplines;






 
typedef struct {
   
  CORE_message_queue_Disciplines  discipline;
}   CORE_message_queue_Attributes;

  





 
  typedef void (*CORE_message_queue_Notify_Handler)( void * );






 
typedef struct {
  

 
  Thread_queue_Control               Wait_queue;
  

 
  CORE_message_queue_Attributes      Attributes;
  

 
  uint32_t                           maximum_pending_messages;
  
 
  uint32_t                           number_of_pending_messages;
  

 
  size_t                             maximum_message_size;
  

 
  Chain_Control                      Pending_messages;
  


 
  CORE_message_queue_Buffer         *message_buffers;
    

 
    CORE_message_queue_Notify_Handler  notify_handler;
     
    void                              *notify_argument;
  

 
  Chain_Control                      Inactive_messages;
}   CORE_message_queue_Control;

 


 







 
typedef struct {
   
  Objects_Control             Object;
   
  rtems_attribute             attribute_set;
   
  CORE_message_queue_Control  message_queue;
}   Message_queue_Control;








 
 





















 
rtems_status_code rtems_message_queue_create(
  rtems_name       name,
  uint32_t         count,
  size_t           max_message_size,
  rtems_attribute  attribute_set,
  rtems_id        *id
);


















 
rtems_status_code rtems_message_queue_ident(
  rtems_name  name,
  uint32_t    node,
  rtems_id   *id
);










 
rtems_status_code rtems_message_queue_delete(
  rtems_id id
);















 
rtems_status_code rtems_message_queue_send(
  rtems_id    id,
  const void *buffer,
  size_t      size
);















 
rtems_status_code rtems_message_queue_urgent(
  rtems_id    id,
  const void *buffer,
  size_t      size
);
















 
rtems_status_code rtems_message_queue_broadcast(
  rtems_id    id,
  const void *buffer,
  size_t      size,
  uint32_t   *count
);





















 
rtems_status_code rtems_message_queue_receive(
  rtems_id        id,
  void           *buffer,
  size_t         *size,
  rtems_option    option_set,
  rtems_interval  timeout
);










 
rtems_status_code rtems_message_queue_flush(
  rtems_id  id,
  uint32_t *count
);










 
rtems_status_code rtems_message_queue_get_number_pending(
  rtems_id  id,
  uint32_t *count
);

 


 






 







 













 
 














 
rtems_status_code rtems_signal_catch(
  rtems_asr_entry   asr_handler,
  rtems_mode        mode_set
);











 
rtems_status_code rtems_signal_send(
  rtems_id         id,
  rtems_signal_set signal_set
);

 


 









 







 











 
 




 
typedef struct {
   
  rtems_id  minimum_id;
   
  rtems_id  maximum_id;
   
  uint32_t  maximum;
   
  _Bool      auto_extend;
   
  uint32_t  unallocated;
} rtems_object_api_class_information;

















 

















 















 
rtems_status_code rtems_object_get_classic_name(
  rtems_id      id,
  rtems_name   *name
);














 
char *rtems_object_get_name(
  rtems_id       id,
  size_t         length,
  char          *name
);
















 
rtems_status_code rtems_object_set_name(
  rtems_id       id,
  const char    *name
);
















 














 














 














 











 











 













 
int rtems_object_api_minimum_class(
  int api
);












 
int rtems_object_api_maximum_class(
  int api
);













 
int rtems_object_id_api_maximum_class(
  int api
);












 
const char *rtems_object_get_api_name(
  int api
);













 
const char *rtems_object_get_api_class_name(
  int the_api,
  int the_class
);















 
rtems_status_code rtems_object_get_class_information(
  int                                 the_api,
  int                                 the_class,
  rtems_object_api_class_information *info
);


 

 




















 







 











 
 



 
typedef struct {
   
  Objects_Control     Object;
   
  void               *starting_address;
   
  intptr_t            length;
   
  uint32_t            buffer_size;
   
  rtems_attribute     attribute_set;
   
  uint32_t            number_of_used_blocks;
   
  Chain_Control       Memory;
}   Partition_Control;













 
rtems_status_code rtems_partition_create(
  rtems_name       name,
  void            *starting_address,
  uint32_t         length,
  uint32_t         buffer_size,
  rtems_attribute  attribute_set,
  rtems_id        *id
);


















 
rtems_status_code rtems_partition_ident(
  rtems_name  name,
  uint32_t    node,
  rtems_id   *id
);













 
rtems_status_code rtems_partition_delete(
  rtems_id id
);












 
rtems_status_code rtems_partition_get_buffer(
  rtems_id   id,
  void     **buffer
);







 
rtems_status_code rtems_partition_return_buffer(
  rtems_id  id,
  void     *buffer
);

 


 



















 







 











 
 




 

typedef struct {
  Objects_Control       Object;
  Thread_queue_Control  Wait_queue;             
  void                 *starting_address;       
  uintptr_t             length;                 
  uintptr_t             page_size;              
  uintptr_t             maximum_segment_size;   
  rtems_attribute       attribute_set;
  uint32_t              number_of_used_blocks;  
  Heap_Control          Memory;
}  Region_Control;













 
rtems_status_code rtems_region_create(
  rtems_name          name,
  void               *starting_address,
  uintptr_t           length,
  uintptr_t           page_size,
  rtems_attribute     attribute_set,
  rtems_id           *id
);
















 
rtems_status_code rtems_region_extend(
  rtems_id            id,
  void               *starting_address,
  uintptr_t           length
);
















 
rtems_status_code rtems_region_ident(
  rtems_name    name,
  rtems_id     *id
);













 
rtems_status_code rtems_region_get_information(
  rtems_id                id,
  Heap_Information_block *the_info
);
















 
rtems_status_code rtems_region_get_free_information(
  rtems_id                id,
  Heap_Information_block *the_info
);













 
rtems_status_code rtems_region_delete(
  rtems_id   id
);






















 
rtems_status_code rtems_region_get_segment(
  rtems_id           id,
  uintptr_t          size,
  rtems_option       option_set,
  rtems_interval     timeout,
  void             **segment
);















 
rtems_status_code rtems_region_get_segment_size(
  rtems_id           id,
  void              *segment,
  uintptr_t         *size
);
















 
rtems_status_code rtems_region_return_segment(
  rtems_id    id,
  void       *segment
);























 
rtems_status_code rtems_region_resize_segment(
  rtems_id    id,
  void       *segment,
  uintptr_t   size,
  uintptr_t  *old_size
);

 


 





















 







 











 
 





 
typedef struct {
   
  Objects_Control  Object;
   
  void            *internal_base;
   
  void            *external_base;
   
  uint32_t         length;
}   Dual_ported_memory_Control;




















 
rtems_status_code rtems_port_create(
  rtems_name    name,
  void         *internal_start,
  void         *external_start,
  uint32_t      length,
  rtems_id     *id
);












 
rtems_status_code rtems_port_ident(
  rtems_name    name,
  rtems_id     *id
);












 
rtems_status_code rtems_port_delete(
  rtems_id   id
);















 
rtems_status_code rtems_port_external_to_internal(
  rtems_id     id,
  void        *external,
  void       **internal
);
















 
rtems_status_code rtems_port_internal_to_external(
  rtems_id     id,
  void        *internal,
  void       **external
);

 


 







 








 










 
 

typedef uint32_t rtems_device_major_number;

typedef uint32_t rtems_device_minor_number;

typedef rtems_status_code rtems_device_driver;

typedef rtems_device_driver (*rtems_device_driver_entry)(
  rtems_device_major_number,
  rtems_device_minor_number,
  void *
);

typedef struct {
  rtems_device_driver_entry initialization_entry;
  rtems_device_driver_entry open_entry;
  rtems_device_driver_entry close_entry;
  rtems_device_driver_entry read_entry;
  rtems_device_driver_entry write_entry;
  rtems_device_driver_entry control_entry;
} rtems_driver_address_table;



 
 





 
rtems_status_code rtems_io_driver_io_error(
  rtems_device_major_number major,
  rtems_device_minor_number minor,
  void *arg
);



















 
rtems_status_code rtems_io_register_driver(
  rtems_device_major_number major,
  const rtems_driver_address_table *driver_table,
  rtems_device_major_number *registered_major
);









 
rtems_status_code rtems_io_unregister_driver(
  rtems_device_major_number major
);









 
rtems_status_code rtems_io_register_name(
  const char *device_name,
  rtems_device_major_number major,
  rtems_device_minor_number minor
);

 











 
rtems_status_code rtems_io_initialize(
  rtems_device_major_number  major,
  rtems_device_minor_number  minor,
  void                      *argument
);











 
rtems_status_code rtems_io_open(
  rtems_device_major_number  major,
  rtems_device_minor_number  minor,
  void                      *argument
);











 
rtems_status_code rtems_io_close(
  rtems_device_major_number  major,
  rtems_device_minor_number  minor,
  void                      *argument
);











 
rtems_status_code rtems_io_read(
  rtems_device_major_number  major,
  rtems_device_minor_number  minor,
  void                      *argument
);











 
rtems_status_code rtems_io_write(
  rtems_device_major_number  major,
  rtems_device_minor_number  minor,
  void                      *argument
);











 
rtems_status_code rtems_io_control(
  rtems_device_major_number  major,
  rtems_device_minor_number  minor,
  void                      *argument
);

 

 

typedef struct {
    const char               *device_name;
    size_t                    device_name_length;
    rtems_device_major_number major;
    rtems_device_minor_number minor;
} rtems_driver_name_t;



 
rtems_status_code rtems_io_lookup_name(
    const char           *name,
    rtems_driver_name_t  *device_info
) ;

extern const size_t _IO_Number_of_drivers;

extern rtems_driver_address_table _IO_Driver_address_table[];

extern _Bool _IO_All_drivers_initialized;





 
void _IO_Initialize_all_drivers( void );


 




 








 











 
 



 
typedef struct {
  const char *file;
  int         line;
  const char *function;
  const char *failed_expression;
} rtems_assert_context;



 
typedef CPU_Exception_frame rtems_exception_frame;





 
static inline void rtems_exception_frame_print(
  const rtems_exception_frame *frame
)
{
  _CPU_Exception_frame_print( frame );
}








 
void rtems_fatal_error_occurred(
  uint32_t   the_error
) ;








 
void rtems_fatal(
  rtems_fatal_source source,
  rtems_fatal_code error
) ;










 
const char *rtems_fatal_source_text( rtems_fatal_source source );










 
const char *rtems_internal_error_text( rtems_fatal_code error );

 


 




















 







 








 








 















 




 
typedef void 	(*BSP_output_char_function_type) 	(char c);




 
typedef int 	(*BSP_polling_getchar_function_type) 	(void);




 
extern 	BSP_output_char_function_type 		BSP_output_char;




 
extern 	BSP_polling_getchar_function_type 	BSP_poll_char;

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 


#pragma diag_push


 
 
#pragma CHECK_MISRA("-19.15")


#pragma diag_pop












 
extern int getchark(void);









 
extern void vprintk(const char *fmt, va_list ap);







 
extern void printk(const char *fmt, ...);







 
extern void putk(const char *s);







 
extern void rtems_putc(char c);












 
typedef int (*rtems_printk_plugin_t)(void *, const char *format, ...);








 
extern int printk_plugin(void *context, const char *fmt, ...);











 
extern int rtems_printf_plugin(void *context, const char *fmt, ...);

 


















 
 




 

typedef struct timespec rtems_rate_monotonic_period_time_t;




 

typedef Timestamp_Control Rate_monotonic_Period_time_t;




 
typedef enum {
  


 
  RATE_MONOTONIC_INACTIVE,

  


 
  RATE_MONOTONIC_OWNER_IS_BLOCKING,

  



 
  RATE_MONOTONIC_ACTIVE,

  


 
  RATE_MONOTONIC_EXPIRED_WHILE_BLOCKING,

  



 
  RATE_MONOTONIC_EXPIRED
}   rtems_rate_monotonic_period_states;




 







 
typedef struct {
   
  uint32_t     count;
   
  uint32_t     missed_count;

   
  rtems_thread_cpu_usage_t             min_cpu_time;
   
  rtems_thread_cpu_usage_t             max_cpu_time;
   
  rtems_thread_cpu_usage_t             total_cpu_time;

   
  rtems_rate_monotonic_period_time_t   min_wall_time;
   
  rtems_rate_monotonic_period_time_t   max_wall_time;
   
  rtems_rate_monotonic_period_time_t   total_wall_time;
}  rtems_rate_monotonic_period_statistics;




 
typedef struct {
   
  uint32_t     count;
   
  uint32_t     missed_count;

   
  Thread_CPU_usage_t                   min_cpu_time;
   
  Thread_CPU_usage_t                   max_cpu_time;
   
  Thread_CPU_usage_t                   total_cpu_time;

   
  Rate_monotonic_Period_time_t         min_wall_time;
   
  Rate_monotonic_Period_time_t         max_wall_time;
   
  Rate_monotonic_Period_time_t         total_wall_time;
}  Rate_monotonic_Statistics;



 
typedef struct {
   
  rtems_id                             owner;

   
  rtems_rate_monotonic_period_states   state;

  



 
  rtems_rate_monotonic_period_time_t   since_last_period;

  



 
  rtems_thread_cpu_usage_t             executed_since_last_period;
}  rtems_rate_monotonic_period_status;




 
typedef struct {
   
  Objects_Control                         Object;

   
  Watchdog_Control                        Timer;

   
  rtems_rate_monotonic_period_states      state;

  


 
  uint32_t                                next_length;

  


 
  Thread_Control                         *owner;

  



 
  Thread_CPU_usage_t                      cpu_usage_period_initiated;

  


 
  Rate_monotonic_Period_time_t            time_period_initiated;

  

 
  Rate_monotonic_Statistics               Statistics;
}   Rate_monotonic_Control;









 
rtems_status_code rtems_rate_monotonic_create(
  rtems_name    name,
  rtems_id     *id
);















 
rtems_status_code rtems_rate_monotonic_ident(
  rtems_name    name,
  rtems_id     *id
);













 
rtems_status_code rtems_rate_monotonic_cancel(
  rtems_id   id
);












 
rtems_status_code rtems_rate_monotonic_delete(
  rtems_id   id
);














 
rtems_status_code rtems_rate_monotonic_get_status(
  rtems_id                             id,
  rtems_rate_monotonic_period_status  *status
);











 
rtems_status_code rtems_rate_monotonic_get_statistics(
  rtems_id                                id,
  rtems_rate_monotonic_period_statistics *statistics
);








 
rtems_status_code rtems_rate_monotonic_reset_statistics(
  rtems_id                                 id
);






 
void rtems_rate_monotonic_reset_all_statistics( void );









 
void rtems_rate_monotonic_report_statistics_with_plugin(
  void                  *context,
  rtems_printk_plugin_t  print
);






 
void rtems_rate_monotonic_report_statistics( void );













 
rtems_status_code rtems_rate_monotonic_period(
  rtems_id        id,
  rtems_interval  length
);

 


 










 







 









 
 






 
void _Multiprocessing_Manager_initialization ( void );







 
void rtems_multiprocessing_announce ( void );


 

 






 








 












 













 
uint32_t rtems_get_processor_count(void);

















 
uint32_t rtems_get_current_processor(void);

 


 





 
const char *rtems_get_version_string(void);




 



 



 




 



 






 



 




 




 







 



 










 














 





 



 







 


 

 






 











 






 










 






 










 






 
 



 





 





 



 













 
void _Chain_Initialize(
  Chain_Control *the_chain,
  void          *starting_address,
  size_t         number_nodes,
  size_t         node_size
);











 
void _Chain_Extract(
  Chain_Node *the_node
);












 
Chain_Node *_Chain_Get(
  Chain_Control *the_chain
);
















 
void _Chain_Insert(
  Chain_Node *after_node,
  Chain_Node *the_node
);








 
void _Chain_Append(
  Chain_Control *the_chain,
  Chain_Node    *the_node
);














 
_Bool _Chain_Append_with_empty_check(
  Chain_Control *the_chain,
  Chain_Node    *the_node
);














 
_Bool _Chain_Prepend_with_empty_check(
  Chain_Control *the_chain,
  Chain_Node    *the_node
);




















 
_Bool _Chain_Get_with_empty_check(
  Chain_Control *the_chain,
  Chain_Node **the_node
);










 
size_t _Chain_Node_count_unprotected( const Chain_Control *chain );








 
static inline void _Chain_Set_off_chain(
  Chain_Node *node
)
{
  node->next = 0;
}











 
static inline _Bool _Chain_Is_node_off_chain(
  const Chain_Node *node
)
{
  return node->next == 0;
}












 
static inline _Bool _Chain_Are_nodes_equal(
  const Chain_Node *left,
  const Chain_Node *right
)
{
  return left == right;
}










 
static inline _Bool _Chain_Is_null_node(
  const Chain_Node *the_node
)
{
  return (the_node == 0);
}









 
static inline Chain_Node *_Chain_Head(
  Chain_Control *the_chain
)
{
  return &the_chain->Head.Node;
}









 
static inline const Chain_Node *_Chain_Immutable_head(
  const Chain_Control *the_chain
)
{
  return &the_chain->Head.Node;
}









 
static inline Chain_Node *_Chain_Tail(
  Chain_Control *the_chain
)
{
  return &the_chain->Tail.Node;
}









 
static inline const Chain_Node *_Chain_Immutable_tail(
  const Chain_Control *the_chain
)
{
  return &the_chain->Tail.Node;
}










 
static inline Chain_Node *_Chain_First(
  Chain_Control *the_chain
)
{
  return _Chain_Head( the_chain )->next;
}










 
static inline const Chain_Node *_Chain_Immutable_first(
  const Chain_Control *the_chain
)
{
  return _Chain_Immutable_head( the_chain )->next;
}










 
static inline Chain_Node *_Chain_Last(
  Chain_Control *the_chain
)
{
  return _Chain_Tail( the_chain )->previous;
}










 
static inline const Chain_Node *_Chain_Immutable_last(
  const Chain_Control *the_chain
)
{
  return _Chain_Immutable_tail( the_chain )->previous;
}









 
static inline Chain_Node *_Chain_Next(
  Chain_Node *the_node
)
{
  return the_node->next;
}









 
static inline const Chain_Node *_Chain_Immutable_next(
  const Chain_Node *the_node
)
{
  return the_node->next;
}









 
static inline Chain_Node *_Chain_Previous(
  Chain_Node *the_node
)
{
  return the_node->previous;
}









 
static inline const Chain_Node *_Chain_Immutable_previous(
  const Chain_Node *the_node
)
{
  return the_node->previous;
}











 
static inline _Bool _Chain_Is_empty(
  const Chain_Control *the_chain
)
{
  return _Chain_Immutable_first( the_chain )
    == _Chain_Immutable_tail( the_chain );
}












 
static inline _Bool _Chain_Is_first(
  const Chain_Node *the_node
)
{
  return (the_node->previous->previous == 0);
}











 
static inline _Bool _Chain_Is_last(
  const Chain_Node *the_node
)
{
  return (the_node->next->next == 0);
}














 
static inline _Bool _Chain_Has_only_one_node(
  const Chain_Control *the_chain
)
{
  return _Chain_Immutable_first( the_chain )
    == _Chain_Immutable_last( the_chain );
}












 
static inline _Bool _Chain_Is_head(
  const Chain_Control *the_chain,
  const Chain_Node    *the_node
)
{
  return (the_node == _Chain_Immutable_head( the_chain ));
}












 
static inline _Bool _Chain_Is_tail(
  const Chain_Control *the_chain,
  const Chain_Node    *the_node
)
{
  return (the_node == _Chain_Immutable_tail( the_chain ));
}







 
static inline void _Chain_Initialize_empty(
  Chain_Control *the_chain
)
{
  Chain_Node *head;
  Chain_Node *tail;

  ( ( void ) 0 );

  head = _Chain_Head( the_chain );
  tail = _Chain_Tail( the_chain );

  head->next = tail;
  head->previous = 0;
  tail->previous = head;
}









 
static inline void _Chain_Extract_unprotected(
  Chain_Node *the_node
)
{
  Chain_Node *next;
  Chain_Node *previous;

  next           = the_node->next;
  previous       = the_node->previous;
  next->previous = previous;
  previous->next = next;
}















 
static inline Chain_Node *_Chain_Get_first_unprotected(
  Chain_Control *the_chain
)
{
  Chain_Node *head = _Chain_Head( the_chain );
  Chain_Node *old_first = head->next;
  Chain_Node *new_first = old_first->next;

  head->next = new_first;
  new_first->previous = head;

  return old_first;
}














 
static inline Chain_Node *_Chain_Get_unprotected(
  Chain_Control *the_chain
)
{
  if ( !_Chain_Is_empty(the_chain))
    return _Chain_Get_first_unprotected(the_chain);
  else
    return 0;
}













 
static inline void _Chain_Insert_unprotected(
  Chain_Node *after_node,
  Chain_Node *the_node
)
{
  Chain_Node *before_node;

  the_node->previous    = after_node;
  before_node           = after_node->next;
  after_node->next      = the_node;
  the_node->next        = before_node;
  before_node->previous = the_node;
}











 
static inline void _Chain_Append_unprotected(
  Chain_Control *the_chain,
  Chain_Node    *the_node
)
{
  Chain_Node *tail = _Chain_Tail( the_chain );
  Chain_Node *old_last = tail->previous;

  the_node->next = tail;
  tail->previous = the_node;
  old_last->next = the_node;
  the_node->previous = old_last;
}









 
static inline void _Chain_Append_if_is_off_chain_unprotected(
  Chain_Control *the_chain,
  Chain_Node    *the_node
)
{
  if ( _Chain_Is_node_off_chain( the_node ) ) {
    _Chain_Append_unprotected( the_chain, the_node );
  }
}











 
static inline void _Chain_Prepend_unprotected(
  Chain_Control *the_chain,
  Chain_Node    *the_node
)
{
  _Chain_Insert_unprotected(_Chain_Head(the_chain), the_node);
}











 
static inline void _Chain_Prepend(
  Chain_Control *the_chain,
  Chain_Node    *the_node
)
{
  _Chain_Insert(_Chain_Head(the_chain), the_node);
}














 
static inline _Bool _Chain_Append_with_empty_check_unprotected(
  Chain_Control *the_chain,
  Chain_Node    *the_node
)
{
  _Bool was_empty = _Chain_Is_empty( the_chain );

  _Chain_Append_unprotected( the_chain, the_node );

  return was_empty;
}














 
static inline _Bool _Chain_Prepend_with_empty_check_unprotected(
  Chain_Control *the_chain,
  Chain_Node    *the_node
)
{
  _Bool was_empty = _Chain_Is_empty( the_chain );

  _Chain_Prepend_unprotected( the_chain, the_node );

  return was_empty;
}


















 
static inline _Bool _Chain_Get_with_empty_check_unprotected(
  Chain_Control *the_chain,
  Chain_Node **the_node
)
{
  _Bool is_empty_now = 1;
  Chain_Node *head = _Chain_Head( the_chain );
  Chain_Node *tail = _Chain_Tail( the_chain );
  Chain_Node *old_first = head->next;

  if ( old_first != tail ) {
    Chain_Node *new_first = old_first->next;

    head->next = new_first;
    new_first->previous = head;

    *the_node = old_first;

    is_empty_now = new_first == tail;
  } else
    *the_node = 0;

  return is_empty_now;
}









 
typedef _Bool ( *Chain_Node_order )(
  const Chain_Node *left,
  const Chain_Node *right
);











 
static inline void _Chain_Insert_ordered_unprotected(
  Chain_Control *chain,
  Chain_Node *to_insert,
  Chain_Node_order order
)
{
  const Chain_Node *tail = _Chain_Immutable_tail( chain );
  Chain_Node *next = _Chain_First( chain );

  while ( next != tail && !( *order )( to_insert, next ) ) {
    next = _Chain_Next( next );
  }

  _Chain_Insert_unprotected( _Chain_Previous( next ), to_insert );
}

 


 








 
 

typedef Chain_Node rtems_chain_node;

typedef Chain_Control rtems_chain_control;



 





 





 



 









 
rtems_status_code rtems_chain_append_with_notification(
  rtems_chain_control *chain,
  rtems_chain_node *node,
  rtems_id task,
  rtems_event_set events
);









 
rtems_status_code rtems_chain_prepend_with_notification(
  rtems_chain_control *chain,
  rtems_chain_node *node,
  rtems_id task,
  rtems_event_set events
);









 
rtems_status_code rtems_chain_get_with_notification(
  rtems_chain_control *chain,
  rtems_id task,
  rtems_event_set events,
  rtems_chain_node **node
);









 
rtems_status_code rtems_chain_get_with_wait(
  rtems_chain_control *chain,
  rtems_event_set events,
  rtems_interval timeout,
  rtems_chain_node **node
);













 
static inline void rtems_chain_initialize(
  rtems_chain_control *the_chain,
  void                *starting_address,
  size_t               number_nodes,
  size_t               node_size
)
{
  _Chain_Initialize(
    the_chain,
    starting_address,
    number_nodes,
    node_size
  );
}







 
static inline void rtems_chain_initialize_empty(
  rtems_chain_control *the_chain
)
{
  _Chain_Initialize_empty( the_chain );
}








 
static inline void rtems_chain_set_off_chain(
  rtems_chain_node *node
)
{
  _Chain_Set_off_chain( node );
}











 
static inline _Bool rtems_chain_is_node_off_chain(
  const rtems_chain_node *node
)
{
  return _Chain_Is_node_off_chain( node );
}










 
static inline _Bool rtems_chain_is_null_node(
  const rtems_chain_node *the_node
)
{
  return _Chain_Is_null_node( the_node );
}









 
static inline rtems_chain_node *rtems_chain_head(
  rtems_chain_control *the_chain
)
{
  return _Chain_Head( the_chain );
}









 
static inline const rtems_chain_node *rtems_chain_immutable_head(
  const rtems_chain_control *the_chain
)
{
  return _Chain_Immutable_head( the_chain );
}









 
static inline rtems_chain_node *rtems_chain_tail(
  rtems_chain_control *the_chain
)
{
  return _Chain_Tail( the_chain );
}









 
static inline const rtems_chain_node *rtems_chain_immutable_tail(
  const rtems_chain_control *the_chain
)
{
  return _Chain_Immutable_tail( the_chain );
}










 
static inline rtems_chain_node *rtems_chain_first(
  rtems_chain_control *the_chain
)
{
  return _Chain_First( the_chain );
}










 
static inline const rtems_chain_node *rtems_chain_immutable_first(
  const rtems_chain_control *the_chain
)
{
  return _Chain_Immutable_first( the_chain );
}










 
static inline rtems_chain_node *rtems_chain_last(
  rtems_chain_control *the_chain
)
{
  return _Chain_Last( the_chain );
}










 
static inline const rtems_chain_node *rtems_chain_immutable_last(
  const rtems_chain_control *the_chain
)
{
  return _Chain_Immutable_last( the_chain );
}









 
static inline rtems_chain_node *rtems_chain_next(
  rtems_chain_node *the_node
)
{
  return _Chain_Next( the_node );
}









 
static inline const rtems_chain_node *rtems_chain_immutable_next(
  const rtems_chain_node *the_node
)
{
  return _Chain_Immutable_next( the_node );
}









 
static inline rtems_chain_node *rtems_chain_previous(
  rtems_chain_node *the_node
)
{
  return _Chain_Previous( the_node );
}









 
static inline const rtems_chain_node *rtems_chain_immutable_previous(
  const rtems_chain_node *the_node
)
{
  return _Chain_Immutable_previous( the_node );
}












 
static inline _Bool rtems_chain_are_nodes_equal(
  const rtems_chain_node *left,
  const rtems_chain_node *right
)
{
  return _Chain_Are_nodes_equal( left, right );
}











 
static inline _Bool rtems_chain_is_empty(
  const rtems_chain_control *the_chain
)
{
  return _Chain_Is_empty( the_chain );
}












 
static inline _Bool rtems_chain_is_first(
  const rtems_chain_node *the_node
)
{
  return _Chain_Is_first( the_node );
}











 
static inline _Bool rtems_chain_is_last(
  const rtems_chain_node *the_node
)
{
  return _Chain_Is_last( the_node );
}











 
static inline _Bool rtems_chain_has_only_one_node(
  const rtems_chain_control *the_chain
)
{
  return _Chain_Has_only_one_node( the_chain );
}












 
static inline _Bool rtems_chain_is_head(
  const rtems_chain_control *the_chain,
  const rtems_chain_node *the_node
)
{
  return _Chain_Is_head( the_chain, the_node );
}












 
static inline _Bool rtems_chain_is_tail(
  const rtems_chain_control *the_chain,
  const rtems_chain_node *the_node
)
{
  return _Chain_Is_tail( the_chain, the_node );
}









 
static inline void rtems_chain_extract(
  rtems_chain_node *the_node
)
{
  _Chain_Extract( the_node );
}








 
static inline void rtems_chain_extract_unprotected(
  rtems_chain_node *the_node
)
{
  _Chain_Extract_unprotected( the_node );
}












 
static inline rtems_chain_node *rtems_chain_get(
  rtems_chain_control *the_chain
)
{
  return _Chain_Get( the_chain );
}



 
static inline rtems_chain_node *rtems_chain_get_unprotected(
  rtems_chain_control *the_chain
)
{
  return _Chain_Get_unprotected( the_chain );
}









 
static inline void rtems_chain_insert(
  rtems_chain_node *after_node,
  rtems_chain_node *the_node
)
{
  _Chain_Insert( after_node, the_node );
}



 
static inline void rtems_chain_insert_unprotected(
  rtems_chain_node *after_node,
  rtems_chain_node *the_node
)
{
  _Chain_Insert_unprotected( after_node, the_node );
}








 
static inline void rtems_chain_append(
  rtems_chain_control *the_chain,
  rtems_chain_node    *the_node
)
{
  _Chain_Append( the_chain, the_node );
}








 
static inline void rtems_chain_append_unprotected(
  rtems_chain_control *the_chain,
  rtems_chain_node    *the_node
)
{
  _Chain_Append_unprotected( the_chain, the_node );
}











 
static inline void rtems_chain_prepend(
  rtems_chain_control *the_chain,
  rtems_chain_node    *the_node
)
{
  _Chain_Prepend( the_chain, the_node );
}











 
static inline void rtems_chain_prepend_unprotected(
  rtems_chain_control *the_chain,
  rtems_chain_node    *the_node
)
{
  _Chain_Prepend_unprotected( the_chain, the_node );
}








 
static inline _Bool rtems_chain_append_with_empty_check(
  rtems_chain_control *chain,
  rtems_chain_node *node
)
{
  return _Chain_Append_with_empty_check( chain, node );
}








 
static inline _Bool rtems_chain_prepend_with_empty_check(
  rtems_chain_control *chain,
  rtems_chain_node *node
)
{
  return _Chain_Prepend_with_empty_check( chain, node );
}












 
static inline _Bool rtems_chain_get_with_empty_check(
  rtems_chain_control *chain,
  rtems_chain_node **node
)
{
  return _Chain_Get_with_empty_check( chain, node );
}










 
static inline size_t rtems_chain_node_count_unprotected(
  const rtems_chain_control *chain
)
{
  return _Chain_Node_count_unprotected( chain );
}

 


 




 

 

 
typedef struct rtems_libio_tt rtems_libio_t;

struct rtems_filesystem_mount_table_entry_tt;
typedef struct rtems_filesystem_mount_table_entry_tt
    rtems_filesystem_mount_table_entry_t;

typedef struct _rtems_filesystem_file_handlers_r
    rtems_filesystem_file_handlers_r;
typedef struct _rtems_filesystem_operations_table
    rtems_filesystem_operations_table;





 
typedef struct rtems_filesystem_location_info_tt {
   rtems_chain_node                         mt_entry_node;
   void                                    *node_access;
   void                                    *node_access_2;
   const rtems_filesystem_file_handlers_r  *handlers;
   rtems_filesystem_mount_table_entry_t    *mt_entry;
} rtems_filesystem_location_info_t;




















 
typedef struct rtems_filesystem_global_location_t {
  rtems_filesystem_location_info_t location;
  int reference_count;

  



 
  struct rtems_filesystem_global_location_t *deferred_released_next;

  


 
  int deferred_released_count;
} rtems_filesystem_global_location_t;



 


 






 













 








 



 
static inline void rtems_bsd_bintime( struct bintime *bt )
{
  _Timecounter_Bintime( bt );
}



 
static inline void rtems_bsd_nanotime( struct timespec *ts )
{
  _Timecounter_Nanotime( ts );
}



 
static inline void rtems_bsd_microtime( struct timeval *tv )
{
  _Timecounter_Microtime( tv );
}



 
static inline void rtems_bsd_binuptime( struct bintime *bt )
{
  _Timecounter_Binuptime( bt );
}



 
static inline void rtems_bsd_nanouptime( struct timespec *ts )
{
  _Timecounter_Nanouptime( ts );
}



 
static inline void rtems_bsd_microuptime( struct timeval *tv )
{
  _Timecounter_Microuptime( tv );
}



 
static inline void rtems_bsd_getbintime( struct bintime *bt )
{
  _Timecounter_Getbintime( bt );
}



 
static inline void rtems_bsd_getnanotime( struct timespec *ts )
{
  _Timecounter_Getnanotime( ts );
}



 
static inline void rtems_bsd_getmicrotime( struct timeval *tv )
{
  _Timecounter_Getmicrotime( tv );
}



 
static inline void rtems_bsd_getbinuptime( struct bintime *bt )
{
  _Timecounter_Getbinuptime( bt );
}



 
static inline void rtems_bsd_getnanouptime( struct timespec *ts )
{
  _Timecounter_Getnanouptime( ts );
}



 
static inline void rtems_bsd_getmicrouptime( struct timeval *tv )
{
  _Timecounter_Getmicrouptime( tv );
}

 




typedef	unsigned int		vm_offset_t;
typedef	long long		vm_ooffset_t;
typedef	unsigned int		vm_pindex_t;
typedef	unsigned int		vm_size_t;


 




































 









































 
































 



typedef uint32_t ioctl_command_t;





 


 



 


 
 






int	ioctl(int, ioctl_command_t, ...);






 




 
struct winsize {
	unsigned short	ws_row;		 
	unsigned short	ws_col;		 
	unsigned short	ws_xpixel;	 
	unsigned short	ws_ypixel;	 
};

						 
						 
						 
						 
						 







 
struct ttysize {
	unsigned short	ts_lines;
	unsigned short	ts_cols;
	unsigned short	ts_xxx;
	unsigned short	ts_yyy;
};








































 



 































 




 






 



struct mdproc {
	int	md_flags;
	int	*md_regs;
};



 
struct socket;
extern int soconnsleep (struct socket *so);
extern void soconnwakeup (struct socket *so);

 
void *memcpy(void *dest, const void *src, size_t n);
void *memset(void *s, int c, size_t n);









 

typedef	u_int64_t	u_quad_t;	 
typedef	int64_t		quad_t;
typedef	quad_t *	qaddr_t;

typedef void __sighandler_t(int);
typedef	__sighandler_t	*sig_t;	 
struct sigaltstack {
       char    *ss_sp;                  
       int     ss_size;                 
       int     ss_flags;                
};

typedef	int		boolean_t;





 





 







 





 

static inline uint32_t __ntohl(uint32_t _x)
{
  return CPU_swap_u32(_x);
}

static inline uint16_t __ntohs(uint16_t _x)
{
  return (((_x&0xff) << 8) | ((_x >> 8)&0xff));
}

static inline uint32_t __htonl(uint32_t _x)
{
  return CPU_swap_u32(_x);
}

static inline uint16_t __htons(uint16_t _x)
{
  return (((_x&0xff) << 8) | ((_x >> 8)&0xff));
}



static inline uint16_t rtems_uint16_from_little_endian( const uint8_t *data)
{
  uint16_t value = 0;
  int i;

  for (i = 1; i >= 0; --i) {
    value = (uint16_t) ((value << 8) + data [i]);
  }

  return value;
}

static inline uint32_t rtems_uint32_from_little_endian( const uint8_t *data)
{
  uint32_t value = 0;
  int i;

  for (i = 3; i >= 0; --i) {
    value = (value << 8) + data [i];
  }

  return value;
}

static inline uint64_t rtems_uint64_from_little_endian( const uint8_t *data)
{
  uint64_t value = 0;
  int i;

  for (i = 7; i >= 0; --i) {
    value = (value << 8) + (uint64_t) data [i];
  }

  return value;
}

static inline uint16_t rtems_uint16_from_big_endian( const uint8_t *data)
{
  uint16_t value = 0;
  int i;

  for (i = 0; i < 2; ++i) {
    value = (uint16_t) ((value << 8) + data [i]);
  }

  return value;
}

static inline uint32_t rtems_uint32_from_big_endian( const uint8_t *data)
{
  uint32_t value = 0;
  int i;

  for (i = 0; i < 4; ++i) {
    value = (value << 8) + (uint32_t) data [i];
  }

  return value;
}

static inline uint64_t rtems_uint64_from_big_endian( const uint8_t *data)
{
  uint64_t value = 0;
  int i;

  for (i = 0; i < 8; ++i) {
    value = (value << 8) + (uint64_t) data [i];
  }

  return value;
}

static inline void rtems_uint16_to_little_endian( uint16_t value, uint8_t *data)
{
  int i;

  for (i = 0; i < 2; ++i) {
    data [i] = (uint8_t) value;
    value >>= 8;
  }
}

static inline void rtems_uint32_to_little_endian( uint32_t value, uint8_t *data)
{
  int i;

  for (i = 0; i < 4; ++i) {
    data [i] = (uint8_t) value;
    value >>= 8;
  }
}

static inline void rtems_uint64_to_little_endian( uint64_t value, uint8_t *data)
{
  int i;

  for (i = 0; i < 8; ++i) {
    data [i] = (uint8_t) value;
    value >>= 8;
  }
}

static inline void rtems_uint16_to_big_endian( uint16_t value, uint8_t *data)
{
  int i;

  for (i = 1; i >= 0; --i) {
    data [i] = (uint8_t) value;
    value >>= 8;
  }
}

static inline void rtems_uint32_to_big_endian( uint32_t value, uint8_t *data)
{
  int i;

  for (i = 3; i >= 0; --i) {
    data [i] = (uint8_t) value;
    value >>= 8;
  }
}

static inline void rtems_uint64_to_big_endian( uint64_t value, uint8_t *data)
{
  int i;

  for (i = 7; i >= 0; --i) {
    data [i] = (uint8_t) value;
    value >>= 8;
  }
}


typedef quad_t          rlim_t;          
typedef	u_int32_t	fixpt_t;	 





 
struct	proc;
struct	pgrp;
struct	ucred;
struct	rusage;
struct	buf;
struct	tty;
struct	uio;
struct	rtems_bsdnet_ifconfig;



 

void *rtems_bsdnet_malloc (size_t size, int type, int flags);
void rtems_bsdnet_free (void *addr, int type);

void rtems_bsdnet_semaphore_obtain (void);
void rtems_bsdnet_semaphore_release (void);
void rtems_bsdnet_semaphore_obtain_recursive (uint32_t nest_count);
uint32_t rtems_bsdnet_semaphore_release_recursive (void);
void rtems_bsdnet_schednetisr (int n);
int rtems_bsdnet_parse_driver_name (const struct rtems_bsdnet_ifconfig *config, char **namep);

static inline unsigned long rtems_bsdnet_seconds_since_boot(void)
{
  return (unsigned long) rtems_clock_get_uptime_seconds() + 1UL;
}

unsigned long rtems_bsdnet_random (void);

rtems_id rtems_bsdnet_newproc (
  char  *name,
  int   stacksize,
  void  (*entry)(void *),
  void  *arg
);


rtems_status_code rtems_bsdnet_event_receive (
  rtems_event_set  event_in,
  rtems_option     option_set,
  rtems_interval   ticks,
  rtems_event_set *event_out
);

static inline rtems_status_code rtems_bsdnet_event_send (
  rtems_id        task_id,
  rtems_event_set event_in
)
{
  return rtems_event_system_send (task_id, event_in);
}



 
extern int rtems_bsdnet_ticks_per_second;
extern int rtems_bsdnet_microseconds_per_tick;
extern struct in_addr rtems_bsdnet_log_host_address;
extern char *rtems_bsdnet_domain_name;



 



 
int sethostname (const char *name, size_t namelen);
void domaininit (void *);
void ifinit (void *);
void ipintr (void);
void arpintr (void);
int socket (int, int, int);
int ioctl (int, ioctl_command_t, ...);






 

struct socket *rtems_bsdnet_fdToSocket(int fd);


 


extern int securelevel;		 

extern int cold;		 
extern const char *panicstr;	 
extern char version[];		 
extern char copyright[];	 

extern int physmem;		 



 
int	nullop(void);
int	ureadc(int, struct uio *);
void	*hashinit(int count, int type, u_long *hashmask);
void	*phashinit(int count, int type, u_long *nentries);

void	rtems_panic(const char *, ...) __attribute__((__noreturn__));
void	boot(int) __attribute__((__noreturn__));
void	cpu_boot(int);
int	kvprintf(char const *, void (*)(int, void*), void *, int,
		      char *);
void	rtems_bsdnet_log(int, const char *, ...);
int	printf(const char *, ...);
int	sprintf(char *buf, const char *, ...);
void	uprintf(const char *, ...);
void	ttyprintf(struct tty *, const char *, ...);


int	copystr(const void *kfaddr, void *kdaddr, size_t len,
		size_t *lencopied);
int	copyinstr(const void *udaddr, void *kaddr, size_t len,
		size_t *lencopied);

int	hzto(struct timeval *tv);































 

 


 
extern u_char const	bcd2bin_data[];
extern u_char const	bin2bcd_data[];
extern char const	hex2ascii_data[];


static __inline int imax(int a, int b) { return (a > b ? a : b); }
static __inline int imin(int a, int b) { return (a < b ? a : b); }
static __inline long lmax(long a, long b) { return (a > b ? a : b); }
static __inline long lmin(long a, long b) { return (a < b ? a : b); }
static __inline u_int max(u_int a, u_int b) { return (a > b ? a : b); }
static __inline u_int min(u_int a, u_int b) { return (a < b ? a : b); }
static __inline quad_t qmax(quad_t a, quad_t b) { return (a > b ? a : b); }
static __inline quad_t qmin(quad_t a, quad_t b) { return (a < b ? a : b); }
static __inline u_long ulmax(u_long a, u_long b) { return (a > b ? a : b); }
static __inline u_long ulmin(u_long a, u_long b) { return (a < b ? a : b); }

 
int	 bcmp(const void *, const void *, size_t);
int	 ffs(int);
int	 fls(int);
int	 locc(int, char *, u_int);
void	 qsort(void *base, size_t nmemb, size_t size,
		    int (*compar)(const void *, const void *));
u_long	rtems_bsdnet_random(void);
char	*index(const char *, int);
char	*rindex(const char *, int);
int	 scanc(u_int, const u_char *, const u_char *, int);
int	 skpc(int, int, char *);
void	 srandom(u_long);
char	*strcat(char * __restrict, const char * __restrict);
int	 strcmp(const char *, const char *);
char	*strdup(const char *s);
char	*strcpy(char * __restrict, const char * __restrict);
size_t	 strlen(const char *);
int	 strncmp(const char *, const char *, size_t);
char	*strncpy(char * __restrict, const char * __restrict, size_t);
char	*strerror(int errnum);


 
typedef void (timeout_t)(void *);  
typedef timeout_t *timeout_func_t;  

void rtems_bsdnet_timeout(timeout_func_t,void *,int);
void untimeout(timeout_func_t, void *);




 
int	tsleep(void *chan, int pri, char *wmesg, int timo);
void	wakeup(void *chan);































 

 


































 





 



 


struct kmemstats {
	long	ks_inuse;	 
	long	ks_calls;	 
	long 	ks_memuse;	 
	u_short	ks_limblocks;	 
	u_short	ks_mapblocks;	 
	long	ks_maxused;	 
	long	ks_limit;	 
	long	ks_size;	 
	long	ks_spare;
};



 
struct kmemusage {
	short ku_indx;		 
	union {
		u_short freecnt; 
		u_short pagecnt; 
	} ku_un;
};



 
struct kmembuckets {
	caddr_t kb_next;	 
	caddr_t kb_last;	 
	long	kb_calls;	 
	long	kb_total;	 
	long	kb_totalfree;	 
	long	kb_elmpercl;	 
	long	kb_highwat;	 
	long	kb_couldfree;	 
};




 



 


extern struct kmemstats kmemstats[];
extern struct kmemusage *kmemusage;
extern char *kmembase;
extern struct kmembuckets bucket[];

void	rtems_bsdnet_free(void *addr,int type);
void	*rtems_bsdnet_malloc(size_t size,int type,int flags);
 
void	*realloc(void * __r, size_t __size);









 








 



 
struct m_hdr {
	struct	mbuf *mh_next;		 
	struct	mbuf *mh_nextpkt;	 
	caddr_t	mh_data;		 
	int	mh_len;			 
	int	mh_flags;		 
	short	mh_type;		 
};



 
struct	pkthdr {
	struct	ifnet *rcvif;		 
	int32_t	len;			 
};



 
struct m_ext {
	caddr_t	ext_buf;		 
	void	(*ext_free)		 
		(caddr_t, u_int);
	u_int	ext_size;		 
	void	(*ext_ref)		 
		(caddr_t, u_int);
};




 
struct mbuf {
	struct	m_hdr m_hdr;
	union {
		struct {
			struct	pkthdr MH_pkthdr;	 
			union {
				struct	m_ext MH_ext;	 
				char	MH_databuf[((128 - sizeof(struct m_hdr)) - sizeof(struct pkthdr))];
			} MH_dat;
		} MH;
		char	M_databuf[(128 - sizeof(struct m_hdr))];		 
	} M_dat;
};



 



 



 



 



 
struct mbstat {
	u_long	m_mbufs;	 
	u_long	m_clusters;	 
	u_long	m_spare;	 
	u_long	m_clfree;	 
	u_long	m_drops;	 
	u_long	m_wait;		 
	u_long	m_drain;	 
	u_short	m_mtypes[256];	 
};


 






 
union mcluster {
	union	mcluster *mcl_next;
	char	mcl_buf[(1 << 11)];
};







 










 









 







 




 




 




 




 




 






 




 

 

 

extern struct mbuf *mbutl;		 
extern char	*mclrefcnt;		 
extern struct mbstat mbstat;
extern uint32_t	nmbclusters;
extern uint32_t	nmbufs;
extern struct mbuf *mmbfree;
extern union mcluster *mclfree;
extern int	max_linkhdr;		 
extern int	max_protohdr;		 
extern int	max_hdr;		 
extern int	max_datalen;		 

struct	mbuf *m_copym(struct mbuf *, int, uint32_t, int);
struct	mbuf *m_copypacket(struct mbuf *, int);
struct	mbuf *m_devget(char *, int, int, struct ifnet *,
			    void (*copy)(char *, caddr_t, u_int));
struct	mbuf *m_free(struct mbuf *);
struct	mbuf *m_get(int, int);
struct	mbuf *m_getclr(int, int);
struct	mbuf *m_gethdr(int, int);
struct	mbuf *m_prepend(struct mbuf *,int,int);
struct	mbuf *m_pullup(struct mbuf *, int);
struct	mbuf *m_retry(int, int);
struct	mbuf *m_retryhdr(int, int);
struct	mbuf *m_split(struct mbuf *,int,int);
void	m_adj(struct mbuf *, int);
void	m_cat(struct mbuf *,struct mbuf *);
int	m_mballoc(int, int);
int	m_clalloc(int, int);
int	m_copyback(struct mbuf *, int, int, caddr_t);
int	m_copydata(const struct mbuf *, int, int, caddr_t);
void	m_freem(struct mbuf *);
void	m_reclaim(void);































 




 



 
struct	mbuf;

struct domain {
	int	dom_family;		 
	char	*dom_name;
	void	(*dom_init)		 
		(void);
	int	(*dom_externalize)	 
		(struct mbuf *);
	void	(*dom_dispose)		 
		(struct mbuf *);
	struct	protosw *dom_protosw, *dom_protoswNPROTOSW;
	struct	domain *dom_next;
	int	(*dom_rtattach)		 
		(void **, int);
	int	dom_rtoffset;		 
	int	dom_maxrtkey;		 
};

extern struct	domain *domains;
extern struct	domain localdomain;

































 


 
struct mbuf;
struct sockaddr;
struct socket;
struct sockproto;
struct stat;






















 
struct protosw {
	short	pr_type;		 
	struct	domain *pr_domain;	 
	short	pr_protocol;		 
	short	pr_flags;		 
 
	void	(*pr_input)(struct mbuf *, int len);
					 
	int	(*pr_output)(struct mbuf *m, struct socket *so);
					 
	void	(*pr_ctlinput)(int, struct sockaddr *, void *);
					 
	int	(*pr_ctloutput)(int, struct socket *, int, int,
				    struct mbuf **);
					 
 
	int	(*pr_ousrreq)(struct socket *, int, struct mbuf *,
				   struct mbuf *, struct mbuf *);
					 
 
	void	(*pr_init)(void);	 
	void	(*pr_fasttimo)(void);
					 
	void	(*pr_slowtimo)(void);
					 
	void	(*pr_drain)(void);
					 
	struct	pr_usrreqs *pr_usrreqs;	 
};










 












 
 
 


struct stat;
struct ifnet;



 
struct pr_usrreqs {
	int	(*pru_abort)(struct socket *so);
	int	(*pru_accept)(struct socket *so, struct mbuf *nam);
	int	(*pru_attach)(struct socket *so, intptr_t proto);
	int	(*pru_bind)(struct socket *so, struct mbuf *nam);
	int	(*pru_connect)(struct socket *so, struct mbuf *nam);
	int	(*pru_connect2)(struct socket *so1, struct socket *so2);
	int	(*pru_control)(struct socket *so, intptr_t cmd, caddr_t data,
				    struct ifnet *ifp);
	int	(*pru_detach)(struct socket *so);
	int	(*pru_disconnect)(struct socket *so);
	int	(*pru_listen)(struct socket *so);
	int	(*pru_peeraddr)(struct socket *so, struct mbuf *nam);
	int	(*pru_rcvd)(struct socket *so, intptr_t flags);
	int	(*pru_rcvoob)(struct socket *so, struct mbuf *m,
				   intptr_t flags);
	



 
	int	(*pru_send)(struct socket *so, int flags, struct mbuf *m, 
			      struct mbuf *addr, struct mbuf *control);
	int	(*pru_sense)(struct socket *so, struct stat *sb);
	int	(*pru_shutdown)(struct socket *so);
	int	(*pru_sockaddr)(struct socket *so, struct mbuf *nam);
};

int	pru_accept_notsupp(struct socket *so, struct mbuf *nam);
int	pru_connect2_notsupp(struct socket *so1, struct socket *so2);
int	pru_control_notsupp(struct socket *so, int cmd, caddr_t data,
				 struct ifnet *ifp);
int	pru_listen_notsupp(struct socket *so);
int	pru_rcvd_notsupp(struct socket *so, int flags);
int	pru_rcvoob_notsupp(struct socket *so, struct mbuf *m, int flags);
int	pru_sense_null(struct socket *so, struct stat *sb);






 
extern	struct pr_usrreqs pru_oldstyle;







 
 
















 



void   pfctlinput(int, struct sockaddr *);
struct protosw *pffindproto(int family, int protocol, int type);
struct protosw *pffindtype(int family, int type);































 




























 






 
typedef	uint8_t		__sa_family_t;
typedef	uint32_t	__socklen_t;




 



 

typedef	__sa_family_t	sa_family_t;

typedef	int	socklen_t;



 



 



 



 



 
struct	linger {
	int	l_onoff;		 
	int	l_linger;		 
};



 
struct socket;
struct	sockwakeup {
	void	(*sw_pfn)(struct socket *, void *);
	void	*sw_arg;
};



 



 





 
struct sockaddr {
	unsigned char	sa_len;			 
	sa_family_t	sa_family;		 
	char	sa_data[14];		 
};




 
struct sockproto {
	unsigned short	sp_family;		 
	unsigned short	sp_protocol;		 
};



 









 









 




 




 
struct msghdr {
	void		*msg_name;		 
	socklen_t	msg_namelen;		 
	struct	iovec *msg_iov;		 
	int	msg_iovlen;		 
	void		*msg_control;		 
	socklen_t	msg_controllen;		 
	int	msg_flags;		 
};







 
struct cmsghdr {
	socklen_t	cmsg_len;		 
	int	cmsg_level;		 
	int	cmsg_type;		 
 
};

 

 


 



 
struct osockaddr {
	unsigned short	sa_family;		 
	char	sa_data[14];		 
};



 
struct omsghdr {
	char	*msg_name;		 
	int	msg_namelen;		 
	struct	iovec *msg_iov;		 
	int	msg_iovlen;		 
	char	*msg_accrights;		 
	int	msg_accrightslen;
};



 



int	accept(int, struct sockaddr * __restrict, socklen_t * __restrict);
int	bind(int, const struct sockaddr *, socklen_t);
int	connect(int, const struct sockaddr *, socklen_t);
int	getpeername(int, struct sockaddr * __restrict, socklen_t * __restrict);
int	getsockname(int, struct sockaddr * __restrict, socklen_t * __restrict);
int	getsockopt(int, int, int, void * __restrict, socklen_t * __restrict);
int	listen(int, int);
ssize_t	recv(int, void *, size_t, int);
ssize_t	recvfrom(int, void *, size_t, int, struct sockaddr * __restrict, socklen_t * __restrict);
ssize_t	recvmsg(int, struct msghdr *, int);
ssize_t	send(int, const void *, size_t, int);
ssize_t	sendto(int, const void *, size_t, int, const struct sockaddr *, socklen_t);
ssize_t	sendmsg(int, const struct msghdr *, int);
int	setsockopt(int, int, int, const void *, socklen_t);
int	shutdown(int, int);
int	socket(int, int, int);
int	socketpair(int, int, int, int *);



 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 



#pragma diag_push
#pragma CHECK_MISRA("-6.3")  
#pragma CHECK_MISRA("-19.1")  
#pragma CHECK_MISRA("-20.1")  
#pragma CHECK_MISRA("-20.2")  



 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 


#pragma diag_push
#pragma CHECK_MISRA("-19.4")  
#pragma CHECK_MISRA("-19.7")  
#pragma CHECK_MISRA("-19.10")  

 
 
 
 
 
 
 
 







#pragma diag_pop



     
     
     
     
    extern int *__c6xabi_errno_addr(void);
    



#pragma diag_pop

typedef int error_t;



 


 



 




typedef unsigned long __ULong;






 

struct _Bigint
{
    struct _Bigint *_next;
    int _k, _maxwds, _sign, _wds;
    __ULong _x[1];
};

 
struct __tm
{
    int   __tm_sec;
    int   __tm_min;
    int   __tm_hour;
    int   __tm_mday;
    int   __tm_mon;
    int   __tm_year;
    int   __tm_wday;
    int   __tm_yday;
    int   __tm_isdst;
};



 


struct _on_exit_args
{
    void   *_fnargs[32];	         
    void 	*_dso_handle[32];
     
    __ULong _fntypes;           	        
 
     
    __ULong _is_cxa;
};

struct _atexit
{
    struct	_atexit *_next;			 
    int	_ind;				 
     
    void	(*_fns[32])(void);	 
    struct _on_exit_args _on_exit_args;
};






 

struct __sbuf
{
    unsigned char *_base;
    int	_size;
};




 

typedef long _fpos_t;		 
 

























 


struct __sFILE
{
    unsigned char *_p;	 
    int	_r;		 
    int	_w;		 
    short	_flags;		 
    short	_file;		 
    struct __sbuf _bf;	 
    int	_lbfsize;	 


     
    void *	_cookie;	 

    _ssize_t (*_read) (void * _cookie, char *_buf, int _n);
    _ssize_t (*_write) (void * _cookie, const char *_buf, int _n);
    _fpos_t (*_seek) (void * _cookie, _fpos_t _offset, int _whence);
    int	(*_close) (void * _cookie);

     
    struct __sbuf _ub;	 
    unsigned char *_up;	 
    int	_ur;		 

     
    unsigned char _ubuf[3];	 
    unsigned char _nbuf[1];	 

     
    struct __sbuf _lb;	 

     
    int	_blksize;	 
    int	_offset;	 

    struct _reent *_data;	 

    _flock_t _lock;	 
};

typedef struct __sFILE   __FILE;

struct _glue
{
    struct _glue *_next;
    int _niobs;
    __FILE *_iobs;
};














 
struct _rand48
{
    unsigned short _seed[3];
    unsigned short _mult[3];
    unsigned short _add;
};

 







 


struct _reent
{
    int _errno;			 

    

 
    __FILE *_stdin, *_stdout, *_stderr;

    int  _inc;			 
    char _emergency[25];

    int _current_category;	 
    const char *_current_locale;

    int __sdidinit;		 

    void (*__cleanup) (struct _reent *);

     
    struct _Bigint *_result;
    int _result_k;
    struct _Bigint *_p5s;
    struct _Bigint **_freelist;

     
    int _cvtlen;			 
    char *_cvtbuf;

    union
    {
        struct
        {
            unsigned int _unused_rand;
            char *_strtok_last;
            char _asctime_buf[26];
            struct __tm _localtime_buf;
            int _gamma_signgam;
              unsigned long long _rand_next;
            struct _rand48 _r48;
            _mbstate_t _mblen_state;
            _mbstate_t _mbtowc_state;
            _mbstate_t _wctomb_state;
            char _l64a_buf[8];
            char _signal_buf[24];
            int _getdate_err;
            _mbstate_t _mbrlen_state;
            _mbstate_t _mbrtowc_state;
            _mbstate_t _mbsrtowcs_state;
            _mbstate_t _wcrtomb_state;
            _mbstate_t _wcsrtombs_state;
        } _reent;
        

 
        struct
        {
            unsigned char *_nextf[30];
            unsigned int _nmalloc[30];
        } _unused;
    } _new;

     
    struct _atexit *_atexit;	 
    struct _atexit _atexit0;	 

     
    void (**(_sig_func))(int);

    

 
    struct _glue __sglue;		 
    __FILE __sf[3];  		 
};









 


extern struct _reent *_impure_ptr ;
extern struct _reent *const _global_impure_ptr ;

void _reclaim_reent (struct _reent *);

 







 
extern int *__c6xabi_errno_addr(void);
extern int __errno;




 
extern  const char *const _sys_errlist[];
extern  int _sys_nerr;



 




#pragma diag_push


 
 
#pragma CHECK_MISRA("-19.15")


#pragma diag_pop









































 





























 






 






 



 



 



 








 




 

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 


#pragma diag_push


 
 
#pragma CHECK_MISRA("-19.15")


#pragma diag_pop

 

 
extern long hostid;
extern char hostname[256];
extern char domainname[256];
extern char kernelname[1024];

 
extern volatile struct timeval mono_time;
extern struct timeval boottime;
extern struct timeval runtime;
 
extern struct timezone tz;			 

extern int rtems_bsdnet_microseconds_per_tick;			 
extern int rtems_bsdnet_ticks_per_second;				 
extern int psratio;			 
extern int stathz;			 
extern int profhz;			 
extern int ticks;

























 
enum sysinit_sub_id {
	SI_SUB_DUMMY		= 0x00000000,	 
	SI_SUB_CONSOLE		= 0x08000000,	 
	SI_SUB_COPYRIGHT	= 0x08000001,	 
	SI_SUB_VM		= 0x10000000,	 
	SI_SUB_KMEM		= 0x18000000,	 
	SI_SUB_CPU		= 0x20000000,	 
	SI_SUB_DEVFS		= 0x22000000,	 
	SI_SUB_DRIVERS		= 0x23000000,	 
	SI_SUB_CONFIGURE	= 0x24000000,	 
	SI_SUB_INTRINSIC	= 0x28000000,	 
	SI_SUB_RUN_QUEUE	= 0x30000000,	 
	SI_SUB_VM_CONF		= 0x38000000,	 
	SI_SUB_VFS		= 0x40000000,	 
	SI_SUB_CLOCKS		= 0x48000000,	 
	SI_SUB_MBUF		= 0x50000000,	 
	SI_SUB_CLIST		= 0x58000000,	 
	SI_SUB_SYSV_SHM		= 0x64000000,	 
	SI_SUB_SYSV_SEM		= 0x68000000,	 
	SI_SUB_SYSV_MSG		= 0x6C000000,	 
	SI_SUB_PSEUDO		= 0x70000000,	 
	SI_SUB_PROTO_BEGIN	= 0x80000000,	 
	SI_SUB_PROTO_IF		= 0x84000000,	 
	SI_SUB_PROTO_DOMAIN	= 0x88000000,	 
	SI_SUB_PROTO_END	= 0x8fffffff,	 
	SI_SUB_KPROF		= 0x90000000,	 
	SI_SUB_KICK_SCHEDULER	= 0xa0000000,	 
	SI_SUB_ROOT		= 0xb0000000,	 
	SI_SUB_ROOT_FDTAB	= 0xb8000000,	 
	SI_SUB_SWAP		= 0xc0000000,	 
	SI_SUB_INTRINSIC_POST	= 0xd0000000,	 
	SI_SUB_KTHREAD_INIT	= 0xe0000000,	 
	SI_SUB_KTHREAD_PAGE	= 0xe4000000,	 
	SI_SUB_KTHREAD_VM	= 0xe8000000,	 
	SI_SUB_KTHREAD_UPDATE	= 0xec000000,	 
	SI_SUB_RUN_SCHEDULER	= 0xffffffff	 
};




 
enum sysinit_elem_order {
	SI_ORDER_FIRST		= 0x00000000,	 
	SI_ORDER_SECOND		= 0x00000001,	 
	SI_ORDER_THIRD		= 0x00000002,	 
	SI_ORDER_MIDDLE		= 0x10000000,	 
	SI_ORDER_ANY		= 0xffffffff	 
};






 
struct sysinit {
	unsigned int	subsystem;		 
	unsigned int	order;			 
	void		(*func)(void *);	 
	void		*udata;			 
};




 




 






 
struct kproc_desc {
	char		*arg0;			 
	void		(*func)(void);	 
	struct proc	**global_procpp;	 
};

void	kproc_start(void *udata);































 












 

				 


 
				 
				 
				 

	 

				 





 






 




































 




































 

































































 



 





 












 





 















 





 












 





 

















 




 










 



struct quehead
{
    struct quehead *qh_link;
    struct quehead *qh_rlink;
};

static __inline void
insque(void *a, void *b)
{
    struct quehead *element = (struct quehead *)a,
                    *head = (struct quehead *)b;

    element->qh_link = head->qh_link;
    element->qh_rlink = head;
    head->qh_link = element;
    element->qh_link->qh_rlink = element;
}

static __inline void
remque(void *a)
{
    struct quehead *element = (struct quehead *)a;

    element->qh_link->qh_rlink = element->qh_rlink;
    element->qh_rlink->qh_link = element->qh_link;
    element->qh_rlink = 0;
}




struct thread;







 








 
struct ctlname {
	char	*ctl_name;	 
	int	ctl_type;	 
};







 







  




 


 




 
struct sysctl_req {
	struct thread	*td;		 
	int		lock;		 
	void		*oldptr;
	size_t		oldlen;
	size_t		oldidx;
	int		(*oldfunc)(struct sysctl_req *, const void *, size_t);
	void		*newptr;
	size_t		newlen;
	size_t		newidx;
	int		(*newfunc)(struct sysctl_req *, void *, size_t);
	size_t		validlen;
};

struct sysctl_oid_list { struct sysctl_oid *slh_first; };




 
struct sysctl_oid {
	struct sysctl_oid_list *oid_parent;
	struct { struct sysctl_oid *sle_next; } oid_link;
	int		oid_number;
	uint32_t	oid_kind;
	void		*oid_arg1;
	int32_t		oid_arg2;
	const char	*oid_name;
	int 		(*oid_handler)(struct sysctl_oid *oidp, void *arg1, int arg2, struct sysctl_req *req);
	const char	*oid_fmt;
	int		oid_refcnt;
	const char	*descr;
};


int sysctl_handle_int(struct sysctl_oid *oidp, void *arg1, int arg2, struct sysctl_req *req);
int sysctl_handle_long(struct sysctl_oid *oidp, void *arg1, int arg2, struct sysctl_req *req);
int sysctl_handle_intptr(struct sysctl_oid *oidp, void *arg1, int arg2, struct sysctl_req *req);
int sysctl_handle_string(struct sysctl_oid *oidp, void *arg1, int arg2, struct sysctl_req *req);
int sysctl_handle_opaque(struct sysctl_oid *oidp, void *arg1, int arg2, struct sysctl_req *req);



 
void sysctl_register_oid(struct sysctl_oid *oidp);
void sysctl_unregister_oid(struct sysctl_oid *oidp);

 

 

 

 
struct sysctl_ctx_entry {
	struct sysctl_oid *entry;
	struct { struct sysctl_ctx_entry *tqe_next; struct sysctl_ctx_entry * *tqe_prev;  } link;
};

struct sysctl_ctx_list { struct sysctl_ctx_entry *tqh_first; struct sysctl_ctx_entry * *tqh_last;  };


 


 


 


 


 


 


 


 


 


 





 




 




 



 



 



 




 








 
extern struct sysctl_oid_list sysctl__children;
extern struct sysctl_oid_list sysctl__kern_children;
extern struct sysctl_oid_list sysctl__sysctl_children;
extern struct sysctl_oid_list sysctl__vm_children;
extern struct sysctl_oid_list sysctl__vfs_children;
extern struct sysctl_oid_list sysctl__net_children;
extern struct sysctl_oid_list sysctl__debug_children;
extern struct sysctl_oid_list sysctl__debug_sizeof_children;
extern struct sysctl_oid_list sysctl__hw_children;
extern struct sysctl_oid_list sysctl__machdep_children;
extern struct sysctl_oid_list sysctl__user_children;
extern struct sysctl_oid_list sysctl__compat_children;

extern char	machine[];
extern char	osrelease[];
extern char	ostype[];
extern char	kern_ident[];

 
struct sysctl_oid *sysctl_add_oid(struct sysctl_ctx_list *clist,
		struct sysctl_oid_list *parent, int nbr, const char *name,
		int kind, void *arg1, int arg2,
		int (*handler) (struct sysctl_oid *oidp, void *arg1, int arg2, struct sysctl_req *req),
		const char *fmt, const char *descr);
int	sysctl_move_oid(struct sysctl_oid *oidp,
		struct sysctl_oid_list *parent);
int	sysctl_remove_oid(struct sysctl_oid *oidp, int del, int recurse);
int	sysctl_ctx_init(struct sysctl_ctx_list *clist);
int	sysctl_ctx_free(struct sysctl_ctx_list *clist);
struct	sysctl_ctx_entry *sysctl_ctx_entry_add(struct sysctl_ctx_list *clist,
		struct sysctl_oid *oidp);
struct	sysctl_ctx_entry *sysctl_ctx_entry_find(struct sysctl_ctx_list *clist,
		struct sysctl_oid *oidp);
int	sysctl_ctx_entry_del(struct sysctl_ctx_list *clist,
		struct sysctl_oid *oidp);

int	kernel_sysctl(struct thread *td, int *name, u_int namelen, void *old,
		      size_t *oldlenp, void *new, size_t newlen,
		      size_t *retval);
int	kernel_sysctlbyname(struct thread *td, char *name,
		void *old, size_t *oldlenp, void *new, size_t newlen,
		size_t *retval);
int	userland_sysctl(struct thread *td, int *name, u_int namelen, void *old,
			size_t *oldlenp, int inkernel, void *new, size_t newlen,
			size_t *retval);
int	sysctl_find_oid(int *name, u_int namelen, struct sysctl_oid **noid,
			int *nindx, struct sysctl_req *req);
int	sysctl_wire_old_buffer(struct sysctl_req *req, size_t len);

































 

 





 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 



 

typedef struct { intmax_t quot, rem; } imaxdiv_t;





 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 










 


 intmax_t  imaxabs(intmax_t j);
 imaxdiv_t imaxdiv(intmax_t numer, intmax_t denom);
 intmax_t  strtoimax(const char * __restrict nptr, 
		    char ** __restrict endptr, int base);
 uintmax_t strtoumax(const char * __restrict nptr, 
		    char ** __restrict endptr, int base);








 


struct ifnet;
struct	ether_header;




 



 

struct if_clonereq {
	int	ifcr_total;		 
	int	ifcr_count;		 
	char	*ifcr_buffer;		 
};




 
struct if_data {
	 
	u_char	ifi_type;		 
	u_char	ifi_physical;		 
	u_char	ifi_addrlen;		 
	u_char	ifi_hdrlen;		 
	u_char	ifi_recvquota;		 
	u_char	ifi_xmitquota;		 
	u_long	ifi_mtu;		 
	u_long	ifi_metric;		 
	u_long	ifi_baudrate;		 
	 
	u_long	ifi_ipackets;		 
	u_long	ifi_ierrors;		 
	u_long	ifi_opackets;		 
	u_long	ifi_oerrors;		 
	u_long	ifi_collisions;		 
	u_long	ifi_ibytes;		 
	u_long	ifi_obytes;		 
	u_long	ifi_imcasts;		 
	u_long	ifi_omcasts;		 
	u_long	ifi_iqdrops;		 
	u_long	ifi_noproto;		 
	u_long	ifi_recvtiming;		 
	u_long	ifi_xmittiming;		 
	struct	timeval ifi_lastchange;	 
};


 



 




 





 
struct if_msghdr {
	u_short	ifm_msglen;	 
	u_char	ifm_version;	 
	u_char	ifm_type;	 
	int	ifm_addrs;	 
	int	ifm_flags;	 
	u_short	ifm_index;	 
	struct	if_data ifm_data; 
};




 
struct ifa_msghdr {
	u_short	ifam_msglen;	 
	u_char	ifam_version;	 
	u_char	ifam_type;	 
	int	ifam_addrs;	 
	int	ifam_flags;	 
	u_short	ifam_index;	 
	int	ifam_metric;	 
};

 
struct mbuf;






 
struct	ifreq {
	char	ifr_name[16];		 
	union {
		struct	sockaddr ifru_addr;
		struct	sockaddr ifru_dstaddr;
		struct	sockaddr ifru_broadaddr;
		short	ifru_flags[2];
		int32_t	ifru_metric;
		int32_t	ifru_mtu;
		int	ifru_phys;
		int	ifru_media;
		caddr_t	ifru_data;
		int	(*ifru_tap)(struct ifnet *, struct ether_header *, struct mbuf *);
	} ifr_ifru;
};

struct ifaliasreq {
	char	ifra_name[16];		 
	struct	sockaddr ifra_addr;
	struct	sockaddr ifra_broadaddr;
	struct	sockaddr ifra_mask;
};

struct ifmediareq {
	char	ifm_name[16];	 
	int	ifm_current;		 
	int	ifm_mask;		 
	int	ifm_status;		 
	int	ifm_active;		 
	int	ifm_count;		 
	int	*ifm_ulist;		 
};






 

struct ifstat {
	char	ifs_name[16];	 
	char	ascii[800 + 1];
};






 
struct	ifconf {
	int	ifc_len;		 
	union {
		caddr_t	ifcu_buf;
		struct	ifreq *ifcu_req;
	} ifc_ifcu;
};


struct if_nameindex {
    unsigned int    if_index;    
    char        *if_name;    
};


void             if_freenameindex(struct if_nameindex *);
char            *if_indextoname(unsigned int, char *);
struct if_nameindex *if_nameindex(void);



 






























 
 
































 

 
























 



 
struct	mbuf;
struct	rtentry;
struct	rt_addrinfo;
struct	socket;
struct	ether_header;




 
struct	ifqueue {
	struct	mbuf *ifq_head;
	struct	mbuf *ifq_tail;
	int	ifq_len;
	int	ifq_maxlen;
	int	ifq_drops;
};





 
struct ifnet {
	void	*if_softc;		 
	char	*if_name;		 
	struct	ifnet *if_next;		 
	struct	ifaddr *if_addrlist;	 
        int	if_pcount;		 
	struct	bpf_if *if_bpf;		 
	u_short	if_index;		 
	short	if_unit;		 
	short	if_timer;		 
	int	if_flags;		 
	void	*if_linkmib;		 
	size_t	if_linkmiblen;		 
	struct	if_data if_data;
 
	int	(*if_output)		 
		(struct ifnet *, struct mbuf *, struct sockaddr *,
		     struct rtentry *);
	void	(*if_start)		 
		(struct ifnet *);
	int	(*if_ioctl)		 
		(struct ifnet *, ioctl_command_t, caddr_t);
	void	(*if_watchdog)		 
		(struct ifnet *);
	int	(*if_poll_recv)		 
		(struct ifnet *, int *);
	int	(*if_poll_xmit)		 
		(struct ifnet *, int *);
	void	(*if_poll_intren)	 
		(struct ifnet *);
	void	(*if_poll_slowinput)	 
		(struct ifnet *, struct mbuf *);
	void	(*if_init)		 
		(void *);
	int	(*if_tap)		 
		(struct ifnet *, struct ether_header *, struct mbuf *);
	struct	ifqueue if_snd;		 
	struct	ifqueue *if_poll_slowq;	 
};

typedef void if_init_f_t(void *);




 






 









 
struct ifaddr {
	struct	sockaddr *ifa_addr;	 
	struct	sockaddr *ifa_dstaddr;	 
	struct	sockaddr *ifa_netmask;	 
	struct	ifnet *ifa_ifp;		 
	struct	ifaddr *ifa_next;	 
	void	(*ifa_rtrequest)	 
		(int, struct rtentry *, struct sockaddr *);
	u_short	ifa_flags;		 
	u_int	ifa_refcnt;		 
	int	ifa_metric;		 
	int (*ifa_claim_addr)		 
		(struct ifaddr *, struct sockaddr *);

};


extern struct	ifnet	*ifnet;
extern int	ifqmaxlen;
extern struct	ifnet	loif[];
extern int	if_index;
extern struct	ifaddr	**ifnet_addrs;

void	if_attach(struct ifnet *);
void	if_down(struct ifnet *);
void	if_up(struct ifnet *);
   
int	ifioctl(struct socket *, u_long, caddr_t, struct proc *);
int	ifpromisc(struct ifnet *, int);

struct	ifaddr *ifa_ifwithaddr(struct sockaddr *);
struct	ifaddr *ifa_ifwithdstaddr(struct sockaddr *);
struct	ifaddr *ifa_ifwithnet(struct sockaddr *);
struct	ifaddr *ifa_ifwithroute(int, struct sockaddr *, struct sockaddr *);
struct	ifaddr *ifaof_ifpforaddr(struct sockaddr *, struct ifnet *);



void	ifafree(struct ifaddr *);

int	looutput(struct ifnet *,
	   struct mbuf *, struct sockaddr *, struct rtentry *);































 



















 



 
struct sockaddr_dl {
	u_char	sdl_len;	 
	u_char	sdl_family;	 
	u_short	sdl_index;	 
	u_char	sdl_type;	 
	u_char	sdl_nlen;	 
	u_char	sdl_alen;	 
	u_char	sdl_slen;	 
	char	sdl_data[46];	
 
};

































 








 





 
struct route {
	struct	rtentry *ro_rt;
	struct	sockaddr ro_dst;
};




 
struct rt_metrics_lite {
	u_long	rmx_mtu;	 
	u_long	rmx_expire;	 
	u_long	rmx_pksent;	 
};

struct rt_metrics {
	u_long	rmx_locks;	 
	u_long	rmx_mtu;	 
	u_long	rmx_hopcount;	 
	u_long	rmx_expire;	 
	u_long	rmx_recvpipe;	 
	u_long	rmx_sendpipe;	 
	u_long	rmx_ssthresh;	 
	u_long	rmx_rtt;	 
	u_long	rmx_rttvar;	 
	u_long	rmx_pksent;	 
	u_long	rmx_filler[4];	 
};





 



 
struct mbuf;








 






























 




 

struct radix_node {
	struct	radix_mask *rn_mklist;	 
	struct	radix_node *rn_parent;	 
	short	rn_bit;			 
	char	rn_bmask;		 
	u_char	rn_flags;		 
	union {
		struct {			 
			caddr_t	rn_Key;		 
			caddr_t	rn_Mask;	 
			struct	radix_node *rn_Dupedkey;
		} rn_leaf;
		struct {			 
			int	rn_Off;		 
			struct	radix_node *rn_L; 
			struct	radix_node *rn_R; 
		} rn_node;
	}		rn_u;
};




 

struct radix_mask {
	short	rm_bit;			 
	char	rm_unused;		 
	u_char	rm_flags;		 
	struct	radix_mask *rm_mklist;	 
	union	{
		caddr_t	rmu_mask;		 
		struct	radix_node *rmu_leaf;	 
	}	rm_rmu;
	int	rm_refs;		 
};



typedef int walktree_f_t(struct radix_node *, void *);

struct radix_node_head {
	struct	radix_node *rnh_treetop;
	int	rnh_addrsize;		 
	int	rnh_pktsize;		 
	struct	radix_node *(*rnh_addaddr)	 
		(void *v, void *mask,
		     struct radix_node_head *head, struct radix_node nodes[]);
	struct	radix_node *(*rnh_addpkt)	 
		(void *v, void *mask,
		     struct radix_node_head *head, struct radix_node nodes[]);
	struct	radix_node *(*rnh_deladdr)	 
		(void *v, void *mask, struct radix_node_head *head);
	struct	radix_node *(*rnh_delpkt)	 
		(void *v, void *mask, struct radix_node_head *head);
	struct	radix_node *(*rnh_matchaddr)	 
		(void *v, struct radix_node_head *head);
	struct	radix_node *(*rnh_lookup)	 
		(void *v, void *mask, struct radix_node_head *head);
	struct	radix_node *(*rnh_matchpkt)	 
		(void *v, struct radix_node_head *head);
	int	(*rnh_walktree)			 
		(struct radix_node_head *head, walktree_f_t *f, void *w);
	int	(*rnh_walktree_from)		 
		(struct radix_node_head *head, void *a, void *m,
		     walktree_f_t *f, void *w);
	void	(*rnh_close)	 
		(struct radix_node *rn, struct radix_node_head *head);
	struct	radix_node rnh_nodes[3];	 
};


void	 rn_init(void);
int	 rn_inithead(void **, int);
int	 rn_refines(void *, void *);
struct radix_node
	 *rn_addmask(void *, int, int),
	 *rn_addroute (void *, void *, struct radix_node_head *,
			struct radix_node [2]),
	 *rn_delete(void *, void *, struct radix_node_head *),
	 *rn_lookup (void *v_arg, void *m_arg,
		        struct radix_node_head *head),
	 *rn_match(void *, struct radix_node_head *);

struct rtentry {
	struct	radix_node rt_nodes[2];	 
	struct	sockaddr *rt_gateway;	 
	u_long	rt_flags;		 
	struct	ifnet *rt_ifp;		 
	struct	ifaddr *rt_ifa;		 
	struct	rt_metrics rt_rmx;	 
	long	rt_refcnt;		 
	struct	sockaddr *rt_genmask;	 
	caddr_t	rt_llinfo;		 
	struct	rtentry *rt_gwroute;	 
	struct	rtentry *rt_parent; 	 
};




 
struct ortentry {
	u_long	rt_hash;		 
	struct	sockaddr rt_dst;	 
	struct	sockaddr rt_gateway;	 
	short	rt_flags;		 
	short	rt_refcnt;		 
	u_long	rt_use;			 
	struct	ifnet *rt_ifp;		 
};


 

 
					 



 
struct	rtstat {
	short	rts_badredirect;	 
	short	rts_dynamic;		 
	short	rts_newgateway;		 
	short	rts_unreach;		 
	short	rts_wildcard;		 
};


 
struct rt_msghdr {
	u_short	rtm_msglen;	 
	u_char	rtm_version;	 
	u_char	rtm_type;	 
	u_short	rtm_index;	 
	int	rtm_flags;	 
	int	rtm_addrs;	 
	pid_t	rtm_pid;	 
	int	rtm_seq;	 
	int	rtm_errno;	 
	int	rtm_use;	 
	u_long	rtm_inits;	 
	struct	rt_metrics rtm_rmx;  
};




 



 



 



 

struct rt_addrinfo {
	int	rti_addrs;
	struct	sockaddr *rti_info[8];
};

struct route_cb {
	int	ip_count;
	int	ipx_count;
	int	ns_count;
	int	iso_count;
	int	any_count;
};


extern struct route_cb route_cb;
extern struct radix_node_head *rt_tables[29+1];

void	 route_init(void);
void	 rt_ifmsg(struct ifnet *);
void	 rt_missmsg(int, struct rt_addrinfo *, int, int);
void	 rt_newaddrmsg(int, struct ifaddr *, int, struct rtentry *);
int	 rt_setgate(struct rtentry *, struct sockaddr *, struct sockaddr *);
void	 rtalloc_ign(struct route *, unsigned long);
void	 rtalloc(struct route *ro);  
struct rtentry *
	 rtalloc1(struct sockaddr *, int, unsigned long);
void	 rtfree(struct rtentry *);
int	 rtinit(struct ifaddr *, int, int);
int	 rtioctl(int, caddr_t, struct proc *);
void	 rtredirect(struct sockaddr *, struct sockaddr *,
	    struct sockaddr *, int, struct sockaddr *, struct rtentry **);
int	 rtrequest(int, struct sockaddr *,
	    struct sockaddr *, struct sockaddr *, int, struct rtentry **);































 










 






 



extern volatile unsigned int	netisr;	 

































 
 



typedef uint32_t        	in_addr_t;

 


typedef	uint16_t		in_port_t;


 
struct in_addr {
	in_addr_t s_addr;
};

 
struct sockaddr_in {
	uint8_t	sin_len;
	sa_family_t	sin_family;
	in_port_t	sin_port;
	struct	in_addr sin_addr;
	char	sin_zero[8];
};





 



 
 
 
 




 

































 






 



 






 




 












 




 



 
struct ip_mreq {
	struct	in_addr imr_multiaddr;	 
	struct	in_addr imr_interface;	 
};




 






 




 




struct ifnet; struct mbuf;	 

int	 in_broadcast(struct in_addr, struct ifnet *);
int	 in_canforward(struct in_addr);
int	 in_cksum(struct mbuf *, int);
int	 in_localaddr(struct in_addr);
char 	*inet_ntoa(struct in_addr);  

 
struct ip;
typedef	int ip_fw_chk_t(struct ip**, int, struct ifnet*, int, struct mbuf**);
typedef	int ip_fw_ctl_t(int, struct mbuf**);
extern	ip_fw_chk_t *ip_fw_chk_ptr;
extern	ip_fw_ctl_t *ip_fw_ctl_ptr;

 
typedef	int ip_nat_t(struct ip**, struct mbuf**, struct ifnet*, int);
typedef	int ip_nat_ctl_t(int, struct mbuf**);
extern	ip_nat_t *ip_nat_ptr;
extern	ip_nat_ctl_t *ip_nat_ctl_ptr;
































 





 








 
typedef u_int16_t n_short;		 
typedef u_int32_t n_long;		 

typedef	u_int32_t n_time;		 

uint32_t	 iptime(void);































 









 
struct in_ifaddr {
	struct	ifaddr ia_ifa;		 
					 
	u_long	ia_net;			 
	u_long	ia_netmask;		 
	u_long	ia_subnet;		 
	u_long	ia_subnetmask;		 
	struct	in_addr ia_netbroadcast;  
	struct	in_ifaddr *ia_next;	 
	struct	sockaddr_in ia_addr;	 
	struct	sockaddr_in ia_dstaddr;  
	struct	sockaddr_in ia_sockmask;  
	struct in_multihead { struct in_multi *lh_first; } ia_multiaddrs;
					 
};

struct	in_aliasreq {
	char	ifra_name[16];		 
	struct	sockaddr_in ifra_addr;
	struct	sockaddr_in ifra_broadaddr;
	struct	sockaddr_in ifra_mask;
};



 



extern	struct	in_ifaddr *in_ifaddr;
extern	struct	ifqueue	ipintrq;		 
extern	struct	in_addr zeroin_addr;
extern	u_char	inetctlerrmap[];




 




 




 

struct router_info {
	struct ifnet *rti_ifp;
	int    rti_type;  
	int    rti_time;  
	struct router_info *rti_next;
};






 
struct in_multi {
	struct { struct in_multi *le_next; struct in_multi * *le_prev; } inm_entry;  
	struct	in_addr inm_addr;	 
	struct	ifnet *inm_ifp;		 
	struct	in_ifaddr *inm_ia;	 
	u_int	inm_refcount;		 
	u_int	inm_timer;		 
	u_int	inm_state;		 
	struct	router_info *inm_rti;	 
};


extern struct sysctl_oid_list sysctl__net_inet_ip_children;
extern struct sysctl_oid_list sysctl__net_inet_raw_children;




 
struct in_multistep {
	struct in_ifaddr *i_ia;
	struct in_multi *i_inm;
};




 







 


struct	in_multi *in_addmulti(struct in_addr *, struct ifnet *);
void	in_delmulti(struct in_multi *);
int	in_control(struct socket *, u_long, caddr_t, struct ifnet *);
void	in_rtqdrain(void);
void	ip_input(struct mbuf *);
































 







 


 



 
struct ip {
	u_char	ip_vhl;			 
	u_char	ip_tos;			 
	u_short	ip_len;			 
	u_short	ip_id;			 
	u_short	ip_off;			 
	u_char	ip_ttl;			 
	u_char	ip_p;			 
	u_short	ip_sum;			 
	struct	in_addr ip_src,ip_dst;	 
} __attribute__((packed)) __attribute__((aligned(4)));






 
 



 




 



 






 



 
struct	ip_timestamp {
	u_char	ipt_code;		 
	u_char	ipt_len;		 
	u_char	ipt_ptr;		 
	u_int	ipt_flg:4,		 
		ipt_oflw:4;		 
	union ipt_timestamp {
		n_long	ipt_time[1];
		struct	ipt_ta {
			struct in_addr ipt_addr;
			n_long ipt_time;
		} ipt_ta[1];
	} ipt_timestamp;
};
















































 
































 








 



 

u_int in_cksum_hdr(const struct ip *);



 

 



 






 
struct ippseudo {
	struct	in_addr	ippseudo_src;	 
	struct	in_addr	ippseudo_dst;	 
	u_char		ippseudo_pad;	 
	u_char		ippseudo_p;	 
	u_short		ippseudo_len;	 
};






























 










 
struct inpcbhead { struct inpcb *lh_first; };

typedef	u_int64_t inp_gen_t;

struct inpcb {
	struct { struct inpcb *le_next; struct inpcb * *le_prev; } inp_hash;  
	struct { struct inpcb *le_next; struct inpcb * *le_prev; } inp_list;  
	struct	inpcbinfo *inp_pcbinfo;	 
	struct	in_addr inp_faddr;	 
	struct	in_addr inp_laddr;	 
	u_short	inp_fport;		 
	u_short	inp_lport;		 
	caddr_t	inp_ppcb;		 
	struct	socket *inp_socket;	 
	struct	route inp_route;	 
	int	inp_flags;		 
	u_char	inp_vflag;		 
	u_char	inp_ip_ttl;		 
	u_char	inp_ip_p;		 
	u_char	inp_ip_minttl;		 

	 
	struct {
		u_char	inp4_ip_tos;		 
		struct	mbuf *inp4_options;	 
		struct	ip_moptions *inp4_moptions;  
	} inp_depend4;
	inp_gen_t	inp_gencnt;	 
};




 

struct inpcbinfo {		 
	struct	inpcbhead *listhead;
	struct	inpcbhead *hashbase;
	unsigned long hashmask;
	unsigned short lastport;
	unsigned short lastlow;
	unsigned short lasthi;
	u_int	ipi_count;	 
	u_int64_t ipi_gencnt;	 
};


 


void	in_losing(struct inpcb *);
int	in_pcballoc(struct socket *, struct inpcbinfo *);
int	in_pcbbind(struct inpcb *, struct mbuf *);
int	in_pcbconnect(struct inpcb *, struct mbuf *);
void	in_pcbdetach(struct inpcb *);
void	in_pcbdisconnect(struct inpcb *);
int	in_pcbladdr(struct inpcb *, struct mbuf *,
	    struct sockaddr_in **);
struct inpcb *
	in_pcblookup(struct inpcbinfo *,
	    struct in_addr, u_int, struct in_addr, u_int, int);
struct inpcb *
	in_pcblookuphash(struct inpcbinfo *,
	    struct in_addr, u_int, struct in_addr, u_int, int);
void	in_pcbnotify(struct inpcbhead *, struct sockaddr *,
	    u_int, struct in_addr, u_int, int, void (*)(struct inpcb *, int));
void	in_pcbrehash(struct inpcb *);
void	in_setpeeraddr(struct inpcb *, struct mbuf *);
void	in_setsockaddr(struct inpcb *, struct mbuf *);































 
 





 



 
struct ipovly {
	caddr_t	ih_next;
	caddr_t ih_prev;		 
	u_char	ih_x1;			 
	u_char	ih_pr;			 
	u_short	ih_len;			 
	struct	in_addr ih_src;		 
	struct	in_addr ih_dst;		 
} __attribute__((packed)) __attribute__((aligned(4)));






 
struct ipq {
	struct	ipq *next,*prev;	 
	u_char	ipq_ttl;		 
	u_char	ipq_p;			 
	u_short	ipq_id;			 
	struct	ipasfrag *ipq_next,*ipq_prev;
					 
	struct	in_addr ipq_src,ipq_dst;
};





 
struct	ipasfrag {
	u_char	ip_hl:4,
		ip_v:4;
	u_char	ipf_mff;		

 
	u_short	ip_len;
	u_short	ip_id;
	u_short	ip_off;
	u_char	ip_ttl;
	u_char	ip_p;
	u_short	ip_sum;
	struct	ipasfrag *ipf_next;	 
	struct	ipasfrag *ipf_prev;	 
};






 

struct ipoption {
	struct	in_addr ipopt_dst;	 
	char	ipopt_list[40];	 
};




 
struct ip_moptions {
	struct	ifnet *imo_multicast_ifp;  
	u_char	imo_multicast_ttl;	 
	u_char	imo_multicast_loop;	 
	u_short	imo_num_memberships;	 
	struct	in_multi *imo_membership[20];
	u_long	imo_multicast_vif;	 
};

struct	ipstat {
	u_long	ips_total;		 
	u_long	ips_badsum;		 
	u_long	ips_tooshort;		 
	u_long	ips_toosmall;		 
	u_long	ips_badhlen;		 
	u_long	ips_badlen;		 
	u_long	ips_fragments;		 
	u_long	ips_fragdropped;	 
	u_long	ips_fragtimeout;	 
	u_long	ips_forward;		 
	u_long	ips_cantforward;	 
	u_long	ips_redirectsent;	 
	u_long	ips_noproto;		 
	u_long	ips_delivered;		 
	u_long	ips_localout;		 
	u_long	ips_odropped;		 
	u_long	ips_reassembled;	 
	u_long	ips_fragmented;		 
	u_long	ips_ofragments;		 
	u_long	ips_cantfrag;		 
	u_long	ips_badoptions;		 
	u_long	ips_noroute;		 
	u_long	ips_badvers;		 
	u_long	ips_rawout;		 
	u_long	ips_toolong;		 
};

 

struct ip;
struct inpcb;
struct route;
struct sockopt;
struct mbuf;

extern struct	ipstat	ipstat;
extern u_short	ip_id;				 
extern int	ip_defttl;			 
extern u_char	ip_protox[];
extern struct socket *ip_rsvpd;	 
extern struct socket *ip_mrouter;  
extern int	(*legal_vif_num)(int);
extern u_long	(*ip_mcast_src)(int);
extern int rsvp_on;

int	 ip_ctloutput(int, struct socket *, int, int, struct mbuf **);
void	 ip_drain(void);
void	 ip_freemoptions(struct ip_moptions *);
void	 ip_init(void);
extern int	 (*ip_mforward)(struct ip *, struct ifnet *, struct mbuf *,
			  struct ip_moptions *);
int	 ip_output(struct mbuf *,
	    struct mbuf *, struct route *, int, struct ip_moptions *);
void	 ip_savecontrol(struct inpcb *, struct mbuf **, struct ip *,
		struct mbuf *);
void	 ip_slowtimo(void);
struct mbuf *
	 ip_srcroute(void);
void	 ip_stripoptions(struct mbuf *, struct mbuf *);
int	 rip_ctloutput(int, struct socket *, int, int, struct mbuf **);
void	 rip_init(void);
void	 rip_input(struct mbuf *, int);
int	 rip_output(struct mbuf *, struct socket *, u_long);
int	 rip_usrreq(struct socket *,
	    int, struct mbuf *, struct mbuf *, struct mbuf *);
void	ipip_input(struct mbuf *, int);
void	rsvp_input(struct mbuf *, int);
int	ip_rsvp_init(struct socket *);
int	ip_rsvp_done(void);
int	ip_rsvp_vif_init(struct socket *, struct mbuf *);
int	ip_rsvp_vif_done(struct socket *, struct mbuf *);
void	ip_rsvp_force_done(struct socket *);

































 







 



 
struct icmp_ra_addr {
	u_int32_t ira_addr;
	u_int32_t ira_preference;
};



 
struct icmp {
	u_char	icmp_type;		 
	u_char	icmp_code;		 
	u_short	icmp_cksum;		 
	union {
		u_char ih_pptr;			 
		struct in_addr ih_gwaddr;	 
		struct ih_idseq {
			uint16_t	icd_id;	 
			uint16_t	icd_seq;  
		} ih_idseq;
		int ih_void;

		 
		struct ih_pmtu {
			uint16_t ipm_void;	 
			uint16_t ipm_nextmtu;	 
		} ih_pmtu;

		struct ih_rtradv {
			u_char irt_num_addrs;
			u_char irt_wpa;
			u_int16_t irt_lifetime;
		} ih_rtradv;
	} icmp_hun;
	union {
		struct id_ts {			 
			


 
			uint32_t its_otime;	 
			uint32_t its_rtime;	 
			uint32_t its_ttime;	 
		} id_ts;
		struct id_ip  {
			struct ip idi_ip;
			 
		} id_ip;
		struct icmp_ra_addr id_radv;
		u_int32_t id_mask;
		char	id_data[1];
	} icmp_dun;
};








 
	 



 



void	icmp_error(struct mbuf *, int, int, n_long, struct ifnet *);
void	icmp_input(struct mbuf *, int);
int	ip_next_mtu(int, int);
































 



































 







 
struct selinfo {
	pid_t	si_pid;		 
	short	si_flags;	 
};





 
extern int select (int __nfds, xx_types_fd_set *__restrict __readfds,
                   xx_types_fd_set *__restrict __writefds,
                   xx_types_fd_set *__restrict __exceptfds,
                   struct timeval *__restrict __timeout);









 
typedef	u_quad_t so_gen_t;

struct socket {
	short	so_type;		 
	short	so_options;		 
	short	so_linger;		 
	short	so_state;		 
	void 	*so_pcb;		 
	struct	protosw *so_proto;	 










 
	struct	socket *so_head;	 
	struct  { struct socket *tqh_first; struct socket * *tqh_last;  } so_incomp;	 
	struct  { struct socket *tqh_first; struct socket * *tqh_last;  } so_comp;	 
	struct { struct socket *tqe_next; struct socket * *tqe_prev;  } so_list;	 
	short	so_qlen;		 
	short	so_incqlen;		
 
	short	so_qlimit;		 
	short	so_timeo;		 
	u_short	so_error;		 
	pid_t	so_pgid;		 
	u_long	so_oobmark;		 


 
	struct sockbuf {
		u_int	sb_cc;		 
		u_int	sb_hiwat;	 
		u_int	sb_mbcnt;	 
		u_int	sb_mbmax;	 
		int	sb_lowat;	 
		struct	mbuf *sb_mb;	 
		struct	selinfo sb_sel;	 
		short	sb_flags;	 
		int	sb_timeo;	 
		void	(*sb_wakeup)(struct socket *, void *);
		void 	*sb_wakeuparg;	 
	} so_rcv, so_snd;

	caddr_t	so_tpcb;		 
	void	(*so_upcall)(struct socket *, void *arg, int);
	void 	*so_upcallarg;		 
};



 






 






 

 

 

 

 

 





 

 



extern u_long	sb_max;

 

struct filedesc;
struct mbuf;
struct sockaddr;
struct stat;



 
int	sockargs(struct mbuf **mp, caddr_t buf, int buflen, int type);
void	sbappend(struct sockbuf *sb, struct mbuf *m);
int	sbappendaddr(struct sockbuf *sb, struct sockaddr *asa,
	    struct mbuf *m0, struct mbuf *control);
int	sbappendcontrol(struct sockbuf *sb, struct mbuf *m0,
	    struct mbuf *control);
void	sbappendrecord(struct sockbuf *sb, struct mbuf *m0);
void	sbcheck(struct sockbuf *sb);
void	sbcompress(struct sockbuf *sb, struct mbuf *m, struct mbuf *n);
struct mbuf *
	sbcreatecontrol(caddr_t p, int size, int type, int level);
void	sbdrop(struct sockbuf *sb, int len);
void	sbdroprecord(struct sockbuf *sb);
void	sbflush(struct sockbuf *sb);
void	sbinsertoob(struct sockbuf *sb, struct mbuf *m0);
void	sbrelease(struct sockbuf *sb);
int	sbreserve(struct sockbuf *sb, u_long cc);
int	sbwait(struct sockbuf *sb);
int	sb_lock(struct sockbuf *sb);
int	soabort(struct socket *so);
int	soaccept(struct socket *so, struct mbuf *nam);
int	sobind(struct socket *so, struct mbuf *nam);
void	socantrcvmore(struct socket *so);
void	socantsendmore(struct socket *so);
int	soclose(struct socket *so);
int	soconnect(struct socket *so, struct mbuf *nam);
int	soconnect2(struct socket *so1, struct socket *so2);
int	socreate(int dom, struct socket **aso, int type, int proto,
	    struct proc *p);
int	sodisconnect(struct socket *so);
void	sofree(struct socket *so);
int	sogetopt(struct socket *so, int level, int optname,
	    struct mbuf **mp);
void	sohasoutofband(struct socket *so);
void	soisconnected(struct socket *so);
void	soisconnecting(struct socket *so);
void	soisdisconnected(struct socket *so);
void	soisdisconnecting(struct socket *so);
int	solisten(struct socket *so, int backlog);
struct socket *
	sodropablereq(struct socket *head);
struct socket *
	sonewconn1(struct socket *head, int connstatus);
int	soreceive(struct socket *so, struct mbuf **paddr, struct uio *uio,
	    struct mbuf **mp0, struct mbuf **controlp, int *flagsp);
int	soreserve(struct socket *so, u_long sndcc, u_long rcvcc);
void	sorflush(struct socket *so);
int	sosend(struct socket *so, struct mbuf *addr, struct uio *uio,
	    struct mbuf *top, struct mbuf *control, int flags);
int	sosetopt(struct socket *so, int level, int optname,
	    struct mbuf *m0);
int	soshutdown(struct socket *so, int how);
void	sowakeup(struct socket *so, struct sockbuf *sb);



int rsvp_on = 0;
static int ip_rsvp_on;
struct socket *ip_rsvpd;

int	ipforwarding = 0;
static struct sysctl_oid sysctl___net_inet_ip_forwarding = { &sysctl__net_inet_ip_children, { 0 }, 1, 2|((0x80000000|0x40000000)), &ipforwarding, 0, "forwarding", sysctl_handle_int, "I", 0, "Enable IP forwarding between interfaces" }; static void const * const __set_sysctl_set_sym_sysctl___net_inet_ip_forwarding __attribute((section("set_" "sysctl_set"))) __attribute__((__used__)) = &sysctl___net_inet_ip_forwarding;

static int	ipsendredirects = 1;  
static struct sysctl_oid sysctl___net_inet_ip_redirect = { &sysctl__net_inet_ip_children, { 0 }, 2, 2|((0x80000000|0x40000000)), &ipsendredirects, 0, "redirect", sysctl_handle_int, "I", 0, "Enable sending IP redirects" }; static void const * const __set_sysctl_set_sym_sysctl___net_inet_ip_redirect __attribute((section("set_" "sysctl_set"))) __attribute__((__used__)) = &sysctl___net_inet_ip_redirect;

int	ip_defttl = 64;
static struct sysctl_oid sysctl___net_inet_ip_ttl = { &sysctl__net_inet_ip_children, { 0 }, 3, 2|((0x80000000|0x40000000)), &ip_defttl, 0, "ttl", sysctl_handle_int, "I", 0, "Maximum TTL on IP packets" }; static void const * const __set_sysctl_set_sym_sysctl___net_inet_ip_ttl __attribute((section("set_" "sysctl_set"))) __attribute__((__used__)) = &sysctl___net_inet_ip_ttl;

static int	ip_dosourceroute = 0;
static struct sysctl_oid sysctl___net_inet_ip_sourceroute = { &sysctl__net_inet_ip_children, { 0 }, 8, 2|((0x80000000|0x40000000)), &ip_dosourceroute, 0, "sourceroute", sysctl_handle_int, "I", 0, "" }; static void const * const __set_sysctl_set_sym_sysctl___net_inet_ip_sourceroute __attribute((section("set_" "sysctl_set"))) __attribute__((__used__)) = &sysctl___net_inet_ip_sourceroute;

static int	ip_acceptsourceroute = 0;
static struct sysctl_oid sysctl___net_inet_ip_accept_sourceroute = { &sysctl__net_inet_ip_children, { 0 }, 13, 2|((0x80000000|0x40000000)), &ip_acceptsourceroute, 0, "accept_sourceroute", sysctl_handle_int, "I", 0, "" }; static void const * const __set_sysctl_set_sym_sysctl___net_inet_ip_accept_sourceroute __attribute((section("set_" "sysctl_set"))) __attribute__((__used__)) = &sysctl___net_inet_ip_accept_sourceroute;

extern	struct domain inetdomain;
extern	struct protosw inetsw[];
u_char	ip_protox[256];
static int	ipqmaxlen = 50;
struct	in_ifaddr *in_ifaddr;			 
struct	ifqueue ipintrq;
static struct sysctl_oid sysctl___net_inet_ip_intr_queue_maxlen = { &sysctl__net_inet_ip_children, { 0 }, 10, 2|(0x80000000), &ipintrq . ifq_maxlen, 0, "intr_queue_maxlen", sysctl_handle_int, "I", 0, "" }; static void const * const __set_sysctl_set_sym_sysctl___net_inet_ip_intr_queue_maxlen __attribute((section("set_" "sysctl_set"))) __attribute__((__used__)) = &sysctl___net_inet_ip_intr_queue_maxlen;
static struct sysctl_oid sysctl___net_inet_ip_intr_queue_drops = { &sysctl__net_inet_ip_children, { 0 }, 11, 2|(0x80000000), &ipintrq . ifq_drops, 0, "intr_queue_drops", sysctl_handle_int, "I", 0, "Number of packets dropped from the IP input queue" }; static void const * const __set_sysctl_set_sym_sysctl___net_inet_ip_intr_queue_drops __attribute((section("set_" "sysctl_set"))) __attribute__((__used__)) = &sysctl___net_inet_ip_intr_queue_drops;

struct ipstat ipstat;

 

static struct ipq ipq[(1 << 6)];
static int    nipq = 0;          
static int    maxnipq;



 
ip_fw_chk_t *ip_fw_chk_ptr;
ip_fw_ctl_t *ip_fw_ctl_ptr;

  
ip_nat_t *ip_nat_ptr;
ip_nat_ctl_t *ip_nat_ctl_ptr;







 
static int	ip_nhops = 0;
static	struct ip_srcrt {
	struct	in_addr dst;			 
	char	nop;				 
	char	srcopt[2 + 1];	 
	struct	in_addr route[40/sizeof(struct in_addr)];
} ip_srcrt;


static void save_rte(u_char *, struct in_addr);
static void	 ip_deq(struct ipasfrag *);
static int	 ip_dooptions(struct mbuf *);
static void	 ip_enq(struct ipasfrag *, struct ipasfrag *);
static void	 ip_forward(struct mbuf *, int);
static void	 ip_freef(struct ipq *);
static struct ip *
	 ip_reass(struct ipasfrag *, struct ipq *, struct ipq *);
static struct in_ifaddr *
	 ip_rtaddr(struct in_addr);
void	ipintr(void);



 
void
ip_init(void)
{
	register struct protosw *pr;
	register int i;

	pr = pffindproto(2, 255, 3);
	if (pr == 0)
		rtems_panic("ip_init: PF_INET not found");

	 
	for (i = 0; i < 256; i++)
		ip_protox[i] = pr - inetsw;
	


 
	for (pr = inetdomain.dom_protosw;
	    pr < inetdomain.dom_protoswNPROTOSW; pr++)
		if (pr->pr_domain->dom_family == 2 &&
		    pr->pr_protocol && pr->pr_protocol != 255) {
			ip_protox[pr->pr_protocol] = pr - inetsw;
		}

	for (i = 0; i < (1 << 6); i++)
	    ipq[i].next = ipq[i].prev = &ipq[i];

	maxnipq = nmbclusters/4;

	 
	ip_id = rtems_bsdnet_seconds_since_boot() & 0xffff;
	ipintrq.ifq_maxlen = ipqmaxlen;

}

static struct	sockaddr_in ipaddr = { sizeof(ipaddr), 2, 0, {0}, {0} };
static struct	route ipforward_rt;




 
void
ip_input(struct mbuf *m)
{
	struct ip *ip = 0;
	struct ipq *fp;
	struct in_ifaddr *ia = 0;
	int    i, hlen;
	u_short sum;

	


 
	if (in_ifaddr == 0)
		goto bad;
	ipstat.ips_total++;

	if (m->M_dat . MH . MH_pkthdr .len < sizeof(struct ip))
		goto tooshort;


	if (m->m_hdr . mh_len < sizeof (struct ip) &&
	    (m = m_pullup(m, sizeof (struct ip))) == 0) {
		ipstat.ips_toosmall++;
		return;
	}
	ip = ((struct ip *)((m)->m_hdr . mh_data));

	if (((ip->ip_vhl) >> 4) != 4) {
		ipstat.ips_badvers++;
		goto bad;
	}

	hlen = ((ip->ip_vhl) & 0x0f) << 2;
	if (hlen < sizeof(struct ip)) {	 
		ipstat.ips_badhlen++;
		goto bad;
	}
	if (hlen > m->m_hdr . mh_len) {
		if ((m = m_pullup(m, hlen)) == 0) {
			ipstat.ips_badhlen++;
			return;
		}
		ip = ((struct ip *)((m)->m_hdr . mh_data));
	}
	if (hlen == sizeof(struct ip)) {
		sum = in_cksum_hdr(ip);
	} else {
		sum = in_cksum(m, hlen);
	}
	if (sum) {
		ipstat.ips_badsum++;
		goto bad;
	}

	

 
	ip->ip_len = __ntohs(ip->ip_len);
	if (ip->ip_len < hlen) {
		ipstat.ips_badlen++;
		goto bad;
	}
	(ip->ip_id) = __ntohs(ip->ip_id);
	ip->ip_off = __ntohs(ip->ip_off);
	printf("ip header \n"); dbgprintf (ip , 20);
	




 
	if (m->M_dat . MH . MH_pkthdr .len < ip->ip_len) {
tooshort:
		ipstat.ips_tooshort++;
		goto bad;
	}
	if (m->M_dat . MH . MH_pkthdr .len > ip->ip_len) {
		if (m->m_hdr . mh_len == m->M_dat . MH . MH_pkthdr .len) {
			m->m_hdr . mh_len = ip->ip_len;
			m->M_dat . MH . MH_pkthdr .len = ip->ip_len;
		} else
			m_adj(m, ip->ip_len - m->M_dat . MH . MH_pkthdr .len);
	}
	








 

	if (ip_fw_chk_ptr) {
		 
		if ((*ip_fw_chk_ptr)(&ip, hlen, 0, 0, &m)) {
			m_freem(m);
			m = 0;
		}
		if (!m)
			return;
	}

        if (ip_nat_ptr && !(*ip_nat_ptr)(&ip, &m, m->M_dat . MH . MH_pkthdr .rcvif, 0x00000001))
		return;

	




 
	ip_nhops = 0;		 
	if (hlen > sizeof (struct ip) && ip_dooptions(m))
		return;

        




 
	if (rsvp_on && ip->ip_p==46) 
		goto ours;

	

 
	for (ia = in_ifaddr; ia; ia = ia->ia_next) {

struct in_addr _in, _in2;
_in.s_addr = (&(((struct in_ifaddr *)(ia))->ia_addr))->sin_addr.s_addr;
printf("our addr: %s\t", inet_ntoa(_in));
_in2.s_addr = ip->ip_dst.s_addr;
printf("dst addr: %s \n", inet_ntoa(_in2));

		if ((&(((struct in_ifaddr *)(ia))->ia_addr))->sin_addr.s_addr == ip->ip_dst.s_addr)
			goto ours;
		if ((&(((struct in_ifaddr *)(ia))->ia_addr))->sin_addr.s_addr == (u_int32_t)0x00000000)
			goto ours;
		if (ia->ia_ifa . ifa_ifp && ia->ia_ifa . ifa_ifp->if_flags & 0x2) {
			if (((struct sockaddr_in *)(&ia->ia_dstaddr))->sin_addr.s_addr ==
			    ip->ip_dst.s_addr)
				goto ours;
			if (ip->ip_dst.s_addr == ia->ia_netbroadcast.s_addr)
				goto ours;
		}
	}
	if ((((u_int32_t)(__ntohl(ip->ip_dst . s_addr)) & 0xf0000000) == 0xe0000000)) {
		struct in_multi *inm;
		if (ip_mrouter) {
			










 
			ip->ip_id = __htons(ip->ip_id);
			if (ip_mforward(ip, m->M_dat . MH . MH_pkthdr .rcvif, m, 0) != 0) {
				ipstat.ips_cantforward++;
				m_freem(m);
				return;
			}
			ip->ip_id = __ntohs(ip->ip_id);

			



 
			if (ip->ip_p == 2)
				goto ours;
			ipstat.ips_forward++;
		}
		


 
		{ register struct in_ifaddr *ia; { for ((ia) = in_ifaddr; (ia) != 0 && (ia)->ia_ifa . ifa_ifp != ((m->M_dat . MH . MH_pkthdr . rcvif)); (ia) = (ia)->ia_next) continue; }; if (ia == 0) (inm) = 0; else for ((inm) = ia->ia_multiaddrs . lh_first; (inm) != 0 && (inm)->inm_addr . s_addr != (ip->ip_dst). s_addr; (inm) = inm->inm_entry . le_next) continue; };
		if (inm == 0) {
			ipstat.ips_cantforward++;
			m_freem(m);
			return;
		}
		goto ours;
	}
	if (ip->ip_dst.s_addr == (u_long)(u_int32_t)0xffffffff)
		goto ours;
	if (ip->ip_dst.s_addr == (u_int32_t)0x00000000)
		goto ours;

	

 
	if (ipforwarding == 0) {
		ipstat.ips_cantforward++;
		m_freem(m);
	} else {
		ip_forward(m, 0);
	}
	return;

ours:

	





 
	if (ip->ip_off &~ (0x4000 | 0x8000)) {
		if (m->m_hdr . mh_flags & 0x0001) {		 
			if ((m = m_pullup(m, sizeof (struct ip))) == 0) {
				ipstat.ips_toosmall++;
				return;
			}
			ip = ((struct ip *)((m)->m_hdr . mh_data));
		}
		sum = (((((ip->ip_src . s_addr) & 0xF) | ((((ip->ip_src . s_addr) >> 8) & 0xF) << 4)) ^ (ip->ip_id)) & ((1 << 6) - 1));
		


 
		for (fp = ipq[sum].next; fp != &ipq[sum]; fp = fp->next)
			if (ip->ip_id == fp->ipq_id &&
			    ip->ip_src.s_addr == fp->ipq_src.s_addr &&
			    ip->ip_dst.s_addr == fp->ipq_dst.s_addr &&
			    ip->ip_p == fp->ipq_p)
				goto found;

		fp = 0;

		 
		if (nipq > maxnipq) {
		    


 
		    if (ipq[sum].prev == &ipq[sum]) {    
			for (i = 0; i < (1 << 6); i++) {
			    if (ipq[i].prev != &ipq[i]) {
				ip_freef(ipq[i].prev);
				break;
			    }
			}
		    } else
			ip_freef(ipq[sum].prev);
		}
found:
		



 
		ip->ip_len -= hlen;
		((struct ipasfrag *)ip)->ipf_mff &= ~1;
		if (ip->ip_off & 0x2000)
			((struct ipasfrag *)ip)->ipf_mff |= 1;
		ip->ip_off <<= 3;

		



 
		if (((struct ipasfrag *)ip)->ipf_mff & 1 || ip->ip_off) {
			ipstat.ips_fragments++;
			ip = ip_reass((struct ipasfrag *)ip, fp, &ipq[sum]);
			if (ip == 0)
				return;
			ipstat.ips_reassembled++;
			m = ((struct mbuf *)((intptr_t)(ip) & ~(128-1)));
		} else
			if (fp)
				ip_freef(fp);
	} else
		ip->ip_len -= hlen;


	

 
	ipstat.ips_delivered++;

	(*inetsw[ip_protox[ip->ip_p]].pr_input)(m, hlen);
	return;
bad:
	m_freem(m);
}



 
void
ipintr(void)
{
	int s;
	struct mbuf *m;

	while(1) {
		s = 0;
		do { (m) = (&ipintrq)->ifq_head; if (m) { if (((&ipintrq)->ifq_head = (m)->m_hdr . mh_nextpkt) == 0) (&ipintrq)->ifq_tail = 0; (m)->m_hdr . mh_nextpkt = 0; (&ipintrq)->ifq_len--; } } while (0);
		do { (s) = 0; (void) (s); } while(0);
		if (m == 0)
			return;
		ip_input(m);
	}
}

;
  





 
static struct ip *
ip_reass(struct ipasfrag *ip, struct ipq *fp, struct ipq *where)
{
	register struct mbuf *m = ((struct mbuf *)((intptr_t)(ip) & ~(128-1)));
	register struct ipasfrag *q;
	struct mbuf *t;
	int hlen = ip->ip_hl << 2;
	int i;
	int32_t next;

	


 
	m->m_hdr . mh_data += hlen;
	m->m_hdr . mh_len -= hlen;

	

 
	if (fp == 0) {
		if ((t = m_get(0x0001, 11)) == 0)
			goto dropfrag;
		fp = ((struct ipq *)((t)->m_hdr . mh_data));
		insque(fp, where);
		nipq++;
		fp->ipq_ttl = 60;
		fp->ipq_p = ip->ip_p;
		fp->ipq_id = ip->ip_id;
		fp->ipq_next = fp->ipq_prev = (struct ipasfrag *)fp;
		fp->ipq_src = ((struct ip *)ip)->ip_src;
		fp->ipq_dst = ((struct ip *)ip)->ip_dst;
		q = (struct ipasfrag *)fp;
		goto insert;
	}

	

 
	for (q = fp->ipq_next; q != (struct ipasfrag *)fp; q = q->ipf_next)
		if (q->ip_off > ip->ip_off)
			break;

	



 
	if (q->ipf_prev != (struct ipasfrag *)fp) {
		i = q->ipf_prev->ip_off + q->ipf_prev->ip_len - ip->ip_off;
		if (i > 0) {
			if (i >= ip->ip_len)
				goto dropfrag;
			m_adj(((struct mbuf *)((intptr_t)(ip) & ~(128-1))), i);
			ip->ip_off += i;
			ip->ip_len -= i;
		}
	}

	


 
	while (q != (struct ipasfrag *)fp && ip->ip_off + ip->ip_len > q->ip_off) {
		struct mbuf *m0;

		i = (ip->ip_off + ip->ip_len) - q->ip_off;
		if (i < q->ip_len) {
			q->ip_len -= i;
			q->ip_off += i;
			m_adj(((struct mbuf *)((intptr_t)(q) & ~(128-1))), i);
			break;
		}
		m0 = ((struct mbuf *)((intptr_t)(q) & ~(128-1)));
		q = q->ipf_next;
		ip_deq(q->ipf_prev);
		m_freem(m0);
	}

insert:


	


 
	ip_enq(ip, q->ipf_prev);
	next = 0;
	for (q = fp->ipq_next; q != (struct ipasfrag *)fp; q = q->ipf_next) {
		if (q->ip_off != next)
			return (0);
		next += q->ip_len;
	}
	if (q->ipf_prev->ipf_mff & 1)
		return (0);

	

 
	if (next + (((((struct ip *)fp->ipq_next)->ip_vhl) & 0x0f) << 2)
							> 65535L) {
		ipstat.ips_toolong++;
		ip_freef(fp);
		return (0);
	}

	

 
	q = fp->ipq_next;
	m = ((struct mbuf *)((intptr_t)(q) & ~(128-1)));
	t = m->m_hdr . mh_next;
	m->m_hdr . mh_next = 0;
	m_cat(m, t);
	q = q->ipf_next;
	while (q != (struct ipasfrag *)fp) {
		t = ((struct mbuf *)((intptr_t)(q) & ~(128-1)));
		q = q->ipf_next;
		m_cat(m, t);
	}


	



 
	ip = fp->ipq_next;
	ip->ip_len = next;
	ip->ipf_mff &= ~1;
	((struct ip *)ip)->ip_src = fp->ipq_src;
	((struct ip *)ip)->ip_dst = fp->ipq_dst;
	remque(fp);
	nipq--;
	(void) m_free(((struct mbuf *)((intptr_t)(fp) & ~(128-1))));
	m = ((struct mbuf *)((intptr_t)(ip) & ~(128-1)));
	m->m_hdr . mh_len += (ip->ip_hl << 2);
	m->m_hdr . mh_data -= (ip->ip_hl << 2);
	 
	if (m->m_hdr . mh_flags & 0x0002) {  
		register int plen = 0;
		for (t = m; m; m = m->m_hdr . mh_next)
			plen += m->m_hdr . mh_len;
		t->M_dat . MH . MH_pkthdr .len = plen;
	}
	return ((struct ip *)ip);

dropfrag:
	ipstat.ips_fragdropped++;
	m_freem(m);
	return (0);
}




 
static void
ip_freef(struct ipq *fp)
{
	register struct ipasfrag *q, *p;

	for (q = fp->ipq_next; q != (struct ipasfrag *)fp; q = p) {
		p = q->ipf_next;
		ip_deq(q);
		m_freem(((struct mbuf *)((intptr_t)(q) & ~(128-1))));
	}
	remque(fp);
	(void) m_free(((struct mbuf *)((intptr_t)(fp) & ~(128-1))));
	nipq--;
}




 
static void
ip_enq(struct ipasfrag *p, struct ipasfrag *prev)
{

	p->ipf_prev = prev;
	p->ipf_next = prev->ipf_next;
	prev->ipf_next->ipf_prev = p;
	prev->ipf_next = p;
}



 
static void
ip_deq(struct ipasfrag *p)
{

	p->ipf_prev->ipf_next = p->ipf_next;
	p->ipf_next->ipf_prev = p->ipf_prev;
}





 
void
ip_slowtimo(void)
{
	register struct ipq *fp;
	int s = 0;
	int i;

	for (i = 0; i < (1 << 6); i++) {
		fp = ipq[i].next;
		if (fp == 0)
			continue;
		while (fp != &ipq[i]) {
			--fp->ipq_ttl;
			fp = fp->next;
			if (fp->prev->ipq_ttl == 0) {
				ipstat.ips_fragtimeout++;
				ip_freef(fp->prev);
			}
		}
	}
	do { (s) = 0; (void) (s); } while(0);
}



 
void
ip_drain(void)
{
	int     i;

	for (i = 0; i < (1 << 6); i++) {
		while (ipq[i].next != &ipq[i]) {
			ipstat.ips_fragdropped++;
			ip_freef(ipq[i].next);
		}
	}
	in_rtqdrain();
}







 
static int
ip_dooptions(struct mbuf *m)
{
	register struct ip *ip = ((struct ip *)((m)->m_hdr . mh_data));
	register u_char *cp;
	register struct ip_timestamp *ipt;
	register struct in_ifaddr *ia;
	int opt, optlen, cnt, off, code, type = 12, forward = 0;
	struct in_addr *sin, dst;
	n_time ntime;

	dst = ip->ip_dst;
	cp = (u_char *)(ip + 1);
	cnt = (((ip->ip_vhl) & 0x0f) << 2) - sizeof (struct ip);
	for (; cnt > 0; cnt -= optlen, cp += optlen) {
		opt = cp[0];
		if (opt == 0)
			break;
		if (opt == 1)
			optlen = 1;
		else {
			optlen = cp[1];
			if (optlen <= 0 || optlen > cnt) {
				code = &cp[1] - (u_char *)ip;
				goto bad;
			}
		}
		switch (opt) {

		default:
			break;

		







 
		case 131:
		case 137:
			if ((off = cp[2]) < 4) {
				code = &cp[2] - (u_char *)ip;
				goto bad;
			}
			ipaddr.sin_addr = ip->ip_dst;
			ia = (struct in_ifaddr *)
				ifa_ifwithaddr((struct sockaddr *)&ipaddr);
			if (ia == 0) {
				if (opt == 137) {
					type = 3;
					code = 5;
					goto bad;
				}
				if (!ip_dosourceroute)
					goto nosourcerouting;
				


 
				break;
			}
			off--;			 
			if (off > optlen - sizeof(struct in_addr)) {
				

 
				if (!ip_acceptsourceroute)
					goto nosourcerouting;
				save_rte(cp, ip->ip_src);
				break;
			}

			if (!ip_dosourceroute) {
				char buf[4*sizeof "123"];

nosourcerouting:
				strcpy(buf, inet_ntoa(ip->ip_dst));
				rtems_bsdnet_log(4, 
				    "attempted source route from %s to %s\n",
				    inet_ntoa(ip->ip_src), buf);
				type = 3;
				code = 5;
				goto bad;
			}

			

 
			(void)memcpy(&ipaddr.sin_addr, cp + off,
			    sizeof(ipaddr.sin_addr));

			if (opt == 137) {
			    if ((ia = (struct in_ifaddr *)ifa_ifwithdstaddr((struct sockaddr *)&ipaddr)) == 0)
				ia = (struct in_ifaddr *)ifa_ifwithnet((struct sockaddr *)&ipaddr);
			} else
				ia = ip_rtaddr(ipaddr.sin_addr);
			if (ia == 0) {
				type = 3;
				code = 5;
				goto bad;
			}
			ip->ip_dst = ipaddr.sin_addr;
			(void)memcpy(cp + off, &((&(((struct in_ifaddr *)(ia))->ia_addr))->sin_addr),
			    sizeof(struct in_addr));
			cp[2] += sizeof(struct in_addr);
			

 
			forward = !(((u_int32_t)(__ntohl(ip->ip_dst . s_addr)) & 0xf0000000) == 0xe0000000);
			break;

		case 7:
			if ((off = cp[2]) < 4) {
				code = &cp[2] - (u_char *)ip;
				goto bad;
			}
			

 
			off--;			 
			if (off > optlen - sizeof(struct in_addr))
				break;
			(void)memcpy(&ipaddr.sin_addr, &ip->ip_dst,
			    sizeof(ipaddr.sin_addr));
			


 
			if ((ia = (struct in_ifaddr *)ifa_ifwithaddr((struct sockaddr *)&ipaddr)) == 0 &&
			    (ia = ip_rtaddr(ipaddr.sin_addr)) == 0) {
				type = 3;
				code = 1;
				goto bad;
			}
			(void)memcpy(cp + off, &((&(((struct in_ifaddr *)(ia))->ia_addr))->sin_addr),
			    sizeof(struct in_addr));
			cp[2] += sizeof(struct in_addr);
			break;

		case 68:
			code = cp - (u_char *)ip;
			ipt = (struct ip_timestamp *)cp;
			if (ipt->ipt_len < 5)
				goto bad;
			if (ipt->ipt_ptr > ipt->ipt_len - sizeof (long)) {
				if (++ipt->ipt_oflw == 0)
					goto bad;
				break;
			}
			sin = (struct in_addr *)(cp + ipt->ipt_ptr - 1);
			switch (ipt->ipt_flg) {

			case 0:
				break;

			case 1:
				if (ipt->ipt_ptr + sizeof(n_time) +
				    sizeof(struct in_addr) > ipt->ipt_len)
					goto bad;
				ipaddr.sin_addr = dst;
				ia = (struct in_ifaddr *)ifaof_ifpforaddr((struct sockaddr *)&ipaddr,
							    m->M_dat . MH . MH_pkthdr .rcvif);
				if (ia == 0)
					continue;
				(void)memcpy(sin, &(&(((struct in_ifaddr *)(ia))->ia_addr))->sin_addr,
				    sizeof(struct in_addr));
				ipt->ipt_ptr += sizeof(struct in_addr);
				break;

			case 3:
				if (ipt->ipt_ptr + sizeof(n_time) +
				    sizeof(struct in_addr) > ipt->ipt_len)
					goto bad;
				(void)memcpy(&ipaddr.sin_addr, sin,
				    sizeof(struct in_addr));
				if (ifa_ifwithaddr((struct sockaddr *)&ipaddr) == 0)
					continue;
				ipt->ipt_ptr += sizeof(struct in_addr);
				break;

			default:
				goto bad;
			}
			ntime = iptime();
			(void)memcpy(cp + ipt->ipt_ptr - 1, &ntime,
			    sizeof(n_time));
			ipt->ipt_ptr += sizeof(n_time);
		}
	}
	if (forward && ipforwarding) {
		ip_forward(m, 1);
		return (1);
	}
	return (0);
bad:
	ip->ip_len -= ((ip->ip_vhl) & 0x0f) << 2;    
	icmp_error(m, type, code, 0, 0);
	ipstat.ips_badoptions++;
	return (1);
}




 
static struct in_ifaddr *
ip_rtaddr(struct in_addr dst)
{
	struct sockaddr_in *sin;

	sin = (struct sockaddr_in *) &ipforward_rt.ro_dst;

	if (ipforward_rt.ro_rt == 0 || dst.s_addr != sin->sin_addr.s_addr) {
		if (ipforward_rt.ro_rt) {
			do { if ((ipforward_rt . ro_rt)->rt_refcnt <= 1) rtfree(ipforward_rt . ro_rt); else (ipforward_rt . ro_rt)->rt_refcnt--; } while (0);
			ipforward_rt.ro_rt = 0;
		}
		sin->sin_family = 2;
		sin->sin_len = sizeof(*sin);
		sin->sin_addr = dst;

		rtalloc_ign(&ipforward_rt, 0x10000);
	}
	if (ipforward_rt.ro_rt == 0)
		return ((struct in_ifaddr *)0);
	return ((struct in_ifaddr *) ipforward_rt.ro_rt->rt_ifa);
}




 
void
save_rte(u_char *option, struct in_addr dst)
{
	unsigned olen;

	olen = option[1];
	if (olen > sizeof(ip_srcrt) - (1 + sizeof(dst)))
		return;
	memcpy((ip_srcrt . srcopt),(option),(olen));
	ip_nhops = (olen - 2 - 1) / sizeof(struct in_addr);
	ip_srcrt.dst = dst;
}





 
struct mbuf *
ip_srcroute(void)
{
	register struct in_addr *p, *q;
	register struct mbuf *m;

	if (ip_nhops == 0)
		return ((struct mbuf *)0);
	m = m_get(0x0001, 10);
	if (m == 0)
		return ((struct mbuf *)0);


	 
	m->m_hdr . mh_len = ip_nhops * sizeof(struct in_addr) + sizeof(struct in_addr) +
	    (sizeof(ip_srcrt . nop) + sizeof(ip_srcrt . srcopt));

	

 
	p = &ip_srcrt.route[ip_nhops - 1];
	*(((struct in_addr *)((m)->m_hdr . mh_data))) = *p--;

	

 
	ip_srcrt.nop = 1;
	ip_srcrt.srcopt[2] = 4;
	(void)memcpy(((caddr_t)((m)->m_hdr . mh_data)) + sizeof(struct in_addr),
	    &ip_srcrt.nop, (sizeof(ip_srcrt . nop) + sizeof(ip_srcrt . srcopt)));
	q = (struct in_addr *)(((caddr_t)((m)->m_hdr . mh_data)) +
	    sizeof(struct in_addr) + (sizeof(ip_srcrt . nop) + sizeof(ip_srcrt . srcopt)));
	


 
	while (p >= ip_srcrt.route) {
		*q++ = *p--;
	}
	

 
	*q = ip_srcrt.dst;
	return (m);
}







 
void
ip_stripoptions(struct mbuf *m, struct mbuf *mopt)
{
	register int i;
	struct ip *ip = ((struct ip *)((m)->m_hdr . mh_data));
	register caddr_t opts;
	int olen;

	olen = (((ip->ip_vhl) & 0x0f) << 2) - sizeof (struct ip);
	opts = (caddr_t)(ip + 1);
	i = m->m_hdr . mh_len - (sizeof (struct ip) + olen);
	memcpy((opts),(opts + olen),((unsigned)i));
	m->m_hdr . mh_len -= olen;
	if (m->m_hdr . mh_flags & 0x0002)
		m->M_dat . MH . MH_pkthdr .len -= olen;
	ip->ip_vhl = ((4) << 4 | (sizeof(struct ip) >> 2));
}

u_char inetctlerrmap[22] = {
	0,		0,		0,		0,
	0,		122,	117,	118,
	118,	118,	111,	111,
	122,	118,	0,		0,
	0,		0,		0,		0,
	109
};














 
static void
ip_forward(struct mbuf *m, int srcrt)
{
	struct ip *ip = ((struct ip *)((m)->m_hdr . mh_data));
	register struct sockaddr_in *sin;
	register struct rtentry *rt;
	int error, type = 0, code = 0;
	struct mbuf *mcopy;
	n_long dest;
	struct ifnet *destifp;

	dest = 0;


	if (m->m_hdr . mh_flags & 0x0100 || in_canforward(ip->ip_dst) == 0) {
		ipstat.ips_cantforward++;
		m_freem(m);
		return;
	}
	(ip->ip_id) = __htons(ip->ip_id);
	if (ip->ip_ttl <= 1) {
		icmp_error(m, 11, 0, dest, 0);
		return;
	}
	ip->ip_ttl -= 1;

	sin = (struct sockaddr_in *)&ipforward_rt.ro_dst;
	if ((rt = ipforward_rt.ro_rt) == 0 ||
	    ip->ip_dst.s_addr != sin->sin_addr.s_addr) {
		if (ipforward_rt.ro_rt) {
			do { if ((ipforward_rt . ro_rt)->rt_refcnt <= 1) rtfree(ipforward_rt . ro_rt); else (ipforward_rt . ro_rt)->rt_refcnt--; } while (0);
			ipforward_rt.ro_rt = 0;
		}
		sin->sin_family = 2;
		sin->sin_len = sizeof(*sin);
		sin->sin_addr = ip->ip_dst;

		rtalloc_ign(&ipforward_rt, 0x10000);
		if (ipforward_rt.ro_rt == 0) {
			icmp_error(m, 3, 1, dest, 0);
			return;
		}
		rt = ipforward_rt.ro_rt;
	}

	


 
	mcopy = m_copym((m), (0), (imin((int)ip->ip_len, 64)), 0x0001);

	






 
	if (rt->rt_ifp == m->M_dat . MH . MH_pkthdr .rcvif &&
	    (rt->rt_flags & (0x10|0x20)) == 0 &&
	    ((struct sockaddr_in *)(((struct sockaddr *)((rt)->rt_nodes->rn_u . rn_leaf . rn_Key))))->sin_addr.s_addr != 0 &&
	    ipsendredirects && !srcrt) {
		u_long src = __ntohl(ip->ip_src . s_addr);

		if (((struct in_ifaddr *)(rt->rt_ifa)) &&
		    (src & ((struct in_ifaddr *)(rt->rt_ifa))->ia_subnetmask) == ((struct in_ifaddr *)(rt->rt_ifa))->ia_subnet) {
		    if (rt->rt_flags & 0x2)
			dest = ((struct sockaddr_in *)(rt->rt_gateway))->sin_addr.s_addr;
		    else
			dest = ip->ip_dst.s_addr;
		     
		    type = 5;
		    code = 1;
		}
	}

	error = ip_output(m, (struct mbuf *)0, &ipforward_rt, 
			  0x1, 0);
	if (error)
		ipstat.ips_cantforward++;
	else {
		ipstat.ips_forward++;
		if (type)
			ipstat.ips_redirectsent++;
		else {
			if (mcopy)
				m_freem(mcopy);
			return;
		}
	}
	if (mcopy == 0)
		return;
	destifp = 0;

	switch (error) {

	case 0:				 
		 
		break;

	case 114:		 
	case 118:
	case 115:
	case 117:
	default:
		type = 3;
		code = 1;
		break;

	case 122:
		type = 3;
		code = 4;
		if (ipforward_rt.ro_rt)
			destifp = ipforward_rt.ro_rt->rt_ifp;
		ipstat.ips_cantfrag++;
		break;

	case 105:
		type = 4;
		code = 0;
		break;
	}
	icmp_error(mcopy, type, code, dest, destifp);
}

void
ip_savecontrol(struct inpcb *inp, struct mbuf **mp, struct ip *ip,
	struct mbuf *m)
{
	if (inp->inp_socket->so_options & 0x0400) {
		struct timeval tv;

		rtems_bsd_microtime(&tv);
		*mp = sbcreatecontrol((caddr_t) &tv, sizeof(tv),
			0x02, 0xffff);
		if (*mp)
			mp = &(*mp)->m_hdr . mh_next;
	}
	if (inp->inp_flags & 0x04) {
		*mp = sbcreatecontrol((caddr_t) &ip->ip_dst,
		    sizeof(struct in_addr), 7, 0);
		if (*mp)
			mp = &(*mp)->m_hdr . mh_next;
	}
	if (inp->inp_flags & 0x80) {
		struct sockaddr_dl sdl;

		sdl.sdl_len = ((size_t)__intaddr__(&(((struct sockaddr_dl *)0)->sdl_data[0])));
		sdl.sdl_family = 18;
		sdl.sdl_index = m->M_dat . MH . MH_pkthdr .rcvif ?
			m->M_dat . MH . MH_pkthdr .rcvif->if_index : 0;
		sdl.sdl_nlen = sdl.sdl_alen = sdl.sdl_slen = 0;
		*mp = sbcreatecontrol((caddr_t) &sdl, sdl.sdl_len,
			20, 0);
		if (*mp)
			mp = &(*mp)->m_hdr . mh_next;
	}
}

int
ip_rsvp_init(struct socket *so)
{
	if (so->so_type != 3 ||
	    so->so_proto->pr_protocol != 46)
		return 95;

	if (ip_rsvpd != 0)
		return 112;

	ip_rsvpd = so;
	


 
	if (!ip_rsvp_on) {
		ip_rsvp_on = 1;
		rsvp_on++;
	}

	return 0;
}

int
ip_rsvp_done(void)
{
	ip_rsvpd = 0;
	


 
	if (ip_rsvp_on) {
		ip_rsvp_on = 0;
		rsvp_on--;
	}
	return 0;
}
