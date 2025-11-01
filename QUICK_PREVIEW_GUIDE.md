# Quick Preview Guide - Run ScaleChord VST3 Without Installation

## Overview
Want to test the ScaleChord plugin without installing it system-wide? This guide shows you several methods to run and preview the plugin directly.

---

## Method 1: Direct Plugin Testing (No DAW Required) ⭐ FASTEST

### Use JUCE's Plugin Host
JUCE comes with a built-in plugin host for testing. This is the fastest way to hear the plugin immediately.

```bash
# Navigate to the plugin directory
cd /workspaces/production-works/plugin

# If you have the JUCE plugin host compiled:
/tmp/JUCE/extras/Build/juce_build/juce_AudioPluginHost_artefacts/Release/juce_AudioPluginHost

# Or use the compiled demo if available:
./build/scalechord_demo
```

**Advantages:**
✅ No DAW installation needed  
✅ Instant testing  
✅ See real-time GUI  
✅ Test MIDI input directly  

**Steps:**
1. Run the plugin host
2. Click "File" → "Scan for Plugins"
3. Select the ScaleChord.vst3 plugin
4. Open "Plugin Windows" → "ScaleChord"
5. Connect MIDI input and play!

---

## Method 2: Use Pre-Built Standalone Demo

### Run the Scalechord Demo Application

```bash
# Navigate to build directory
cd /workspaces/production-works/plugin/build

# Run the standalone demo
./scalechord_demo

# Or the advanced demo
./scalechord_advanced
```

**What you get:**
✅ Fully functional standalone application  
✅ No plugin host needed  
✅ Direct MIDI keyboard input  
✅ All features working  

**Expected Output:**
```
Starting ScaleChord Standalone Demo...
Initializing JUCE Audio Engine...
Opening Audio Device...
Waiting for MIDI Input...
Ready for keyboard input!
```

---

## Method 3: Copy Plugin to DAW's Test Folder (Non-Destructive)

### Temporary Plugin Folder Testing

Instead of installing system-wide, create a temporary test folder:

```bash
# Create a test VST3 folder
mkdir -p ~/vst3_test
cp -r /workspaces/production-works/plugin/vst3_build/ScaleChordPlugin_artefacts/Release/VST3/ScaleChord.vst3 ~/vst3_test/

# Now point your DAW to scan this folder
# Most DAWs allow custom plugin paths in preferences
```

**DAW Setup (Example: Linux):**

**Studio One:**
```
Preferences → Plug-In Manager → Locations
Add: ~/vst3_test/
Rescan Plug-Ins
```

**Reaper:**
```
Options → Preferences → Plug-Ins → VST3
Add directory: ~/vst3_test/
Rescan plugins
```

**Bitwig:**
```
Settings → Plugin Browser → VST3 Locations
Add: ~/vst3_test/
Rescan
```

**Ableton Live:**
```
Live → Preferences → File/Folder → Use External Audio Effect Plugins
Add: ~/vst3_test/
Rescan
```

**Advantages:**
✅ No system pollution  
✅ Easy to remove (just delete folder)  
✅ Multiple versions can coexist  
✅ Safe testing environment  

---

## Method 4: Development Build System

### Run with CMake Build Directory

```bash
# Stay in the build directory
cd /workspaces/production-works/plugin/build

# Rebuild the plugin (if you made changes)
make -j4

# Test the compiled binary directly
ls -lh ScaleChordPlugin_artefacts/Release/VST3/ScaleChord.vst3/
```

**Direct File Testing:**
```bash
# Check if plugin is valid VST3 bundle
file ScaleChordPlugin_artefacts/Release/VST3/ScaleChord.vst3/

# Output should show it's a directory (VST3 bundle)
# Then copy to test location:
cp -r ScaleChordPlugin_artefacts/Release/VST3/ScaleChord.vst3 ~/vst3_test/
```

---

## Method 5: Docker/Container Testing (Advanced)

### Run Plugin in Isolated Environment

```bash
# If you want to test without affecting system
# Create a container with VST3 support

cd /workspaces/production-works/plugin

# Build Docker image with JUCE
docker build -t scalechord-test .

# Run container with audio
docker run -it \
  -v /workspaces/production-works/plugin:/plugin:ro \
  -e DISPLAY=$DISPLAY \
  -v /tmp/.X11-unix:/tmp/.X11-unix \
  scalechord-test \
  /plugin/build/scalechord_demo
```

---

## Method 6: Command-Line MIDI Testing

### Test Without GUI (Linux/macOS)

