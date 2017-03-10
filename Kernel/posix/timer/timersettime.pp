




 










 


 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 


 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 


#pragma diag_push
#pragma CHECK_MISRA("-19.4")  

 
 
 

 
 
 
 
 
 

 
 
 

 
 
 
 

#pragma diag_pop
































 







 


 





 
 






























 


 

 






































 





 





















 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 

 
 
 






















 








 







 





 




 




 











 









 












 








 








 





 




 


 




































 






typedef signed char __int8_t ;
typedef unsigned char __uint8_t ;

typedef signed short __int16_t;
typedef unsigned short __uint16_t;

typedef __int16_t __int_least16_t;
typedef __uint16_t __uint_least16_t;


typedef signed long __int32_t;
typedef unsigned long __uint32_t;

typedef __int32_t __int_least32_t;
typedef __uint32_t __uint_least32_t;


typedef signed long long __int64_t;
typedef unsigned long long __uint64_t;







typedef unsigned int clock_t;

typedef unsigned int time_t;

typedef unsigned int  clockid_t;

typedef unsigned int timer_t;

typedef unsigned size_t;





 
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



 


 



 


 








 


 
 


 
 

 

 

 
 

 
 

 
 

 

 
 

 
 


 
 


 

 


 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 



 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 



















































 



















 































 

 










 









 

 








 



 

 



 


 


 





 



 

typedef int _LOCK_T;
typedef int _LOCK_RECURSIVE_T;



typedef long _off_t;
  typedef long long _off64_t;

typedef int _ssize_t;

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 


#pragma diag_push
#pragma CHECK_MISRA("-19.7")  
#pragma CHECK_MISRA("-20.1")  
#pragma CHECK_MISRA("-20.2")  



typedef int ptrdiff_t;



typedef unsigned short wchar_t;


#pragma diag_push
#pragma CHECK_MISRA("-19.10")  



#pragma diag_pop



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


 

 








 








 










 








 








 










 






 
 



 

 
 

 
 

 

 

 

 
 

 


 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 



#pragma diag_push
#pragma CHECK_MISRA("-19.15")  

 
 
 
 
 
 
 

#pragma diag_pop

  



 

  



 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 



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
   
  POSIX_ABSOLUTE_TIMEOUT_INVALID,
   
  POSIX_ABSOLUTE_TIMEOUT_IS_IN_PAST,
   
  POSIX_ABSOLUTE_TIMEOUT_IS_NOW,
   
  POSIX_ABSOLUTE_TIMEOUT_IS_IN_FUTURE,
} POSIX_Absolute_timeout_conversion_results_t;















 
POSIX_Absolute_timeout_conversion_results_t _POSIX_Absolute_timeout_to_ticks(
  const struct timespec *abstime,
  Watchdog_Interval     *ticks_out
);

 








 












 






 
 








 








 











 
 




 
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

 

 





 
void _POSIX_Timer_Manager_initialization(void);



 
int timer_create(
  clockid_t        clock_id,
  struct sigevent *evp,
  timer_t         *timerid
);



 
int timer_delete(
  timer_t timerid
);



 
int timer_settime(
  timer_t                  timerid,
  int                      flags,
  const struct itimerspec *value,
  struct itimerspec       *ovalue
);



 
int timer_gettime(
  timer_t            timerid,
  struct itimerspec *value
);







 
int timer_getoverrun(
  timer_t   timerid
);

 








 








 









 








 





















 









 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 









 

 
typedef __uint8_t	u_int8_t;
typedef __uint16_t	u_int16_t;
typedef __uint32_t	u_int32_t;

typedef __uint64_t	u_int64_t;

 
typedef	__uint64_t	u_quad_t;
typedef	__int64_t	quad_t;
typedef	quad_t 	*qaddr_t;






 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 



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
char *  ctermid (char *__s );
char *  cuserid (char *__s );
int     dup (int __fildes );
int     dup2 (int __fildes, int __fildes2 );
int     execl (const char *__path, const char *, ... );
int     execle (const char *__path, const char *, ... );
int     execlp (const char *__file, const char *, ... );
int     execv (const char *__path, char * const __argv[] );
int     execve (const char *__path, char * const __argv[], char * const __envp[] );
int     execvp (const char *__file, char * const __argv[] );
int	faccessat (int __dirfd, const char *__path, int __mode, int __flags);
int     fchdir (int __fildes);
int     fchmod (int __fildes, mode_t __mode );
int     fchown (int __fildes, uid_t __owner, gid_t __group );
int	fchownat (int __dirfd, const char *__path, uid_t __owner, gid_t __group, int __flags);
pid_t   fork (void );
long    fpathconf (int __fd, int __name );
int     fsync (int __fd);
int     fdatasync (int __fd);
char *  getcwd (char *__buf, size_t __size );
gid_t   getegid (void );
uid_t   geteuid (void );
gid_t   getgid (void );
int     getgroups (int __gidsetsize, gid_t __grouplist[] );
char *  getlogin (void );
int getlogin_r (char *name, size_t namesize);
char *  getpass (const char *__prompt);
int	getpagesize (void);
pid_t   getpgid (pid_t);
pid_t   getpgrp (void );
pid_t   getpid (void );
pid_t   getppid (void );
pid_t   getsid (pid_t);
uid_t   getuid (void );
int     isatty (int __fildes );
int        issetugid (void);
int     lchown (const char *__path, uid_t __owner, gid_t __group );
int     link (const char *__path1, const char *__path2 );
int     linkat (int __dirfd1, const char *__path1, int __dirfd2, const char *__path2, int __flags );
int	nice (int __nice_value );
off_t   lseek (int __fildes, off_t __offset, int __whence );
long    pathconf (const char *__path, int __name );
int     pause (void );
int     pipe (int __fildes[2] );
ssize_t pread (int __fd, void *__buf, size_t __nbytes, off_t __offset);
ssize_t pwrite (int __fd, const void *__buf, size_t __nbytes, off_t __offset);
_ssize_t read (int __fd, void *__buf, size_t __nbyte );
int     rmdir (const char *__path );
void *  sbrk (ptrdiff_t __incr);
int     setegid (gid_t __gid );
int     seteuid (uid_t __uid );
int     setgid (gid_t __gid );
int	sethostname (const char *, size_t);
int     setpgid (pid_t __pid, pid_t __pgid );
int     setpgrp (void );
pid_t   setsid (void );
int     setuid (uid_t __uid );
unsigned sleep (unsigned int __seconds );
void    swab (const void *__restrict, void *__restrict, ssize_t);
long    sysconf (int __name );
pid_t   tcgetpgrp (int __fildes );
int     tcsetpgrp (int __fildes, pid_t __pgrp_id );
char *  ttyname (int __fildes );
int     ttyname_r (int, char *, size_t);
int     unlink (const char *__path );
int 	usleep (useconds_t __useconds);
int     vhangup (void );
_ssize_t write (int __fd, const void *__buf, size_t __nbyte );

