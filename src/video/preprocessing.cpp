#include "preprocessing.h"
// #include "esp_camera.h"
// #include "esp_heap_caps.h"

#include "camera.h"

// Lookup table for quick median calculation of 9 values
static inline uint8_t median9(uint8_t *v) {
    uint8_t t;
    // Simple bubble sort for 9 elements (optimal for small n)
    #define SWAP(a,b) { t = v[a]; v[a] = v[b]; v[b] = t; }
    
    // Perform bubble sort (9 elements, 36 comparisons max)
    for (int i = 0; i < 9; i++) {
        for (int j = i + 1; j < 9; j++) {
            if (v[j] < v[i]) {
                SWAP(i, j);
            }
        }
    }
    return v[4]; // Median is the middle element
}

bool jpeg_to_grayscale_roi(const camera_fb_t *fb, uint8_t *output) {
    if (!fb || !output || fb->format != PIXFORMAT_JPEG) {
        log_e("Invalid parameters for grayscale conversion");
        return false;
    }

    // Decode JPEG to RGB565
    camera_fb_t *rgb_fb = camera_get_frame();
    if (!rgb_fb) {
        log_e("Failed to get frame buffer");
        return false;
    }

    // Convert RGB565 to grayscale for ROI
    uint16_t *rgb = (uint16_t *)rgb_fb->buf;
    uint8_t *out_ptr = output;
    
    for (int y = ROI_Y_OFFSET; y < ROI_Y_OFFSET + ROI_HEIGHT; y++) {
        for (int x = ROI_X_OFFSET; x < ROI_X_OFFSET + ROI_WIDTH; x++) {
            uint16_t pixel = rgb[y * (rgb_fb->width) + x];
            // Extract RGB565 components and convert to grayscale using standard weights
            uint8_t r = (pixel >> 11) & 0x1F;
            uint8_t g = (pixel >> 5) & 0x3F;
            uint8_t b = pixel & 0x1F;
            *out_ptr++ = (r * 77 + g * 150 + b * 29) >> 8;  // Standard weights: 0.299R + 0.587G + 0.114B
        }
    }
    
    camera_return_frame(rgb_fb);
    return true;
}

void median_filter_3x3(const uint8_t *input, uint8_t *output, uint16_t width, uint16_t height) {
    if (!input || !output) return;
    
    uint8_t window[9];
    
    // Process inner pixels (ignore border pixels for simplicity)
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            // Fill 3x3 window
            int i = 0;
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    window[i++] = input[(y + dy) * width + (x + dx)];
                }
            }
            // Get median and store in output
            output[y * width + x] = median9(window);
        }
    }
    
    // Handle border pixels (copy from input)
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (y == 0 || y == height - 1 || x == 0 || x == width - 1) {
                output[y * width + x] = input[y * width + x];
            }
        }
    }
}

void sobel_edge_detect(const uint8_t *input, uint8_t *output, uint16_t width, uint16_t height, uint8_t threshold) {
    if (!input || !output) return;
    
    // Sobel kernels
    static const int8_t sobel_x[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    static const int8_t sobel_y[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
    
    // Process inner pixels (ignore border pixels)
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            int gx = 0, gy = 0;
            int i = 0;
            
            // Apply Sobel kernels
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    uint8_t pixel = input[(y + dy) * width + (x + dx)];
                    gx += pixel * sobel_x[i];
                    gy += pixel * sobel_y[i++];
                }
            }
            
            // Calculate gradient magnitude (approximate with |Gx| + |Gy| for speed)
            int magnitude = abs(gx) + abs(gy);
            
            // Apply threshold
            output[y * width + x] = (magnitude > threshold) ? 255 : 0;
        }
    }
    
    // Clear border pixels
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (y == 0 || y == height - 1 || x == 0 || x == width - 1) {
                output[y * width + x] = 0;
            }
        }
    }
}

void debug_visualize_binary(const uint8_t *image, uint16_t width, uint16_t height, uint8_t scale) {
    if (!image || scale == 0) return;
    
    // ASCII characters for different intensity levels
    static const char *chars = " .:-=+*#%@";
    const int num_chars = 10;
    
    // Scale down the output
    int step = 1 << scale;  // 1, 2, or 4
    
    log_printf("\nBinary Image (%dx%d):\n", width, height);
    
    for (int y = 0; y < height; y += step) {
        for (int x = 0; x < width; x += step) {
            // Sample the pixel (simple point sampling)
            log_printf("%c", image[y * width + x] > 128 ? '#' : ' ');
        }
        log_printf("\n");
    }
    log_printf("\n");
}
