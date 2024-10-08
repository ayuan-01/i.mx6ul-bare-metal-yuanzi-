# C语言知识点，边学边记

#### 函数指针

- 函数指针是指向函数的指针。它允许动态地调用函数，即在运行中决定调用哪个函数。

  ```C
  typedef void (*system_irq_handler_t)(unsigned int gicciar, void *param);
  ```

  这条语句将一个函数指针类型定义为 `system_irq_handler_t`，它指向一个返回类型为 `void` 的函数。

  **只要满足输入参数类型，返回参数类型与函数指针定义相匹配，都可以被函数指针指向。**

  示例：

  ```c
  /* 定义函数 */
  #include <stdio.h>
  
  // 定义符合 system_irq_handler_t 类型的函数
  void my_irq_handler(unsigned int gicciar, void *param) {
      printf("IRQ Handler called with gicciar: %u, param: %p\n", gicciar, param);
  }
  
  ```

  ```C
  // 定义函数指针类型
  typedef void (*system_irq_handler_t)(unsigned int gicciar, void *param);
  ```

  ```C
  // 声明和使用函数指针
  int main() {
      // 声明一个 system_irq_handler_t 类型的函数指针
      system_irq_handler_t handler;
  
      // 将函数指针指向 my_irq_handler 函数
      handler = my_irq_handler;
  
      // 调用函数指针
      handler(123, (void *)0xDEADBEEF);
  
      return 0;
  }
  ```

#### 内联函数

在C语言中，`inline`关键字用于建议编译器将函数的代码直接插入到调用点，而不是通过常规的函数调用机制来调用。这样可以减少函数调用的开销，比如避免了调用和返回时的栈操作，以及可能的跳转指令，从而提高程序的执行效率。

`inline` 详细解释

- **减少函数调用开销**：通常，函数调用涉及将参数压栈、跳转到函数代码、执行代码、返回并清理栈。`inline`函数通过将函数代码直接插入到调用点，避免了这些开销。
- **编译器优化建议**：`inline` 只是一个建议，编译器可以选择忽略它。如果函数体过大，编译器可能会忽略 `inline` 提示，因为内联较大的函数可能会增加程序的代码量，导致性能下降。
- **注意事项**：
  - 递归函数不能被内联，因为编译器无法确定递归的结束条件。
  - 过多使用 `inline` 可能导致代码膨胀，反而降低性能。

#### while

```
i = 5;
while(i--) {}
```

`i` 的值会是 4, 3, 2, 1, 0。解释如下：

- `i--` 是一个后缀递减操作符。它会在每次迭代时首先使用 `i` 的当前值，然后将 `i` 减 1。
- 当 `i` 的值为 5 时，`i--` 会先将 `i` 的值 5 用于判断循环条件，然后将 `i` 减 1 变成 4。
- 循环会继续执行直到 `i` 的值变成 0。在 `i` 为 0 时，`i--` 会将 `i` 用于判断条件（0）后再将 `i` 减 1。此时 `i` 变成 -1，但循环已经结束，因为判断条件 `i--` 在每次迭代后会使用 `i` 的值。

所以，`i` 的值在循环结束时是 `-1`，但循环过程中 `i` 的值为 5, 4, 3, 2, 1, 0。

# 正点原子Makefile文件详解

```makefile
CROSS_COMPILE 	?= arm-linux-gnueabihf-		# 变量，交叉编译工具链的前缀
TARGET 			?= i2c						# 生成的目标文件的名称
CC				:= $(CROSS_COMPILE)gcc		# 定义gcc编译器
LD				:= $(CROSS_COMPILE)ld		# 链接器
OBJCOPY			:= $(CROSS_COMPILE)objcopy	# 生成elf可执行文件
OBJDUMP			:= $(CROSS_COMPILE)objdump	# 反汇编

# LIBPATH：链接器的库路径和库文件。在这里指定了库路径和 -lgcc 选项。
# 告诉链接器在链接阶段需要使用哪些库和哪些库文件
# -lgcc -l是链接器选项的标志，表示链接一个库 gcc是要链接库的名称，它通常是GCC编译器自带的运行时库
# -L 是链接器选项的标志，表示指定一个库文件搜索路径
# -lgcc：告诉链接器链接 GCC 的运行时库（libgcc），这是必要的，因为它提供了一些基本的支持功能。
# -L /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/lib/gcc/arm-linux-gnueabihf/4.9.4：指定了一个库搜索路径，链接器会在这个路径下查找 libgcc 及其他库文件。
LIBPATH			:= -lgcc -L /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/lib/gcc/arm-linux-gnueabihf/4.9.4

# 头文件所在的路径
INCUDIRS		:= 	imx6u 		\
					bsp/clk		\
					bsp/delay	\
					bsp/led		\
					bsp/beep	\
					bsp/key		\
					bsp/gpio	\
					bsp/int		\
					bsp/exit	\
					bsp/epit	\
					bsp/keyfilter\
					bsp/uart	\
					bsp/lcd		\
					bsp/rtc		\
					bsp/ap3216c	\
					bsp/i2c		\
					stdio/include

# 源文件所在的路径
SRCDIRS			:=	project		\
					bsp/clk		\
					bsp/delay	\
					bsp/led		\
					bsp/beep	\
					bsp/key		\
					bsp/gpio	\
					bsp/int		\
					bsp/exit	\
					bsp/epit	\
					bsp/keyfilter\
					bsp/uart	\
					bsp/lcd		\
					bsp/rtc		\
					bsp/ap3216c	\
					bsp/i2c		\
					stdio/lib
# 这个函数用于替换字符串中的模式，%，表示匹配 INCUDIRS 中的每一个元素。即将INCUDIRS中的每一个元素前都加上-I
# -I的作用-I选项在编译器中用于指定头文件的搜索路径。它告诉编译器在哪里查找 #include 指令中指定的头文件。
INCLUDE			:= $(patsubst %, -I %, $(INCUDIRS))

# 查找所有汇编文件和C文件（文件路径）
# $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.S))
# foreach函数遍历列表中的每一个元素并 执行指定的操作（wildcard函数操作）
# 查找SRCDIRS中每个路径并且复制给dir
# wildcard函数用于匹配符合指定模式的文件，找到每个dir路径中的.S文件

# 工作流程
# 遍历 $(SRCDIRS) 变量中的每一个目录:
# $(foreach dir, $(SRCDIRS), ...) 会依次将 $(SRCDIRS) 中的每个目录赋值给 dir。

# 对于每个目录，使用 wildcard 查找汇编源文件:
# $(wildcard $(dir)/*.S) 查找当前目录 $(dir) 下所有扩展名为 .S 的文件。

# 将所有找到的文件合并成一个列表:
# foreach 函数将每次 wildcard 找到的文件列表合并成一个大的文件列表，最终赋值给 SFILES 变量。
SFILES			:= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.S))
CFILES			:= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))

# 获取源文件的文件名（不包括路径）
SFILENDIR		:= $(notdir $(SFILES))
CFILENDIR		:= $(notdir $(CFILES))

# 对应的目标文件（.o），放在obj/下
SOBJS			:= $(patsubst %, obj/%, $(SFILENDIR:.S=.o))
COBJS			:= $(patsubst %, obj/%, $(CFILENDIR:.c=.o))

# 所有目标文件列表
OBJS 			:= $(SOBJS)$(COBJS)

# 指定make查找源文件的路径
VPATH			:= $(SRCDIRS)

# 构建规则
$(TARGET).bin: $(OBJS)
	$(LD) -Timx6u.lds -o $(TARGET).elf $^ $(LIBPATH)
	$(OBJCOPY) -O binary -S $(TARGET).elf $@
	$(OBJDUMP) -D -m arm $(TARGET).elf > $(TARGET).dis

$(COBJS): obj/%.o : %.c
	$(CC) $(INCLUDE) -Wa,-mimplicit-it=thumb -fno-builtin -c -Wall -nostdlib -O2 -o $@ $<
$(SOBJS): obj/%.o : %.S
	$(CC) $(INCLUDE) -fno-builtin -c -Wall -nostdlib -O2 -o $@ $<

.PHONY:clean
clean:
	rm -rf $(OBJS) $(TARGET).elf $(TARGET).bin $(TARGET).dis load.imx
	rm -rf *.elf *.bin *.dis load.imx

```



# 启动流程

#### 汇编启动程序

- 设置中断向量表

- 定义中断服务程序
  - 复位中断服务程序
    - 关闭全局中断(cpsid i)。
    - 使用CP15协处理器管理STCLR寄存器，关闭ICache，DCache，MMU。
    - **清除BSS段。**链接脚本(.lds)中设置BSS段的起始和终止地址，使用循环清除这个地址范围内的所有内容。
    - 分别设置处理器的9种工作模式并定义对应的SP指针。查看各种工作模式下16个寄存器的共用模式。最后设置CPU工作模式为**SVC模式**
    - 打开IRQ中断(cpsie i)。
    - 跳转到main函数。
    
  - 编写IRQ中断服务函数
  
    - 1. 保护现场。触发中断的机制首先会保存(push)一些寄存器，一些不会自动保存的寄存器(r0-r3, r12)需要我们手动保存，`push {lr}` 用于保存函数调用返回地址。
  
         ```csv
         mrs r0, spsr	// 保存spsr
         push {r0}
         ```

      2. 读取GICC_IAR寄存器，这个寄存器保存着当前发生中断的中断号，通过这个中断号来决定来调用哪个中断服务函数。
  
      3. 进入svc模式，push {lr}，跳转执行system_irqhandler。
  
      4. 执行完后pop {lr}。回到irq模式。
  
      5. 中断执行完成，写GICC_EOIR寄存器。处理完具体中断后，需要将对应的中断ID值写入到GICC_EOIR中 通知GIC当前处理中断已经处理完毕，允许GIC分配下一个中断。
  
      6. 恢复现场。
  
         ```csv
         pop {r0-r3, r12}
         subs pc, lr, #4
         /* pc = lr-4是因为arm的指令是三级流水线结构，假如在执行0x2000指令时触发中断，pc此时是  * 指向0x2008的，如果中断结束后返回到0x2008开始执行那么将会漏掉一个指令，这个后果会很严重 */
         ```
  
  
  - C语言中编写中断相关函数。
  
    - 中断初始化函数。
  
    - 定义中断处理函数(函数指针)，有两个参数，包括iar，param。创建包括函数指针和参数的中断处理结构体。
  
      ```C
      typedef struct _sys_irq_handler
      {
          system_irq_handler_t irqHandler;            /* 中断处理函数 */
          void *userParam;                            /* 中断处理函数的参数 */
      }sys_irq_handle_t;
      ```
  
    - 定义中断处理函数表(存放中断处理函数结构体类型的数组)。
  
      ```c
      static sys_irq_handle_t irqTable[NUMBER_OF_INT_VECTORS];            /* 160个 */
      ```
  
      相当于给了160个中断处理函数。对应着相应的中断号。
  
    - 初始化中断处理函数表。
  
      ```c
      void system_irq_init(void)
      {
          unsigned int i = 0;
          irqNesting = 0;
          for (i = 0; i < NUMBER_OF_INT_VECTORS; i++)
          {
              irqTable[i].irqHandler = default_irqHandler;
              irqTable[i].userParam  = NULL;
          } 
      }
      ```
  
    - 注册中断处理函数
  
      ```c
      void system_register_irqhandler(IRQn_Type irq, system_irq_handler_t handler, void *userparam)
      {
          irqTable[irq].irqHandler = handler;
          irqTable[irq].userParam  = userparam;
      }
      ```
  
    - 具体的中断处理函数
  
      ```c
      void system_irqhandler(unsigned int gicciar)
      {
          /* 检查中断ID */
          uint32_t intNum = gicciar & 0x3ff;
          if(intNum >= NUMBER_OF_INT_VECTORS)
          {
              return;
          }
          irqNesting++;
          /* 根据中断ID号，读取中断处理函数，然后执行 */
          irqTable[intNum].irqHandler(intNum, irqTable[intNum].userParam);
          irqNesting--;
      }
      ```
  
      

