import spidev
import RPi.GPIO as GPIO
import time

# SPI setup
spi = spidev.SpiDev()
spi.open(0, 0)
spi.max_speed_hz = 5000000
spi.mode = 0

# GPIO setup
RST_PIN = 22  # adjust if RST is on a different pin
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(RST_PIN, GPIO.OUT)

def write_reg(addr, data):
    spi.xfer2([addr | 0x80, data])

def read_reg(addr):
    return spi.xfer2([addr & 0x7F, 0x00])[1]

# Hardware reset
GPIO.output(RST_PIN, GPIO.LOW)
time.sleep(0.02)
GPIO.output(RST_PIN, GPIO.HIGH)
time.sleep(0.05)

# Init sequence
write_reg(0x01, 0x00)
time.sleep(0.02)
write_reg(0x01, 0x80)
time.sleep(0.02)
write_reg(0x01, 0x81)
time.sleep(0.05)

# 433 MHz
write_reg(0x06, 0x6C)
write_reg(0x07, 0x80)
write_reg(0x08, 0x00)

# BW 125kHz, CR 4/5, explicit header
write_reg(0x1D, 0x72)

# SF7, CRC off
write_reg(0x1E, 0x70)

# Preamble 8 symbols
write_reg(0x20, 0x00)
write_reg(0x21, 0x08)

# FIFO base addresses
write_reg(0x0E, 0x00)
write_reg(0x0F, 0x00)

# DIO0 = RxDone
write_reg(0x40, 0x00)

# High power
write_reg(0x4D, 0x87)

# Continuous RX
write_reg(0x01, 0x85)
time.sleep(0.1)

# Verify
print(f"OpMode: 0x{read_reg(0x01):02X}")        # should be 0x85
print(f"ModemCfg1: 0x{read_reg(0x1D):02X}")     # should be 0x72
print(f"ModemCfg2: 0x{read_reg(0x1E):02X}")     # should be 0x70
print(f"Freq MSB: 0x{read_reg(0x06):02X}")      # should be 0x6C

# RX loop
print("Listening...")
while True:
    irq = read_reg(0x12)
    if irq & 0x40:  # RxDone flag
        write_reg(0x12, 0xFF)  # clear IRQ
        nb_bytes = read_reg(0x13)
        current_addr = read_reg(0x10)
        write_reg(0x0D, current_addr)
        payload = []
        for i in range(nb_bytes):
            payload.append(read_reg(0x00))
        rssi = read_reg(0x1A) - 157
        print(f"Received: {''.join(chr(b) for b in payload)} | RSSI: {rssi} dBm")
    time.sleep(0.01)