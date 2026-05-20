SUMMARY = "Device tree overlay for RC522 SPI RFID"
SECTION = "bsp"
LICENSE = "CLOSED"

COMPATIBLE_MACHINE = "raspberrypi"

SRC_URI = "file://rc522-overlay.dts"

S = "${WORKDIR}"

inherit devicetree

do_deploy:append() {
    install -d ${DEPLOYDIR}/overlays
    install -m 0644 ${B}/rc522-overlay.dtbo ${DEPLOYDIR}/overlays/rc522.dtbo
}
