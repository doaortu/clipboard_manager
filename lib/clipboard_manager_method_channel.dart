import 'package:flutter/foundation.dart';
import 'package:flutter/services.dart';

import 'clipboard_manager_platform_interface.dart';

/// An implementation of [ClipboardManagerPlatform] that uses method channels.
class MethodChannelClipboardManager extends ClipboardManagerPlatform {
  /// The method channel used to interact with the native platform.
  @visibleForTesting
  final methodChannel = const MethodChannel('clipboard_manager');

  @override
  Future<String?> getPlatformVersion() async {
    final version = await methodChannel.invokeMethod<String>('getPlatformVersion');
    return version;
  }
}
