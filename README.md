# ESP32-CAM Robotic Hand Control System

This project implements a computer vision-based control system for a robotic hand using an ESP32-CAM module. The system is designed to detect and grasp specific objects (primarily cups and bottles) by processing camera input and controlling the robotic hand's movements. The entire application is developed in pure C/C++ and leverages a state machine for robust process management.

## Project Overview

The core objective is to enable the ESP32-CAM to act as the "eyes" for a robotic hand, guiding it through a sequence of actions:
1.  **Object Detection:** Identify target objects within the camera's field of view.
2.  **Alignment:** Position the robotic hand relative to the detected object.
3.  **Grasping:** Extend the hand, close fingers upon contact, and retract the object.

The system is built with modularity and performance in mind, given the resource constraints of the ESP32 platform.

## Architecture and Implementation

The project is structured into several key modules, orchestrated by a central state machine:

### 1. Camera & Image Acquisition ([src/video/camera.cpp](cci:7://file:///home/et/workspace/esp32cam/src/video/camera.cpp:0:0-0:0), [src/video/camera.h](cci:7://file:///home/et/workspace/esp32cam/src/video/camera.h:0:0-0:0))
-   **Functionality:** Initializes the OV2640 camera sensor, configures resolution (e.g., QVGA 320x240), and captures image frames.
-   **Details:** Handles camera setup parameters and provides functions for acquiring raw frame buffers.

### 2. Image Pre-processing ([src/video/preprocessing.cpp](cci:7://file:///home/et/workspace/esp32cam/src/video/preprocessing.cpp:0:0-0:0), [src/video/preprocessing.h](cci:7://file:///home/et/workspace/esp32cam/src/video/preprocessing.h:0:0-0:0))
-   **Functionality:** Prepares raw camera frames for analysis.
-   **Details:** Includes cropping the image to a Region of Interest (ROI) to reduce processing load and applying noise reduction techniques (e.g., median filtering).

### 3. Edge & Contour Detection (Integrated within `src/video/` modules)
-   **Functionality:** Identifies object boundaries within the pre-processed image.
-   **Details:** Utilizes the Sobel operator for efficient edge detection and a contour-following algorithm (e.g., Moore-Neighbor tracing) to extract significant object contours.

### 4. Object Analysis & Signature Matching ([src/video/object_analysis.cpp](cci:7://file:///home/et/workspace/esp32cam/src/video/object_analysis.cpp:0:0-0:0), [src/video/object_analysis.h](cci:7://file:///home/et/workspace/esp32cam/src/video/object_analysis.h:0:0-0:0))
-   **Functionality:** Analyzes detected contours to determine if they match predefined target object "signatures."
-   **Details:** Calculates key metrics such as bounding box dimensions, centroid, area, and aspect ratio. These metrics are then compared against configurable thresholds to identify target objects like cups or bottles.

### 5. Robot Control ([src/motors/hand.cpp](cci:7://file:///home/et/workspace/esp32cam/src/motors/hand.cpp:0:0-0:0), [src/motors/hand.h](cci:7://file:///home/et/workspace/esp32cam/src/motors/hand.h:0:0-0:0))
-   **Functionality:** Translates vision-based decisions into physical commands for the robotic hand.
-   **Details:** Provides functions for controlling the hand's movements, including alignment, extension, retraction, and opening/closing of fingers. These functions abstract the underlying communication with the robotic hardware.

### 6. State Machine ([src/state_machine/state_machine.h](cci:7://file:///home/et/workspace/esp32cam/src/state_machine/state_machine.h:0:0-0:0), [src/main.cpp](cci:7://file:///home/et/workspace/esp32cam/src/main.cpp:0:0-0:0))
-   **Functionality:** Manages the overall operational flow of the system.
-   **Details:** Implemented using the `boost.SML` (State Machine Library), a header-only C++14 library. It defines distinct states (e.g., `Idle`, `Searching`, `Aligning`, `Extending`, `Grasping`, `Retracting`, `Releasing`) and transitions between them based on events triggered by the vision modules and robotic hand feedback.

### 7. Network Communication ([src/network/network.cpp](cci:7://file:///home/et/workspace/esp32cam/src/network/network.cpp:0:0-0:0), [src/network/network.h](cci:7://file:///home/et/workspace/esp32cam/src/network/network.h:0:0-0:0))
-   **Functionality:** Handles Wi-Fi connectivity and TCP/UDP communication.
-   **Details:** Manages connection to a Wi-Fi network and sets up a TCP server, likely for receiving commands or sending status/debug information.

### 8. Utilities ([src/utils/macros.h](cci:7://file:///home/et/workspace/esp32cam/src/utils/macros.h:0:0-0:0))
-   **Functionality:** Provides common utility functions and macros.
-   **Details:** Includes macros for debug printing to the serial monitor, useful for development and troubleshooting.

### 9. LED Blinking (`src/blink/`)
-   **Functionality:** Provides visual feedback through the onboard LED.
-   **Details:** Implements various blinking sequences to indicate system status (e.g., boot, idle, busy).

## Calibration and Debugging

The project emphasizes robust calibration and debugging tools:
-   **In-firmware Debug Mode:** A special mode can be enabled to provide real-time feedback on image processing metrics (e.g., contour area, aspect ratio) and visual feedback (ASCII art representation of images) via the serial monitor.
-   **On-the-fly Parameter Tuning:** Key thresholds (e.g., Sobel edge threshold, object area/aspect ratio) can be adjusted via serial commands without recompiling, facilitating rapid tuning.
-   **Performance Monitoring:** A strategy is in place to track KPIs such as frame processing time, memory usage (heap, PSRAM), and frame rate to identify and address performance bottlenecks.

## Getting Started

To get this project running, you will typically need:
-   An ESP32-CAM development board.
-   A robotic hand system compatible with the control signals.
-   The ESP-IDF framework and Arduino IDE setup for ESP32.
-   The `boost.SML` library (header-only).
-   Proper configuration of Wi-Fi credentials in [src/network/network.h](cci:7://file:///home/et/workspace/esp32cam/src/network/network.h:0:0-0:0).

Further details on specific algorithms, tuning procedures, and performance considerations can be found in the `braindumps` directory.