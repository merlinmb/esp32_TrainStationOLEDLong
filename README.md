# ESP32 Train Station OLED Long

Firmware for an ESP32-S3 based long OLED train departures display.

## Features

- Reads UK rail departure data via National Rail SOAP API.
- Renders departures and status on a 640x180 display.
- Publishes status and data over MQTT.
- Includes OTA firmware update page.
- Stores user display settings in SPIFFS (`/config.ini`).

## Hardware / Platform

- Board: ESP32-S3 (configured for `esp32-s3-devkitc-1` in PlatformIO)
- Display: long 3.4" class display using local display/touch drivers
- Framework: Arduino (PlatformIO)

## Project Structure

- `src/main.cpp`: main firmware loop and rendering logic
- `include/connectionDetails.h`: local credentials and endpoint settings (ignored by git)
- `include/connectionDetails.example.h`: template for `connectionDetails.h`
- `data/config.example.ini`: template for SPIFFS runtime configuration
- `data/config.ini`: local runtime configuration (ignored by git)

## Quick Start

1. Install [PlatformIO](https://platformio.org/) in VS Code.
2. Create local credentials file:
   - Copy `include/connectionDetails.example.h` to `include/connectionDetails.h`
   - Update WiFi, MQTT, OTA, and National Rail token values.
3. Create local runtime config file:
   - Copy `data/config.example.ini` to `data/config.ini`
   - Adjust station and display values as needed.
4. Build and upload:

```bash
pio run -e esp32-s3-devkitc-1
pio run -e esp32-s3-devkitc-1 -t upload
```

5. (Optional) Upload SPIFFS data files if needed by your workflow.

## Runtime Config (`data/config.ini`)

Supported keys:

- `station`: station CRS code (example: `NBY`)
- `flipscreen`: `true` or `false`
- `brightness`: `0` to `255`

Example:

```ini
station=NBY
flipscreen=false
brightness=255
```

## Security Notes

- Do not commit personal credentials or tokens.
- `include/connectionDetails.h` and `data/config.ini` are git-ignored.
- Keep private certificate key files out of source control.

## License

No license file is currently included in this repository.
Add a `LICENSE` file if you plan to share or reuse this project publicly.
