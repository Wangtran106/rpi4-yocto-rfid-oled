#include <linux/module.h>
#include <linux/spi/spi.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/delay.h>

#define DRIVER_NAME "rc522_spi"

static struct spi_device *rc522_spi_device;

// Helper function to write to an RC522 register
static void rc522_write_reg(uint8_t addr, uint8_t val) {
    uint8_t tx[2];
    tx[0] = (addr << 1) & 0x7E;
    tx[1] = val;
    spi_write(rc522_spi_device, tx, 2);
}

// Helper function to read from an RC522 register
static uint8_t rc522_read_reg(uint8_t addr) {
    uint8_t tx = ((addr << 1) & 0x7E) | 0x80;
    uint8_t rx = 0x00;

    /*
     * MFRC522 register reads use one address byte followed by one dummy byte
     * to clock the response back from the chip.
     */
    spi_write_then_read(rc522_spi_device, &tx, 1, &rx, 1);
    return rx;
}

static ssize_t rc522_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    char result_str[64];
    int len;
    uint8_t version;

    if (!rc522_spi_device)
        return -ENODEV;

    // Read Version Register (Address 0x37)
    version = rc522_read_reg(0x37);

    // Version 0x91 or 0x92 are valid MFRC522 chips.
    if (version == 0x00 || version == 0xFF) {
        len = snprintf(result_str, sizeof(result_str), "ERROR: No RC522 found via SPI (Ver: 0x%02X)\n", version);
    } else {
        len = snprintf(result_str, sizeof(result_str), "SUCCESS: RC522 Found! SPI Version: 0x%02X\n", version);
    }

    if (*ppos > 0)
        return 0;

    if (copy_to_user(buf, result_str, len))
        return -EFAULT;

    *ppos += len;
    return len;
}

static const struct file_operations rc522_fops = {
    .owner = THIS_MODULE,
    .read = rc522_read,
};

static struct miscdevice rc522_misc = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "rc522",
    .fops = &rc522_fops,
};

static int rc522_probe(struct spi_device *spi)
{
    int ret;
    pr_info("RC522 probe called! Initial Max Speed: %d Hz\n", spi->max_speed_hz);
    rc522_spi_device = spi;

    spi->mode = SPI_MODE_0;
    spi->bits_per_word = 8;
    spi->max_speed_hz = 1000000;
    ret = spi_setup(spi);
    if (ret < 0) {
        pr_err("RC522: SPI setup failed: %d\n", ret);
        return ret;
    }

    pr_info("RC522 SPI configured: mode=%u bits=%u speed=%u Hz\n",
        spi->mode, spi->bits_per_word, spi->max_speed_hz);

    ret = misc_register(&rc522_misc);
    if (ret < 0) {
        pr_err("RC522: Failed to register misc device\n");
        return ret;
    }

    // Soft reset the RC522
    rc522_write_reg(0x01, 0x0F);
    msleep(50);

    pr_info("RC522 SPI Driver probed. /dev/rc522 created.\n");
    return 0;
}

static void rc522_remove(struct spi_device *spi)
{
    misc_deregister(&rc522_misc);
    rc522_spi_device = NULL;
    pr_info("RC522 SPI Driver removed.\n");
}

static const struct of_device_id rc522_dt_ids[] = {
    { .compatible = "nxp,rc522" },
    {},
};
MODULE_DEVICE_TABLE(of, rc522_dt_ids);

static const struct spi_device_id rc522_spi_ids[] = {
    { "rc522", 0 },
    {},
};
MODULE_DEVICE_TABLE(spi, rc522_spi_ids);

static struct spi_driver rc522_driver = {
    .driver = {
        .name = DRIVER_NAME,
        .of_match_table = rc522_dt_ids,
    },
    .probe = rc522_probe,
    .remove = rc522_remove,
    .id_table = rc522_spi_ids,
};

module_spi_driver(rc522_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tran Dinh Quang");
MODULE_DESCRIPTION("Custom SPI Character Driver for MFRC522 RFID Reader");
