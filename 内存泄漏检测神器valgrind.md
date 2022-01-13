----



title: 内存泄漏检测神器valgrind

categories:[内存检测]

tags:[valgrind]

date: 2022/01/08

<div align = 'right'>作者：hackett</div>

<div align = 'right'>微信公众号：加班猿</div>

----



## 1.概述

Valgrind是一款用于内存调试、内存泄漏检测以及性能分析的软件开发工具。

Valgrind 发行版目前包括七个生产质量工具：一个内存错误检测器、两个线程错误检测器、一个缓存和分支预测分析器、一个调用图生成缓存和分支预测分析器，以及两个不同的堆分析器。它还包括一个实验性的 SimPoint 基本块矢量生成器。它可在以下平台上运行：X86/Linux、AMD64/Linux、ARM/Linux、ARM64/Linux、PPC32/Linux、PPC64/Linux、PPC64LE/Linux、S390X/Linux、MIPS32/Linux、MIPS64/Linux、X86/Solaris , AMD64/Solaris, ARM/Android (2.3.x 及更高版本), ARM64/Android, X86/Android (4.0 及更高版本), MIPS32/Android, X86/FreeBSD, AMD64/FreeBSD, X86/Darwin 和 AMD64/Darwin (Mac OS X 10.12）

## 1.1 工具

它一般包含下列工具：

### 1.Memcheck

最常用的工具，用来检测程序中出现的内存问题，所有对内存的读写都会被检测到，一切对malloc()/free()/new/delete的调用都会被捕获。所以，它能检测以下问题：

- 对未初始化内存的使用；

- 读/写释放后的内存块；

- 读/写超出malloc分配的内存块；

- 读/写不适当的栈中内存块；

- 内存泄漏，指向一块内存的指针永远丢失；

- 不正确的malloc/free或new/delete匹配；

- memcpy()相关函数中的dst和src指针重叠。

### 2.Callgrind

和gprof类似的分析工具，但它对程序的运行观察更是入微，能给我们提供更多的信息。和gprof不同，它不需要在编译源代码时附加特殊选项，但加上调试选项是推荐的。Callgrind收集程序运行时的一些数据，建立函数调用关系图，还可以有选择地进行cache模拟。在运行结束时，它会把分析数据写入一个文件。callgrind_annotate可以把这个文件的内容转化成可读的形式。

### 3.Cachegrind

Cache分析器，它模拟CPU中的一级缓存I1，Dl和二级缓存，能够精确地指出程序中cache的丢失和命中。如果需要，它还能够为我们提供cache丢失次数，内存引用次数，以及每行代码，每个函数，每个模块，整个程序产生的指令数。这对优化程序有很大的帮助。

### 4.Helgrind

它主要用来检查多线程程序中出现的竞争问题。Helgrind寻找内存中被多个线程访问，而又没有一贯加锁的区域，这些区域往往是线程之间失去同步的地方，而且会导致难以发掘的错误。Helgrind实现了名为“Eraser”的竞争检测算法，并做了进一步改进，减少了报告错误的次数。不过，Helgrind仍然处于实验阶段。

### 5.Massif

堆栈分析器，它能测量程序在堆栈中使用了多少内存，告诉我们堆块，堆管理块和栈的大小。Massif能帮助我们减少内存的使用，在带有虚拟内存的现代系统中，它还能够加速我们程序的运行，减少程序停留在交换区中的几率。
此外，lackey和nulgrind也会提供。Lackey是小型工具，很少用到；Nulgrind只是为开发者展示如何创建一个工具。

## 1.2原理

Memcheck 能够检测出内存问题，关键在于其建立了两个全局表。

### Valid-Value 表

对于进程的整个地址空间中的每一个字节(byte)，都有与之对应的 8 个 bits；对于CPU的每个寄存器，也有一个与之对应的bit向量。这些bits负责记录该字节或者寄存器值是否具有有效的、已初始化的值。

### Valid-Address 表

对于进程整个地址空间中的每一个字节(byte)，还有与之对应的1个bit，负责记录该地址是否能够被读写。

#### 检测原理：

当要读写内存中某个字节时，首先检查这个字节对应的 A bit。如果该A bit显示该位置是无效位置，memcheck则报告读写错误。
内核（core）类似于一个虚拟的 CPU 环境，这样当内存中的某个字节被加载到真实的 CPU 中时，该字节对应的 V bit 也被加载到虚拟的 CPU 环境中。一旦寄存器中的值，被用来产生内存地址，或者该值能够影响程序输出，则 memcheck 会检查对应的V bits，如果该值尚未初始化，则会报告使用未初始化内存错误。

## 2.安装使用

### 2.1下载

去官网https://valgrind.org/下载最新版本

centos下载：

```shell
yum install valgrind
```

Ubuntu下载

```shell
sudo apt-get install valgrind
```

### 2.2使用

用法:valgrind[options] prog-and-args [options]: 

常用选项，适用于所有Valgrind工具

-tool=<name> 最常用的选项。运行 valgrind中名为toolname的工具。默认memcheck。
h –help 显示帮助信息。

-version 显示valgrind内核的版本，每个工具都有各自的版本。

q –quiet 安静地运行，只打印错误信息。

v –verbose 更详细的信息, 增加错误数统计。

-trace-children=no|yes 跟踪子线程? [no]

-track-fds=no|yes 跟踪打开的文件描述？[no]

-time-stamp=no|yes 增加时间戳到LOG信息? [no]

-log-fd=<number> 输出LOG到描述符文件 [2=stderr]

-log-file=<file> 将输出的信息写入到filename.PID的文件里，PID是运行程序的进行ID

-log-file-exactly=<file> 输出LOG信息到 file

-log-file-qualifier=<VAR> 取得环境变量的值来做为输出信息的文件名。 [none]

-log-socket=ipaddr:port 输出LOG到socket ，ipaddr:port

LOG信息输出:

-xml=yes 将信息以xml格式输出，只有memcheck可用

-num-callers=<number> show <number> callers in stack traces [12]

-error-limit=no|yes 如果太多错误，则停止显示新错误? [yes]

-error-exitcode=<number> 如果发现错误则返回错误代码 [0=disable]

-db-attach=no|yes 当出现错误，valgrind会自动启动调试器gdb。[no]

-db-command=<command> 启动调试器的命令行选项[gdb -nw %f %p]

适用于Memcheck工具的相关选项：

-leak-check=no|summary|full 要求对leak给出详细信息? [summary]

-leak-resolution=low|med|high how much bt merging in leak check [low]

-show-reachable=no|yes show reachable blocks in leak check? [no]

## 3.应用例子

### 3.1数组越界

malloc1.c

```c
#include <stdio.h>
  
int main(int argc, char **argv) {

    int *x = malloc(8*sizeof(int));

    x[9] = 0; // 数组下标越界 没有释放内存x

    free(x);
    return 0;
}

```

编译：

```shell
gcc -Wall malloc1.c -g -o malloc1
```

使用Valgrind检查程序BUG：

```shell
# --leak-check=full 所有泄露检查
valgrind --tool=memcheck --leak-check=full ./malloc1
```

运行结果：

```shell
[root@hackett valgrind]# valgrind --tool=memcheck --leak-check=full ./malloc1
==550168== Memcheck, a memory error detector
==550168== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==550168== Using Valgrind-3.17.0 and LibVEX; rerun with -h for copyright info
==550168== Command: ./malloc1
==550168== 
==550168== Invalid write of size 4
==550168==    at 0x4005FB: main (malloc1.c:7)
==550168==  Address 0x520b064 is 4 bytes after a block of size 32 alloc'd
==550168==    at 0x4C360A5: malloc (vg_replace_malloc.c:380)
==550168==    by 0x4005EE: main (malloc1.c:5)
==550168== 
==550168== 
==550168== HEAP SUMMARY:
==550168==     in use at exit: 0 bytes in 0 blocks
==550168==   total heap usage: 1 allocs, 1 frees, 32 bytes allocated
==550168== 
==550168== All heap blocks were freed -- no leaks are possible
==550168== 
==550168== For lists of detected and suppressed errors, rerun with: -s
==550168== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
```

分析：

1、每一行开头的数字代表是进程ID，这里的进程ID为550038

2、==550038== Invalid write of size 4
      ==550038==    at 0x4005BB: main (malloc1.c:7)

这两行显示的是错误出现的位置，就是 x 分配了 10 byte 的空间，但是向第 11 个 byte 写数据, 所以就会显示 Invalid write 的错误，代码在malloc1.c的第7行

### 3.2内存释放后进行读写

malloc2.c

```c
#include <stdio.h>
  
int main(int argc, char **argv) {

    char *p = malloc(1);

    *p = 'a';

    char c = *p;

    printf("[%c]\n",c);

    free(p); // 释放

    c = *p; //取值 读

    return 0;
}
```

编译：

```shell
gcc -Wall malloc2.c -g -o malloc2
```

使用Valgrind检查程序BUG：

```shell
# --leak-check=full 所有泄露检查
valgrind --tool=memcheck --leak-check=full ./malloc2
```

运行结果：

```shell
[root@hackett valgrind]# valgrind --tool=memcheck --leak-check=full ./malloc2
==550063== Memcheck, a memory error detector
==550063== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==550063== Using Valgrind-3.17.0 and LibVEX; rerun with -h for copyright info
==550063== Command: ./malloc2
==550063== 
[a]
==550063== Invalid read of size 1
==550063==    at 0x400679: main (malloc2.c:15)
==550063==  Address 0x520b040 is 0 bytes inside a block of size 1 free'd
==550063==    at 0x4C38A03: free (vg_replace_malloc.c:755)
==550063==    by 0x400674: main (malloc2.c:13)
==550063==  Block was alloc'd at
==550063==    at 0x4C360A5: malloc (vg_replace_malloc.c:380)
==550063==    by 0x40063E: main (malloc2.c:5)
==550063== 
==550063== 
==550063== HEAP SUMMARY:
==550063==     in use at exit: 0 bytes in 0 blocks
==550063==   total heap usage: 2 allocs, 2 frees, 1,025 bytes allocated
==550063== 
==550063== All heap blocks were freed -- no leaks are possible
==550063== 
==550063== For lists of detected and suppressed errors, rerun with: -s
==550063== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
```

分析：

1、每一行开头的数字代表是进程ID，这里的进程ID为550063

2、==550063== Invalid read of size 1
==550063==    at 0x400679: main (malloc2.c:15)
==550063==  Address 0x520b040 is 0 bytes inside a block of size 1 free'd
==550063==    at 0x4C38A03: free (vg_replace_malloc.c:755)
==550063==    by 0x400674: main (malloc2.c:13)

这五行显示的是错误出现的位置，就是 p指向的内存已经释放了，还对其进行读操作，所以就会显示 Invalid read  的错误，代码在malloc2.c的第15行

### 3.3无效读写

malloc3.c

```c
#include <stdio.h>
  
int main(int argc, char **argv) {

    char *p = malloc(1);

    *p = 'a';

    char c = *(p+1); // 地址加1 无效读

    printf("[%c]\n",c);

    free(p); // 释放
    
    return 0;
}
```

编译：

```shell
gcc -Wall malloc3.c -g -o malloc3
```

使用Valgrind检查程序BUG：

```shell
# --leak-check=full 所有泄露检查
valgrind --tool=memcheck --leak-check=full ./malloc3
```

运行结果：

```shell
[root@iZwz97bu0gr8vx0j8l6kkzZ valgrind]# valgrind --tool=memcheck --leak-check=full ./malloc3
==550135== Memcheck, a memory error detector
==550135== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==550135== Using Valgrind-3.17.0 and LibVEX; rerun with -h for copyright info
==550135== Command: ./malloc3
==550135== 
==550135== Invalid read of size 1
==550135==    at 0x40064E: main (malloc3.c:9)
==550135==  Address 0x520b041 is 0 bytes after a block of size 1 alloc'd
==550135==    at 0x4C360A5: malloc (vg_replace_malloc.c:380)
==550135==    by 0x40063E: main (malloc3.c:5)
==550135== 
[]
==550135== 
==550135== HEAP SUMMARY:
==550135==     in use at exit: 0 bytes in 0 blocks
==550135==   total heap usage: 2 allocs, 2 frees, 1,025 bytes allocated
==550135== 
==550135== All heap blocks were freed -- no leaks are possible
==550135== 
==550135== For lists of detected and suppressed errors, rerun with: -s
==550135== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
```

分析：

1、每一行开头的数字代表是进程ID，这里的进程ID为550135

2、==550135== Invalid read of size 1
==550135==    at 0x40064E: main (malloc3.c:9)

这五行显示的是错误出现的位置，就是对未分配内存的空间进行读取，所以就会显示 Invalid read 的错误，代码在malloc3.c的第9行

### 3.4内存泄漏

malloc4.c

```c
#include <stdio.h>
  
int main(int argc, char **argv) {

    char *p = malloc(1);

    *p = 'a';

    char c = *p; 

    printf("[%c]\n",c); // 申请后没有释放p 内存泄漏


    return 0;
}
```

编译：

```shell
gcc -Wall malloc4.c -g -o malloc4
```

使用Valgrind检查程序BUG：

```shell
# --leak-check=full 所有泄露检查
valgrind --tool=memcheck --leak-check=full ./malloc4
```

运行结果：

```shell
[root@hackett valgrind]# valgrind --tool=memcheck --leak-check=full ./malloc4
==550195== Memcheck, a memory error detector
==550195== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==550195== Using Valgrind-3.17.0 and LibVEX; rerun with -h for copyright info
==550195== Command: ./malloc4
==550195== 
[a]
==550195== 
==550195== HEAP SUMMARY:
==550195==     in use at exit: 1 bytes in 1 blocks
==550195==   total heap usage: 2 allocs, 1 frees, 1,025 bytes allocated
==550195== 
==550195== 1 bytes in 1 blocks are definitely lost in loss record 1 of 1
==550195==    at 0x4C360A5: malloc (vg_replace_malloc.c:380)
==550195==    by 0x4005EE: main (malloc4.c:5)
==550195== 
==550195== LEAK SUMMARY:
==550195==    definitely lost: 1 bytes in 1 blocks
==550195==    indirectly lost: 0 bytes in 0 blocks
==550195==      possibly lost: 0 bytes in 0 blocks
==550195==    still reachable: 0 bytes in 0 blocks
==550195==         suppressed: 0 bytes in 0 blocks
==550195== 
==550195== For lists of detected and suppressed errors, rerun with: -s
==550195== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
```

分析：

1、每一行开头的数字代表是进程ID，这里的进程ID为550195

2、==550195== 1 bytes in 1 blocks are definitely lost in loss record 1 of 1
==550195==    at 0x4C360A5: malloc (vg_replace_malloc.c:380)
==550195==    by 0x4005EE: main (malloc4.c:5)

这三行显示的是错误出现的位置，是 stack trace内存泄漏，代码在malloc1.c的第5行，申请了内存没有进行对应的free

### 3.5内存多次释放

malloc5.c

```c
#include <stdio.h>
  
int main(int argc, char **argv) {

    char *p = malloc(1);

    *p = 'a';

    char c = *p; // 地址加1

    printf("[%c]\n",c);

    free(p);
    free(p);// 内存多次释放
    free(p);// 内存多次释放

    return 0;
}
```

编译：

```shell
gcc -Wall malloc5.c -g -o malloc5
```

使用Valgrind检查程序BUG：

```shell
# --leak-check=full 所有泄露检查
valgrind --tool=memcheck --leak-check=full ./malloc5
```

运行结果：

```shell
[root@hackett valgrind]# valgrind --tool=memcheck --leak-check=full ./malloc5
==550227== Memcheck, a memory error detector
==550227== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==550227== Using Valgrind-3.17.0 and LibVEX; rerun with -h for copyright info
==550227== Command: ./malloc5
==550227== 
[a]
==550227== Invalid free() / delete / delete[] / realloc()
==550227==    at 0x4C38A03: free (vg_replace_malloc.c:755)
==550227==    by 0x400680: main (malloc5.c:14)
==550227==  Address 0x520b040 is 0 bytes inside a block of size 1 free'd
==550227==    at 0x4C38A03: free (vg_replace_malloc.c:755)
==550227==    by 0x400674: main (malloc5.c:13)
==550227==  Block was alloc'd at
==550227==    at 0x4C360A5: malloc (vg_replace_malloc.c:380)
==550227==    by 0x40063E: main (malloc5.c:5)
==550227== 
==550227== Invalid free() / delete / delete[] / realloc()
==550227==    at 0x4C38A03: free (vg_replace_malloc.c:755)
==550227==    by 0x40068C: main (malloc5.c:15)
==550227==  Address 0x520b040 is 0 bytes inside a block of size 1 free'd
==550227==    at 0x4C38A03: free (vg_replace_malloc.c:755)
==550227==    by 0x400674: main (malloc5.c:13)
==550227==  Block was alloc'd at
==550227==    at 0x4C360A5: malloc (vg_replace_malloc.c:380)
==550227==    by 0x40063E: main (malloc5.c:5)
==550227== 
==550227== 
==550227== HEAP SUMMARY:
==550227==     in use at exit: 0 bytes in 0 blocks
==550227==   total heap usage: 2 allocs, 4 frees, 1,025 bytes allocated
==550227== 
==550227== All heap blocks were freed -- no leaks are possible
==550227== 
==550227== For lists of detected and suppressed errors, rerun with: -s
==550227== ERROR SUMMARY: 2 errors from 2 contexts (suppressed: 0 from 0)
```

分析：

1、每一行开头的数字代表是进程ID，这里的进程ID为550227

2、==550227== Invalid free() / delete / delete[] / realloc()
==550227==    at 0x4C38A03: free (vg_replace_malloc.c:755)
==550227==    by 0x400680: main (malloc5.c:14)

这三行显示的是第一个错误出现的位置，就是多次释放内存问题, 所以就会显示 Invalid free() / delete / delete[] / realloc() 的错误，代码在malloc5.c的第14行

3、==550227== Invalid free() / delete / delete[] / realloc()
==550227==    at 0x4C38A03: free (vg_replace_malloc.c:755)
==550227==    by 0x40068C: main (malloc5.c:15)

这三行显示的是第二个错误出现的位置，就是多次释放内存问题, 所以就会显示 Invalid free() / delete / delete[] / realloc() 的错误，代码在malloc5.c的第15行

### 3.6动态内存管理

常见的内存分配方式分三种：静态存储，栈上分配，堆上分配。全局变量属于静态存储，它们是在编译时就被分配了存储空间，函数内的局部变量属于栈上分配，而最灵活的内存使用方式当属堆上分配，也叫做内存动态分配了。常用的内存动态分配函数包括：malloc, alloc, realloc, new等，动态释放函数包括free, delete。

一旦成功申请了动态内存，我们就需要自己对其进行内存管理，而这又是最容易犯错误的。

malloc6.c

```c
#include <stdio.h>
// 内存动态管理

int main(int argc, char **argv) {

    int i;
    char *p = (char *)malloc(10);

    char *pt = p;
    for(i = 0; i < 10; i++) {
        p[i] = 'A'+i;
    }

    free(p);

    pt[1] = 'x';

    free(pt);

    return 0;
}
```

编译：

```shell
gcc -Wall malloc6.c -g -o malloc6
```

使用Valgrind检查程序BUG：

```shell
# --leak-check=full 所有泄露检查
valgrind --tool=memcheck --leak-check=full ./malloc6
```

运行结果：

```shell
[root@hackett valgrind]# valgrind --tool=memcheck --leak-check=full ./malloc6
==550239== Memcheck, a memory error detector
==550239== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==550239== Using Valgrind-3.17.0 and LibVEX; rerun with -h for copyright info
==550239== Command: ./malloc6
==550239== 
==550239== Invalid write of size 1
==550239==    at 0x400639: main (malloc6.c:16)
==550239==  Address 0x520b041 is 1 bytes inside a block of size 10 free'd
==550239==    at 0x4C38A03: free (vg_replace_malloc.c:755)
==550239==    by 0x400630: main (malloc6.c:14)
==550239==  Block was alloc'd at
==550239==    at 0x4C360A5: malloc (vg_replace_malloc.c:380)
==550239==    by 0x4005EE: main (malloc6.c:7)
==550239== 
==550239== Invalid free() / delete / delete[] / realloc()
==550239==    at 0x4C38A03: free (vg_replace_malloc.c:755)
==550239==    by 0x400647: main (malloc6.c:18)
==550239==  Address 0x520b040 is 0 bytes inside a block of size 10 free'd
==550239==    at 0x4C38A03: free (vg_replace_malloc.c:755)
==550239==    by 0x400630: main (malloc6.c:14)
==550239==  Block was alloc'd at
==550239==    at 0x4C360A5: malloc (vg_replace_malloc.c:380)
==550239==    by 0x4005EE: main (malloc6.c:7)
==550239== 
==550239== 
==550239== HEAP SUMMARY:
==550239==     in use at exit: 0 bytes in 0 blocks
==550239==   total heap usage: 1 allocs, 2 frees, 10 bytes allocated
==550239== 
==550239== All heap blocks were freed -- no leaks are possible
==550239== 
==550239== For lists of detected and suppressed errors, rerun with: -s
==550239== ERROR SUMMARY: 2 errors from 2 contexts (suppressed: 0 from 0)
```

分析：

1、每一行开头的数字代表是进程ID，这里的进程ID为550239

2、==550239== Invalid write of size 1
==550239==    at 0x400639: main (malloc6.c:16)

这一行显示这里有一个错误，就是发生非法写操作, 所以就会显示 Invalid write 的错误，代码在malloc6.c的第16行

3、==550239== Invalid free() / delete / delete[] / realloc()
==550239==    at 0x4C38A03: free (vg_replace_malloc.c:755)
==550239==    by 0x400647: main (malloc6.c:18)
==550239==  Address 0x520b040 is 0 bytes inside a block of size 10 free'd
==550239==    at 0x4C38A03: free (vg_replace_malloc.c:755)
==550239==    by 0x400630: main (malloc6.c:14)

这五行显示的是第二个错误出现的位置，指针p和pt指向的是同一块内存，却被先后释放两次。系统会在堆上维护一个动态内存链表，如果被释放，就意味着该块内存可以继续被分配给其他部分，如果内存被释放后再访问，就可能覆盖其他部分的信息，这是一种严重的错误，上述程序第16行中就在释放后仍然写这块内存。

总结：

1.申请内存在使用完成后就要释放。如果没有释放，或少释放了就是内存泄露；多释放也会产生问题。

2.注意数组的大小，别越界读写访问非法内存

3.malloc和new要对应free和delete使用。



如果你觉得文章还不错，可以给个"**三连**"，文章同步到个人微信公众号[**加班猿**]

我是**hackett**，我们下期见