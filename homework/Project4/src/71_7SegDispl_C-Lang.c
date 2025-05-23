#if defined(D_NEXYS_A7)
#include <bsp_printf.h>
#include <bsp_mem_map.h>
#include <bsp_version.h>
#else
PRE_COMPILED_MSG("no platform was defined")
#endif
#include "psp_api.h"

#define SegEn_ADDR    0x80001038
#define SegDig_ADDR   0x8000103C
#define GPIO_SWs      0x80001400
#define GPIO_LEDs     0x80001404
#define GPIO_INOUT    0x80001408
#define DELAY         10000000

int main(void)
{
    int i, LED_state, Sw_current_state, Sw_next_state, count = 0;

    /* Configure LEDs and Switches */
    M_PSP_WRITE_REGISTER_32(GPIO_INOUT, 0xFFFF);

    /* Configure 7-Seg Displays */
    M_PSP_WRITE_REGISTER_32(SegEn_ADDR, 0x0);

    /* Initialize UART */
    uartInit();

    /* Init states */
    LED_state = 0;
    M_PSP_WRITE_REGISTER_32(GPIO_LEDs, LED_state);
    Sw_current_state = (M_PSP_READ_REGISTER_32(GPIO_SWs) >> 16) & 0x1;

    while (1) {
        /* Invert LED-0 when SW-0 goes high */
        Sw_next_state = (M_PSP_READ_REGISTER_32(GPIO_SWs) >> 16) & 0x1;
        if (Sw_current_state == 0 && Sw_next_state == 1) {
            LED_state = !LED_state;
            M_PSP_WRITE_REGISTER_32(GPIO_LEDs, LED_state);
        }
        Sw_current_state = Sw_next_state;

        /* Increase 7-Seg Displays and print to monitor */
        M_PSP_WRITE_REGISTER_32(SegDig_ADDR, count);
        printfNexys("7-Seg Display Value: %d\n", count);
        count++;

        /* Delay */
        for (i = 0; i < DELAY; i++);
    }

    return 0;
}