extern char *optarg;			 
extern int optind, opterr, optopt;
int	 getopt(int, char * const [], const char *);
extern int optreset;			 

pid_t   vfork (void );


int     ftruncate (int __fd, off_t __length);
int     truncate (const char *, off_t __length);

int	getdtablesize (void);
int	setdtablesize (int);
useconds_t ualarm (useconds_t __useconds, useconds_t __interval);
 
 int	gethostname (char *__name, size_t __len);
char *	mktemp (char *);

void    sync (void);

ssize_t readlink (const char *__restrict __path, char *__restrict __buf, size_t __buflen);
ssize_t        readlinkat (int __dirfd1, const char *__restrict __path, char *__restrict __buf, size_t __buflen);
int     symlink (const char *__name1, const char *__name2);
int	symlinkat (const char *, int, const char *);
int	unlinkat (int, const char *, int);







 


 
 
 



 




 



 


 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 


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


















 




 





 




 



 








 



 




 





 











 



 

int	pthread_mutexattr_init (pthread_mutexattr_t *__attr);
int	pthread_mutexattr_destroy (pthread_mutexattr_t *__attr);
int	pthread_mutexattr_getpshared (const pthread_mutexattr_t *__attr, int *__pshared);
int	pthread_mutexattr_setpshared (pthread_mutexattr_t *__attr, int __pshared);

 

int	pthread_mutex_init (pthread_mutex_t *__mutex, const pthread_mutexattr_t *__attr);
int	pthread_mutex_destroy (pthread_mutex_t *__mutex);




 



 

int	pthread_mutex_lock (pthread_mutex_t *__mutex);
int	pthread_mutex_trylock (pthread_mutex_t *__mutex);
int	pthread_mutex_unlock (pthread_mutex_t *__mutex);


int	pthread_mutex_timedlock (pthread_mutex_t *__mutex, const struct timespec *__timeout);


 

int	pthread_condattr_init (pthread_condattr_t *__attr);
int	pthread_condattr_destroy (pthread_condattr_t *__attr);
int	pthread_condattr_getpshared (const pthread_condattr_t *__attr, int *__pshared);
int	pthread_condattr_setpshared (pthread_condattr_t *__attr, int __pshared);

 

int	pthread_cond_init (pthread_cond_t *__cond, const pthread_condattr_t *__attr);
int	pthread_cond_destroy (pthread_cond_t *__mutex);




 


 

int	pthread_cond_signal (pthread_cond_t *__cond);
int	pthread_cond_broadcast (pthread_cond_t *__cond);

 

int	pthread_cond_wait (pthread_cond_t *__cond, pthread_mutex_t *__mutex);

int	pthread_cond_timedwait (pthread_cond_t *__cond, pthread_mutex_t *__mutex, const struct timespec *__abstime);


 

int	pthread_attr_setscope (pthread_attr_t *__attr, int __contentionscope);
int	pthread_attr_getscope (const pthread_attr_t *__attr, int *__contentionscope);
int	pthread_attr_setinheritsched (pthread_attr_t *__attr, int __inheritsched);
int	pthread_attr_getinheritsched (const pthread_attr_t *__attr, int *__inheritsched);
int	pthread_attr_setschedpolicy (pthread_attr_t *__attr, int __policy);
int	pthread_attr_getschedpolicy (const pthread_attr_t *__attr, int *__policy);


int	pthread_attr_setschedparam (pthread_attr_t *__attr, const struct sched_param *__param);
int	pthread_attr_getschedparam (const pthread_attr_t *__attr, struct sched_param *__param);


 

int	pthread_getschedparam (pthread_t __pthread, int *__policy, struct sched_param *__param);
int	pthread_setschedparam (pthread_t __pthread, int __policy, struct sched_param *__param);



 

int	pthread_mutexattr_setprotocol (pthread_mutexattr_t *__attr, int __protocol);
int	pthread_mutexattr_getprotocol (const pthread_mutexattr_t *__attr, int *__protocol);
int	pthread_mutexattr_setprioceiling (pthread_mutexattr_t *__attr, int __prioceiling);
int	pthread_mutexattr_getprioceiling (const pthread_mutexattr_t *__attr, int *__prioceiling);



 

int	pthread_mutex_setprioceiling (pthread_mutex_t *__mutex, int __prioceiling, int *__old_ceiling);
int	pthread_mutex_getprioceiling (pthread_mutex_t *__mutex, int *__prioceiling);


 

int	pthread_attr_init (pthread_attr_t *__attr);
int	pthread_attr_destroy (pthread_attr_t *__attr);
int	pthread_attr_getstacksize (const pthread_attr_t *__attr, size_t *__stacksize);
int	pthread_attr_setstacksize (pthread_attr_t *__attr, size_t stacksize);
int	pthread_attr_getstackaddr (const pthread_attr_t *__attr, void * *__stackaddr);
int	pthread_attr_setstackaddr (pthread_attr_t *__attr, void *__stackaddr);
int	pthread_attr_getdetachstate (const pthread_attr_t *__attr, int *__detachstate);
int	pthread_attr_setdetachstate (pthread_attr_t *__attr, int __detachstate);

 

int	pthread_create (pthread_t *__pthread, const pthread_attr_t *__attr, void *(*__start_routine)( void * ), void *__arg);

 

int	pthread_join (pthread_t __pthread, void * *__value_ptr);

 

int	pthread_detach (pthread_t __pthread);

 

void	pthread_exit (void *__value_ptr);

 

pthread_t	pthread_self (void);

 

int	pthread_equal (pthread_t __t1, pthread_t __t2);

 





 


int	pthread_once (pthread_once_t *__once_control, void (*__init_routine)(void));

 

int	pthread_key_create (pthread_key_t *__key, void (*__destructor)( void * ));

 

int	pthread_setspecific (pthread_key_t __key, const void *__value);
void 	*pthread_getspecific (pthread_key_t __key);

 

int	pthread_key_delete (pthread_key_t __key);

 




int	pthread_cancel (pthread_t __pthread);

 

int	pthread_setcancelstate (int __state, int *__oldstate);
int	pthread_setcanceltype (int __type, int *__oldtype);
void 	pthread_testcancel (void);

 

