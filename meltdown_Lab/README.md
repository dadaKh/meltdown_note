## CacheTime
这是一个测量 Cache Threshold 的简单Toy

编译并运行它：
```
gcc -march=native CacheTime.c -o cachetime

./cachetime
```
你可以看到一下输出：
```
Access time for array[0*4096]: 246 CPU cycles
Access time for array[1*4096]: 308 CPU cycles
Access time for array[2*4096]: 322 CPU cycles
Access time for array[3*4096]: 66 CPU cycles
Access time for array[4*4096]: 250 CPU cycles
Access time for array[5*4096]: 254 CPU cycles
Access time for array[6*4096]: 250 CPU cycles
Access time for array[7*4096]: 66 CPU cycles
Access time for array[8*4096]: 272 CPU cycles
Access time for array[9*4096]: 256 CPU cycles
```
于是，我们可以看出 Cache Threshold 大约为 70~100 个时钟周期

## Kernel
这是一个内核模块，我们用于模拟 `Secret` 数据，我们将定义好的字符串放在内核模块中，编译并运行模块，它会在 `ring` 缓冲区中打印 `Secret address`。

我们根据这个地址，用 meltdown 攻击获取到我们定义好的字符串，以此来模拟 meltdown 攻击。

+ 首先编译模块：
    ```
    make
    ```
+ 安装模块：
    ```
    sudo insmod YYHKernel.ko
    ```
+ 查看打印的地址：
  ```
  sudo dmesg | grep "secret data addr"
  ```
    你会看到类似的如下输出：
    ```
    [1102539.849372] secret data addr:ffffffffc0581000
    ```
`0xffffffffc0581000` 就是我们 `Secret data` 的地址，但是通过地址直接访问会出现段错误，因为它在内核中。

在 Test 中做了这样的尝试。

下面我们采用 Meltdown 获取`Secret data`

## Meltdown
编译：
```
make
```
运行：
```
./meltdown 0xffffffffc0581000
```
你会看到类似的如下输出：
```
The secret value is K
The secret value is H
The secret value is A
The secret value is O
The secret value is S
The secret value is Y
The secret value is Y
The secret value is H
```

