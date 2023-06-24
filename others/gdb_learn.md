# GDB

## 基本gdb命令

### 运行有关命令

```shell
run（简写r）： 运行程序，当遇到断点后，程序会在断点处停止运行，等待用户输入下一步的命令。

continue（简写c）：继续执行，到下一个断点处（或运行结束）

next（简写n）： 单步跟踪程序，当遇到函数调用时，直接调用，不进入此函数体；

step（简写s）：单步调试如果有函数调用，则进入函数；与命令n不同，n是不进入调用的函数的

until：运行程序直到退出循环体; / until+行号： 运行至某行

finish： 运行程序，直到当前函数完成返回，并打印函数返回时的堆栈地址和返回值及参数值等信息。

quit：简记为 q ，退出gdb


```

### 断点有关命令

```shell


break n（简写b n）:在第n行处设置断点 ;可以带上代码路径和代码名称： b OAGUPDATE.cpp:578）

break func：在函数func()的入口处设置断点，如：break cb_button

delete 断点号n：删除第n个断点

disable 断点号n：暂停第n个断点

enable 断点号n：开启第n个断点

clear 行号n：清除第n行的断点

info breakpoints（简写info b） ：显示当前程序的断点设置情况


```



### 显示有关的命令

```shell
list（简写l）：列出程序的源代码，默认每次显示10行。

list 行号：将显示当前文件以“行号”为中心的前后10行代码，如：list 12

list 函数名：将显示“函数名”所在函数的源代码，如：list main

print 表达式：简记为 p ，其中“表达式”可以是任何当前正在被测试程序的有效表达式，比如当前正在调试C语言的程序，那么“表达式”可以是任何C语言的有效表达式，包括数字，变量甚至是函数调用。

print a：将显示整数 a 的值

print name：将显示字符串 name 的值

display 表达式：在单步运行时将非常有用，使用display命令设置一个表达式后，它将在每次单步进行指令后，紧接着输出被设置的表达式及值。如： display a

watch 表达式：设置一个监视点，一旦被监视的“表达式”的值改变，gdb将强行终止正在被调试的程序。如： watch a

where/bt ：当前运行的堆栈列表

set args 参数:指定运行时的参数

show args：查看设置好的参数

info program： 来查看程序的是否在运行，进程号，被暂停的原因
```



## 多进程有关的命令

### follow-fork-mode

```shell
set follow-fork-mode [parent| child]   设置调试进程
show follow-fork-mode             	   查看当前进程
```



### detach-on-fork

```shell
set detach-on-fork [on|off] 	调试当前进程时，其它进程是运行还是挂起
```



### inferiors

```shell
info inferiors    查询当前进程
inferior <inferior num>  切换要调试的进程
```



## 多线程有关的命令

```shell
info threads  查看线程

thread <ID>   切换线程

set secheduler-locking off|on|step  设置运行的线程
```





## esp32_jtag 示例

我们用jtag章节中的代码作为示例，主要演示设置断点，观察变量，观察堆栈区等操作

### 断点演示

#### 设置断点

设置断点，在第65 83 89行设置断点

![image-20230607150309682](https://ayu-990121-1302263000.cos.ap-nanjing.myqcloud.com/makedown/20230624223538.png)



#### 查看断点

可以使用info breakpoint查看设置的断点

![image-20230607150543238](https://ayu-990121-1302263000.cos.ap-nanjing.myqcloud.com/makedown/20230624223540.png)



#### 删除断点

同样可以使用delete 删除相关断点

![image-20230607150640369](https://ayu-990121-1302263000.cos.ap-nanjing.myqcloud.com/makedown/20230624223541.png)



### 运行，查看变量和堆栈区

使用c来运行程序到断点处

#### 查看变量

使用以下命令来观察相关变量

```
display + 变量名 
```

使用以下命令来观察堆栈

```
bt
```

使用以下命令来观察内存

```c
x /20xh 0x1000000

/*
格式: x /nfu
x 是 examine 的缩写，意思是检查。
n表示要显示的内存单元的个数，比如：20

f表示显示方式, 可取如下值：
x 按十六进制格式显示变量。
d 按十进制格式显示变量。
u 按十进制格式显示无符号整型。
o 按八进制格式显示变量。
t 按二进制格式显示变量。
a 按十六进制格式显示变量。
i 指令地址格式
c 按字符格式显示变量。
f 按浮点数格式显示变量。


u表示一个地址单元的长度：
b表示单字节，
h表示双字节，
w表示四字节，
g表示八字节

*/
```

![image-20230607151004922](https://ayu-990121-1302263000.cos.ap-nanjing.myqcloud.com/makedown/20230624223544.png)

![image-20230607151216235](https://ayu-990121-1302263000.cos.ap-nanjing.myqcloud.com/makedown/20230624223545.png)