#### 链接脚本

##### 链接脚本具体解释

这个代码段是一个GNU LD（Linker Script）脚本，它定义了如何将程序的不同部分布局到内存中。具体来说，这个脚本定义了各个段（sections）在内存中的位置和对齐方式。

1. **起始地址设置**:

   ```ld
   . = 0X87800000;
   ```

   这行代码将链接器的当前位置（即内存地址）设置为`0x87800000`。这意味着接下来的所有段都会从这个地址开始放置。

2. **.text段**:

   ```ld
   .text : 
   {
       obj/start.o
       *(.text)
   }
   ```

   `.text`段通常包含程序的代码部分。这里指定了`obj/start.o`文件中的内容首先放入`.text`段，然后将所有输入文件中的`.text`段内容依次放入。

3. **.rodata段**:

   ```ld
   .rodata ALIGN(4) : {*(.rodata*)}
   ```

   `.rodata`段用于存储只读数据。这里将所有输入文件中的`.rodata`段内容放入，并且将其对齐到4字节边界。

4. **.data段**:

   ```ld
   .data ALIGN(4) : {*(.data)}
   ```

   `.data`段用于存储已初始化的全局变量和静态变量。这里将所有输入文件中的`.data`段内容放入，并且将其对齐到4字节边界。

5. **对齐指针**:

   ```ld
   . = ALIGN(4);
   ```

   这行代码将当前位置对齐到4字节边界。

6. **__bss_start符号**:

   ```ld
   __bss_start = .;
   ```

   定义一个符号`__bss_start`，其值为当前位置（即`bss`段的起始地址）。

7. **.bss段**:

   ```ld
   .bss ALIGN(4) : {*(.bss) *(common)}
   ```

   `.bss`段用于存储未初始化的全局变量和静态变量。这里将所有输入文件中的`.bss`段和`common`段内容放入，并且将其对齐到4字节边界。

8. **__bss_end符号**:

   ```ld
   __bss_end = .;
   ```

   定义一个符号`__bss_end`，其值为当前位置（即`bss`段的结束地址）。

#### CP15协处理器寄存器

CP15寄存器是ARM架构中的协处理器15寄存器，主要用于配置和管理系统控制、内存管理、调试等功能。CP15寄存器在ARM处理器中扮演着关键角色，以下是一些主要功能和作用：

1. **系统控制**：
   - 控制寄存器（如SCTLR）：用于配置处理器的运行模式和特性，例如启用或禁用缓存、MMU、异常处理等。
2. **内存管理**：
   - TLB（Translation Lookaside Buffer）控制：管理TLB的刷新、无效化等操作。
   - 页表基地址寄存器：存储页表的基地址，用于地址转换。
3. **中断和异常处理**：
   - 中断控制寄存器：配置中断的优先级、使能状态等。
   - *异常向量表基地址寄存器：指定异常向量表的地址*。
4. **性能监控**：
   - 性能监控寄存器：用于性能计数、事件监控，帮助开发人员优化代码性能。
5. **调试和跟踪**：
   - 调试控制寄存器：配置调试功能、断点、观察点等。
   - 跟踪控制寄存器：配置跟踪功能，用于程序执行流的记录和分析。
6. **电源管理**：
   - 电源控制寄存器：配置低功耗模式、节能特性等。

CP15寄存器在ARM处理器的运行和管理中发挥着重要作用，使系统能够灵活、高效地进行配置和控制。



#### SCTLR寄存器

SCTLR（System Control Register）是ARM架构中的一个重要寄存器，位于CP15协处理器中。它用于控制处理器的各种系统级特性和操作模式。SCTLR的作用非常广泛，下面是其主要功能和控制位：

1. **MMU启用**：
   - M位：控制MMU（内存管理单元）的启用和禁用。
     - 0：禁用MMU
     - 1：启用MMU
2. **对齐检查**：
   - A位：控制未对齐数据访问的检查。
     - 0：禁用对齐检查
     - 1：启用对齐检查
3. **数据缓存启用**：
   - C位：控制数据缓存的启用和禁用。
     - 0：禁用数据缓存
     - 1：启用数据缓存
4. **指令缓存启用**：
   - I位：控制指令缓存的启用和禁用。
     - 0：禁用指令缓存
     - 1：启用指令缓存
5. **系统保护**：
   - S位：配置系统保护，使系统区和用户区的访问权限不同。
     - 0：禁用系统保护
     - 1：启用系统保护
6. **写缓冲区**：
   - B位：控制写缓冲区的启用和禁用。
     - 0：禁用写缓冲区
     - 1：启用写缓冲区
7. **小端模式/大端模式**：
   - E位：配置处理器的字节序模式。
     - 0：小端模式
     - 1：大端模式
8. **对称多处理支持**：
   - SMP位：启用或禁用对称多处理（SMP）支持。
     - 0：禁用SMP
     - 1：启用SMP
9. **指令预取**：
   - Z位：控制指令预取。
     - 0：禁用预取
     - 1：启用预取
10. **分支预测**：
    - P位：控制分支预测功能。
      - 0：禁用分支预测
      - 1：启用分支预测

SCTLR寄存器中的这些控制位使得系统能够根据需求配置处理器的运行模式和特性，从而优化性能和安全性。在实际使用中，操作系统或固件会设置这些位，以确保处理器以预期的方式运行。



#### 中断偏移地址和程序入口地址

1. 中断向量表和程序起始地址通常不会设置相同的值，这会导致两者之间的冲突。然而，在某些特定情况下特别是在嵌入式系统中，可以通过特殊的配置和使用不同方法来实现这种设置。
2. 中断向量表在_start:中设置。

# GPIO中断程序

- 汇编文件

  - 在汇编启动程序中配置好中断向量表和中断服务函数IRQ_Hanlder。

- BSP_INT(中断配置文件)

  - 创建中断函数指针类型。
  - 创建中断函数结构体，包括中断服务函数和中断服务函数参数。
  - 创建变量记录中断嵌套数量。
  - 创建中断处理函数表，这是一个数组。
  - 创建初始化中断函数表函数，中断数量清零。
  - 中断初始化函数(初始化GIC，初始化中断函数表，设置中断向量偏移)。
  - 中断处理注册函数。
  - 系统中断处理函数(IRQ跳转)，检查中断处理函数ID号，中断数加一，运行中断irqTable[intNum]。

- BSP_GPIO中断文件配置

  - 定义枚举数据中断出发类型gpio_interrupt_mode_t。
  - GPIO配置结构体中加上中断类型。
  - GPIO中断配置函数，输入参数GPIO*，pin，gpio_interrupt_mode_t。
    - 定义icr指针。并`icrShift = pin;`
    - SEL置1时设置ICR寄存器无效，所以给此寄存器清零。
    - 低16位`icr = &(base->ICR1);`高16位`icrShift -= 16;`
    - 判断中断出发类型，设置ICR寄存器。
  - GPIO初始化函数中加入GPIO中断配置函数。
  - 定义函数：使能GPIO中断，禁止GPIO中断，清除中断标志位(该函数作用位退出中断)

- BSP_EXIT外部中断配置文件

  - 外部中断初始化函数

    - 定义gpio配置结构体

    - 引脚复用，电气属性上拉输入。

    - GPIO初始化。

    - GIC使能中断。

    - 注册中断服务函数。

    - gpio中断使能。

      ```c
      GIC_EnableIRQ(GPIO1_Combined_16_31_IRQn);
      system_register_irqhandler(GPIO1_Combined_16_31_IRQn, gpio1_io18_irqhandler, NULL);
      /* 使能中断之前需要先注册中断服务函数，否则使能后来中断可能会出问题 */
      gpio_enableint(GPIO1, 18);
      ```

  - 中断服务函数。

# EPIT中断

- 中断初始化函数

  - 输入参数为分频值和载入值。

    ```c
    void epit1_int(unsigned int frac, unsigned int value)
    ```

  - 判断分频数是否大于4095。

  - 配置EPIT中断寄存器。

    ```c
    /* 配置EPIT的CR寄存器 */
    EPIT1->CR = 0;
    /**
      * bit1=1，计数器从load值或者0xffffffff开始计数
      * bit2=1使能比较中断
      * bit3=1从寄存器记录的值重新计数
      * bit4-15分配值frac
      * bit24=1设置定时器时钟源
      */
    EPIT1->CR = (1 << 1) | (1 << 2) | (1 << 3) | (frac << 4) | (1 << 24);
    EPIT1->LR = value;
    EPIT1->CMPR =0;
    ```

  - GIC使能

  - 注册中断服务函数

  - 开启中断

    ```c
    /* 初始化中断 */
    GIC_EnableIRQ(EPIT1_IRQn);
    system_register_irqhandler(EPIT1_IRQn, epit1_irqhandler, NULL);   
    /* 打开EPIT1 */ 
    EPIT1->CR |= (1 << 0);
    ```

- EPIT1中断服务函数

  ```c
  void epit1_irqhandler(unsigned int gicciar, void *param)
  {
      static unsigned char state = 0;
      state = !state;
      if (EPIT1->SR & (1 << 0))   /* 中断发生了 */
      {
          led_switch(LED0, state);
      }
      /* 清除中断标志位 */
      EPIT1->SR |= (1 << 0);
  }
  ```

# 定时器按键消抖