```bash
# List available ALSA MIDI devices
aconnect -l

# Or use amidi to test MIDI connectivity
amidi -l

# Test the plugin with raw MIDI messages
amidi -p hw:1,0,0 < midi_test_file.mid
```

**Create a Test MIDI File:**
```bash
# Simple C major chord (C3, E3, G3)
cat > test_notes.mid << 'EOF'
# MIDI Note On messages for testing
EOF

# Play through the plugin
# (Requires MIDI routing setup)
```

---

## Method 7: JACK Audio Server (Linux)

### Use JACK for Low-Latency Testing

```bash
# Start JACK server
jackd -d alsa -d hw:0 -r 48000 -p 512 &

# Install carla (JACK plugin host)
sudo apt-get install carla

# Run Carla
carla

# Load ScaleChord.vst3 plugin
# Connect MIDI input → Carla → Audio output
```

**Carla Interface:**
```
┌─────────────────────────────────┐
│ Carla Plugin Host               │
├─────────────────────────────────┤
│ [Add Plugin]                    │
│                                 │
│ ScaleChord                      │
│ ├─ MIDI Input                   │
│ ├─ Audio Output                 │
│ └─ Controls                     │
│                                 │
│ [Play] [Record] [Monitor]       │
└─────────────────────────────────┘
```

---

## Method 8: Use REAPER Portable (Easiest for Testing)

### REAPER with VST3 Support

REAPER doesn't require installation on Linux, making it perfect for testing:

```bash
# Download and extract REAPER portable
cd ~/Downloads
wget https://www.reaper.fm/files/6.x/reaper6_linux_x86_64.tar.xz
tar xf reaper6_linux_x86_64.tar.xz

# Navigate to REAPER directory
cd reaper

# Create symlink to test plugin
ln -s /workspaces/production-works/plugin/vst3_build/ScaleChordPlugin_artefacts/Release/VST3/ScaleChord.vst3 \
      UserPlugins/

# Run REAPER portable
./reaper
```

**In REAPER:**
1. Go to Extensions → Refresh Plug-Ins
2. Filter for "ScaleChord"
3. Insert on MIDI track
4. Play MIDI notes and test!

---

## Quick Start Comparison

| Method | Setup Time | Testing Speed | Pros | Cons |
|--------|-----------|---------------|------|------|
| **Standalone Demo** | 1 min | Instant | No dependencies | Limited DAW features |
| **Temp Folder** | 2 min | 5 min | Safe, reversible | Need DAW open |
| **JUCE Host** | 3 min | 2 min | Official tool | Less intuitive |
| **Docker** | 10 min | 3 min | Isolated | Requires Docker |
| **JACK/Carla** | 5 min | 4 min | Professional | Linux only |
| **REAPER Portable** | 5 min | 2 min | User-friendly | Requires download |

---

## Step-by-Step: Fastest Preview (Under 2 Minutes)

### Using Standalone Demo

**Step 1: Open Terminal**
```bash
cd /workspaces/production-works/plugin/build
```

**Step 2: Run Demo**
```bash
./scalechord_demo
```

**Step 3: Connect MIDI Keyboard**
```
If using external keyboard:
- Connect USB MIDI keyboard to computer
- Demo should auto-detect it

If using software:
- Use DAW's virtual keyboard
- Or use on-screen MIDI piano
```

**Step 4: Play Music**
```
Simply play notes on your MIDI keyboard!
The plugin will:
- Analyze incoming chords
- Generate intelligent voicings
- Output voiced notes in real-time
- Display information on screen
```

---

## Advanced: Create a Test Script

### Automated Testing Script

Create a file `test_plugin.sh`:

```bash
#!/bin/bash

# ScaleChord Plugin Quick Test Script

echo "╔════════════════════════════════════════════════════╗"
echo "║   ScaleChord VST3 Plugin - Quick Preview          ║"
echo "╚════════════════════════════════════════════════════╝"
echo ""

# Navigate to build directory
cd /workspaces/production-works/plugin/build

# Check if plugin exists
if [ ! -d "ScaleChordPlugin_artefacts/Release/VST3/ScaleChord.vst3" ]; then
    echo "❌ Plugin not found! Rebuilding..."
    cd ..
    mkdir -p build
    cd build
    cmake -DCMAKE_BUILD_TYPE=Release ..
    make -j4
fi

echo "✅ Plugin found!"
echo ""
echo "Plugin Location:"
echo "  $(pwd)/ScaleChordPlugin_artefacts/Release/VST3/ScaleChord.vst3"
echo ""
echo "File size:"
du -sh ScaleChordPlugin_artefacts/Release/VST3/ScaleChord.vst3/
echo ""

# Option to copy to test folder
read -p "Copy to test folder ~/vst3_test/? (y/n) " -n 1 -r
echo ""
if [[ $REPLY =~ ^[Yy]$ ]]; then
    mkdir -p ~/vst3_test
    cp -r ScaleChordPlugin_artefacts/Release/VST3/ScaleChord.vst3 ~/vst3_test/
    echo "✅ Copied to ~/vst3_test/"
fi

echo ""
echo "Next steps:"
echo "1. Open your DAW"
echo "2. Add custom plugin path: ~/vst3_test/"
echo "3. Rescan VST3 plugins"
echo "4. Add ScaleChord to a MIDI track"
echo "5. Play and enjoy!"
```

