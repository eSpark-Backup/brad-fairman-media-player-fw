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

    for (const auto& video_file : video_files) {
        g_printerr("File : %s\n", video_file.c_str());

        loop = g_main_loop_new(NULL, FALSE);

        setup_pipeline(video_file.c_str(), pipeline, sink);

        // Configure the video sink to use the screen resolution
        sink = gst_bin_get_by_interface(GST_BIN(pipeline), GST_TYPE_VIDEO_RENDERER);
        if (sink) {
            g_object_set(sink, "fullscreen", TRUE, NULL);
            g_object_set(sink, "width", screen_width, NULL);
            g_object_set(sink, "height", screen_height, NULL);
            gst_object_unref(sink);
        } else {
            g_printerr("Failed to find a video sink element\n");
        }

        bus = gst_element_get_bus(pipeline);
        gst_bus_add_watch(bus, (GstBusFunc)on_bus_message, loop);

        g_main_loop_run(loop);

        gst_element_set_state(pipeline, GST_STATE_NULL);
        gst_object_unref(bus);
        gst_object_unref(pipeline);
        g_main_loop_unref(loop);
    }

    return 0;
}