- 外部中断初始化

  ```c
  void keyfilter_init(void)
  {
      gpio_pin_config_t key_config;
      IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0);                 //设置引脚复用，GPIO1_18
      IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0xf080);         //设置引脚电气属性 设置为上拉输入
  
      /* GPIO初始化 */
      key_config.direction = kGPIO_DigitalInput;
      key_config.interruptMode = kGPIO_IntFallingEdge;
      key_config.outputLogic = 1;
      gpio_init(GPIO1, 18, &key_config);      //设置引脚为输入
  
      GIC_EnableIRQ(GPIO1_Combined_16_31_IRQn);
      system_register_irqhandler(GPIO1_Combined_16_31_IRQn, gpio1_16_32_irqhandler, NULL);
      /* 使能中断之前需要先注册中断服务函数，否则使能后来中断可能会出问题 */
      gpio_enableint(GPIO1, 18);
      filtertimer_init(66000000/100); //定时器初始化放在外部中断初始化函数中
  }
  ```

  

- 定时器中断初始化，初始化定时器,但不使能，要在按键下降沿中断触发之后在按键的中断服务函数中开启，开始计数，计数满足定义的时间触发定时器中断。

  ```c
  void filtertimer_init(unsigned int value)
  {
      EPIT1->CR = 0;
      EPIT1->CR = (1 << 1) | (1 << 2) | (1 << 3) | (1 << 24);
      EPIT1->LR = value;
      EPIT1->CMPR =0;
      
      /* 初始化中断 */
      GIC_EnableIRQ(EPIT1_IRQn);
      system_register_irqhandler(EPIT1_IRQn, filtertimer_irqhandler, NULL);   
  }
  ```

- 外部中断服务函数

  ```c
  void gpio1_16_32_irqhandler(unsigned int gicciar, void *param)
  {
      /* 开启定时器 */
      filtertimer_restart(66000000/100);
      /* 清除中断标志位 */
      gpio_clearintflags(GPIO1, 18);
  }
  ```

- 定时器中断服务函数

  ```c
  void filtertimer_irqhandler(unsigned int gicciar, void *param)
  {
      static unsigned char state = OFF;
      
      if(EPIT1->SR & (1 << 0))
      {
          /* 关闭定时器 */
          filtertimer_stop();
          if (gpio_pinread(GPIO1, 18) == 0)
          {
              state = !state;
              beep_switch(state);
          }
      }
      /* 清除中断标志位 */
      EPIT1->SR |= (1 << 0);
  }
  ```

- 关闭EPIT1定时器

  ```C
  /* 关闭EPIT1定时器 */
  void filtertimer_stop(void)
  {
      EPIT1->CR &= ~(1 << 0);
  }
  ```

  

- 开启EPIT1定时器

  ```c
  /* 开启EPIT1定时器 */
  void filtertimer_restart(unsigned int value)
  {
      EPIT1->CR &= ~(1 << 0);
      EPIT1->LR = value;
      EPIT1->CR |= ( 1<< 0);
  }
  ```

# GPT定时器实现高精度延时

#### 实验一：进入GPT中断实现LED闪烁

- 延时初始化函数

  - 初始化相关寄存器

    CR寄存器，先全部清零，软件复位，设置初始值，时钟源，设置模式为restart模式即比较中断后重新计时。设置分频值，使定时器频率为1M。设置OCR[0]为1000000/2即500ms进入一次比较中断。

  - 打开通道1比较中断，GIC使能，注册中断服务函数，打开GPT1

  ```c
  /* 延时初始化函数 */
  void delay_init(void)
  {
      GPT1->CR = 0;
  
      /* 软复位，置1复位，等待自动清零 */
      GPT1->CR = (1 << 15);
      while ((GPT1->CR >> 15) & 0x01);
  
      /**
       * bit1: 1：初始值为0
       * bit6-8: 1：时钟源选择ipg_clk = 66MHz
       * bit9: 0：restart模式
       */
      GPT1->CR |= (1 << 1) | (1 << 6);
  
      /* 分频设置,0-11位设置分频值 */
      GPT1->PR = 65;      // 66分频
      /* 配置输出比较通道 */
      GPT1->OCR[0] = 1000000/2;    /* 设置中断周期为500ms */
  
      /* 打开通道1比较中断 */
      GPT1->IR = 1 << 0;
  
      GIC_EnableIRQ(GPT1_IRQn);
      system_register_irqhandler(GPT1_IRQn, gpt1_irqhandler, NULL);
      GPT1->CR |= (1 << 0);   /* 打开GPT1 */
  }
  
  void gpt1_irqhandler(unsigned int gicciar, void *param)
  {
      static unsigned char state  = 0;
  
      /* 判断是哪个中断 */
      if (GPT1->SR & (1 << 0))
      {
          state = !state;
          led_switch(LED0, state);
      }
  
      /* 清除中断状态 */
      GPT1->SR |= (1 << 0);
  }
  ```

  

#### 实验二：高精度延时

- 延时初始化函数

  ```c
  /* 延时初始化函数 */
  void delay_init(void)
  {
      GPT1->CR = 0;
  
      /* 软复位，置1复位，等待自动清零 */
      GPT1->CR = (1 << 15);
      while ((GPT1->CR >> 15) & 0x01);
  
      /**
       * bit1: 1：初始值为0
       * bit6-8: 1：时钟源选择ipg_clk = 66MHz
       * bit9: 0：restart模式
       */
      GPT1->CR |= (1 << 1) | (1 << 6);
  
      /* 分频设置,0-11位设置分频值 */
      GPT1->PR = 65;      // 66分频
      /* 1M的频率计1个数就是1us */
      GPT1->OCR[0] = 0xffffffff;
  
      GPT1->CR |= (1 << 0);   /* 打开GPT1 */
  }
  ```

  

- us延时函数

  ```c
  void delay_us(unsigned int usdelay)
  {
      unsigned long oldcnt, newcnt;
      unsigned long tcntvalue = 0;
  
      oldcnt = GPT1->CNT;
      while (1)
      {
          newcnt = GPT1->CNT;
          if(newcnt != oldcnt)
          {
              if(newcnt > oldcnt)
              {
                  tcntvalue += newcnt - oldcnt;
              }
              else
              {
                  tcntvalue += 0xffffffff - oldcnt + newcnt;
              }
              oldcnt = newcnt;
              if (tcntvalue >= usdelay)
              {
                  break;
              }
          } 
      }
  }
  ```

  

- ms延时函数

```c
void delay_ms(unsigned int msdelay)
{
    unsigned int i = 0;
    for (i = 0; i < msdelay; i++)
    {
        delay_us(1000);
    }    
}
```

- 主函数中使用高精度延时函数实现LED闪烁。

# UART

- 串口IO初始化函数

- 串口初始化

  - 初始化IO

  - 初始化复位UART1

  - 配置相关寄存器

    - UCR1为0
    - UCR2配置数据位，奇偶校验位，停止位
    - UCR3的bti2必须为1

  - 设置波特率

    Baud Rate = 𝑅𝑒𝑓 𝐹𝑟𝑒𝑞 / (16 ×(𝑈𝐵𝑀𝑅 + 1) / (𝑈𝐵𝐼𝑅 + 1)) 

  - 使能串口

  ```c
  /**
   * @brief 初始化UART
   * @brief 波特率固定115200
   * @param void:void
   */
  void uart_init(void)
  {
      /* 初始化串口IO */
      uart_io_init();
  
      /* 初始化UART1 */
      uart_disable(UART1);    /* 关闭UART1 */
      uart_softreset(UART1);   /* 复位UART1 */
  
      /* 配置UART1 */
      UART1->UCR1 = 0;
  
      /* 配置UART1的数据位，奇偶校验，停止位等 */
      /* bit1-2发送和接收的使能 bit5设置数据位长度8位 bit6一位停止位 bit14忽略RTS脚 */
      UART1->UCR2 = 0;
      UART1->UCR2 |= (1 << 1) | (1 << 2) | (1 << 5) | (1 << 14);
  
      /* bit2必须为1 */
      UART1->UCR3 |= (1 << 2);
  
      /* 设置波特率 */
      UART1->UFCR &= ~(7 << 7);   /* 对RFDIV进行清零 */
      UART1->UFCR = 5 << 7;       /* 1分频，80M */
      UART1->UBIR = 71;
      UART1->UBMR = 3124;
      /* 使能串口 */
      uart_enable(UART1);
  }
  
  ```

- 打开串口

  ```c
  /**
   * @brief 打开串口
   * @param UART_Type
   */
  void uart_enable(UART_Type *base)
  {
      base->UCR1 |= (1 << 0);
  }
  ```

- 关闭串口

  ```c
  /**
   * @brief 关闭串口
   * @param UART_Type
   */
  void uart_disable(UART_Type *base)
  {
      base->UCR1 &= ~(1 << 0);
  }
  ```

- 复位串口

  ```c
  /**
   * @brief 复位UART
   * @param UART_Type
   */
  void uart_softreset(UART_Type *base)
  {
      base->UCR2 &= ~(1 << 0);
      while((base->UCR2 & 0x01) == 0);
  }
  ```

- 发送字符

  ```c
  void putc(unsigned char c)
  {
      /* 在判断上次数据是否发送完成 */
      while (((UART1->USR2 >> 3) & 0x01) == 0); /* 数据是否在发送中 */
      UART1->UTXD = c;
  }
  ```

- 接收字符

  ```c
  unsigned char getc(void)
  {
      while ((UART1->USR2 & 0x01) == 0); /* 是否准备好接收 */
      return UART1->URXD;
  }
  ```

  

- 发送字符串

  ```c
  /**
   * @brief 通过串口发送一串字符
   * @param str char *
   */
  void puts(char *str)
  {
      char *p = str;
      while(*p != '\0')
      {
          putc(*p++);
      }
  }
  ```

# printf配置

- 添加官方的波特率计算设置函数

  ```c
  /**
   * @brief        	    : 波特率计算公式，可以用此函数计算出指定串口对应的UFCR，UBIR和UBMR这三个寄存器的值
   * @param base		    : 要计算的串口。
   * @param baudrate	    : 要使用的波特率。
   * @param srcclock_hz	: 串口时钟源频率，单位Hz
   * @return		: 无
   */
  void uart_setbaudrate(UART_Type *base, unsigned int baudrate, unsigned int srcclock_hz)
  ```

  - 函数中用到数学库，编译时需要指定路径

    ```makefile
    LIBPATH			:= -lgcc -L /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/lib/gcc/arm-linux-gnueabihf/4.9.4
    ```

  - ```c
    /* raise函数防止编译报错 */
    void raise(int sig_nr)
    {
        
    }
    ```

