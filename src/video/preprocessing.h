#pragma once

#ifndef _PREPROCESSING_H_
#define _PREPROCESSING_H_

#include <stdint.h>
#include <stdbool.h>
#include "esp_camera.h"

#include "utils/macros.h"

// Target ROI dimensions (center 160x120 of 320x240 QVGA)
#define ROI_WIDTH 160
#define ROI_HEIGHT 120
#define ROI_X_OFFSET 80  // (320-160)/2
#define ROI_Y_OFFSET 60   // (240-120)/2

// Image buffer structure for processed frames
typedef struct {
    uint8_t *data;      // Pointer to image data
    uint16_t width;     // Image width
    uint16_t height;    // Image height
    size_t size;        // Size of the data buffer
} image_buffer_t;

/**
 * @brief Convert JPEG frame to grayscale ROI
 * 
 * @param fb Pointer to camera frame buffer (JPEG)
 * @param output Pre-allocated buffer for grayscale output (must be at least ROI_WIDTH * ROI_HEIGHT bytes)
 * @return true on success, false on failure
 */
bool jpeg_to_grayscale_roi(const camera_fb_t *fb, uint8_t *output);

/**
 * @brief Apply 3x3 median filter to grayscale image
 * 
 * @param input Input grayscale image
 * @param output Output buffer (can be same as input for in-place operation)
 * @param width Image width
 * @param height Image height
 */
void median_filter_3x3(const uint8_t *input, uint8_t *output, uint16_t width, uint16_t height);

/**
 * @brief Simple Sobel edge detection with thresholding
 * 
 * @param input Input grayscale image
 * @param output Binary output (0 or 255)
 * @param width Image width
 * @param height Image height
 * @param threshold Edge threshold (0-255)
 */
void sobel_edge_detect(const uint8_t *input, uint8_t *output, uint16_t width, uint16_t height, uint8_t threshold);

/**
 * @brief Simple debug visualization of binary image to serial
 * 
 * @param image Binary image data (0 or 255)
 * @param width Image width
 * @param height Image height
 * @param scale Scale factor for ASCII art (1-4)
 */
void debug_visualize_binary(const uint8_t *image, uint16_t width, uint16_t height, uint8_t scale);

#endif // _PREPROCESSING_H_
