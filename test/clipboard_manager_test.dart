import 'package:flutter_test/flutter_test.dart';
import 'package:clipboard_manager/clipboard_manager.dart';
import 'package:clipboard_manager/clipboard_manager_platform_interface.dart';
import 'package:clipboard_manager/clipboard_manager_method_channel.dart';
import 'package:plugin_platform_interface/plugin_platform_interface.dart';

class MockClipboardManagerPlatform
    with MockPlatformInterfaceMixin
    implements ClipboardManagerPlatform {

  @override
  Future<String?> getPlatformVersion() => Future.value('42');
}

void main() {
  final ClipboardManagerPlatform initialPlatform = ClipboardManagerPlatform.instance;

  test('$MethodChannelClipboardManager is the default instance', () {
    expect(initialPlatform, isInstanceOf<MethodChannelClipboardManager>());
  });

  test('getPlatformVersion', () async {
    ClipboardManager clipboardManagerPlugin = ClipboardManager();
    MockClipboardManagerPlatform fakePlatform = MockClipboardManagerPlatform();
    ClipboardManagerPlatform.instance = fakePlatform;

    expect(await clipboardManagerPlugin.getPlatformVersion(), '42');
  });
}