- 添加stdio文件

  - 修改Makefile，编译c文件时加入指令-Wa,-mimplicit-it=thumb避免报错

    ```makefile
    $(COBJS): obj/%.o : %.c
    	$(CC) $(INCLUDE) -Wa,-mimplicit-it=thumb -fno-builtin -c -Wall -nostdlib -O2 -o $@ $<
    ```

- 主函数

  ```c
  while(1){
          printf("请输入两个整数，使用空格隔开：");
          scanf("%d %d", &a, &b);
          printf("\r\n数据%d+%d=%d\r\n", a, b, a+b);
          printf("%d的十六进制：%#x\r\n", c);
      }
  ```

  

# DDR3

DDR3（Double Data Rate 3）是第三代双倍数据率同步动态随机存取存储器（SDRAM），广泛应用于计算机、服务器和嵌入式系统中。相比其前代 DDR2，DDR3 在速度、带宽和能效方面都有显著提升。以下是 DDR3 的基本原理和一些重要参数的介绍。

#### DDR3 的工作原理

##### 1. **双倍数据率（Double Data Rate）**:

- DDR3 的核心特点是双倍数据率，即在时钟信号的上升沿和下降沿都传输数据。这样，在相同的时钟频率下，DDR3 可以实现比传统 SDRAM 更高的数据传输速率。

##### 2. **内部结构**:

- DDR3 内部采用 8n 预取架构。每个时钟周期 DDR3 可以从内存阵列中预取 8 个字节的数据，并在 4 个时钟周期内分 2 次输出这些数据。这种架构有助于在更高频率下稳定工作。

##### 3. **时钟和时序**:

- DDR3 使用差分时钟（Differential Clock，CK 和 CK#）信号来同步内存操作。时序参数（如 CL、tRCD、tRP 等）决定了 DDR3 内存模块在特定操作下的延迟和性能。

##### 4. **低电压**:

- DDR3 运行电压为 1.5V，比 DDR2 的 1.8V 更低，这有助于降低功耗，尤其是在大型服务器和数据中心环境中。

#### DDR3 的重要参数

##### 1. **数据速率**:

- DDR3 的数据传输速率从 800 MT/s（Mega Transfers per second）到 2133 MT/s 不等，常见的速率有 DDR3-800、DDR3-1066、DDR3-1333、DDR3-1600、DDR3-1866 和 DDR3-2133。

##### 2. **时钟频率**:

- DDR3 的实际时钟频率（内核时钟）从 400 MHz 到 1066 MHz，但由于双倍数据率技术，其数据传输速率是时钟频率的两倍。

##### 3. **带宽**:

- 带宽取决于内存模块的位宽和数据速率。DDR3 的带宽计算公式为：`带宽 = 数据速率 × 位宽 ÷ 8`。例如，DDR3-1600 的单通道带宽为 `1600 × 64 ÷ 8 = 12.8 GB/s`。

##### 4. **时序（Latency）**:

- 时序是指内存模块在执行不同操作时的延迟，通常以一系列参数表示，如 CL（CAS Latency）、tRCD（Row to Column Delay）、tRP（Row Precharge Time）、tRAS（Row Active Time）。这些参数直接影响内存的访问速度。
- 例如，DDR3-1600 CL9 的时序可能标记为 `9-9-9-24`，意味着 CAS Latency 为 9 个时钟周期，tRCD 为 9 个时钟周期，tRP 为 9 个时钟周期，tRAS 为 24 个时钟周期。

##### 5. **容量**:

- DDR3 内存模块的容量通常从 512 MB 到 32 GB 不等，具体取决于内存芯片的密度和模块设计（如单通道、双通道或多通道配置）。

##### 6. **电压**:

- DDR3 标准电压为 1.5V，低电压版本 DDR3L 的电压为 1.35V，超低电压版本 DDR3U 的电压为 1.25V。这些低电压版本旨在进一步降低功耗。

##### 7. **预取长度**:

- DDR3 的预取长度为 8n，意味着每次内存访问可以预取 8 个字节的数据。这是 DDR3 相比于 DDR2 提高数据带宽的重要技术手段。

##### 8. **模块类型**:

- DDR3 内存模块包括常见的 UDIMM（Unbuffered DIMM）、RDIMM（Registered DIMM）、SO-DIMM（用于笔记本电脑的小型 DIMM）等。RDIMM 带有寄存器，用于稳定信号，适合用于服务器。

#### DDR3 相比 DDR2 和 DDR4 的优势

- **相比 DDR2**:
  - 更高的带宽：DDR3 的数据速率远高于 DDR2，带宽相应增加。
  - 更低的功耗：DDR3 的工作电压比 DDR2 低，降低了能耗。
  - 更大的容量：DDR3 支持更高密度的内存芯片，使得单个模块的容量更大。
- **相比 DDR4**:
  - DDR4 进一步提升了数据速率、降低了电压，并引入了更多先进功能（如 Bank Group 技术），但 DDR3 依然在成本敏感的应用中广泛使用。

#### 总结

DDR3 是一种性能强大且应用广泛的内存技术，凭借其高带宽、低功耗和大容量，成为了许多计算机系统的主流选择。理解 DDR3 的工作原理和重要参数，有助于在系统设计和优化中做出更明智的决策。

# LCD

- 读屏幕ID

  - 打开模拟开关，设置GPIO（引脚复用冲突解决）

    ```c
     /* 打开模拟开关，设置LCD_VSYNC为高电平 */
        gpio_pin_config_t lcdio_config;
        IOMUXC_SetPinMux(IOMUXC_LCD_VSYNC_GPIO3_IO03, 0);
        IOMUXC_SetPinConfig(IOMUXC_LCD_VSYNC_GPIO3_IO03,0x10b0);
    
        /* GPIO初始化 */
        lcdio_config.direction = kGPIO_DigitalOutput;
        lcdio_config.outputLogic = 1;
        gpio_init(GPIO3, 3, &lcdio_config);
    ```

    

  - 将3个ID口复用为GPIO输入模式

    ```c
     /* 读取ID值，设置G7 B7 R7为输入 */
    	IOMUXC_SetPinMux(IOMUXC_LCD_DATA07_GPIO3_IO12, 0);		/* B7(M2) */
    	IOMUXC_SetPinMux(IOMUXC_LCD_DATA15_GPIO3_IO20, 0);		/* G7(M1) */
    	IOMUXC_SetPinMux(IOMUXC_LCD_DATA23_GPIO3_IO28, 0);		/* R7(M0) */
    
        IOMUXC_SetPinConfig(IOMUXC_LCD_DATA07_GPIO3_IO12, 0xF080);
    	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA15_GPIO3_IO20, 0xF080);
    	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA23_GPIO3_IO28, 0xF080);
    
        lcdio_config.direction = kGPIO_DigitalInput;
    	gpio_init(GPIO3, 12, &lcdio_config);
    	gpio_init(GPIO3, 20, &lcdio_config);
    	gpio_init(GPIO3, 28, &lcdio_config);
    ```

  - return屏幕ID
  - 使用串口初始屏幕ID

- 初始化屏幕IO

  - 配置24个数据引脚

  - 4个功能引脚（LCDIF控制器，不用作GPIO所以不用初始换GPIO）

  - 点亮背光（GPIO）

  - 复位10ms

    ```c
    /* 初始化屏幕IO */
    	lcdgpio_init();
    	lcd_reset();
    	delay_ms(10);
    	lcd_noreset();
    ```

- 根据不同的屏幕ID来设置屏幕参数

  ```c
  if(lcdid == ATK7016) {
  		tftlcd_dev.height = 600;	
  		tftlcd_dev.width = 1024;
  		tftlcd_dev.vspw = 3;
  		tftlcd_dev.vbpd = 20;
  		tftlcd_dev.vfpd = 12;
  		tftlcd_dev.hspw = 20;
  		tftlcd_dev.hbpd = 140;
  		tftlcd_dev.hfpd = 160;
  		lcdclk_init(32, 3, 5);	/* 初始化LCD时钟 51.2MHz */
  	}
  ```

- 设置公共屏幕参数

  ```c
  tftlcd_dev.pixsize = 4;	/* 每个像素4个字节 */
  	tftlcd_dev.framebuffer = LCD_FRAMEBUF_ADDR;
  	tftlcd_dev.forecolor = LCD_RED;
  	tftlcd_dev.backcolor = LCD_BLACK;
  ```

- 配置LCDIF控制器相关寄存器 

- 使能，清屏

  ```c
  lcd_enable();			/* 使能LCD */
  
  delay_ms(20);
  lcd_clear(LCD_WHITE);
  ```

- 时钟配置函数

  ```c
  void lcdclk_init(unsigned char loopDiv, unsigned char prediv, unsigned char div)
  {
  	/* 不使用小数分频器 */
  	CCM_ANALOG->PLL_VIDEO_NUM = 0;
  	CCM_ANALOG->PLL_VIDEO_DENOM = 0;
  
  	/*
       * PLL_VIDEO寄存器设置
       * bit[13]:    1   使能VIDEO PLL时钟
       * bit[20:19]  2  设置postDivider为1分频
       * bit[6:0] : 32  设置loopDivider寄存器
  	 */
  	CCM_ANALOG->PLL_VIDEO =  (2 << 19) | (1 << 13) | (loopDiv << 0); 
  
  	/*
       * MISC2寄存器设置
       * bit[31:30]: 0  VIDEO的post-div设置，时钟源来源于postDivider，1分频
  	 */
  	CCM_ANALOG->MISC2 &= ~(3 << 30);
  
  	/* LCD时钟源来源与PLL5，也就是VIDEO           PLL  */
  	CCM->CSCDR2 &= ~(7 << 15);  	
  	CCM->CSCDR2 |= (2 << 15);			/* 设置LCDIF_PRE_CLK使用PLL5 */
  
  	/* 设置LCDIF_PRE分频 */
  	CCM->CSCDR2 &= ~(7 << 12);		
  	CCM->CSCDR2 |= (prediv - 1) << 12;	/* 设置分频  */
  
  	/* 设置LCDIF分频 */
  	CCM->CBCMR &= ~(7 << 23);					
  	CCM->CBCMR |= (div - 1) << 23;		
  
  	/* 设置LCD时钟源为LCDIF_PRE时钟 */
  	CCM->CSCDR2 &= ~(7 << 9);					/* 清除原来的设置		 	*/
  	CCM->CSCDR2 |= (0 << 9);					/* LCDIF_PRE时钟源选择LCDIF_PRE时钟 */
  }
  ```

- 功能函数
  - 画点函数
  - 读点函数
  - 清屏函数
  - 矩形填充函数

# RTC