void 	pthread_cleanup_push (void (*__routine)( void * ), void *__arg);
void 	pthread_cleanup_pop (int __execute);


 

int	pthread_getcpuclockid (pthread_t __pthread_id, clockid_t *__clock_id);

 

int	pthread_attr_setcputime (pthread_attr_t *__attr, int __clock_allowed);

int	pthread_attr_getcputime (pthread_attr_t *__attr, int *__clock_allowed);





int	pthread_barrierattr_init (pthread_barrierattr_t *__attr);
int	pthread_barrierattr_destroy (pthread_barrierattr_t *__attr);
int	pthread_barrierattr_getpshared (const pthread_barrierattr_t *__attr, int *__pshared);
int	pthread_barrierattr_setpshared (pthread_barrierattr_t *__attr, int __pshared);


int	pthread_barrier_init (pthread_barrier_t *__barrier, const pthread_barrierattr_t *__attr, unsigned __count);
int	pthread_barrier_destroy (pthread_barrier_t *__barrier);
int	pthread_barrier_wait (pthread_barrier_t *__barrier);



int	pthread_spin_init (pthread_spinlock_t *__spinlock, int __pshared);
int	pthread_spin_destroy (pthread_spinlock_t *__spinlock);
int	pthread_spin_lock (pthread_spinlock_t *__spinlock);
int	pthread_spin_trylock (pthread_spinlock_t *__spinlock);
int	pthread_spin_unlock (pthread_spinlock_t *__spinlock);



int	pthread_rwlockattr_init (pthread_rwlockattr_t *__attr);
int	pthread_rwlockattr_destroy (pthread_rwlockattr_t *__attr);
int	pthread_rwlockattr_getpshared (const pthread_rwlockattr_t *__attr, int *__pshared);
int	pthread_rwlockattr_setpshared (pthread_rwlockattr_t *__attr, int __pshared);

int	pthread_rwlock_init (pthread_rwlock_t *__rwlock, const pthread_rwlockattr_t *__attr);
int	pthread_rwlock_destroy (pthread_rwlock_t *__rwlock);
int	pthread_rwlock_rdlock (pthread_rwlock_t *__rwlock);
int	pthread_rwlock_tryrdlock (pthread_rwlock_t *__rwlock);
int	pthread_rwlock_timedrdlock (pthread_rwlock_t *__rwlock, const struct timespec *__abstime);
int	pthread_rwlock_wrlock (pthread_rwlock_t *__rwlock);
int	pthread_rwlock_trywrlock (pthread_rwlock_t *__rwlock);
int	pthread_rwlock_timedwrlock (pthread_rwlock_t *__rwlock, const struct timespec *__abstime);


int pthread_delay (int ticks);
int pthread_suspend (pthread_t thread);
int pthread_resume (pthread_t thread);
unsigned long long sys_timestamp ();

 






 
 



 
typedef struct {
  Objects_Control   Object;
  Watchdog_Control  Timer;       
  pthread_t         thread_id;   
  char              state;       
  struct sigevent   inf;         
  struct itimerspec timer_data;  
  uint32_t          ticks;       
  uint32_t          overrun;     
  struct timespec   time;        
} POSIX_Timer_Control;

 


 







 








 








 








 











 








 









 










 








 







 








 






 








 










 








 














 








 



 






 



 




 



 










 
 




 
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
  do { do { (level) = (cpu_st_get() & 0x00000001); __asm("\tDINT"); } while (0); ( ( void ) 0 ); ; } while (0);
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
  do { do { (level) = (cpu_st_get() & 0x00000001); __asm("\tDINT"); } while (0); ( ( void ) 0 ); ; } while (0);
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
  do { do { (level) = (cpu_st_get() & 0x00000001); __asm("\tDINT"); } while (0); ( ( void ) 0 ); ; } while (0);
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
  do { do { (level) = (cpu_st_get() & 0x00000001); __asm("\tDINT"); } while (0); ( ( void ) 0 ); ; } while (0);
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
  do { do { (level) = (cpu_st_get() & 0x00000001); __asm("\tDINT"); } while (0); ( ( void ) 0 ); ; } while (0);
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
  do { do { (level) = (cpu_st_get() & 0x00000001); __asm("\tDINT"); } while (0); ( ( void ) 0 ); ; } while (0);
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
  do { do { (level) = (cpu_st_get() & 0x00000001); __asm("\tDINT"); } while (0); ( ( void ) 0 ); ; } while (0);
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
  do { do { (level) = (cpu_st_get() & 0x00000001); __asm("\tDINT"); } while (0); ( ( void ) 0 ); ; } while (0);
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
  do { do { (level) = (cpu_st_get() & 0x00000001); __asm("\tDINT"); } while (0); ( ( void ) 0 ); ; } while (0);
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
  do { do { (level) = (cpu_st_get() & 0x00000001); __asm("\tDINT"); } while (0); ( ( void ) 0 ); ; } while (0);
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
  do { do { (level) = (cpu_st_get() & 0x00000001); __asm("\tDINT"); } while (0); ( ( void ) 0 ); ; } while (0);
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
  do { do { (level) = (cpu_st_get() & 0x00000001); __asm("\tDINT"); } while (0); ( ( void ) 0 ); ; } while (0);
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
  do { do { (level) = (cpu_st_get() & 0x00000001); __asm("\tDINT"); } while (0); ( ( void ) 0 ); ; } while (0);
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
  do { do { (level) = (cpu_st_get() & 0x00000001); __asm("\tDINT"); } while (0); ( ( void ) 0 ); ; } while (0);
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
  do { do { (level) = (cpu_st_get() & 0x00000001); __asm("\tDINT"); } while (0); ( ( void ) 0 ); ; } while (0);
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
  do { do { (level) = (cpu_st_get() & 0x00000001); __asm("\tDINT"); } while (0); ( ( void ) 0 ); ; } while (0);
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
  do { do { (level) = (cpu_st_get() & 0x00000001); __asm("\tDINT"); } while (0); ( ( void ) 0 ); ; } while (0);
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
  do { do { (level) = (cpu_st_get() & 0x00000001); __asm("\tDINT"); } while (0); ( ( void ) 0 ); ; } while (0);
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
  do { do { (level) = (cpu_st_get() & 0x00000001); __asm("\tDINT"); } while (0); ( ( void ) 0 ); ; } while (0);
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








































 







 








 












 
 








 

















 








 










 








 













 









 










 








 


 

 










 








 










 








 

















 
 



 







 
typedef uint32_t   Priority_Control;

 





 

 

 



 
extern uint8_t rtems_maximum_priority;


 

 










 
 







 
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

 


 







 








 











 








 







 
 



 
int POSIX_NOT_IMPLEMENTED( void );





 
extern const char _RTEMS_version[];



 
extern const char _Copyright_Notice[];

 



 
 








 
 





 
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

 


 







 
 




 
typedef struct {
  

 
  Objects_Control Object;

  

 
  CORE_mutex_Control Mutex;

  


 
  _Bool previous_thread_life_protection;
} API_Mutex_Control;








 
void _API_Mutex_Initialization( uint32_t maximum_mutexes );




 
void _API_Mutex_Allocate( API_Mutex_Control **mutex );





 
void _API_Mutex_Lock( API_Mutex_Control *mutex );





 
void _API_Mutex_Unlock( API_Mutex_Control *mutex );





 
_Bool _API_Mutex_Is_owner( const API_Mutex_Control *mutex );

 











 
 






 
extern API_Mutex_Control *_RTEMS_Allocator_Mutex;

