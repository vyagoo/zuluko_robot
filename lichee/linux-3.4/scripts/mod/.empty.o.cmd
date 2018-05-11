cmd_scripts/mod/empty.o := arm-openwrt-linux-muslgnueabi-gcc -Wp,-MD,scripts/mod/.empty.o.d  -nostdinc -isystem /home/celema/workspace/sdk/Vego_PI_zuluko/vego/prebuilt/gcc/linux-x86/arm/toolchain-sunxi/toolchain/bin/../lib/gcc/arm-openwrt-linux-muslgnueabi/5.2.0/include -I/home/celema/workspace/sdk/Vego_PI_zuluko/vego/lichee/linux-3.4/arch/arm/include -Iarch/arm/include/generated -Iinclude  -include /home/celema/workspace/sdk/Vego_PI_zuluko/vego/lichee/linux-3.4/include/linux/kconfig.h -D__KERNEL__ -mlittle-endian -Iarch/arm/mach-sunxi/include -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -Os -marm -fno-dwarf2-cfi-asm -fno-omit-frame-pointer -mapcs -mno-sched-prolog -mabi=aapcs-linux -mno-thumb-interwork -D__LINUX_ARM_ARCH__=7 -march=armv7-a -msoft-float -Uarm -Wframe-larger-than=1024 -fno-stack-protector -Wno-unused-but-set-variable -fno-omit-frame-pointer -fno-optimize-sibling-calls -g -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -DCC_HAVE_ASM_GOTO    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(empty)"  -D"KBUILD_MODNAME=KBUILD_STR(empty)" -c -o scripts/mod/empty.o scripts/mod/empty.c

source_scripts/mod/empty.o := scripts/mod/empty.c

deps_scripts/mod/empty.o := \

scripts/mod/empty.o: $(deps_scripts/mod/empty.o)

$(deps_scripts/mod/empty.o):
