#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"

volatile static int started = 0;

// start() jumps here in supervisor mode on all CPUs.
void
main()
{
  if(cpuid() == 0){
    consoleinit();
    printfinit();
    printf("\n");
    printf("xv6 kernel is booting\n");
    printf("\n");
    kinit();         // physical page allocator          设置好页表分配器（page allocator）
    kvminit();       // create kernel page table         设置好虚拟内存
    kvminithart();   // turn on paging                   打开页表
    procinit();      // process table                    设置好初始进程或者说设置好进程表单
    trapinit();      // trap vectors                     设置好user/kernel mode转换代码
    trapinithart();  // install kernel trap vector
    plicinit();      // set up interrupt controller      设置好中断控制器
    plicinithart();  // ask PLIC for device interrupts    
    binit();         // buffer cache                     PLIC分配buffer cache
    iinit();         // inode table                      初始化inode缓存
    fileinit();      // file table                       初始化文件系统
    virtio_disk_init(); // emulated hard disk            初始化磁盘
    userinit();      // first user process               最后当所有的设置都完成了，操作系统也运行起来了，会通过userinit运行第一个进
    __sync_synchronize();
    started = 1;
  } else {
    while(started == 0)
      ;
    __sync_synchronize();
    printf("hart %d starting\n", cpuid());
    kvminithart();    // turn on paging
    trapinithart();   // install kernel trap vector
    plicinithart();   // ask PLIC for device interrupts
  }

  scheduler();        
}