static inline void _RTEMS_Lock_allocator( void )
{
  _API_Mutex_Lock( _RTEMS_Allocator_Mutex );
}

static inline void _RTEMS_Unlock_allocator( void )
{
  _API_Mutex_Unlock( _RTEMS_Allocator_Mutex );
}

static inline _Bool _RTEMS_Allocator_is_owner( void )
{
  return _API_Mutex_Is_owner( _RTEMS_Allocator_Mutex );
}

extern API_Mutex_Control *_Once_Mutex;

static inline void _Once_Lock( void )
{
  _API_Mutex_Lock( _Once_Mutex );
}

static inline void _Once_Unlock( void )
{
  _API_Mutex_Unlock( _Once_Mutex );
}

 


 


 








 







 








 









 








 












 



 


 




struct Thread_Control;

struct Scheduler_Context;










 

 




 
typedef struct {
} Per_CPU_Stats;





 
typedef struct Per_CPU_Control {
    

 
    CPU_Per_CPU_control cpu_per_cpu;

    


 
    void  *interrupt_stack_low;

    


 
    void  *interrupt_stack_high;

  


 
  uint32_t isr_nest_level;

  


 
  volatile uint32_t thread_dispatch_disable_level;

  








 
  struct Thread_Control *executing;

  










 
  struct Thread_Control *heir;

  












 
  volatile _Bool dispatch_necessary;

   
  Timestamp_Control time_of_last_context_switch;


  Per_CPU_Stats Stats;
} Per_CPU_Control;

typedef struct {
  Per_CPU_Control per_cpu;
} Per_CPU_Control_envelope;





 
extern Per_CPU_Control_envelope _Per_CPU_Information[] ;













 


static inline Per_CPU_Control *_Per_CPU_Get_by_index( uint32_t index )
{
  return &_Per_CPU_Information[ index ].per_cpu;
}

static inline uint32_t _Per_CPU_Get_index( const Per_CPU_Control *cpu )
{
  const Per_CPU_Control_envelope *per_cpu_envelope =
    ( const Per_CPU_Control_envelope * ) cpu;

  return ( uint32_t ) ( per_cpu_envelope - &_Per_CPU_Information[ 0 ] );
}

static inline _Bool _Per_CPU_Is_processor_started(
  const Per_CPU_Control *cpu
)
{
  (void) cpu;

  return 1;
}





 









 
static inline struct Thread_Control *_Thread_Get_executing( void )
{
  struct Thread_Control *executing;


  executing = ( &_Per_CPU_Information[ ((uint_least32_t)(0)) ]. per_cpu )->executing;


  return executing;
}

 




 






 













 










 

static inline void _Profiling_Thread_dispatch_disable(
  Per_CPU_Control *cpu,
  uint32_t previous_thread_dispatch_disable_level
)
{
  (void) cpu;
  (void) previous_thread_dispatch_disable_level;
}

static inline void _Profiling_Thread_dispatch_disable_critical(
  Per_CPU_Control        *cpu,
  uint32_t                previous_thread_dispatch_disable_level,
  const ISR_lock_Context *lock_context
)
{
  (void) cpu;
  (void) previous_thread_dispatch_disable_level;
  (void) lock_context;
}

static inline void _Profiling_Thread_dispatch_enable(
  Per_CPU_Control *cpu,
  uint32_t new_thread_dispatch_disable_level
)
{
  (void) cpu;
  (void) new_thread_dispatch_disable_level;
}

static inline void _Profiling_Update_max_interrupt_delay(
  Per_CPU_Control *cpu,
  CPU_Counter_ticks interrupt_delay
)
{
  (void) cpu;
  (void) interrupt_delay;
}

void _Profiling_Outer_most_interrupt_entry_and_exit(
  Per_CPU_Control *cpu,
  CPU_Counter_ticks interrupt_entry_instant,
  CPU_Counter_ticks interrupt_exit_instant
);

 










 








 
static inline _Bool _Thread_Dispatch_is_enabled(void)
{
  _Bool enabled;


  enabled = ( &_Per_CPU_Information[ ((uint_least32_t)(0)) ]. per_cpu )->thread_dispatch_disable_level == 0;


  return enabled;
}





 
static inline uint32_t _Thread_Dispatch_get_disable_level(void)
{
  return ( &_Per_CPU_Information[ ((uint_least32_t)(0)) ]. per_cpu )->thread_dispatch_disable_level;
}





 
static inline void _Thread_Dispatch_initialization( void )
{
  ( &_Per_CPU_Information[ ((uint_least32_t)(0)) ]. per_cpu )->thread_dispatch_disable_level = 1;
}

  



 
  static inline uint32_t _Thread_Dispatch_increment_disable_level(void)
  {
    uint32_t disable_level = ( &_Per_CPU_Information[ ((uint_least32_t)(0)) ]. per_cpu )->thread_dispatch_disable_level;

    ++disable_level;
    ( &_Per_CPU_Information[ ((uint_least32_t)(0)) ]. per_cpu )->thread_dispatch_disable_level = disable_level;


    return disable_level;
  }

  



 
  static inline uint32_t _Thread_Dispatch_decrement_disable_level(void)
  {
    uint32_t disable_level = ( &_Per_CPU_Information[ ((uint_least32_t)(0)) ]. per_cpu )->thread_dispatch_disable_level;

    --disable_level;
    ( &_Per_CPU_Information[ ((uint_least32_t)(0)) ]. per_cpu )->thread_dispatch_disable_level = disable_level;


    return disable_level;
  }

  static inline void _Giant_Acquire( Per_CPU_Control *cpu_self )
  {
    (void) cpu_self;
  }

  static inline void _Giant_Release( Per_CPU_Control *cpu_self )
  {
    (void) cpu_self;
  }

















 
