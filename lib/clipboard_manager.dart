import 'package:flutter/services.dart';

class ClipboardManager {
  static const MethodChannel _channel = MethodChannel('clipboard_manager');

  static Future<String?> getPrimarySelection() async {
    final String? selection = await _channel.invokeMethod('getPrimarySelection');
    return selection;
  }

  static Future<String?> getClipboard() async {
    final String? clipboard = await _channel.invokeMethod('getClipboard');
    return clipboard;
  }

  static Future<void> setClipboard(String text) async {
    await _channel.invokeMethod('setClipboard', text);
  }
}