- 时间相关的宏定义，时间相关的结构体

  ```c
  /* 跟时间有关的宏定义 */
  #define SECONDS_IN_A_DAY    (86400)
  #define SECONDS_IN_A_HOUR   (3600)
  #define SECONDS_IN_A_MINUTE (60)
  #define DAYS_IN_A_YEAR      (365)
  #define YEAR_RANGE_START    (1970)
  #define YEAR_RANGE_END      (2099)
  
  /* 跟时间有关的结构体 */
  struct rtc_datetime{
  unsigned short  year;
  unsigned char   month;
  unsigned char   day;
  unsigned char   hour;
  unsigned char   minute;
  unsigned char   second;
  };
  ```

- RTC初始化函数

  - 开启信任模式

  - 使能RTC

    ```c
    void rtc_init(void)
    {
        struct rtc_datetime rtcDate;
        SNVS->HPCOMR |= (1 << 31) | (1 << 8);
    #if 0
        rtcDate.year    = 2024;
        rtcDate.month   = 8;
        rtcDate.day     = 14;
        rtcDate.hour    = 23;
        rtcDate.minute  = 32;
        rtcDate.second  = 36;
    #endif
        rtc_setdatetime(&rtcDate);
    
        /* 开启RTC */
        rtc_enable();
    }
    ```

- 功能函数
  - 使能RTC
  - 关闭RTC
  - **时间转化为秒数**
  - **设置时间**
  - 得到秒数（读寄存器）
  - 秒数转化为时间
  - **获取时间（得到秒数，转化时间，传入时间结构体地址）**
  - 判断闰年

# IIC

SCL和SDA需要接上拉电阻一般4.7K，3.3V。

I2C总线支持多从机，通过从机地址开区分访问哪个从机

IIC写时序

![my image](.\pic\Snipaste_2024-08-21_22-33-14.png)

IIC读时序

![my image](.\pic\Snipaste_2024-08-21_22-40-33.png)

写时序有三部分，读时序有四部分，比写多一部分。分别写四个函数以实现四部分内容

#### IIC驱动

- 初始化IIC

  ```c
  void i2c_init(I2C_Type *base)
  {
      base->I2CR &= ~(1 << 7);        /* 关闭I2C */
      base->IFDR = 0x15;              /* 640分频 103.125KHz*/
      base->I2CR |= (1 <<7);          /* 打开I2C */
  }
  ```

- 主机

  - start函数

    使用哪个IIC，设备地址，写方向

    开始使用IIC时判断IIC是否在忙

    设置为主机发送

    寄存器中写入地址，自动产生start信号

    ```c
    unsigned int i2c_master_start(I2C_Type *base, unsigned char address, enum i2c_direction direction)
    {
        if (base->I2SR & (1 << 5))  /* I2C忙 */
            return 1;
        
        /* 设置为主机模式1左移5设置为主机，1左移4设置为发送 */
        base->I2CR |= (1 << 5) | (1 << 4);
        
        /* 产生start，发送从机地址时（I2C_I2DR寄存器中写入从机地址），start信号就会同步产生，不用手动设置 */
        base->I2DR = ((unsigned int)address << 1) | (direction == kI2C_Read ? 1 : 0);        /* 高7位是地址，最低位是读写，所以要左移一位 */
    
        return 0;
    }
    ```

  - stop函数

    停止哪个IIC

    寄存器清零停止IIC

    等待IIC工作完成后return OK

    ```c
    unsigned char i2c_master_stop(I2C_Type *base)
    {
        unsigned short timeout = 0xffff;    /* 超时时间 */
    
        /* 清除i2c的bit5:3
         * 5清零从模式
         * 4清零接收模式
         * 3清零，在接收到一个字节的数据后在第九个时钟位向总线发送确认信号，这个位仅仅在接收模式下写1
         */
        base->I2CR &= ~(0x7 << 3);
    
        /* 等待i2c忙结束 */
        while(base->I2SR & (1 << 5))
        {
            timeout--;
            if(timeout == 0)    /* 超时跳出 */
                return I2C_STATUS_TIMEOUT;
        }
        return I2C_STATUS_OK;
    }
    ```

  - repeated start函数

    判断是否忙碌或者工作在从机模式

    设置发送产生restart

    发送地址以及读方向，return OK

    ```c
    unsigned int i2c_master_repeated_start(I2C_Type *base, unsigned char address, enum i2c_direction direction)
    {
        /* I2C是否忙或者工作在从机模式下，因为restart信号是在信号传输中进行的，所以要检查在这个过程中是否被设置成了从机模式 */
        if((base->I2SR & (1 << 5)) && (base->I2CR & (1 << 5)) == 0)
            return 1;
        /* 1<<4为发送，1<<2为产生restart信号 */
        base->I2CR |= (1 << 4) | (1 << 2);
        base->I2DR = ((unsigned int)address << 1) | (direction == kI2C_Read ? 1 : 0);        /* 高7位是地址，最低位是读写，所以要左移一位 */
        return I2C_STATUS_OK;
    }
    ```

  - 错误检查函数

    ```c
    unsigned char i2c_check_and_clear_error(I2C_Type *base, unsigned int status)
    {
        /* 先检查是否为仲裁丢失错误 */
        if(base->I2SR & (1 << 4)) {
            base->I2SR &= ~(1 << 4);
            base->I2CR &= ~(1 << 7);
            base->I2CR |= (1 << 7);
            return I2C_STATUS_ARBITRATION_LOST;
        }
        else if(base->I2SR & (1 << 0)) {    // 检测到NOACK信号
            return I2C_STATUS_NAK;
        }
        return I2C_STATUS_OK;
    }
    ```

  - write函数

    中间过程，等待传输完成而是检测是否处于忙碌

    清除中断，设置发送

    while函数将要写入的数据依次给I2DR寄存器，每一次输入要判断SR寄存器是否当前字节数据是否传输完成，然后才开始下一个数据的传输。

    检查是否有传输错误

    清除标志位，停止传输

    ```c
    void i2c_master_write(I2C_Type *base, const unsigned char *buf, unsigned int size)
    {
        /* 等待传输完成 */
        while(!(base->I2SR & (1 << 7)));
    
        base->I2SR &= ~(1 << 1);            /* 清除中断 */
        base->I2CR |= (1 << 4);             /* 设置为发送 */
    
        while (size--)
        {
            base->I2DR = *buf++;            /* 将要发送的数据写入I2DR */
            while(!(base->I2SR & (1 << 1)));    /* 等待传输完成 */
            
            /* 检查ACK */
            if(i2c_check_and_clear_error(base, base->I2SR))
                break;  // 检测到有错误发生中断发送
        }
        base->I2SR &= ~(1 << 1);
        i2c_master_stop(base);
    }
    ```

  - read函数

    等待传输完成

    清除标志位，设置接收

    ```c
    void i2c_master_read(I2C_Type *base, unsigned char *buf, unsigned int size)
    {
        volatile uint8_t dummy = 0;     /* 假读 */
        dummy++;                        /* 防止编译报错 */
        /* 等待传输完成 */
        while(!(base->I2SR & (1 << 7)));
    
        base->I2SR &= ~(1 << 1);            /* 清除中断 */
        base->I2CR &= ~((1 << 4) | (1 << 3));
    
        /* 当有多个数据的时候，读倒数第二个数据的时候主机向从机发送NACK信号，当只有一个数据的时候主机直接向从机发送NACK */
        if (size == 1)
        {
            base->I2CR |= (1 << 3);                 /* NACK */
        }
    
        while(size--) {
            while(!(base->I2SR & (1 << 1)));        /* 等待数据传输完成 */
            base->I2SR &= ~(1 << 1);
    
            if (size == 0)                          /* 数据发送完成 */
            {
                i2c_master_stop(base);
            }
            if (size == 1)
            {
                base->I2CR |= (1 << 3);             /* NACK */
            }
            
            *buf++ = base->I2DR;
        }
    }
    ```

  - 传输函数

    ```c
    unsigned char i2c_master_transfer(I2C_Type *base, struct i2c_transfer *xfer)
    {
    	unsigned char ret = 0;
    	enum i2c_direction direction = xfer->direction;	
    
    	base->I2SR &= ~((1 << 1) | (1 << 4));			/* 清除标志位 */
    
    	/* 等待传输完成 */
    	while(!((base->I2SR >> 7) & 0X1)){};
    
    	/* 如果是读的话，要先发送寄存器地址，所以要先将方向改为写 */
        if ((xfer->subaddressSize > 0) && (xfer->direction == kI2C_Read))
        {
            direction = kI2C_Write;
        }
    
    	ret = i2c_master_start(base, xfer->slaveAddress, direction); /* 发送开始信号 */
        if(ret)
        {	
    		return ret;
    	}
    
    	while(!(base->I2SR & (1 << 1))){};			/* 等待传输完成 */
    
        ret = i2c_check_and_clear_error(base, base->I2SR);	/* 检查是否出现传输错误 */
        if(ret)
        {
          	i2c_master_stop(base); 						/* 发送出错，发送停止信号 */
            return ret;
        }
    	
        /* 发送寄存器地址 */
        if(xfer->subaddressSize)
        {
            do
            {
    			base->I2SR &= ~(1 << 1);			/* 清除标志位 */
                xfer->subaddressSize--;				/* 地址长度减一 */
    			
                base->I2DR =  ((xfer->subaddress) >> (8 * xfer->subaddressSize)); //向I2DR寄存器写入子地址
      
    			while(!(base->I2SR & (1 << 1)));  	/* 等待传输完成 */
    
                /* 检查是否有错误发生 */
                ret = i2c_check_and_clear_error(base, base->I2SR);
                if(ret)
                {
                 	i2c_master_stop(base); 				/* 发送停止信号 */
                 	return ret;
                }  
            } while ((xfer->subaddressSize > 0) && (ret == I2C_STATUS_OK));
    
            if(xfer->direction == kI2C_Read) 		/* 读取数据 */
            {
                base->I2SR &= ~(1 << 1);			/* 清除中断挂起位 */
                i2c_master_repeated_start(base, xfer->slaveAddress, kI2C_Read); /* 发送重复开始信号和从机地址 */
        		while(!(base->I2SR & (1 << 1))){};/* 等待传输完成 */
    
                /* 检查是否有错误发生 */
    			ret = i2c_check_and_clear_error(base, base->I2SR);
                if(ret)
                {
                 	ret = I2C_STATUS_ADDRNAK;
                    i2c_master_stop(base); 		/* 发送停止信号 */
                    return ret;  
                }
            }
        }
    
        /* 发送数据 */
        if ((xfer->direction == kI2C_Write) && (xfer->dataSize > 0))
        {
        	i2c_master_write(base, xfer->data, xfer->dataSize);
    	}
    
        /* 读取数据 */
        if ((xfer->direction == kI2C_Read) && (xfer->dataSize > 0))
        {
           	i2c_master_read(base, xfer->data, xfer->dataSize);
    	}
    	return 0;	
    }
    ```

