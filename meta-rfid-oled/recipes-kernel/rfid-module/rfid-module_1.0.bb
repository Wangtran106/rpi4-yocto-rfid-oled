SUMMARY = "Custom RC522 SPI Kernel Module"
LICENSE = "CLOSED"

inherit module

SRC_URI = "file://Makefile \
           file://rc522_spi.c \
          "

S = "${WORKDIR}"
