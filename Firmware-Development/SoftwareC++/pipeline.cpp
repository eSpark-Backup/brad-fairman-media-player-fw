#include "pipeline.h"
#include <iostream>

void setup_pipeline(const std::string &file_location, GstElement *&pipeline, GstElement *&sink) {
    std::string pipeline_description = "filesrc location=" + file_location +
                                       " ! decodebin name=decoder ! videoconvert ! autovideosink";
    
    pipeline = gst_parse_launch(pipeline_description.c_str(), NULL);
    sink = gst_bin_get_by_name(GST_BIN(pipeline), "autovideosink0");

    if (sink) {
        g_print("Found sink element: %s\n", GST_OBJECT_NAME(sink));
        // If fullscreen properties are supported, set them here
        // e.g., g_object_set(sink, "fullscreen", TRUE, NULL);
        gst_element_set_state(pipeline, GST_STATE_PLAYING);
        gst_object_unref(sink);
    } else {
        std::cerr << "Failed to find the 'autovideosink' element\n";
    }
}
