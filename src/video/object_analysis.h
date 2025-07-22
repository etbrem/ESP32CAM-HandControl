#pragma once

#include "esp_camera.h"
#include "camera.h"

// Basic struct for a 2D point
typedef struct {
    int x;
    int y;
} point_t;

// Struct to hold information for re-identifying an object
typedef struct {
    int x;
    int y;
    int w;
    int h;
} tracking_data_t;

// Struct to hold all calculated metrics for a detected object
typedef struct {
    point_t centroid;
    int width;
    int height;
    int area;
    float aspect_ratio;
    float signature_score; // Score indicating how well it matches the target
    bool is_valid;         // Flag to indicate if the metrics are for a valid object
} object_metrics_t;


// Main entry point for the vision pipeline.
// Finds the best matching object in the frame and returns its metrics.
// Also populates tracking_info for the state machine.
object_metrics_t find_best_object(camera_fb_t* fb, tracking_data_t* tracking_info);

// Analyzes a single contour to calculate its metrics and signature score.
object_metrics_t analyze_contour(const point_t* contour, int num_points);

// Tracks a previously identified object to see if it's still in the frame.
// Returns the object's current metrics.
object_metrics_t track_object(camera_fb_t* fb, const tracking_data_t* current_target);

// Checks if the robot is centered on the target.
// The implementation of this will likely depend on the state machine's tracking data.
bool robot_is_centered();



