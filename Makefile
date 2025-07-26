# Defaults

VENV ?= "${HOME}/.platformio/penv/"

PIO_ENV ?= esp32cam


.PHONY: help build upload monitor clean requirements

SHELL := /bin/bash

help:
	@echo "make requirements build upload monitor clean"

# Build the project
build:
	source "$(VENV)/bin/activate"	&& \
	pio run -e "$(PIO_ENV)"

# Upload firmware to the device
upload:
	source "$(VENV)/bin/activate"	&& \
    pio run -e "$(PIO_ENV)" -t upload

# Open serial monitor
monitor:
	source "$(VENV)/bin/activate"	&& \
    pio device monitor -e "$(PIO_ENV)"

# Clean build files
clean:
	source "$(VENV)/bin/activate"	&& \
    pio run -e "$(PIO_ENV)" -t clean

# 	TODO: pio init --board esp32cam
requirements:
	source "$(VENV)/bin/activate"	&& \
	pip install jupyter opencv-python