- 从机函数

  - 宏定义，器件地址和寄存器地址

    ```c
    #define AP3216C_ADDR    	0X1E	/* AP3216C器件地址 */
    
    /* AP3316C寄存器 */
    #define AP3216C_SYSTEMCONG	0x00	/* 配置寄存器 			*/
    #define AP3216C_INTSTATUS	0X01	/* 中断状态寄存器 			*/
    #define AP3216C_INTCLEAR	0X02	/* 中断清除寄存器 			*/
    #define AP3216C_IRDATALOW	0x0A	/* IR数据低字节 			*/
    #define AP3216C_IRDATAHIGH	0x0B	/* IR数据高字节 			*/
    #define AP3216C_ALSDATALOW	0x0C	/* ALS数据低字节 		*/
    #define AP3216C_ALSDATAHIGH	0X0D	/* ALS数据高字节			*/
    #define AP3216C_PSDATALOW	0X0E	/* PS数据低字节 			*/
    #define AP3216C_PSDATAHIGH	0X0F	/* PS数据高字节 			*/
    ```

  - 初始化函数

    IO初始化

    I2C接口初始化

    传感器初始化

    ​	找一个寄存器，先写后读，判断读写的数据是否一致

  - 读一个字节

  - 写一个字节

  - 读数据

    ```c
    void ap3216c_readdata(unsigned short *ir, unsigned short *ps, unsigned short *als)
    {
        unsigned char buf[6];
        unsigned char i;
    
    	/* 循环读取所有传感器数据 */
        for(i = 0; i < 6; i++)	
        {
            buf[i] = ap3216c_readonebyte(AP3216C_ADDR, AP3216C_IRDATALOW + i);	
        }
    	
        if(buf[0] & 0X80) 	/* IR_OF位为1,则数据无效 */
    		*ir = 0;					
    	else 				/* 读取IR传感器的数据   		*/
    		*ir = ((unsigned short)buf[1] << 2) | (buf[0] & 0X03); 			
    	
    	*als = ((unsigned short)buf[3] << 8) | buf[2];	/* 读取ALS传感器的数据 			 */  
    	
        if(buf[4] & 0x40)	/* IR_OF位为1,则数据无效 			*/
    		*ps = 0;    													
    	else 				/* 读取PS传感器的数据    */
    		*ps = ((unsigned short)(buf[5] & 0X3F) << 4) | (buf[4] & 0X0F); 	
    }
    ```

# SPI

SPI相对于IIC最大的优势有两点

​	速度快，最大可以达到几十MHz，甚至上百MHz

​	全双工

一个SPI接口可以连接多个SPI外设

SPI（Serial Peripheral Interface）是一种同步串行通信协议，主要用于短距离通信，通常用于微控制器与外围设备（如传感器、存储设备、显示器等）之间的通信。SPI 通信协议具有高速、简单、占用资源少等特点。

## SPI协议介绍

#### SPI 通信协议的基本特性

1. **主从模式（Master-Slave）**：
   - SPI 是一种主从架构的协议。在通信过程中，主设备（Master）控制整个通信过程，而从设备（Slave）则被动地响应主设备的请求。
   - 通常只有一个主设备，但可以有多个从设备。
2. **全双工通信**：
   - SPI 是全双工的，这意味着数据可以同时在两个方向上传输，即主设备和从设备可以同时发送和接收数据。
3. **同步通信**：
   - SPI 是一种同步通信协议，这意味着数据传输是由时钟信号同步的。主设备生成的时钟信号用于同步数据的发送和接收。

#### SPI 的四条基本信号线

1. **MOSI（Master Out Slave In）**：
   - 主设备发送数据到从设备的数据线。
2. **MISO（Master In Slave Out）**：
   - 从设备发送数据到主设备的数据线。
3. **SCLK（Serial Clock）**：
   - 串行时钟线，由主设备生成，用于同步数据的传输。
4. **SS/CS（Slave Select/Chip Select）**：
   - 从设备选择线，由主设备控制。当某个从设备的 SS 线被拉低（即选中）时，该从设备开始与主设备通信。
   - 对于多从设备系统，每个从设备通常有一个独立的 SS 线。

#### SPI 的工作模式

SPI 的时钟有四种工作模式，这由时钟极性（CPOL）和时钟相位（CPHA）决定：

1. **CPOL（Clock Polarity）**：时钟极性决定空闲时钟线的电平。
   - CPOL = 0: 空闲状态时，时钟线（SCLK）为低电平。
   - CPOL = 1: 空闲状态时，时钟线（SCLK）为高电平。
2. **CPHA（Clock Phase）**：时钟相位决定数据采样点的位置。
   - CPHA = 0: 数据在第一个时钟边沿采样。
   - CPHA = 1: 数据在第二个时钟边沿采样。

根据这两个参数，SPI 可以有四种工作模式：

- 模式0（CPOL = 0, CPHA = 0）
- 模式1（CPOL = 0, CPHA = 1）
- 模式2（CPOL = 1, CPHA = 0）
- 模式3（CPOL = 1, CPHA = 1）

#### SPI 的优缺点

**优点**：

- 高速传输，支持全双工通信。
- 简单的硬件实现，易于使用。
- 通信时没有时钟线拉伸的问题（不像I2C）。
- 支持多从设备通信（只需增加额外的 SS 线）。

**缺点**：

- 需要多条数据线，尤其是在多从设备系统中。
- 没有明确的标准，厂商实现可能不同。
- 通信距离较短，通常只适合板级通信。

#### SPI 的应用

SPI 通信广泛应用于微控制器与各种外围设备之间的通信，如：

- 温度传感器
- 存储设备（EEPROM、闪存）
- 显示屏
- ADC/DAC转换器

SPI 协议由于其高速和灵活性，特别适合需要快速传输数据的应用场合。

## SPI时序介绍

SPI（Serial Peripheral Interface）的时序是 SPI 通信协议中的一个重要部分，它决定了数据在主设备和从设备之间的传输方式。SPI 的时序主要由时钟信号（SCLK）的极性和相位以及数据采样时刻决定。了解这些时序对于正确实现 SPI 通信非常关键。

### SPI 时钟（SCLK）

SPI 通信的时钟由主设备（Master）生成，并在通信过程中同步主设备和从设备的数据传输。SPI 的时钟有两个关键参数：

1. **时钟极性（CPOL, Clock Polarity）**：确定时钟线（SCLK）在空闲状态时的电平。
   - **CPOL = 0**：空闲状态时，时钟线为低电平。
   - **CPOL = 1**：空闲状态时，时钟线为高电平。
2. **时钟相位（CPHA, Clock Phase）**：确定数据在时钟的哪个边沿被采样和发送。
   - **CPHA = 0**：数据在第一个时钟边沿采样（即第一个边沿是数据采样点）。
   - **CPHA = 1**：数据在第二个时钟边沿采样（即第二个边沿是数据采样点）。

根据这两个参数，SPI 有四种工作模式：

1. **模式 0 (CPOL = 0, CPHA = 0)**：
   - 时钟线空闲时为低电平（CPOL = 0）。
   - 在时钟的上升沿采样数据（CPHA = 0）。
2. **模式 1 (CPOL = 0, CPHA = 1)**：
   - 时钟线空闲时为低电平（CPOL = 0）。
   - 在时钟的下降沿采样数据（CPHA = 1）。
3. **模式 2 (CPOL = 1, CPHA = 0)**：
   - 时钟线空闲时为高电平（CPOL = 1）。
   - 在时钟的下降沿采样数据（CPHA = 0）。
4. **模式 3 (CPOL = 1, CPHA = 1)**：
   - 时钟线空闲时为高电平（CPOL = 1）。
   - 在时钟的上升沿采样数据（CPHA = 1）。

### SPI 时序图

以下是每种模式下的典型 SPI 时序图说明：

- **模式 0 (CPOL = 0, CPHA = 0)**:
  - 时钟空闲时为低电平。
  - 时钟上升沿时，主设备发送数据，从设备在同一时钟边沿采样数据。
- **模式 1 (CPOL = 0, CPHA = 1)**:
  - 时钟空闲时为低电平。
  - 时钟下降沿时，主设备发送数据，从设备在下一个时钟上升沿采样数据。
- **模式 2 (CPOL = 1, CPHA = 0)**:
  - 时钟空闲时为高电平。
  - 时钟下降沿时，主设备发送数据，从设备在同一时钟边沿采样数据。
- **模式 3 (CPOL = 1, CPHA = 1)**:
  - 时钟空闲时为高电平。
  - 时钟上升沿时，主设备发送数据，从设备在下一个时钟下降沿采样数据。

### 数据传输过程

1. **数据发送**：在主设备的时钟信号作用下，主设备通过 MOSI 线发送数据位，每个数据位在时钟的指定边沿发送。
2. **数据采样**：从设备在时钟的指定边沿采样接收到的数据，并将数据位存储在其接收缓冲区。
3. **数据接收**：从设备通过 MISO 线将数据发送回主设备，主设备在时钟的相应边沿采样接收到的数据。

### SPI 通信的时序分析

1. **时钟信号的选择**：CPOL 和 CPHA 的选择取决于具体的 SPI 设备和其要求的时序。一般来说，设备的时序要求会在其数据手册中详细说明。
2. **多从设备时的时序管理**：当有多个从设备时，主设备需要管理不同的从设备选择信号（SS/CS），以确保只有一个从设备处于通信状态，避免总线冲突。

了解 SPI 的时序有助于在硬件设计中合理地配置时钟信号，确保数据传输的正确性和可靠性。在实际使用中，应该根据具体设备的要求选择合适的 SPI 工作模式，并调试验证时序是否满足通信要求。

## 程序设计

### SPI支持包

- SPI初始化函数

  ```c
  /**
   * @brief SPI初始化
   * 
   * @param base : ECSPIn
   */
  void spi_init(ECSPI_Type *base)
  {
      base->CONREG = 0;
      /* bit0置1，使能SPI。
       * bit3置1，表示当向TXFIFO写入数据后马上开启SPI突发访问，也就是发送数据。
       * bit7:4设置SPI通道主从模式，bit7为通道3，bit4为通道0。
       * 我们使用了SS0，也就是通道0.实际上本实验片选由软件实现
       * bit31:20设置突发访问长度，设置为7，即长度为8bit，一个字节。
       */
      base->CONREG |= (1 << 0) | (1 << 3) | (1 << 4) | (7 << 20);
  
      /**
       * bit14:0设置wait states时间，设置为0x2000;
       * bit15设置时钟源为SPI clk将此位设置为0.
       * bit21:16表示片选信号得延时，设置为0.我们采用软件片选
       */
      base->PERIODREG = 0;
      base->PERIODREG = 2000;
  
      /* SPI时钟ICM20608最高只能到8M，将SPI CLK = 6M */
      base->CONREG &= ~((0xf << 12) | (0xf << 8));
      base->CONREG |= (9 << 12);          /* 一级10分频 */    
  }
  ```

