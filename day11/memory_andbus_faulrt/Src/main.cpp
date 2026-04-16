#include <stdint.h>
#include <stdio.h>

/* ---------------- SCB + MPU REGISTERS ---------------- */
#define SHCSR      (*(volatile uint32_t*)0xE000ED24)

#define MPU_TYPE   (*(volatile uint32_t*)0xE000ED90)
#define MPU_CTRL   (*(volatile uint32_t*)0xE000ED94)
#define MPU_RNR    (*(volatile uint32_t*)0xE000ED98)
#define MPU_RBAR   (*(volatile uint32_t*)0xE000ED9C)
#define MPU_RASR   (*(volatile uint32_t*)0xE000EDA0)

/* ---------------- C LINKAGE (VERY IMPORTANT) ---------------- */
extern "C" void UsageFault_Handler(void);
extern "C" void HardFault_Handler(void);
extern "C" void MemManage_Handler(void);
extern "C" void BusFault_Handler(void);
extern "C" void UsageFault_Handler_c(uint32_t *pMSP);

/* ---------------- DIVIDE FUNCTION ---------------- */
int fun_divide(int x , int y)
{
    return x / y;
}

/* ---------------- MAIN ---------------- */
int main(void)
{
    /* Enable configurable faults */
    uint32_t *pSHCSR = (uint32_t*)0xE000ED24; // SHCSR from ARM CORTEX Ref manual

    *pSHCSR |= (1 << 16); // MemManage
    *pSHCSR |= (1 << 17); // BusFault
   *pSHCSR |= (1 << 18); // UsageFault ENABLED

    /* Enable divide-by-zero trap */
    uint32_t *pCCR = (uint32_t*)0xE000ED14;
    *pCCR |= (1 << 4);// enable DIV_0 TRAP


/*//this part is totrigger usage fault
  //Force runtime division (avoid optimization)
   volatile int a = 10;
    volatile int b = 0;

   fun_divide(a, b);   //  UsageFault
   */

    /*//this part is to trigger bus fault
     *
       volatile uint32_t *invalid = (uint32_t*)0x00001234;
       *invalid = 0x55;
     */

    //this part is sto trigermemnry manage,nt fault





    /* 2. Disable MPU before config */
    MPU_CTRL = 0;

    /* 3. Select region 0 */
    MPU_RNR = 0;

   // /* 4. Set base address (SRAM) */
    MPU_RBAR = 0x20001000;  // protect small block

    MPU_RASR = (0 << 24) |   // NO ACCESS
               (10 << 1) |   // smaller size
               (1 << 0);

    /* 6. Enable MPU + default memory map */
    MPU_CTRL = (1 << 0) | (1 << 2);

     //7. Access restricted memory MemManage Fault
    volatile int *p = (int*)0x20001000;
    *p = 10;





    while(1);
}

/* ---------------- FAULT HANDLERS ---------------- */

/*extern "C" void HardFault_Handler(void)
{

	__asm("BKPT #0");   // Debugger stops
    while(1);
}*/
/*
extern "C" void MemManage_Handler(void)
{
    __asm("BKPT #0");
    while(1);
}
*/
/*
extern "C" void BusFault_Handler(void)
{
    __asm("BKPT #0");
    while(1);
}
*/

/* ---------------- NAKED memeorymanagement FAULT HANDLER ---------------- */
extern "C" __attribute__((naked)) void  MemManage_Handler(void)
{
    __asm volatile(
        "MRS r0, MSP        \n"  // Pass MSP to C handler
        "B MemManage_Handler_c \n"
    );
}

/* ---------------- NAKED BUS FAULT HANDLER ---------------- */
extern "C" __attribute__((naked))void BusFault_Handler(void)
{
    __asm volatile(
        "MRS r0, MSP        \n"  // Pass MSP to C handler
        "B BusFault_Handler_c \n"
    );
}

/* ---------------- NAKED USAGE FAULT HANDLER ---------------- */
extern "C" __attribute__((naked)) void UsageFault_Handler(void)
{
    __asm volatile(
        "MRS r0, MSP        \n"  // Pass MSP to C handler
        "B UsageFault_Handler_c \n"
    );
}

