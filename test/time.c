#include <stdio.h>
#include <time.h>

int main(void) 
{ 
    struct time t; 
    gmtime(&t); 
    printf("The current time is: %2d:%02d:%02d.%02d/n", 
           t.ti_hour, t.ti_min, t.ti_sec, t.ti_hund); 
    return 0; 
} 
