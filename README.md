# mo_grbl

This project is a customized fork of [GRBL](https://github.com/gnea/grbl), tailored to support multiple machine types and board configurations. The firmware includes build targets for each machine, allowing seamless configuration and flashing.

## Setup

### Prerequisites

1. **Chocolatey Installation**: Chocolatey is a package manager for Windows. To install it, follow these steps:

   - Open an elevated Command Prompt (Run as Administrator).
   - Run the following command to install Chocolatey:

     ```bash
     @"%SystemRoot%\\System32\\WindowsPowerShell\\v1.0\\powershell.exe" -NoProfile -InputFormat None -ExecutionPolicy Bypass -Command "iex ((New-Object System.Net.WebClient).DownloadString('https://chocolatey.org/install.ps1'))" && SET \"PATH=%PATH%;%ALLUSERSPROFILE%\\chocolatey\\bin\"
     ```

2. **Arduino CLI**:

   - **macOS**: Ensure Arduino CLI is installed. On macOS, you can install it with Homebrew:

   ```bash
   brew install arduino-cli
   ```

   - **Windows**: Download the Arduino CLI from the [official Arduino CLI website](https://arduino.github.io/arduino-cli/installation/). Follow the instructions to add it to your system's PATH.

3. **Arduino AVR Core**: Install the AVR core for Arduino boards:

   ```bash
   arduino-cli core install arduino:avr
   ```

4. **Make Utility**:

   - **macOS**: The make utility is typically pre-installed on macOS. If not, install it:

   ```bash
   brew install make
   ```

   - **Windows**: Install `make` using Chocolatey. First, install Chocolatey, then run the following command in an elevated command prompt:

   ```bash
   choco install make
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
