#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "ssd1306_i2c.h"

int main() {
    int rc522_fd;
    char buffer[128];
    int len;

    printf("Starting Smart OLED & RFID Access System...\n");

    // 1. Initialize OLED via I2C
    if (ssd1306_init("/dev/i2c-1") < 0) {
        printf("Failed to init OLED. Are you sure I2C is enabled on /dev/i2c-1?\n");
        return -1;
    }

    ssd1306_clear();
    ssd1306_print_string(0, 0, "SYSTEM BOOT OK!");
    ssd1306_print_string(0, 2, "Waiting RFID...");
    printf("OLED Initialized. Waiting for RFID Tag...\n");

    // 2. Continuous Polling Loop
    while (1) {
        // We open and close the device to trigger the probe/read sequence
        // In a real interrupt-driven driver, we would just block on read()
        rc522_fd = open("/dev/rc522", O_RDONLY);
        if (rc522_fd >= 0) {
            len = read(rc522_fd, buffer, sizeof(buffer) - 1);
            if (len > 0) {
                buffer[len] = '\0';
                
                // Print to UART (Terminal)
                printf("[SPI_LOG] %s", buffer);

                // Print to OLED
                ssd1306_print_string(0, 5, "                "); // Clear line 5
                ssd1306_print_string(0, 6, "                "); // Clear line 6
                
                if (strstr(buffer, "SUCCESS")) {
                    ssd1306_print_string(0, 5, "Tag Detected!");
                    ssd1306_print_string(0, 6, "Access Granted");
                } else if (strstr(buffer, "ERROR")) {
                    ssd1306_print_string(0, 5, "SPI ERROR");
                }
            }
            close(rc522_fd);
        } else {
            ssd1306_print_string(0, 5, "ERR: /dev/rc522 ");
            ssd1306_print_string(0, 6, "Module not load ");
            printf("Cannot open /dev/rc522. Is the kernel module loaded via insmod?\n");
        }
        
        sleep(2); // Poll every 2 seconds
    }

    ssd1306_close();
    return 0;
}