**Usage:**
```bash
chmod +x test_plugin.sh
./test_plugin.sh
```

---

## Environment Variables for Testing

### Control Plugin Behavior

```bash
# Enable debug logging
export JUCE_ENABLE_DEBUG=1

# Force specific audio device
export JUCE_AUDIO_DEVICE="hw:0"

# Set MIDI input device
export JUCE_MIDI_INPUT="USB MIDI"

# Run plugin with environment
JUCE_ENABLE_DEBUG=1 ./scalechord_demo
```

---

## Troubleshooting Preview Issues

### Plugin Not Found

```bash
# Verify plugin exists
ls -la /workspaces/production-works/plugin/vst3_build/ScaleChordPlugin_artefacts/Release/VST3/ScaleChord.vst3/

# If missing, rebuild
cd /workspaces/production-works/plugin/build
make clean
make -j4
```

### No MIDI Input

```bash
# Check MIDI devices (Linux)
aconnect -l

# Test with software keyboard
# Or connect USB MIDI device

# In JUCE Host, manually select MIDI input
```

### Audio Not Working

```bash
# Check audio devices
aplay -l

# Test audio system
speaker-test -c 2 -t sine -f 1000

# Restart JACK/audio server
jackctl stop
jackctl start
```

### Plugin Crashes on Load

```bash
# Rebuild in debug mode
cd /workspaces/production-works/plugin/build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make

# Run with debug output
gdb ./scalechord_demo
(gdb) run
```

---

## Performance Testing Without Installation

### Benchmark the Plugin

```bash
# Create test MIDI file with many notes
python3 << 'PYTHON'
import midiutil

MyMIDI = midiutil.MIDIFile(1)
track = 0
channel = 0
time = 0
duration = 1
tempo = 120

# Create 100 note sequence
for i in range(100):
    pitch = 60 + (i % 12)
    MyMIDI.addNote(track, channel, pitch, time, duration, 100)
    time += 0.5

with open("benchmark.mid", "wb") as output_file:
    MyMIDI.writeFile(output_file)
PYTHON

# Play through plugin and monitor CPU
# In JUCE Host:
# 1. Load plugin
# 2. Import MIDI file
# 3. Play and watch CPU meter
```

---

## Visual Preview Without MIDI

### See the GUI Without Audio Setup

```bash
# Screenshots of running plugin
scrot -u plugin_screenshot.png &
./scalechord_demo

# Or record a video demonstration
ffmpeg -f x11grab -i :0 -vcodec libx264 -preset fast demo.mp4 &
./scalechord_demo
```

---

## Summary: Your Quick Test Checklist

```
☐ 1. Open terminal
☐ 2. cd /workspaces/production-works/plugin/build
☐ 3. ./scalechord_demo
☐ 4. Connect MIDI keyboard (or use software keyboard)
☐ 5. Play notes and hear intelligent voicing!

OR

☐ 1. Create test folder: mkdir ~/vst3_test
☐ 2. Copy plugin: cp -r vst3_build/.../ScaleChord.vst3 ~/vst3_test/
☐ 3. Open your favorite DAW
☐ 4. Add custom VST3 path: ~/vst3_test
☐ 5. Rescan and load ScaleChord
☐ 6. Add to MIDI track and play!
```

---

## Related Files

- 📘 [Full Build Guide](/workspaces/production-works/plugin/BUILDING_VST3.md)
- 🎵 [Plugin Features](/workspaces/production-works/plugin/FEATURES_ADVANCED.md)
- 🛠️ [Development Setup](/workspaces/production-works/plugin/DEVELOPMENT.md)
- 🎨 [UI Mockup](/workspaces/production-works/PLUGIN_UI_MOCKUP.md)

---

**Ready to preview? Pick your favorite method above and get started! 🚀**
