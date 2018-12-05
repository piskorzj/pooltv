#include <fmt/format.h>
#include <gst/gst.h>
#include <memory>

struct data_t {
  std::unique_ptr<GMainLoop, decltype(&g_main_loop_unref)> loop {nullptr, g_main_loop_unref};
  std::unique_ptr<GstElement, decltype(&gst_object_unref)> pipeline {nullptr, gst_object_unref};
};

static void error_cb(GstBus *bus, GstMessage *msg, data_t *data) {
  GError *err;
  gchar *debug_info;

  /* Print error details on the screen */
  gst_message_parse_error(msg, &err, &debug_info);
  fmt::print("Error from element{}: {}\n", GST_OBJECT_NAME(msg->src), err->message);
  fmt::print("Debug info: {}\n", debug_info ? debug_info : "none");
  g_clear_error(&err);
  g_free(debug_info);

  g_main_loop_quit(data->loop.get());
}

int main(int argc, char** argv) {
  gst_init (&argc, &argv);

  data_t data{};

  GError *error = nullptr;
  auto pipeline = gst_parse_launch_full(fmt::format(
    "rtpbin name=rtpbin do-retransmission=true rtp-profile=avpf"
    " fdsrc ! h264parse ! rtph264pay ! rtpbin.send_rtp_sink_0"
    " rtpbin.send_rtp_src_0 ! udpsink host={addr} port=4000 multicast-iface={iface}"
    " rtpbin.send_rtcp_src_0 ! udpsink name=sink_rtcp host={addr} port=5001 multicast-iface={iface} sync=false async=false"
    " udpsrc port=5002 address={addr} multicast-iface={iface} ! rtpbin.recv_rtcp_sink_0",
    fmt::arg("addr", "224.100.100.100"), fmt::arg("iface", "eth0")).c_str(),
    nullptr, GST_PARSE_FLAG_FATAL_ERRORS, &error);

  if(pipeline == nullptr) {
    std::string error_msg = error->message;
    int code = error->code;
    g_error_free(error);
    fmt::print("GStreamer error {}: {}\n", code, error_msg);
    std::exit(-1);
  }

  data.pipeline.reset(pipeline);

  auto bus = gst_element_get_bus(data.pipeline.get());
  gst_bus_add_signal_watch(bus);
  g_signal_connect(G_OBJECT (bus), "message::error", (GCallback)error_cb, &data);
  gst_object_unref(bus);

  gst_element_set_state(data.pipeline.get(), GST_STATE_PLAYING);

  data.loop.reset(g_main_loop_new(nullptr, false));
  g_main_loop_run(data.loop.get());

  gst_element_set_state(data.pipeline.get(), GST_STATE_NULL);
}
