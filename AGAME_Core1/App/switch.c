#include "switch.h"
#include "common.h"
#include "include.h"

int DIP_Switch_read()
{    
    int temp = 0;
    if(gpio_get(PTD4) == 1) temp += 2;
    if(gpio_get(PTD5) == 1) temp += 1;
    return temp;
}