/* ---------------- NAKED HARD FAULT HANDLER ---------------- */
extern "C" __attribute__((naked)) void HardFault_Handler(void)
{
    __asm volatile(
        "MRS r0, MSP        \n"  // Pass MSP to C handler
        "B HardFault_Handler_c \n"
    );
}
/* ---------------- C HANDLER ---------------- */
extern "C" void MemManage_Handler_c(uint32_t *pBaseStackFrame)
{
    uint32_t *pBFSR = (uint32_t*)0xE000ED29;//Usage fault status register (UFSR)

    printf("\n---MemManage fault Occurred ---\n");
    printf("UFSR = %lx\n", (*pBFSR) & 0xFFFF); // 1111 1111 1111 1111
   // uint8_t status= (*pUFSR & (1<<9));
    //printf()
   // printf("statu of MemManage fault in hardfault  = %lx\n",status);

    printf("R0  = %lx\n", pBaseStackFrame[0]);
    printf("R1  = %lx\n", pBaseStackFrame[1]);
    printf("R2  = %lx\n", pBaseStackFrame[2]);
    printf("R3  = %lx\n", pBaseStackFrame[3]);
    printf("R12 = %lx\n", pBaseStackFrame[4]);
    printf("LR  = %lx\n", pBaseStackFrame[5]);
    printf("PC  = %lx\n", pBaseStackFrame[6]);
    printf("XPSR= %lx\n", pBaseStackFrame[7]);

    /*  Reliable debug stop */
    __asm("BKPT #0");

    while(1);
}


/* ---------------- C HANDLER ---------------- */
extern "C" void BusFault_Handler_c(uint32_t *pBaseStackFrame)
{
    uint32_t *pMFSR = (uint32_t*)0xE000ED28;//Usage fault status register (UFSR)

    printf("\n--- busfault Occurred ---\n");
    printf("UFSR = %lx\n", (*pMFSR) & 0xFF); // 1111 1111 1111 1111
   // uint8_t status= (*pUFSR & (1<<9));
    //printf()
   // printf("statu of busfault in hardfault  = %lx\n",status);

    printf("R0  = %lx\n", pBaseStackFrame[0]);
    printf("R1  = %lx\n", pBaseStackFrame[1]);
    printf("R2  = %lx\n", pBaseStackFrame[2]);
    printf("R3  = %lx\n", pBaseStackFrame[3]);
    printf("R12 = %lx\n", pBaseStackFrame[4]);
    printf("LR  = %lx\n", pBaseStackFrame[5]);
    printf("PC  = %lx\n", pBaseStackFrame[6]);
    printf("XPSR= %lx\n", pBaseStackFrame[7]);

    /*  Reliable debug stop */
    __asm("BKPT #0");

    while(1);
}

/* ---------------- C HANDLER ---------------- */
extern "C" void HardFault_Handler_c(uint32_t *pBaseStackFrame)
{
    uint32_t *pHFSR = (uint32_t*)0xE000ED2C;//Usage fault status register (UFSR)

    printf("\n--- hardFault Occurred ---\n");
    printf("UFSR = %lx\n", (*pHFSR) & 0xFFFF); // 1111 1111 1111 1111
    uint8_t status= (*pHFSR & (1<<30));
    //printf()
    printf("statu of hardfault  = %lx\n",status);

    printf("R0  = %lx\n", pBaseStackFrame[0]);
    printf("R1  = %lx\n", pBaseStackFrame[1]);
    printf("R2  = %lx\n", pBaseStackFrame[2]);
    printf("R3  = %lx\n", pBaseStackFrame[3]);
    printf("R12 = %lx\n", pBaseStackFrame[4]);
    printf("LR  = %lx\n", pBaseStackFrame[5]);
    printf("PC  = %lx\n", pBaseStackFrame[6]);
    printf("XPSR= %lx\n", pBaseStackFrame[7]);

    /*  Reliable debug stop */
    __asm("BKPT #0");

    while(1);
}

extern "C" void UsageFault_Handler_c(uint32_t *pBaseStackFrame)
{
    uint32_t *pUFSR = (uint32_t*)0xE000ED2A;//Usage fault status register (UFSR)

    printf("\n--- UsageFault Occurred ---\n");
    printf("UFSR = %lx\n", (*pUFSR) & 0xFFFF); // 1111 1111 1111 1111
    uint8_t status= (*pUFSR & (1<<9));
    //printf()
    printf("statu of usage fault in usage fault  = %lx\n",status);

    printf("R0  = %lx\n", pBaseStackFrame[0]);
    printf("R1  = %lx\n", pBaseStackFrame[1]);
    printf("R2  = %lx\n", pBaseStackFrame[2]);
    printf("R3  = %lx\n", pBaseStackFrame[3]);
    printf("R12 = %lx\n", pBaseStackFrame[4]);
    printf("LR  = %lx\n", pBaseStackFrame[5]);
    printf("PC  = %lx\n", pBaseStackFrame[6]);
    printf("XPSR= %lx\n", pBaseStackFrame[7]);

    /*  Reliable debug stop */
    __asm("BKPT #0");

    while(1);
}
