#include "pi.h"
#include "libs/timer.h"

void pi_reboot(void) {
        int* PM_RSTC = (int*)0x3F10001c;
        int* PM_WDOG = (int*)0x3F100024;
        const int PM_PASSWORD = 0x5a000000;
        const int PM_RSTC_WRCFG_FULL_RESET = 0x00000020;

    timer_delay_ms(1000);

        // timeout = 1/16th of a second? (whatever)
//        PUT32(PM_WDOG, PM_PASSWORD | 1);
//        PUT32(PM_RSTC, PM_PASSWORD | PM_RSTC_WRCFG_FULL_RESET);
    *(PM_WDOG) = PM_PASSWORD | 1;
    *(PM_RSTC) = PM_PASSWORD | PM_RSTC_WRCFG_FULL_RESET;
	while(1);
}

