#include <stdio.h>
#include <thread>

#include "hv/hobjectpool.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

void msleep(unsigned int ms) {
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(ms*1000);
#endif
}

class Task {
public:
    Task() {printf("Task()\n");}
    ~Task() {printf("~Task()\n");}

    void Do() {
        printf("%p start do...\n", this);
        msleep(4000);
        printf("%p end do\n", this);
    }
};

HObjectPool<Task> task_pool(1, 5);

void task_thread(int id) {
    printf("thread %d run...\n", id);
    HPoolObject<Task> pTask(task_pool);
    if (pTask) {
        pTask->Do();
    }
    else {
        printf("No available task in pool\n");
    }
    printf("thread %d exit\n", id);
}

int main(int argc, char** argv) {
    for (int i = 0; i < 10; ++i) {
        new std::thread(task_thread, i);
    }
    msleep(5000);
    for (int i = 10; i < 20; ++i) {
        new std::thread(task_thread, i);
    }
    msleep(10000);
    return 0;
}

/*
HObjectPool 类模板:
	•	这是对象池的核心类模板，管理对象的分配、借用和归还。
	•	构造函数：接受初始对象数量、最大对象数量和超时时间三个参数，默认值分别为0、4和3000毫秒。
	•	成员变量：
	•	_object_num：当前对象池中的对象总数。
	•	_max_num：对象池允许的最大对象数量。
	•	_timeout：等待空闲对象的最大时间。
	•	主要方法：
	•	ObjectNum()：返回对象池中的对象总数。
	•	IdleNum()：返回当前空闲的对象数量。
	•	BorrowNum()：返回当前已借用的对象数量。
	•	TryBorrow()：尝试借用一个对象，如果有空闲对象则返回，没有则返回空指针。
	•	Borrow()：借用一个对象，如果没有空闲对象且总对象数未达到上限则创建新的对象，否则等待一段时间。
	•	Return()：归还一个对象到对象池中，归还后可以再次借用。
	•	Add()：手动添加一个对象到对象池中。
	•	Remove()：从对象池中移除一个对象。
	•	Clear()：清空对象池，释放所有对象。
	•	对象池内部使用 std::list 作为存储空闲对象的容器，std::shared_ptr<T> 用于管理对象的生命周期。
*/