- SPI发送/接收函数

  ```c
   * @brief SPI发送/接收函数
   * 
   * @param base : ECSPIn
   * @param txdata : 要发送的数据，当接收数据时，该参数给0xff
   * @return unsigned char 
   */
  unsigned char spich0_readwrite_byte(ECSPI_Type *base, unsigned char txdata)
  {
      uint32_t spirxdata = 0;
      uint32_t spitxdata = txdata;
  
      /* 选择通道0 */
      base->CONREG &= ~(3 << 18);
      base->CONREG |= (0 << 18);
  
      /* 数据发送，发送数据前一定要等待STATREG的bit0为空 */
      while((base->STATREG & (1 << 0)) == 0);
      base->TXDATA = spitxdata;
  
      /* 数据接收 */
      while((base->STATREG & (1 << 3)) == 0);
      spirxdata = base->RXDATA;
  
      return spirxdata;
  }
  ```

  

### ICM20608支持包

- 初始化函数

  ```c
  unsigned char icm20608_init(void)
  {
      unsigned char regval = 0;
      // 1.SPI引脚的初始化
      IOMUXC_SetPinMux(IOMUXC_UART2_RX_DATA_ECSPI3_SCLK, 0);
      IOMUXC_SetPinMux(IOMUXC_UART2_CTS_B_ECSPI3_MOSI, 0);
      IOMUXC_SetPinMux(IOMUXC_UART2_RTS_B_ECSPI3_MISO, 0);
  
      IOMUXC_SetPinConfig(IOMUXC_UART2_RX_DATA_ECSPI3_SCLK, 0x10b0);
      IOMUXC_SetPinConfig(IOMUXC_UART2_CTS_B_ECSPI3_MOSI, 0x10b0);
      IOMUXC_SetPinConfig(IOMUXC_UART2_RTS_B_ECSPI3_MISO, 0x10b0);
  
      // 片选引脚初始化
      IOMUXC_SetPinMux(IOMUXC_UART2_TX_DATA_GPIO1_IO20, 0);
      IOMUXC_SetPinConfig(IOMUXC_UART2_TX_DATA_GPIO1_IO20, 0x10b0);
  
      gpio_pin_config_t cs_config;
      cs_config.direction = kGPIO_DigitalOutput;
      cs_config.outputLogic = 0;
      gpio_init(GPIO1, 20, &cs_config);
  
      // 2.SPI控制器的初始化
      spi_init(ECSPI3);
  
      icm20608_write_reg(ICM20_PWR_MGMT_1, 0x80);		/* 复位，复位后为0x40,睡眠模式 			*/
  	delay_ms(50);
  	icm20608_write_reg(ICM20_PWR_MGMT_1, 0x01);		/* 关闭睡眠，自动选择时钟 					*/
  	delay_ms(50);
  
      // 3.IMC20608的初始化
      regval = icm20608_read_reg(ICM20_WHO_AM_I);
      printf("icm20608 id = %#X\r\n", regval);
      if((regval != ICM20608G_ID) && (regval != ICM20608D_ID))
          return 1;
      icm20608_write_reg(ICM20_SMPLRT_DIV, 0x00); 	/* 输出速率是内部采样率					*/
  	icm20608_write_reg(ICM20_GYRO_CONFIG, 0x18); 	/* 陀螺仪±2000dps量程 				*/
  	icm20608_write_reg(ICM20_ACCEL_CONFIG, 0x18); 	/* 加速度计±16G量程 					*/
  	icm20608_write_reg(ICM20_CONFIG, 0x04); 		/* 陀螺仪低通滤波BW=20Hz 				*/
  	icm20608_write_reg(ICM20_ACCEL_CONFIG2, 0x04); 	/* 加速度计低通滤波BW=21.2Hz 			*/
  	icm20608_write_reg(ICM20_PWR_MGMT_2, 0x00); 	/* 打开加速度计和陀螺仪所有轴 				*/
  	icm20608_write_reg(ICM20_LP_MODE_CFG, 0x00); 	/* 关闭低功耗 						*/
  	icm20608_write_reg(ICM20_FIFO_EN, 0x00);		/* 关闭FIFO						*/
      return 0;
  }
  ```

- 读写函数

  芯片的读写函数参考芯片的手册编写

  ```c
  /* ICM20608通过SPI接口读数据，写入地址，读回数据 */
  unsigned char icm20608_read_reg(unsigned char reg)
  {
      unsigned char reg_val = 0;
      reg |= 0x80;                /* 地址的bit7置1，表示读取数据 */
      ICM20608_CSN(0);            /* 片选拉低 */
      spich0_readwrite_byte(ECSPI3, reg);                 /* 写入要读取寄存器的地址 */
      reg_val = spich0_readwrite_byte(ECSPI3, 0xff);
  
      ICM20608_CSN(1);            /* 片选拉高，停止访问 */
      return reg_val;
  }
  /* ICM20608通过SPI接口写数据 */
  void icm20608_write_reg(unsigned char reg, unsigned char val)
  {
      reg &= ~0x80;                /* 地址的bit7置1，表示读取数据 */
      ICM20608_CSN(0);            /* 片选拉低 */
      spich0_readwrite_byte(ECSPI3, reg);                 /* 写入要读取寄存器的地址 */
      spich0_readwrite_byte(ECSPI3, val);
  
      ICM20608_CSN(1);            /* 片选拉高，停止访问 */
  }
  ```

- 顺序读取多个寄存器的值

  ```c
  /**
   * @param	    : 读取ICM20608连续多个寄存器
   * @param reg	: 要读取的寄存器地址
   * @return 		: 读取到的寄存器值
   */
  void icm20608_read_len(unsigned char reg, unsigned char *buf, unsigned char len)
  {  
  	unsigned char i;
  	
  	/* ICM20608在使用SPI接口的时候寄存器地址，只有低7位有效,
  	 * 寄存器地址最高位是读/写标志位读的时候要为1，写的时候要为0。
  	 */
  	reg |= 0x80; 
  		
     	ICM20608_CSN(0);               				/* 使能SPI传输	 		*/
    	spich0_readwrite_byte(ECSPI3, reg);			/* 发送寄存器地址  		*/   	   
   	for(i = 0; i < len; i++)					/* 顺序读取寄存器的值 			*/
   	{
  		buf[i] = spich0_readwrite_byte(ECSPI3, 0XFF);	
  	}
   	ICM20608_CSN(1);                			/* 禁止SPI传输 			*/
  }
  ```

- 读取芯片寄存器的值并进行处理后得加速度等数据量的读取值

  ```c
  void icm20608_getdata(void)
  {
  	unsigned char data[14];
  	
  	icm20608_read_len(ICM20_ACCEL_XOUT_H, data, 14);
  
      /* ADC传感器数据 */
  	icm20608_dev.accel_x_adc = (signed short)((data[0] << 8) | data[1]); 
  	icm20608_dev.accel_y_adc = (signed short)((data[2] << 8) | data[3]); 
  	icm20608_dev.accel_z_adc = (signed short)((data[4] << 8) | data[5]); 
  	icm20608_dev.temp_adc    = (signed short)((data[6] << 8) | data[7]); 
  	icm20608_dev.gyro_x_adc  = (signed short)((data[8] << 8) | data[9]); 
  	icm20608_dev.gyro_y_adc  = (signed short)((data[10] << 8) | data[11]);
  	icm20608_dev.gyro_z_adc  = (signed short)((data[12] << 8) | data[13]);
  }
  ```

- 显示整数部分的函数

- 显示小数部分的函数

- 硬件浮点数处理

  - 在使用浮点数计算时需要使用IMX6UL的硬件浮点单元

    这个函数用于使能 I.MX6UL 处理器的硬件 NEON 和 FPU（浮点运算单元）。在 ARM 处理器中，NEON 是一种用于加速多媒体处理的 SIMD（单指令多数据）扩展，而 FPU 则用于加速浮点数运算。

    这个函数通过操作 `CPACR` 和 `FPEXC` 寄存器，开启 NEON 和 FPU 功能。它的操作分为两个部分：

    1. **配置 CPACR 寄存器**：开启对协处理器 10 和协处理器 11 的完全访问，这两个协处理器分别对应 NEON 和 FPU。
    2. **配置 FPEXC 寄存器**：使能 FPU 的高级扩展。

    *这个函数是底层硬件配置的一部分，通常在系统启动时调用，以确保在接下来的应用程序中可以使用 NEON 和 FPU 加速。*

    ```c
    /*
     * @description	: 使能I.MX6U的硬件NEON和FPU
     * @param 		: 无
     * @return 		: 无
     */
     void imx6ul_hardfpu_enable(void)
    {
    	uint32_t cpacr;
    	uint32_t fpexc;
    
    	/* 使能NEON和FPU */
    	cpacr = __get_CPACR();
    	cpacr = (cpacr & ~(CPACR_ASEDIS_Msk | CPACR_D32DIS_Msk))
    		   |  (3UL << CPACR_cp10_Pos) | (3UL << CPACR_cp11_Pos);
    	__set_CPACR(cpacr);
    	fpexc = __get_FPEXC();
    	fpexc |= 0x40000000UL;	
    	__set_FPEXC(fpexc);
    }
    ```

  - 编译的时候指定硬件浮点

    这些编译选项用于编译针对 ARM 架构的代码，特别是针对 ARMv7-A 处理器架构，并启用了 NEON 和 VFPv4 浮点运算单元。具体解释如下：

    1. **-march=armv7-a**：
       - **含义**：指定目标架构为 ARMv7-A。ARMv7-A 是一个 32 位的处理器架构，常用于智能手机、平板电脑等嵌入式设备中。这个架构支持高级指令集，并且通常集成了 NEON 和 VFP（向量浮点运算单元）。
       - **作用**：告诉编译器生成的机器代码应针对 ARMv7-A 架构进行优化。
    2. **-mfpu=neon-vfpv4**：
       - **含义**：指定使用 NEON 和 VFPv4 浮点运算单元。NEON 是一个 SIMD（单指令多数据）协处理器，常用于加速多媒体和信号处理任务。VFPv4 是一个支持双精度浮点运算的浮点单元。
       - **作用**：告诉编译器生成代码时，可以使用 NEON 和 VFPv4 指令集进行浮点和向量运算的优化。
    3. **-mfloat-abi=hard**：
       - **含义**：指定使用硬浮点 ABI（应用二进制接口）。硬浮点 ABI 意味着浮点运算将直接使用硬件浮点单元，而不是通过软件模拟。
       - **作用**：通过使用硬件浮点运算，提升浮点计算的效率。

    ### 总结

    这些选项组合在一起，指示编译器为 ARMv7-A 架构生成优化的代码，利用 NEON 和 VFPv4 指令集，并直接使用硬件的浮点运算单元。这些优化对于多媒体处理、信号处理和其他计算密集型应用特别有效。

  - ```makefile
    -march-armv7-a -mfpu=neon-vfpv4 -mfloat-abi=hard
    ```

    

