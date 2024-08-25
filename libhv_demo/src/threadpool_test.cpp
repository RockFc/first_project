#include <stdio.h>

#include "hv/hthreadpool.h"
#include "hv/hthread.h"
#include "hv/htime.h"

void print_task(int i) {
    printf("thread[%ld]: task[%d]\n", hv_gettid(), i);
    hv_sleep(1);
}

int main(int argc, char** argv) {
    HThreadPool tp(1, 4);
    tp.start();

    int i = 0;
    for (; i < 10; ++i) {
        tp.commit(print_task, i);
    }

    tp.wait();

    for (; i < 20; ++i) {
        tp.commit(print_task, i);
    }

    tp.wait();

    tp.commit([]()
    {
        printf("thread[%ld]: task in lambda fun\n", hv_gettid());
        hv_sleep(1);
    }).get();
    

    return 0;
}
