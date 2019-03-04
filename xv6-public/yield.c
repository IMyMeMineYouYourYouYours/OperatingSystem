#include "types.h"
#include "stat.h"
#include "user.h"

int main(){
    int pid=fork(); 
    while(1){
        if(pid==0){
            printf(1,"child\n");
            yield();
            //sleep(20);
         }else if(pid<0){
            printf(1,"error\n");
         }else{
            printf(1,"parent\n");
            yield();
            //sleep(20);
         }
    }
}
