cmd_/root/github/sv_protect/kernel/process.o := gcc -Wp,-MD,/root/github/sv_protect/kernel/.process.o.d  -nostdinc -isystem /usr/lib/gcc/i686-redhat-linux/4.4.7/include -Iinclude  -I/usr/src/kernels/2.6.32-573.26.1.el6.i686/include/uapi -I/usr/src/kernels/2.6.32-573.26.1.el6.i686/arch/x86/include -Iarch/include/generated -Iinclude -include /usr/src/kernels/2.6.32-573.26.1.el6.i686/include/linux/kconfig.h -D__KERNEL__ -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -O2 -m32 -msoft-float -mregparm=3 -freg-struct-return -mpreferred-stack-boundary=2 -march=i686 -mtune=generic -Wa,-mtune=generic32 -ffreestanding -fstack-protector -DCONFIG_AS_CFI=1 -DCONFIG_AS_CFI_SIGNAL_FRAME=1 -DCONFIG_AS_CFI_SECTIONS=1 -DCONFIG_AS_AVX=1 -pipe -Wno-sign-compare -fno-asynchronous-unwind-tables -mno-sse -mno-mmx -mno-sse2 -mno-3dnow -Wframe-larger-than=1024 -Wno-unused-but-set-variable -fomit-frame-pointer -g -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fno-dwarf2-cfi-asm -fconserve-stack  -DMODULE -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(process)"  -D"KBUILD_MODNAME=KBUILD_STR(sv_protect)" -D"DEBUG_HASH=27" -D"DEBUG_HASH2=11" -c -o /root/github/sv_protect/kernel/.tmp_process.o /root/github/sv_protect/kernel/process.c

deps_/root/github/sv_protect/kernel/process.o := \
  /root/github/sv_protect/kernel/process.c \
  /usr/src/kernels/2.6.32-573.26.1.el6.i686/include/linux/kconfig.h \
    $(wildcard include/config/h.h) \
    $(wildcard include/config/.h) \
    $(wildcard include/config/booger.h) \
    $(wildcard include/config/foo.h) \

/root/github/sv_protect/kernel/process.o: $(deps_/root/github/sv_protect/kernel/process.o)

$(deps_/root/github/sv_protect/kernel/process.o):
