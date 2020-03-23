# stm32f4_linux_prj

---
## 说明
1. stm32f4_linux_prj为使用arm-linux-gcc与makefile进行编译，j-link进行烧写，搭建stm32f407开发环境进行开发，可以用vs_code进行开发，敲出更多的bug++
2. stm32f4_linux_prj在user目录下包含多工程，stm32_single为基本demo工程，使用脚本可以进行编译、烧写。
3.  后续会增加更多的第三方开源库以及OS，物联网插件，结合博客进行物联网产品开发分享。

---
## 运行
```
# 编译
cd stm32f4_linux_prj/stm32f4_linux_mk/user/
sh build.sh stm32_single 1.0.0  # 第二入参user目录下工程名、第三入参固件版本号

# j-link烧写运行--生成bin文件在output文件下
sh load.sh ./stm32_single/output/1.0.0/stm32_single_app_v1.0.0.bin #第二入参，烧录文件相对路径，使用jlink-swd下载
sh build.sh stm32_single 1.0.0 loadbin #加入第三入参loadbin，进行一键编译+烧写

# 清除
sh build.sh stm32_single 1.0.0 clean #加入第三入参clean进行编译中间文件清除
```
---

Enjoy

Tuu