void _Thread_Dispatch( void );













 
void _Thread_Do_dispatch( Per_CPU_Control *cpu_self, ISR_Level level );











 
static inline Per_CPU_Control *_Thread_Dispatch_disable_critical(
  const ISR_lock_Context *lock_context
)
{
  Per_CPU_Control *cpu_self;
  uint32_t         disable_level;

  cpu_self = ( &_Per_CPU_Information[ ((uint_least32_t)(0)) ]. per_cpu );
  disable_level = cpu_self->thread_dispatch_disable_level;
  _Profiling_Thread_dispatch_disable_critical(
    cpu_self,
    disable_level,
    lock_context
  );
  cpu_self->thread_dispatch_disable_level = disable_level + 1;

  return cpu_self;
}







 
static inline Per_CPU_Control *_Thread_Dispatch_disable( void )
{
  Per_CPU_Control  *cpu_self;
  ISR_lock_Context  lock_context;


  cpu_self = _Thread_Dispatch_disable_critical( &lock_context );


  return cpu_self;
}









 
static inline void _Thread_Dispatch_enable( Per_CPU_Control *cpu_self )
{
  uint32_t disable_level = cpu_self->thread_dispatch_disable_level;

  if ( disable_level == 1 ) {
    ISR_Level level;

    do { do { (level) = (cpu_st_get() & 0x00000001); __asm("\tDINT"); } while (0); ; } while (0);

    if ( cpu_self->dispatch_necessary ) {
      _Thread_Do_dispatch( cpu_self, level );
    } else {
      cpu_self->thread_dispatch_disable_level = 0;
      _Profiling_Thread_dispatch_enable( cpu_self, 0 );
    }

    do { ; do { SetCSRBit(level); { __asm(" nop");; __asm(" nop");; }; } while (0); } while (0);
  } else {
    cpu_self->thread_dispatch_disable_level = disable_level - 1;
  }
}



 
static inline void _Thread_Disable_dispatch( void )
{
  _Thread_Dispatch_increment_disable_level();
  ;
}

static inline void _Thread_Enable_dispatch_body( void )
{
  Per_CPU_Control *cpu_self = ( &_Per_CPU_Information[ ((uint_least32_t)(0)) ]. per_cpu );

  _Giant_Release( cpu_self );
  _Thread_Dispatch_enable( cpu_self );
}





 
  void _Thread_Enable_dispatch( void );






 
static inline void _Thread_Unnest_dispatch( void )
{
  ;
  _Thread_Dispatch_decrement_disable_level();
}

 








 



 
typedef _Bool    (*Objects_Name_comparators)(
  void       *  ,
  void       *  ,
  uint16_t      
);




 
typedef enum {
  OBJECTS_INTERNAL_NO_CLASS =  0,
  OBJECTS_INTERNAL_THREADS  =  1,
  OBJECTS_INTERNAL_MUTEXES  =  2
} Objects_Internal_API;

 




 
typedef enum {
  OBJECTS_CLASSIC_NO_CLASS     = 0,
  OBJECTS_RTEMS_TASKS          = 1,
  OBJECTS_RTEMS_TIMERS         = 2,
  OBJECTS_RTEMS_SEMAPHORES     = 3,
  OBJECTS_RTEMS_MESSAGE_QUEUES = 4,
  OBJECTS_RTEMS_PARTITIONS     = 5,
  OBJECTS_RTEMS_REGIONS        = 6,
  OBJECTS_RTEMS_PORTS          = 7,
  OBJECTS_RTEMS_PERIODS        = 8,
  OBJECTS_RTEMS_EXTENSIONS     = 9,
  OBJECTS_RTEMS_BARRIERS       = 10
} Objects_Classic_API;

 




 
typedef enum {
  OBJECTS_POSIX_NO_CLASS            = 0,
  OBJECTS_POSIX_THREADS             = 1,
  OBJECTS_POSIX_KEYS                = 2,
  OBJECTS_POSIX_INTERRUPTS          = 3,
  OBJECTS_POSIX_MESSAGE_QUEUE_FDS   = 4,
  OBJECTS_POSIX_MESSAGE_QUEUES      = 5,
  OBJECTS_POSIX_MUTEXES             = 6,
  OBJECTS_POSIX_SEMAPHORES          = 7,
  OBJECTS_POSIX_CONDITION_VARIABLES = 8,
  OBJECTS_POSIX_TIMERS              = 9,
  OBJECTS_POSIX_BARRIERS            = 10,
  OBJECTS_POSIX_SPINLOCKS           = 11,
  OBJECTS_POSIX_RWLOCKS             = 12
} Objects_POSIX_API;

 




 
typedef enum {
  OBJECTS_FAKE_OBJECTS_NO_CLASS   = 0,
  OBJECTS_FAKE_OBJECTS_SCHEDULERS = 1
} Objects_Fake_objects_API;





 
typedef enum {
  OBJECTS_REMOTE = 2,          
  OBJECTS_LOCAL  = 0,          
  OBJECTS_ERROR  = 1           
} Objects_Locations;





 
typedef void ( *Objects_Thread_queue_Extract_callout )( void * );




 
typedef struct {
   
  Objects_APIs      the_api;
   
  uint16_t          the_class;
   
  Objects_Id        minimum_id;
   
  Objects_Id        maximum_id;
   
  Objects_Maximum   maximum;
   
  _Bool              auto_extend;
   
  Objects_Maximum   allocation_size;
   
  size_t            size;
   
  Objects_Control **local_table;
   
  Chain_Control     Inactive;
   
  Objects_Maximum   inactive;
   
  uint32_t         *inactive_per_block;
   
  void            **object_blocks;
     
    _Bool              is_string;
   
  uint16_t          name_length;
     
    Objects_Thread_queue_Extract_callout extract;
     
    Chain_Control    *global_table;
}   Objects_Information;



 
extern uint16_t       _Objects_Local_node;



 
extern uint16_t    _Objects_Maximum_nodes;





 
extern Objects_Information
    **_Objects_Information_table[OBJECTS_POSIX_API + 1];





 