## 协处理器

协处理器（Coprocessor）是一个计算机处理器，它专门用于执行特定类型的计算任务，通常用于加速主处理器（也称为中央处理器，CPU）的某些操作。协处理器通常与主处理器协同工作，可以处理特定类型的指令集或执行复杂的计算任务，这些任务在主处理器上可能会更慢或不够高效。

### 协处理器的类型和功能

1. **浮点运算协处理器（FPU）**：
   - **功能**：处理浮点数的加减乘除、平方根等运算。浮点运算在很多科学计算、工程应用、图形处理等领域非常常见，但这类运算相对复杂，使用 FPU 可以大大提高处理速度。
   - **例子**：早期的 x86 处理器没有内置 FPU，而是通过一个叫做 80387 的协处理器来处理浮点运算。现代的 CPU 通常已经将 FPU 集成在了主处理器内部。
2. **图形协处理器（GPU）**：
   - **功能**：专门用于处理图形渲染和并行计算任务。GPU 可以处理大量的矩阵运算和向量计算，在图像处理、机器学习等应用中起到了关键作用。
   - **例子**：NVIDIA 和 AMD 的显卡就是典型的 GPU，它们通过与主 CPU 协作，处理复杂的图形计算任务。
3. **数字信号处理器（DSP）**：
   - **功能**：用于处理数字信号（如音频、视频信号）的协处理器。DSP 通常用于实时处理任务，如音频编码解码、滤波、调制解调等。
   - **例子**：在手机的音频子系统中，通常会有一个 DSP 协处理器来处理音频信号的滤波和增强。
4. **加密协处理器**：
   - **功能**：用于处理加密解密任务的协处理器，提供硬件加速的加密算法计算，如 AES、RSA 等。
   - **例子**：现代处理器中经常集成有加密协处理器，用于加速安全通信和数据保护。
5. **NEON 协处理器**（在 ARM 架构中）：
   - **功能**：ARM 处理器中的 NEON 是一个 SIMD（单指令多数据）协处理器，专门用于加速多媒体处理、数字信号处理和其他计算密集型任务。
   - **例子**：在 ARM 处理器中，NEON 协处理器被广泛应用于手机、平板电脑等设备中，用于图像处理、音频处理等。

### 协处理器的作用

- **提高效率**：协处理器可以加速特定任务，减轻主处理器的负担，使得主处理器能够更快地完成其他任务。
- **专用计算**：协处理器通常针对特定的计算任务进行了优化，因此在这些任务上能提供远超主处理器的性能。
- **并行处理**：协处理器可以与主处理器并行工作，增加系统的整体计算能力。
- **节能省电**：在一些嵌入式系统中，使用协处理器可以降低整体功耗，因为协处理器可以以更低的能耗完成特定任务。

### 总结

协处理器是专门为处理某类特定任务而设计的硬件模块，它可以显著提升计算性能，特别是在需要大量特定类型计算的场景下。通过将任务分配给适合的协处理器，整个系统能够更高效地运行。

# 多点电容触摸屏实验

- 初始化ft5426
  - IO初始化
    - 初始化IIC引脚
    - 初始化中断引脚
    - 初始化复位引脚
    - 复位芯片
  - IIC接口初始化
  - FT5426初始化
    - （测试，读取固件版本号）
    - 设置FT5426模式（通过IIC写寄存器）
    - 标记FT5426初始化完成
- 中断服务函数
- 功能函数
  - 写一个字节数据
  - 读取一个字节数据
  - 读取多个寄存器数据
  - 读取当前所有触摸点的坐标

# PWM

1. 6ULL的PWM是16位计数器

2. 有4个16位的**FIFO**

3. 1个12位的分频器PWMCR[15 : 4]，66M/666=1M.

4. 正点原子LCD屏幕的背光IO连接到了GPIO_IO8上。其可以复用为PWM1_OUT信号。

   PWM计数器从0x0000开始计数，当计数器的值等于PWMPR+1的时候定时器就会重新开始下一个周期的运行，PWMPR控制PWM的频率。该寄存器设置PWM信号的周期

5. FIFO保存着采样值。每当读取一次PWMSAR寄存器FIFO里的数据就会减一。FIFO为空时，无法再产生PWM信号。FIFO为空时会产生中断，可以配置FIFO整剩余多少数据时判断为空。使用SAR寄存器设置占空比。

初始化

```c
/*
 * @description	: 初始化背光PWM
 * @param		: 无
 * @return 		: 无
 */
void backlight_init(void)
{
	unsigned char i = 0;
	
	/* 1、背光PWM IO初始化 */
	IOMUXC_SetPinMux(IOMUXC_GPIO1_IO08_PWM1_OUT, 0); /* 复用为PWM1_OUT */

	/* 配置PWM IO属性	
	 *bit 16:0 HYS关闭
	 *bit [15:14]: 10 100K上拉
	 *bit [13]: 1 pull功能
	 *bit [12]: 1 pull/keeper使能
	 *bit [11]: 0 关闭开路输出
	 *bit [7:6]: 10 速度100Mhz
	 *bit [5:3]: 010 驱动能力为R0/2
	 *bit [0]: 0 低转换率
	 */
	IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO08_PWM1_OUT, 0XB090);
	
	/* 2、初始化PWM1		*/
	/*
   	 * 初始化寄存器PWMCR
   	 * bit[27:26]	: 01  当FIFO中空余位置大于等于2的时候FIFO空标志值位
   	 * bit[25]		:
 0  停止模式下PWM不工作
   	 * bit[24]		: 0	  休眠模式下PWM不工作
   	 * bit[23]		: 0   等待模式下PWM不工作
   	 * bit[22]		: 0   调试模式下PWM不工作
   	 * bit[21]		: 0   关闭字节交换
   	 * bit[20]		: 0	  关闭半字数据交换
   	 * bit[19:18]	: 00  PWM输出引脚在计数器重新计数的时候输出高电平
   	 *					  在计数器计数值达到比较值以后输出低电平
   	 * bit[17:16]	: 01  PWM时钟源选择IPG CLK = 66MHz
   	 * bit[15:4]	: 65  分频系数为65+1=66，PWM时钟源 = 66MHZ/66=1MHz
   	 * bit[3]		: 0	  PWM不复位
   	 * bit[2:1]		: 00  FIFO中的sample数据每个只能使用一次。
   	 * bit[0]		: 0   先关闭PWM，后面再使能
	 */
	PWM1->PWMCR = 0;	/* 寄存器先清零 */
	PWM1->PWMCR |= (1 << 26) | (1 << 16) | (65 << 4);

	/* 设置PWM周期为1000,那么PWM频率就是1M/1000 = 1KHz。 */
	pwm1_setperiod_value(1000);

	/* 设置占空比，默认50%占空比   ,写四次是因为有4个FIFO */
	backlight_dev.pwm_duty = 50;
	for(i = 0; i < 4; i++)
	{
		pwm1_setduty(backlight_dev.pwm_duty);	
	}
	
	/* 使能FIFO空中断，设置寄存器PWMIR寄存器的bit0为1 */
	PWM1->PWMIR |= 1 << 0;
	system_register_irqhandler(PWM1_IRQn, (system_irq_handler_t)pwm1_irqhandler, NULL);	/* 注册中断服务函数 */
	GIC_EnableIRQ(PWM1_IRQn);	/* 使能GIC中对应的中断 */
	PWM1->PWMSR = 0;			/* PWM中断状态寄存器清零 */
	
	pwm1_enable();				/* 使能PWM1 */
}
```

中断处理函数

```c
/*
 * @description	: pwm1中断处理函数
 * @param		: 无
 * @return 		: 无
 */
void pwm1_irqhandler(void)
{

	if(PWM1->PWMSR & (1 << 3)) 	/* FIFO为空中断 */
	{
		/* 将占空比信息写入到FIFO中,其实就是设置占空比 */
		pwm1_setduty(backlight_dev.pwm_duty); 
		PWM1->PWMSR |= (1 << 3); /* 写1清除中断标志位 */ 
	}
}
```

功能函数*使能*，*SAM设置*，*周期设置*， *占空比设置*

```c
/*
 * @description	: 使能PWM
 * @param		: 无
 * @return 		: 无
 */
void pwm1_enable(void)
{
	PWM1->PWMCR |= 1 << 0;	 
}

/*
 * @description		: 设置Sample寄存器，Sample数据会写入到FIFO中，
 * 					  所谓的Sample寄存器，就相当于比较寄存器，假如PWMCR中的POUTC
 *				  	  设置为00的时候。当PWM计数器中的计数值小于Sample的时候
 *					  就会输出高电平，当PWM计数器值大于Sample的时候输出底电平,
 *					  因此可以通过设置Sample寄存器来设置占空比
 * @param -  value	: 寄存器值，范围0~0XFFFF
 * @return 			: 无
 */
void pwm1_setsample_value(unsigned int value)
{
	PWM1->PWMSAR = (value & 0XFFFF);	
}

/*
 * @description		: 设置PWM周期，就是设置寄存器PWMPR，PWM周期公式如下
 *					  PWM_FRE = PWM_CLK / (PERIOD + 2)， 比如当前PWM_CLK=1MHz
 *					  要产生1KHz的PWM，那么PERIOD = 1000000/1K - 2 = 	998
 * @param -  value	: 周期值，范围0~0XFFFF
 * @return 			: 无
 */
void pwm1_setperiod_value(unsigned int value)
{
	unsigned int regvalue = 0;

	if(value < 2)
		regvalue = 2;
	else 
		regvalue = value - 2;
	PWM1->PWMPR = (regvalue & 0XFFFF);
}

/*
 * @description		: 设置PWM占空比
 * @param -  value	: 占空比0~100，对应0%~100%
 * @return 			: 无
 */
void pwm1_setduty(unsigned char duty)
{
	unsigned short preiod;
	unsigned short sample;

	backlight_dev.pwm_duty = duty;
	preiod = PWM1->PWMPR + 2;
	sample = (unsigned short)(preiod * backlight_dev.pwm_duty / 100.0f); // 100.0f是为了编译为硬件浮点
	pwm1_setsample_value(sample);
}
```

