#include <gst/gst.h>
#include <glib.h>
#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
#include "pipeline.h"
#include "utils.h"

static gboolean on_bus_message(GstBus *bus, GstMessage *msg, gpointer user_data) {
    GMainLoop *loop = (GMainLoop *)user_data;

    switch (GST_MESSAGE_TYPE(msg)) {
        case GST_MESSAGE_EOS:
            g_main_loop_quit(loop);
            break;
        case GST_MESSAGE_ERROR: {
            gchar *debug;
            GError *error;

            gst_message_parse_error(msg, &error, &debug);
            g_printerr("Error received from element %s: %s\n",
                       GST_OBJECT_NAME(msg->src), error->message);
            g_printerr("Debugging info: %s\n", debug ? debug : "none");
            g_error_free(error);
            g_free(debug);
            g_main_loop_quit(loop);
            break;
        }
        default:
            break;
    }

    return TRUE;
}

bool is_sdcard_present() {
    return std::filesystem::exists("/sdcard");
}

std::vector<std::string> get_video_files(const std::string& dir_path) {
    std::vector<std::string> video_files;
    std::vector<std::string> video_extensions = { ".mp4", ".avi", ".mkv", ".mov", ".webm" };

    for (const auto& entry : std::filesystem::directory_iterator(dir_path)) {
        std::string path = entry.path().string();
        for (const auto& ext : video_extensions) {
            if (path.ends_with(ext)) {
                video_files.push_back(path);
                break;
            }
        }
    }

    return video_files;
}

static void on_pad_added(GstElement *element, GstPad *pad, GstElement *sink) {
    GstPad *sink_pad = gst_element_get_static_pad(sink, "sink");
    if (gst_pad_is_linked(sink_pad)) {
        gst_object_unref(sink_pad);
        return;
    }
    gst_pad_link(pad, sink_pad);
    gst_object_unref(sink_pad);
}

int main(int argc, char *argv[]) {
    GstElement *pipeline;
    GstElement *sink;
    GstBus *bus;
    GMainLoop *loop;

    int screen_width, screen_height;

    gst_init(&argc, &argv);

    if (get_display_resolution(screen_width, screen_height) != 0) {
        return -1;
    }

    if (!is_sdcard_present()) {
        g_printerr("SD card not found.\n");
        return -1;
    }

    std::vector<std::string> video_files = get_video_files("/sdcard");

    if (video_files.empty()) {
        g_printerr("No supported video files found on SD card.\n");
        return -1;
    }

    while (1)
    {
        for (const auto& video_file : video_files) {
            g_printerr("File : %s\n", video_file.c_str());

            loop = g_main_loop_new(NULL, FALSE);
            pipeline = gst_pipeline_new("video-player");

            if (!pipeline) {
                g_printerr("Failed to create pipeline\n");
                return -1;
            }

            GstElement *source = gst_element_factory_make("filesrc", "file-source");
            GstElement *decoder = gst_element_factory_make("decodebin", "decoder");
            GstElement *videoscale = gst_element_factory_make("videoscale", "videoscale");
            GstElement *capsfilter = gst_element_factory_make("capsfilter", "capsfilter");
            GstCaps *caps = gst_caps_new_simple("video/x-raw",
                                                "width", G_TYPE_INT, screen_width,
                                                "height", G_TYPE_INT, screen_height,
                                                NULL);

            if (!source || !decoder || !videoscale || !capsfilter) {
                g_printerr("Failed to create elements\n");
                return -1;
            }

            g_object_set(source, "location", video_file.c_str(), NULL);
            g_object_set(capsfilter, "caps", caps, NULL);

            gst_bin_add_many(GST_BIN(pipeline), source, decoder, videoscale, capsfilter, NULL);
            gst_element_link(source, decoder);
            gst_element_link(decoder, videoscale);
            gst_element_link(videoscale, capsfilter);

            sink = gst_element_factory_make("autovideosink", "video-sink");
            if (!sink) {
                g_printerr("Failed to create video sink\n");
                return -1;
            }

            gst_bin_add(GST_BIN(pipeline), sink);
            gst_element_link(capsfilter, sink);

            g_signal_connect(decoder, "pad-added", G_CALLBACK(on_pad_added), videoscale);

            gst_element_set_state(pipeline, GST_STATE_PLAYING);

            bus = gst_element_get_bus(pipeline);
            gst_bus_add_watch(bus, (GstBusFunc)on_bus_message, loop);

            g_main_loop_run(loop);

            gst_element_set_state(pipeline, GST_STATE_NULL);
            gst_object_unref(bus);
            gst_object_unref(pipeline);
            gst_caps_unref(caps);
            g_main_loop_unref(loop);
        }
    }
    
    return 0;
}
