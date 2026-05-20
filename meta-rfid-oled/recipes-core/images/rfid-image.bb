SUMMARY = "Custom Minimal Image with RFID and OLED support"
LICENSE = "CLOSED"

inherit core-image

# Add standard features and our custom packages
IMAGE_INSTALL += " \
    packagegroup-core-boot \
    kernel-modules \
    i2c-tools \
    rfid-module \
    rfid-app \
    rpi-rfid-overlay \
"

# Force standard device tree support for RPi
MACHINE_FEATURES += " i2c spi"
