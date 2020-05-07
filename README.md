# 换个心情-在ubuntu下开发STM32F4
---
**前言：**
现在开发stm32已经有很多人性化的ide了，像常用的keil，以及后面ST新推出的跨平台的STM32cubeide，都可以搭建自己的工程来开发。
但是本猿有时候就喜欢玩点另类的，比如这次我在ubuntu上搭建了stm32的开发环境，用vscode来开发，脚本编译甚至用来仿真（j-link），虽然整体功能上没有keil这些配套ide来的强大，但是有vscode的加持与开放的环境，整体用起来还是有一些独到的好处的。
比如整体会搞清楚很多东西，像工程的编译过程，生成最后烧写文件，最后程序执行；以及makefile与shell的使用；编译的参数是什么意思（对应到keil ide里面就是工程的配置）等。

---
**本篇目标：了解ubuntu如何开发stm32，以及相关的一些内容。**
**资料准备：**
1. 可用的ubuntu（这里是18.04版本）系统，可以安装虚拟机，也可以直接安装ubuntu系统哦。
2. 选择一款趁手的源代码编辑器，这里推荐选择的是vscode哦，如果没有接触过vscode，可以[看这里](https://blog.csdn.net/q361750389/article/details/105655593)
    快速入门哦。
3. 我们这里需要用jlink来烧写，所以需要安装[jlink驱动（Linux, DEB installer, 64-bit）](https://www.segger.com/downloads/jlink/#J-LinkSoftwareAndDocumentationPack)
哦。
4. 搭建完成的工程：[stm32f4_linux_prj](https://github.com/jinhaotutu/stm32f4_linux_prj)

---

## 工程介绍
**1. 工程功能：**
	
 - 支持linux下stm32f407一键编译烧写，支持jlink。
 - 支持vscode进行一键编译，jlink烧录、甚至仿真。
 - 硬件方面已支持以太网lan8720、后续将支持rtl8189无线芯片。
 - 工程已经或将支持的一些第三方开源库：freertos、lwip、logger、flash存储、cjson、http、mqtt、mbedtls等。
 - 后续将支持自己编写的应用组件：智能灯泡、智能插座、智能红外遥控器等组件。
 - 后续将安排在此工程上创建更多的关于物联网、智能家居相关的工程。

**2. 先将工程clone到本地：**

```c
git clone https://github.com/jinhaotutu/stm32f4_linux_prj.git
```

```c
.
├── LICENSE
├── README.md
└── stm32f4_linux_mk
    ├── doc										//相关文档
    ├── fwlib									//STM32库函数文件
    ├── misis									//STM32相关配置头文件
    ├── obj										//编译相关文件夹
    │   ├── makefile												//makefile文件
    │   ├── objects.mk
    │   ├── sources.mk
    │   ├── STM32F407ZGTX_FLASH.ld			//stm32的flash映射文件
    │   ├── subdir.mk												//编译规则
    │   └── tools														//arm-linux编译器
    ├── Startup
    │   └── startup_stm32f407zgtx.s				//stm32启动文件
    ├── sys										//stm32外设、接口文件
    ├── thirdlib								//第三方开源库
    └── user
        ├── build.sh							//编译脚本
        ├── load.sh							//jlink烧写脚本
        ├── stm32_lwip_withos					//stm32+lwip+freertos工程
        ├── stm32_rtos_single					//stm32+freertos工程
        ├── stm32_single								//stm32示例工程
        └── stm32_socket								//stm32+lwip+freertos+socket工程
          .____ 待更新
```

**3. 编译烧写stm32工程：**

```powershell
# 单编译
# 1. 进入工程根目录
cd stm32f4_linux_prj/stm32f4_linux_mk/user/
# 2. 调用build.sh脚本进行编译：第二入参user目录下工程名(stm32_single)、第三入参固件版本号(1.0.0) -- 生成bin文件在工程output文件夹
sh build.sh stm32_single 1.0.0 

# 使用 j-link进行烧写
# 1. 单烧录：第二入参，烧录bin文件相对路径，使用jlink-swd模式下载，所以保证烧录前安装了jlink驱动以及连接了stm32
sh load.sh ./stm32_single/output/1.0.0/stm32_single_app_v1.0.0.bin
# 2. 编译+烧写：在单编译加入第三入参loadbin，进行一键编译+烧写
sh build.sh stm32_single 1.0.0 loadbin

# 清除
sh build.sh stm32_single 1.0.0 clean #加入第三入参clean进行编译中间文件清除
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200506212026288.gif)
**4. 编译过程解析：**

1. 调用build.sh脚本进行自动编译，在脚本里会对编译器环境进行检测，执行make命令进行编译，并将生成的产物输入到指定路径以供使用，同时如果需要烧录会调用load.sh脚本进行jlink烧录。
2. 在obj目录下是有关编译的文件，主要包括makefile、subdir.mk、STM32F407ZGTX_FLASH.ld以及 tools文件夹下的编译器。
3. makefile借鉴了stm32cubeide生成的makefile结构。首先包含subdir.mk等文件，将所有.c编译成.o、.d等中间文件保存在output文件夹，并将.h的路径包含进去。然后调用make all将所有.o文件结合stm32的flash分配表（STM32F407ZGTX_FLASH.ld）生成elf文件。
4. 通过elf文件生成.list、.map、.bin文件，.bin文件是最后的二进制烧写文件，其他文件都是用来方便查找问题的，比如elf可以通过addr2line命令进行地址查找代码的具体位置。
5. 后续会通过将不同的模块编译成.lib的形式，最后进行链接生成二进制文件，这样在项目庞大的情况下可以优化整个编译的效率。

---
## 其他可玩功能
你以为就以上编译一个demo工程就完了吗？那就太没意思了，来点其他好玩的：

**1. 使用vscode进行一键编译烧写（可通过ctrl+shift+b来快速执行任务）：**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200507174017974.gif#pic_center)
**2. 使用vscode进行jlink仿真（可通过F5来快速进入仿真）：**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200507174834288.gif#pic_center)

---
## 后续相关更新

 - mqtt客户端
 - http客户端
 - 简单的web服务器
 - 智能家居系列：灯泡、插座、红外
 - 小程序界面
 - 等等....

上述的所有以及之前已有的一些功能我也希望通过后续的文章进行仔细介绍，比如移植rtos、移植lwip，mqtt与http等必要的知识点。

物联网可能在持续爆发，但是知识一直在那里，要时刻进行学习～

**借此推广一下自己的公众号，与博客一样，推送一些关于物联网的知识、技术、产品介绍~希望能帮到有需要的人：**
<img src="https://img-blog.csdnimg.cn/20200507175326677.JPG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3EzNjE3NTAzODk=,size_16,color_FFFFFF,t_70#pic_center" width=300 height=300> 
Tuu

上述来源于我的[blog](https://blog.csdn.net/q361750389)
