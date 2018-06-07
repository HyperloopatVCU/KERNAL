all:
	avr-gcc -mmcu=atmega328p -o kernel.out -Wall base_includes.c main.c \
	DEVICES/NetWizard/NETWIZARD.c DEVICES/SPIWizard/SPIWIZARD.c DEVICES/TWIWizard/TWIWIZARD.c \
	SENSORS/GyroWizard/GYROWIZARD.c SENSORS/HPSWiz/HPSWIZARD.c SENSORS/VPSWiz/VPSWIZARD.c
