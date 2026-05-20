# Smart OLED & RFID Access System (Raspberry Pi 4)

Dự án này trình diễn kỹ năng viết Kernel Module cho SPI (RFID RC522) và ứng dụng C User-space giao tiếp I2C (OLED SSD1306). 
Dự án được thiết kế độc lập, dễ hiểu, có thể cross-compile bằng toolchain của Buildroot/Yocto hoặc biên dịch trực tiếp trên Raspberry Pi.

## Sơ đồ đấu dây
- **OLED SSD1306:** Nối vào I2C-1 (SDA: GPIO 2, SCL: GPIO 3, VCC: 3.3V, GND: GND)
- **RC522 RFID:** Nối vào SPI0 (MOSI: GPIO 10, MISO: GPIO 9, SCLK: GPIO 11, CS/SDA: GPIO 8, VCC: 3.3V, GND: GND)

## Hướng dẫn sử dụng

### Bước 1: Kích hoạt phần cứng (Device Tree Overlay)
Bạn cần báo cho Linux biết là chân SPI đang nối với một con RC522 thông qua Device Tree.
```bash
cd kernel_module
# Biên dịch dts sang dtbo
dtc -@ -I dts -O dtb -o rc522.dtbo rc522-overlay.dts
# Copy vào thư mục boot
sudo cp rc522.dtbo /boot/overlays/
```
Mở file cấu hình boot (VD: `/boot/config.txt` hoặc `/boot/firmware/config.txt`) và thêm 2 dòng này vào cuối:
```text
dtparam=spi=on
dtparam=i2c_arm=on
dtoverlay=rc522
```
*Khởi động lại Raspberry Pi (`sudo reboot`).*

### Bước 2: Biên dịch & Load Kernel Module (SPI Driver)
Chạy Makefile để biên dịch ra file `.ko`:
```bash
cd kernel_module
# Nếu cross-compile: make KDIR=/path/to/buildroot/kernel
# Nếu biên dịch trực tiếp trên Pi:
make
```
Cài đặt module vào nhân hệ điều hành:
```bash
sudo insmod rc522_spi.ko
```
Kiểm tra log xem khởi tạo thành công chưa:
```bash
dmesg | tail
ls -l /dev/rc522
```

### Bước 3: Biên dịch & Chạy Ứng dụng C (I2C App)
Ứng dụng sẽ liên tục quét `/dev/rc522` và đẩy dữ liệu hiển thị qua màn hình OLED I2C.
```bash
cd user_app
# Biên dịch (dùng CC mặc định hoặc cross-compiler: make CC=aarch64-linux-gcc)
make
# Chạy chương trình
sudo ./rfid_app
```
Nếu phần cứng đấu nối đúng, màn hình OLED sẽ sáng lên, báo "SYSTEM BOOT OK", và terminal sẽ in liên tục dữ liệu lấy từ SPI Kernel Driver.
