import 'package:plugin_platform_interface/plugin_platform_interface.dart';

import 'clipboard_manager_method_channel.dart';

abstract class ClipboardManagerPlatform extends PlatformInterface {
  /// Constructs a ClipboardManagerPlatform.
  ClipboardManagerPlatform() : super(token: _token);

  static final Object _token = Object();

  static ClipboardManagerPlatform _instance = MethodChannelClipboardManager();

  /// The default instance of [ClipboardManagerPlatform] to use.
  ///
  /// Defaults to [MethodChannelClipboardManager].
  static ClipboardManagerPlatform get instance => _instance;

  /// Platform-specific implementations should set this with their own
  /// platform-specific class that extends [ClipboardManagerPlatform] when
  /// they register themselves.
  static set instance(ClipboardManagerPlatform instance) {
    PlatformInterface.verifyToken(instance, _token);
    _instance = instance;
  }

  Future<String?> getPlatformVersion() {
    throw UnimplementedError('platformVersion() has not been implemented.');
  }
}
