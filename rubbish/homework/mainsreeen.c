#include <stdio.h>
#include <stdlib.h>

int main_screen(){
    system("clear");
    puts("              ******************************************************");
    puts("              *                                                    *");
    puts("              *                                                    *");
    puts("              *       1. Rotating array, low efficiency            *");
    puts("              *                                                    *");
    puts("              *                                                    *");
    puts("              *       2. Rotating array, high efficiency           *");
    puts("              *                                                    *");
    puts("              *                                                    *");
    puts("              *       3. Rotating linked list                      *");
    puts("              *                                                    *");
    puts("              *                                                    *");
    puts("              ******************************************************");
    printf("              Input your option:");
    int option=0;
    scanf("%d",&option);
    return option;
}

int main(void){
    main_screen();
    return 0;
}