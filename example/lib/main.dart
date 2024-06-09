import 'package:clipboard_manager/clipboard_manager.dart';
import 'package:flutter/material.dart';


void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text('Clipboard Plugin Example'),
        ),
        body: const ClipboardExample(),
      ),
    );
  }
}

class ClipboardExample extends StatefulWidget {
  const ClipboardExample({super.key});

  @override
  _ClipboardExampleState createState() => _ClipboardExampleState();
}

class _ClipboardExampleState extends State<ClipboardExample> {
  String? _primarySelection;
  String? _clipboard;

  @override
  Widget build(BuildContext context) {
    return Center(
      child: Column(
        mainAxisAlignment: MainAxisAlignment.center,
        children: <Widget>[
          ElevatedButton(
            onPressed: () async {
              final primarySelection = await ClipboardManager.getPrimarySelection();
              setState(() {
                _primarySelection = primarySelection;
              });
            },
            child: const Text('Get Primary Selection'),
          ),
          Text(_primarySelection ?? 'No Primary Selection'),
          const SizedBox(height: 20),
          ElevatedButton(
            onPressed: () async {
              final clipboard = await ClipboardManager.getClipboard();
              setState(() {
                _clipboard = clipboard;
              });
            },
            child: const Text('Get Clipboard'),
          ),
          Text(_clipboard ?? 'No Clipboard'),
          const SizedBox(height: 20),
          ElevatedButton(
            onPressed: () async {
              await ClipboardManager.setClipboard('Hello, Clipboard!');
            },
            child: const Text('Set Clipboard'),
          ),
        ],
      ),
    );
  }
}