void _Objects_Extend_information(
  Objects_Information *information
);










 
void _Objects_Shrink_information(
  Objects_Information *information
);





















 
void _Objects_Initialize_information (
  Objects_Information *information,
  Objects_APIs         the_api,
  uint16_t             the_class,
  uint32_t             maximum,
  uint16_t             size,
  _Bool                 is_string,
  uint32_t             maximum_name_length
  ,
  _Bool                 supports_global,
  Objects_Thread_queue_Extract_callout extract
);










 
unsigned int _Objects_API_maximum_class(
  uint32_t api
);















 
Objects_Control *_Objects_Allocate_unprotected(
  Objects_Information *information
);







































 
Objects_Control *_Objects_Allocate( Objects_Information *information );
























































 
void _Objects_Free(
  Objects_Information *information,
  Objects_Control     *the_object
);










 
typedef enum {
  OBJECTS_NAME_OR_ID_LOOKUP_SUCCESSFUL,
  OBJECTS_INVALID_NAME,
  OBJECTS_INVALID_ADDRESS,
  OBJECTS_INVALID_ID,
  OBJECTS_INVALID_NODE
} Objects_Name_or_id_lookup_errors;




 




 
















 
Objects_Name_or_id_lookup_errors _Objects_Name_to_id_u32(
  Objects_Information *information,
  uint32_t             name,
  uint32_t             node,
  Objects_Id          *id
);















 
Objects_Name_or_id_lookup_errors _Objects_Name_to_id_string(
  Objects_Information *information,
  const char          *name,
  Objects_Id          *id
);

















 
Objects_Name_or_id_lookup_errors _Objects_Id_to_name (
  Objects_Id      id,
  Objects_Name   *name
);


























 
Objects_Control *_Objects_Get (
  Objects_Information *information,
  Objects_Id           id,
  Objects_Locations   *location
);



























 
Objects_Control *_Objects_Get_isr_disable(
  Objects_Information *information,
  Objects_Id           id,
  Objects_Locations   *location,
  ISR_lock_Context    *lock_context
);


























 
Objects_Control *_Objects_Get_no_protection(
  Objects_Information *information,
  Objects_Id           id,
  Objects_Locations   *location
);











 
Objects_Control *_Objects_Get_next(
    Objects_Information *information,
    Objects_Id           id,
    Objects_Locations   *location_p,
    Objects_Id          *next_id_p
);













 
Objects_Information *_Objects_Get_information(
  Objects_APIs   the_api,
  uint16_t       the_class
);











 
Objects_Information *_Objects_Get_information_id(
  Objects_Id  id
);














 
char *_Objects_Get_name_as_string(
  Objects_Id   id,
  size_t       length,
  char        *name
);













 
_Bool _Objects_Set_name(
  Objects_Information *information,
  Objects_Control     *the_object,
  const char          *name
);








 
void _Objects_Namespace_remove(
  Objects_Information  *information,
  Objects_Control      *the_object
);









 
void _Objects_Close(
  Objects_Information  *information,
  Objects_Control      *the_object
);







 
Objects_Maximum _Objects_Active_count(
  const Objects_Information *information
);








 
static inline _Bool _Objects_Is_api_valid(
  uint32_t   the_api
)
{
  if ( !the_api || the_api > OBJECTS_POSIX_API )
   return 0;
  return 1;
}










 
static inline _Bool _Objects_Is_local_node(
  uint32_t   node
)
{
  return ( node == _Objects_Local_node );
}











 
static inline _Bool _Objects_Is_local_id(
  Objects_Id id
)
{
  return _Objects_Is_local_node( _Objects_Get_node(id) );
}










 
static inline _Bool _Objects_Are_ids_equal(
  Objects_Id left,
  Objects_Id right
)
{
  return ( left == right );
}










 
static inline Objects_Control *_Objects_Get_local_object(
  Objects_Information *information,
  uint16_t             index
)
{
  



 
  return information->local_table[ index ];
}













 

static inline void _Objects_Set_local_object(
  Objects_Information *information,
  uint32_t             index,
  Objects_Control     *the_object
)
{
  



 

  information->local_table[ index ] = the_object;
}













 

static inline void _Objects_Invalidate_Id(
  Objects_Information  *information,
  Objects_Control      *the_object
)
{
  ( ( void ) 0 );
  ( ( void ) 0 );

  _Objects_Set_local_object(
    information,
    _Objects_Get_index( the_object->id ),
    0
  );
}








 
static inline void _Objects_Open(
  Objects_Information *information,
  Objects_Control     *the_object,
  Objects_Name         name
)
{
  ( ( void ) 0 );
  ( ( void ) 0 );

  the_object->name = name;

  _Objects_Set_local_object(
    information,
    _Objects_Get_index( the_object->id ),
    the_object
  );
}








 
static inline void _Objects_Open_u32(
  Objects_Information *information,
  Objects_Control     *the_object,
  uint32_t             name
)
{
   
  the_object->name.name_u32 = name;

  _Objects_Set_local_object(
    information,
    _Objects_Get_index( the_object->id ),
    the_object
  );
}








 
static inline void _Objects_Open_string(
  Objects_Information *information,
  Objects_Control     *the_object,
  const char          *name
)
{
     
    the_object->name.name_p = name;

  _Objects_Set_local_object(
    information,
    _Objects_Get_index( the_object->id ),
    the_object
  );
}






 
static inline void _Objects_Put(
  Objects_Control *the_object
)
{
  (void) the_object;
  _Thread_Enable_dispatch();
}







 
static inline void _Objects_Put_without_thread_dispatch(
  Objects_Control *the_object
)
{
  (void) the_object;
  _Thread_Unnest_dispatch();
}














 
static inline void _Objects_Allocator_lock( void )
{
  _RTEMS_Lock_allocator();
}







 
static inline void _Objects_Allocator_unlock( void )
{
  _RTEMS_Unlock_allocator();
}

 









 








 







 










 






 
 



 





 





 



 













 
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

 


 










 
 







 
void _Objects_MP_Handler_initialization(void);







 
void _Objects_MP_Handler_early_initialization(void);
















 
void _Objects_MP_Open (
  Objects_Information *information,
  Objects_MP_Control  *the_global_object,
  uint32_t             the_name,
  Objects_Id           the_id
);



















 
_Bool _Objects_MP_Allocate_and_open (
  Objects_Information *information,
  uint32_t             the_name,
  Objects_Id           the_id,
  _Bool                 is_fatal_error
);






 
void _Objects_MP_Close (
  Objects_Information *information,
  Objects_Id           the_id
);


















 
Objects_Name_or_id_lookup_errors _Objects_MP_Global_name_search (
  Objects_Information *information,
  Objects_Name         the_name,
  uint32_t             nodes_to_search,
  Objects_Id          *the_id
);




















 
void _Objects_MP_Is_remote (
  Objects_Information  *information,
  Objects_Id            the_id,
  Objects_Locations    *location,
  Objects_Control     **the_object
);



 
extern uint32_t       _Objects_MP_Maximum_global_objects;




 
extern Chain_Control  _Objects_MP_Inactive_global_objects;



 

