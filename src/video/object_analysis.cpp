#include "video/object_analysis.h"
#include "utils/macros.h"

// Placeholder implementation for analyzing a contour.
// In a real implementation, this would calculate metrics from the contour points.
object_metrics_t analyze_contour(const point_t* contour, int num_points) {
    object_metrics_t metrics = {0};
    metrics.is_valid = true;
    metrics.signature_score = 0.9f; // High score for testing
    metrics.centroid = {160, 120}; // Dummy centroid
    return metrics;
}

// Placeholder implementation for finding the best object.
// This function simulates finding an object to test the state machine flow.
object_metrics_t find_best_object(camera_fb_t* fb, tracking_data_t* tracking_info) {
    log_d("ANALYSIS: Searching for best object...");
    
    // Simulate finding one object
    object_metrics_t best_object = analyze_contour(nullptr, 0);

    if (best_object.is_valid) {
        log_d("Found a valid object with score: %.2f", best_object.signature_score);
        // Populate tracking_info for the state machine
        tracking_info->x = best_object.centroid.x - 20;
        tracking_info->y = best_object.centroid.y - 30;
        tracking_info->w = 40;
        tracking_info->h = 60;
    } else {
        log_d("No valid objects found.");
    }

    return best_object;
}

// Placeholder for tracking an object. 
// This simulates finding the object slightly off-center to test alignment.
// After a few calls, it will report the object as centered.
object_metrics_t track_object(camera_fb_t* fb, const tracking_data_t* current_target) {
    static int alignment_steps = 0;
    object_metrics_t metrics = {0};

    // Simulate losing the track after 10 steps for testing `object_lost`
    if (alignment_steps > 10) {
        log_d("TRACKING: Object lost.");
        metrics.is_valid = false;
        alignment_steps = 0; // Reset for next time
        return metrics;
    }

    // Simulate the object being centered after 5 steps
    if (alignment_steps < 5) {
        log_d("TRACKING: Object is off-center.");
        metrics.centroid = { current_target->x + 20, current_target->y - 15 };
    } else {
        log_d("TRACKING: Object is now centered.");
        metrics.centroid = { 160, 120 }; // Exact center
    }

    metrics.is_valid = true;
    alignment_steps++;

    return metrics;
}

bool robot_is_centered() {
    log_d("GUARD: Checking if robot is centered...");
    // TODO: Implement actual logic to check if the object's centroid
    // is within a tolerance range of the ROI's center.
    return true; // For now, always return true to allow state transition
}
