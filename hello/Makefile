obj-m := hello.o

KDIR = /media/einfochip/work/ankur/imx-yocto-bsp/imx-build1/tmp/work/imx8mp_lpddr4_evk-poky-linux/linux-imx/5.15.52+gitAUTOINC+36363d8623-r0/build/

AR = arm64

COM = aarch64-poky-linux-

all:
  make ARCH=$(AR) CROSS_COMPILE=$(COM) -C $(KDIR) M=$(PWD) modules

clean:
  make -C $(KDIR) M=$(PWD) clean
  make ARCH=$(AR) CROSS_COMPILE=$(COM) -C $(KDIR) M=$(PWD) clean
