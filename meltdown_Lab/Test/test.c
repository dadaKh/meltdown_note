#include <stdio.h>

int main()
{
    char* kernel_data_addr = (char*)0xffffffffc04f5000;
    char kernel_data = *kernel_data_addr;
    printf("kernel data is %c",kernel_data);
    return 0;
}