static inline Objects_MP_Control *_Objects_MP_Allocate_global_object (
  void
)
{
  return (Objects_MP_Control *)
           _Chain_Get( &_Objects_MP_Inactive_global_objects );
}



 

static inline void _Objects_MP_Free_global_object (
  Objects_MP_Control *the_object
)
{
  _Chain_Append(
    &_Objects_MP_Inactive_global_objects,
    &the_object->Object.Node
  );
}



 

static inline _Bool _Objects_MP_Is_null_global_object (
  Objects_MP_Control *the_object
)
{
  return( the_object == 0 );
}

 


 


 


 

 

 

 

 





 





 
void _POSIX_Timer_Manager_initialization(void);





 
void _POSIX_Timer_TSR(Objects_Id timer, void *data);



 
_Bool _POSIX_Timer_Insert_helper(
  Watchdog_Control               *timer,
  Watchdog_Interval               ticks,
  Objects_Id                      id,
  Watchdog_Service_routine_entry  TSR,
  void                           *arg
);




 
extern Objects_Information  _POSIX_Timer_Information;






 
static inline POSIX_Timer_Control *_POSIX_Timer_Allocate( void )
{
  return (POSIX_Timer_Control *) _Objects_Allocate( &_POSIX_Timer_Information );
}






 
static inline void _POSIX_Timer_Free (
  POSIX_Timer_Control *the_timer
)
{
  _Objects_Free( &_POSIX_Timer_Information, &the_timer->Object );
}









 
static inline POSIX_Timer_Control *_POSIX_Timer_Get (
  timer_t            id,
  Objects_Locations *location
)
{
  return (POSIX_Timer_Control *)
    _Objects_Get( &_POSIX_Timer_Information, (Objects_Id) id, location );
}


 






 








 








 








 





 






 













 








 













 











 












 

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

 



 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 











 
 



 



 



 



 



 




 



 



 



 



 



 

 





 







 







 
 



 
typedef struct {
  





 
  uint32_t seconds_trigger;

  




 
  _Bool is_set;
} TOD_Control;

extern TOD_Control _TOD;





 
void _TOD_Handler_initialization(void);








 
void _TOD_Set_with_timestamp(
  const Timestamp_Control *tod_as_timestamp
);












 
static inline void _TOD_Set(
  const struct timespec *tod_as_timespec
)
{
  Timestamp_Control tod_as_timestamp;

  _Timestamp_Set(
    &tod_as_timestamp,
    tod_as_timespec->tv_sec,
    tod_as_timespec->tv_nsec
  );
  _TOD_Set_with_timestamp( &tod_as_timestamp );
}





 
static inline void _TOD_Get(
  Timestamp_Control *time
)
{
  _Timecounter_Bintime(time);
}





 
static inline void _TOD_Get_as_timespec(
  struct timespec *time
)
{
  _Timecounter_Nanotime(time);
}










 
static inline void _TOD_Get_uptime(
  Timestamp_Control *time
)
{
  _Timecounter_Binuptime( time );
}








 
static inline void _TOD_Get_zero_based_uptime(
  Timestamp_Control *time
)
{
  _Timecounter_Binuptime( time );
  --time->sec;
}







 
static inline void _TOD_Get_zero_based_uptime_as_timespec(
  struct timespec *time
)
{
  _Timecounter_Nanouptime( time );
  --time->tv_sec;
}






 
static inline uint32_t _TOD_Seconds_since_epoch( void )
{
  return (uint32_t) _Timecounter_Time_second;
}






 
void _TOD_Tickle_ticks( void );








 
uint32_t TOD_TICKS_PER_SECOND_method(void);








 



 

static inline void _TOD_Get_timeval(
  struct timeval *time
)
{
  _Timecounter_Microtime( time );
}








 
void _TOD_Adjust(
  const Timestamp_Control timestamp
);





 
static inline _Bool _TOD_Is_set( void )
{
  return _TOD.is_set;
}

 


 







 








 







 





 




 
typedef struct {
  

 
  Chain_Node Node;

  

 
  Watchdog_Interval delta_interval;

  


 
  Chain_Node *current;
} Watchdog_Iterator;



 
typedef struct {
  

 
  

  

 
  Chain_Control Watchdogs;

  




 
  Chain_Control Iterators;
} Watchdog_Header;





 
extern Watchdog_Header _Watchdog_Ticks_header;





 
extern Watchdog_Header _Watchdog_Seconds_header;

static inline void _Watchdog_Acquire(
  Watchdog_Header  *header,
  ISR_lock_Context *lock_context
)
{
  do { do { (( lock_context )->isr_level) = (cpu_st_get() & 0x00000001); __asm("\tDINT"); } while (0); ( ( void ) 0 ); ; } while (0);
}

static inline void _Watchdog_Release(
  Watchdog_Header  *header,
  ISR_lock_Context *lock_context
)
{
  do { ; ( ( void ) 0 ); do { SetCSRBit(( lock_context )->isr_level); { __asm(" nop");; __asm(" nop");; }; } while (0); } while (0);
}

static inline void _Watchdog_Flash(
  Watchdog_Header  *header,
  ISR_lock_Context *lock_context
)
{
  do { ; ( ( void ) 0 ); do { do { SetCSRBit(( lock_context )->isr_level); { __asm(" nop");; __asm(" nop");; }; } while (0); __asm("\tDINT"); } while (0); ; } while (0);
}







 
void _Watchdog_Handler_initialization( void );





 
void _Watchdog_Tick( void );










 
Watchdog_States _Watchdog_Remove (
  Watchdog_Header  *header,
  Watchdog_Control *the_watchdog
);







 
void _Watchdog_Adjust_backward(
  Watchdog_Header   *header,
  Watchdog_Interval  units
);











 
void _Watchdog_Adjust_backward_locked(
  Watchdog_Header   *header,
  Watchdog_Interval  units
);









 
void _Watchdog_Adjust_forward(
  Watchdog_Header   *header,
  Watchdog_Interval  units
);













 
void _Watchdog_Adjust_forward_locked(
  Watchdog_Header   *header,
  Watchdog_Interval  units,
  ISR_lock_Context  *lock_context
);











 
void _Watchdog_Insert (
  Watchdog_Header  *header,
  Watchdog_Control *the_watchdog
);













 
void _Watchdog_Insert_locked(
  Watchdog_Header  *header,
  Watchdog_Control *the_watchdog,
  ISR_lock_Context *lock_context
);










 
void _Watchdog_Tickle (
  Watchdog_Header *header
);








 
static inline void _Watchdog_Preinitialize(
  Watchdog_Control *the_watchdog
)
{
  the_watchdog->state = WATCHDOG_INACTIVE;
}





 

