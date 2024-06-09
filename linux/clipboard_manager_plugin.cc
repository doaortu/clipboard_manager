#include "include/clipboard_manager/clipboard_manager_plugin.h"

#include <flutter_linux/flutter_linux.h>
#include <gtk/gtk.h>
#include <sys/utsname.h>

#include <cstring>

#include "clipboard_manager_plugin_private.h"

#define CLIPBOARD_MANAGER_PLUGIN(obj)                                          \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), clipboard_manager_plugin_get_type(),      \
                              ClipboardManagerPlugin))

struct _ClipboardManagerPlugin {
  GObject parent_instance;
};

G_DEFINE_TYPE(ClipboardManagerPlugin, clipboard_manager_plugin,
              g_object_get_type())

// Get primary selection
FlMethodResponse *get_primary_selection() {
  GtkClipboard *clipboard = gtk_clipboard_get(GDK_SELECTION_PRIMARY);
  gchar *text = gtk_clipboard_wait_for_text(clipboard);
  if (text) {
    FlValue *result = fl_value_new_string(text);
    g_free(text);
    return FL_METHOD_RESPONSE(fl_method_success_response_new(result));
  } else {
    return FL_METHOD_RESPONSE(fl_method_error_response_new(
        "UNAVAILABLE", "Primary selection is unavailable", nullptr));
  }
}

// Get clipboard contents
FlMethodResponse *get_clipboard() {
  GtkClipboard *clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
  gchar *text = gtk_clipboard_wait_for_text(clipboard);
  if (text) {
    FlValue *result = fl_value_new_string(text);
    g_free(text);
    return FL_METHOD_RESPONSE(fl_method_success_response_new(result));
  } else {
    return FL_METHOD_RESPONSE(fl_method_error_response_new(
        "UNAVAILABLE", "Clipboard is unavailable", nullptr));
  }
}

// Set clipboard contents
FlMethodResponse *set_clipboard(FlValue *value) {
  if (fl_value_get_type(value) != FL_VALUE_TYPE_STRING) {
    return FL_METHOD_RESPONSE(fl_method_error_response_new(
        "INVALID_ARGUMENT", "Expected string", nullptr));
  }

  const gchar *text = fl_value_get_string(value);
  GtkClipboard *clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
  gtk_clipboard_set_text(clipboard, text, -1);
  return FL_METHOD_RESPONSE(
      fl_method_success_response_new(fl_value_new_null()));
}

// Called when a method call is received from Flutter.
static void
clipboard_manager_plugin_handle_method_call(ClipboardManagerPlugin *self,
                                            FlMethodCall *method_call) {
  g_autoptr(FlMethodResponse) response = nullptr;

  const gchar *method = fl_method_call_get_name(method_call);

  if (strcmp(method, "getPrimarySelection") == 0) {
    response = get_primary_selection();
  } else if (strcmp(method, "getClipboard") == 0) {
    response = get_clipboard();
  } else if (strcmp(method, "setClipboard") == 0) {
    FlValue *args = fl_method_call_get_args(method_call);
    response = set_clipboard(args);
  } else {
    response = FL_METHOD_RESPONSE(fl_method_not_implemented_response_new());
  }

  fl_method_call_respond(method_call, response, nullptr);
}

// FlMethodResponse *get_platform_version() {
//   struct utsname uname_data = {};
//   uname(&uname_data);
//   g_autofree gchar *version = g_strdup_printf("Linux %s", uname_data.version);
//   g_autoptr(FlValue) result = fl_value_new_string(version);
//   return FL_METHOD_RESPONSE(fl_method_success_response_new(result));
// }

static void clipboard_manager_plugin_dispose(GObject *object) {
  G_OBJECT_CLASS(clipboard_manager_plugin_parent_class)->dispose(object);
}

static void
clipboard_manager_plugin_class_init(ClipboardManagerPluginClass *klass) {
  G_OBJECT_CLASS(klass)->dispose = clipboard_manager_plugin_dispose;
}

static void clipboard_manager_plugin_init(ClipboardManagerPlugin *self) {}

static void method_call_cb(FlMethodChannel *channel, FlMethodCall *method_call,
                           gpointer user_data) {
  ClipboardManagerPlugin *plugin = CLIPBOARD_MANAGER_PLUGIN(user_data);
  clipboard_manager_plugin_handle_method_call(plugin, method_call);
}

void clipboard_manager_plugin_register_with_registrar(
    FlPluginRegistrar *registrar) {
  ClipboardManagerPlugin *plugin = CLIPBOARD_MANAGER_PLUGIN(
      g_object_new(clipboard_manager_plugin_get_type(), nullptr));

  g_autoptr(FlStandardMethodCodec) codec = fl_standard_method_codec_new();
  g_autoptr(FlMethodChannel) channel =
      fl_method_channel_new(fl_plugin_registrar_get_messenger(registrar),
                            "clipboard_manager", FL_METHOD_CODEC(codec));
  fl_method_channel_set_method_call_handler(
      channel, method_call_cb, g_object_ref(plugin), g_object_unref);

  g_object_unref(plugin);
}
