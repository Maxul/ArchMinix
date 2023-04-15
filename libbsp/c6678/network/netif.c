#include <stdio.h>
#include <string.h>
#include <unistd.h>

void* memcpy(void* dest, const void* src, size_t n);
void* memset(void* s, int c, size_t n);

void free(void* _ptr);
void* malloc(size_t _size);

void dbgprintf(char* p, int len)
{
    int jj;
    for (jj = 0; jj < len; ++jj)
        printf("%02x ", 0xff & (*(p + jj)));
    printf("\n");
}

//---------------------------------------------
// Fast Crit Sections
//---------------------------------------------
#include <rtems.h>
#include <rtems/error.h>
static ISR_Level lv;
uint OEMSysCritOn()
{
    _ISR_Disable(lv);
    return 1;
}

void OEMSysCritOff(uint key)
{
    _ISR_Enable(lv);
}

#include <malloc.h>
void* mmAlloc(uint Size)
{
    return malloc(Size);
}
void mmFree(void* pv)
{
    free(pv);
}
void mmCopy(void* pDst, void* pSrc, uint Size)
{
    memcpy(pDst, pSrc, Size);
}
void mmZeroInit(void* pDst, uint Size)
{
    memset(pDst, 0, Size);
}

void* mmBulkAlloc(size_t Size)
{
    return malloc(Size);
}
void mmBulkFree(void* pMemory)
{
    free(pMemory);
}
