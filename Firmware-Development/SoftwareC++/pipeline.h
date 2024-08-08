#ifndef PIPELINE_H
#define PIPELINE_H

#include <gst/gst.h>
#include <string>

void setup_pipeline(const std::string &file_location, GstElement *&pipeline, GstElement *&sink);

#endif // PIPELINE_H
