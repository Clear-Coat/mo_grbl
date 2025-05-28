PROJECT_NAME = grbl
SRC_DIR = $(PROJECT_NAME)
INO_FILE = $(PROJECT_NAME)/grbl.ino
BUILD_DIR = build
BOARD = arduino:avr:uno
VERSION = 1.2h

# Default build target
.PHONY: all
all: build_rcmini build_bamboo

# Build target for RC Mini
.PHONY: build_rcmini
build_rcmini:
	@mkdir -p $(BUILD_DIR)
	@arduino-cli compile --fqbn $(BOARD) \
		--build-path $(BUILD_DIR)/rcmini \
		--build-property "build.extra_flags=-DMACHINE_TYPE=RCMINI -DGRBL_VERSION=\"$(VERSION)\"" \
		$(INO_FILE)
	@cp $(BUILD_DIR)/rcmini/$(PROJECT_NAME).ino.hex $(BUILD_DIR)/rcmini$(VERSION).hex
	@echo "Built RC Mini firmware: $(BUILD_DIR)/rcmini$(VERSION).hex"

# Build target for Bamboo
.PHONY: build_bamboo
build_bamboo:
	@mkdir -p $(BUILD_DIR)
	@arduino-cli compile --fqbn $(BOARD) \
		--build-path $(BUILD_DIR)/bamboo \
		--build-property "build.extra_flags=-DMACHINE_TYPE=BAMBOO -DGRBL_VERSION=\"$(VERSION)\"" \
		$(INO_FILE)
	@cp $(BUILD_DIR)/bamboo/$(PROJECT_NAME).ino.hex $(BUILD_DIR)/bamboo$(VERSION).hex
	@echo "Built Bamboo firmware: $(BUILD_DIR)/bamboo$(VERSION).hex"

# Flash RC Mini firmware
.PHONY: flash_rcmini
flash_rcmini: build_rcmini
ifeq ($(OS),Windows_NT)
	@PORT=$$(powershell -Command 'Get-WmiObject Win32_SerialPort | Where-Object { $$_.Description -like "*Arduino*" } | Select-Object -First 1 -ExpandProperty DeviceID'); \
	arduino-cli upload -p $$PORT --fqbn $(BOARD) --input-dir $(BUILD_DIR)/rcmini --verbose && \
	echo "Flashed RC Mini firmware to $$PORT"
else
	@PORT=$$(ls /dev/cu.usbmodem* 2>/dev/null | head -n 1); \
	arduino-cli upload -p $$PORT --fqbn $(BOARD) --input-dir $(BUILD_DIR)/rcmini --verbose && \
	echo "Flashed RC Mini firmware to $$PORT"
endif

# Flash Bamboo firmware
.PHONY: flash_bamboo
flash_bamboo: build_bamboo
ifeq ($(OS),Windows_NT)
	@PORT=$$(powershell -Command 'Get-WmiObject Win32_SerialPort | Where-Object { $$_.Description -like "*Arduino*" } | Select-Object -First 1 -ExpandProperty DeviceID'); \
	arduino-cli upload -p $$PORT --fqbn $(BOARD) --input-dir $(BUILD_DIR)/bamboo --verbose && \
	echo "Flashed Bamboo firmware to $$PORT"
else
	@PORT=$$(ls /dev/cu.usbmodem* 2>/dev/null | head -n 1); \
	arduino-cli upload -p $$PORT --fqbn $(BOARD) --input-dir $(BUILD_DIR)/bamboo --verbose && \
	echo "Flashed Bamboo firmware to $$PORT"
endif

# Clean the build files
.PHONY: clean
clean:
ifeq ($(OS),Windows_NT)
	@if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR)
else
	@rm -rf $(BUILD_DIR)
endif
	@echo "Build directory cleaned"
