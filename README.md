# mo_grbl

This project is a customized fork of [GRBL](https://github.com/gnea/grbl), tailored to support multiple machine types and board configurations. The firmware includes build targets for each machine, allowing seamless configuration and flashing.

## Setup

### Prerequisites

1. **Arduino CLI**: Ensure Arduino CLI is installed. On macOS, you can install it with Homebrew:

```bash
brew install arduino-cli
```

Arduino AVR Core: Install the AVR core for Arduino boards:

```bash
arduino-cli core install arduino:avr
```

Make Utility: The make utility is typically pre-installed on macOS. If not, install it:

```bash
brew install make
```

### Building the Firmware

This GRBL fork supports different configurations for machine-specific firmware. The build process uses make commands to compile firmware for the desired machine.
Build Targets

<b>RC Mini Firmware:</b>

```bash
make build_rcmini
```

The compiled firmware will be output as build/rcmini.hex.

<b>Bamboo Firmware:</b>

```bash
make build_bamboo
```

The compiled firmware will be output as build/bamboo.hex.

Both Machines:

```bash
make
```

This builds firmware for both RC Mini and Bamboo, placing the output files in the build directory.

### Flashing the Firmware

After building, you can flash the firmware to your connected Arduino.

<b>Flash RC Mini firmware:</b>

```bash
make flash_rcmini
```

<b>Flash Bamboo firmware:</b>

```bash
make flash_bamboo
```

These commands will:

- Detect a connected Arduino on a /dev/cu.usbmodem\* port.
- Compile the project if necessary.
- Flash the respective firmware to the Arduino.

To remove compiled files and clean the build directory:

```bash
make clean
```

### Troubleshooting

No Arduino Detected: Ensure your Arduino is connected and recognized by your computer. Run the following command to verify its presence:

```bash
ls /dev/cu.usbmodem*
```