static inline void _Watchdog_Initialize(
  Watchdog_Control               *the_watchdog,
  Watchdog_Service_routine_entry  routine,
  Objects_Id                      id,
  void                           *user_data
)
{
  ( ( void ) 0 );
  the_watchdog->routine   = routine;
  the_watchdog->id        = id;
  the_watchdog->user_data = user_data;
}




 

static inline _Bool _Watchdog_Is_active(
  Watchdog_Control *the_watchdog
)
{

  return ( the_watchdog->state == WATCHDOG_ACTIVE );

}




 

static inline void _Watchdog_Activate(
  Watchdog_Control *the_watchdog
)
{

  the_watchdog->state = WATCHDOG_ACTIVE;

}




 

static inline void _Watchdog_Tickle_ticks( void )
{

  _Watchdog_Tickle( &_Watchdog_Ticks_header );

}




 

static inline void _Watchdog_Tickle_seconds( void )
{

  _Watchdog_Tickle( &_Watchdog_Seconds_header );

}






 

static inline void _Watchdog_Insert_ticks(
  Watchdog_Control      *the_watchdog,
  Watchdog_Interval      units
)
{

  the_watchdog->initial = units;

  _Watchdog_Insert( &_Watchdog_Ticks_header, the_watchdog );

}






 

static inline void _Watchdog_Insert_seconds(
  Watchdog_Control      *the_watchdog,
  Watchdog_Interval      units
)
{

  the_watchdog->initial = units;

  _Watchdog_Insert( &_Watchdog_Seconds_header, the_watchdog );

}

static inline Watchdog_States _Watchdog_Remove_ticks(
  Watchdog_Control *the_watchdog
)
{
  return _Watchdog_Remove( &_Watchdog_Ticks_header, the_watchdog );
}

static inline Watchdog_States _Watchdog_Remove_seconds(
  Watchdog_Control *the_watchdog
)
{
  return _Watchdog_Remove( &_Watchdog_Seconds_header, the_watchdog );
}






 

static inline void _Watchdog_Reset_ticks(
  Watchdog_Control *the_watchdog
)
{

  _Watchdog_Remove_ticks( the_watchdog );

  _Watchdog_Insert( &_Watchdog_Ticks_header, the_watchdog );

}




 

static inline Watchdog_Control *_Watchdog_Next(
  Watchdog_Control *the_watchdog
)
{

  return ( (Watchdog_Control *) the_watchdog->Node.next );

}




 

static inline Watchdog_Control *_Watchdog_Previous(
  Watchdog_Control *the_watchdog
)
{

  return ( (Watchdog_Control *) the_watchdog->Node.previous );

}




 

static inline Watchdog_Control *_Watchdog_First(
  Watchdog_Header *header
)
{

  return ( (Watchdog_Control *) _Chain_First( &header->Watchdogs ) );

}




 

static inline Watchdog_Control *_Watchdog_Last(
  Watchdog_Header *header
)
{

  return ( (Watchdog_Control *) _Chain_Last( &header->Watchdogs ) );

}

static inline _Bool _Watchdog_Is_empty(
  const Watchdog_Header *header
)
{
  return _Chain_Is_empty( &header->Watchdogs );
}

static inline void _Watchdog_Header_initialize(
  Watchdog_Header *header
)
{
  ;
  _Chain_Initialize_empty( &header->Watchdogs );
  _Chain_Initialize_empty( &header->Iterators );
}

 


 







 








 







 
 

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 


#pragma diag_push


 
 
#pragma CHECK_MISRA("-19.15")


#pragma diag_pop







 






 

 
 

int timer_settime(
  timer_t                  timerid,
  int                      flags,
  const struct itimerspec *__restrict value,
  struct itimerspec       *__restrict ovalue
)
{
  POSIX_Timer_Control *ptimer;
  Objects_Locations    location;
  _Bool                 activated;
  uint32_t             initial_period;
  struct itimerspec    normalize;

  if ( !value )
    do { (* __c6xabi_errno_addr()) = ( 22 ); return ( -1 ); } while ( 0 );

  


 
  if ( !_Timespec_Is_valid( &(value->it_value) ) ) {
    do { (* __c6xabi_errno_addr()) = ( 22 ); return ( -1 ); } while ( 0 );
  }
  if ( !_Timespec_Is_valid( &(value->it_interval) ) ) {
    do { (* __c6xabi_errno_addr()) = ( 22 ); return ( -1 ); } while ( 0 );
  }

  if ( flags != 0x1 && flags != 0 ) {
    do { (* __c6xabi_errno_addr()) = ( 22 ); return ( -1 ); } while ( 0 );
  }

  normalize = *value;

   
  if (flags == 0x1) {
    struct timespec now;
    _TOD_Get_as_timespec( &now );
     
    if ( _Timespec_Less_than( &normalize . it_value, &now ) )
      do { (* __c6xabi_errno_addr()) = ( 22 ); return ( -1 ); } while ( 0 );
    _Timespec_Subtract( &now, &normalize.it_value, &normalize.it_value );
  }

  


 

  ptimer = _POSIX_Timer_Get( timerid, &location );
  switch ( location ) {

    case OBJECTS_LOCAL:
       
      if ( normalize.it_value.tv_sec == 0 && normalize.it_value.tv_nsec == 0 ) {
          
         _Watchdog_Remove_ticks( &ptimer->Timer );
          
         if ( ovalue )
           *ovalue = ptimer->timer_data;
          
         ptimer->timer_data = normalize;
          
         ptimer->state = 0x04;
          
        _Objects_Put( &ptimer->Object );
        return 0;
       }

        
       ptimer->ticks  = _Timespec_To_ticks( &value->it_interval );
       initial_period = _Timespec_To_ticks( &normalize.it_value );


       activated = _POSIX_Timer_Insert_helper(
         &ptimer->Timer,
         initial_period,
         ptimer->Object.id,
         _POSIX_Timer_TSR,
         ptimer
       );
       if ( !activated ) {
         _Objects_Put( &ptimer->Object );
         return 0;
       }

       


 
       if ( ovalue )
         *ovalue = ptimer->timer_data;
       ptimer->timer_data = normalize;

        
       ptimer->state = 0x03;
       _TOD_Get_as_timespec( &ptimer->time );
      _Objects_Put( &ptimer->Object );
       return 0;

    case OBJECTS_REMOTE:
    case OBJECTS_ERROR:
      break;
  }

  do { (* __c6xabi_errno_addr()) = ( 22 ); return ( -1 ); } while ( 0 );
}
