SUMMARY = "RFID and OLED User-space Application"
LICENSE = "CLOSED"

SRC_URI = "file://main.c \
           file://ssd1306_i2c.c \
           file://ssd1306_i2c.h \
           file://Makefile \
           file://rfid-setup"

S = "${WORKDIR}"

TARGET_CC_ARCH += "${LDFLAGS}"

inherit update-rc.d

INITSCRIPT_NAME = "rfid-setup"
INITSCRIPT_PARAMS = "start 99 S ."

do_compile() {
    oe_runmake
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 ${S}/rfid_app ${D}${bindir}

    install -d ${D}${sysconfdir}/init.d
    install -m 0755 ${S}/rfid-setup ${D}${sysconfdir}/init.d/rfid-setup
}
