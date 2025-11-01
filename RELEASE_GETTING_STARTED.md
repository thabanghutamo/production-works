# Getting Started with ScaleChord v1.0.0

**Welcome to ScaleChord!** A revolutionary MIDI processor that intelligently harmonizes your playing using AI-powered chord mapping and real-time scale detection.

---

## 🎯 System Requirements

### Minimum Requirements
- **CPU**: 2+ cores @ 2GHz minimum
- **RAM**: 4GB minimum (8GB recommended)
- **Storage**: 500MB for installation
- **OS Support**: macOS 10.8+, Windows 7+, Linux (kernel 3.10+)

### Supported DAWs
| DAW | Version | Platform | Status |
|-----|---------|----------|--------|
| Ableton Live | 11+ | macOS, Windows | ✅ Verified |
| Logic Pro | 10.5+ | macOS | ✅ Verified |
| Reaper | 6.0+ | macOS, Windows, Linux | ✅ Verified |
| Studio One | 5.0+ | macOS, Windows | ✅ Likely Compatible |
| Cubase | 11+ | macOS, Windows | ✅ Likely Compatible |

---

## 📥 Installation

### macOS Installation

#### Step 1: Download
Download ScaleChord-1.0.0-macOS-Universal.zip from the releases page

#### Step 2: Extract
```bash
unzip ScaleChord-1.0.0-macOS-Universal.zip
```

#### Step 3: Install AU/VST3
**For AU (Audio Unit)**:
```bash
cp -r ScaleChord.component ~/Library/Audio/Plug-Ins/Components/
```

**For VST3**:
```bash
cp -r ScaleChord.vst3 ~/Library/Audio/Plug-Ins/VST3/
```

#### Step 4: Code Signing Verification
If prompted about unsigned code, temporarily allow in Security & Privacy:
- Open System Preferences → Security & Privacy
- Click "Allow anyway" if prompted
- Close and reopen your DAW

#### Step 5: Rescan Plugins
In your DAW:
- **Ableton**: Preferences → File Folder → Rescan Plugins
- **Logic**: Preferences → Plugins → Rescan
- **Reaper**: Options → Show REAPER resource path in explorer/finder → Rescan

---

### Windows Installation

#### Step 1: Download
Download ScaleChord-1.0.0-Windows-x64.zip

#### Step 2: Extract
Right-click → Extract All to your Downloads folder

#### Step 3: Install VST3
```
C:\Program Files\Common Files\VST3\
```
Copy the ScaleChord.vst3 folder to this location

#### Step 4: Rescan in DAW
- **Ableton**: Preferences → File Folder → Rescan Plugins
- **Reaper**: Options → Rescan → VST plugins

#### Step 5: Security Warning (If Prompted)
Windows may prompt about unverified software. Click "Run anyway" to proceed.

---

### Linux Installation

#### Step 1: Download & Extract
```bash
unzip ScaleChord-1.0.0-Linux-x64.zip
cd ScaleChord-1.0.0-Linux-x64
```

#### Step 2: Install VST3
```bash
mkdir -p ~/.vst3
cp -r ScaleChord.vst3 ~/.vst3/
```

#### Step 3: Verify Permissions
```bash
chmod +x ~/.vst3/ScaleChord.vst3/Contents/x86_64-linux/ScaleChord.so
```

#### Step 4: Rescan in DAW
For Reaper:
```bash
reaper --rescan-vst
```

---

## ✅ Verify Installation

### Quick Test Steps
1. **Create MIDI Track**: New MIDI track in your DAW
2. **Add ScaleChord**: Load ScaleChord as an instrument
3. **Play a Note**: Play middle C (C4)
4. **See Harmonization**: You should see harmony notes play automatically
5. **Select Scale**: Click scale selector to change from default C Major

### Troubleshooting Installation

**Plugin doesn't appear after rescan**:
- Verify installation path (check DAW documentation for plugin folders)
- Restart your DAW completely
- Try reinstalling in the other format (AU vs VST3)

**"Code is not signed" error (macOS)**:
- This is normal for pre-release software
- Go to System Preferences → Security & Privacy
- Click "Open Anyway" next to ScaleChord error

**"Unsigned driver" error (Windows)**:
- Click "Run anyway" to continue
- No security risk - this is expected for initial release

**Very high CPU usage on first load**:
- This is normal as UI renders for first time
- Should stabilize to < 5% after 1-2 seconds

---

## 🎹 Your First 5 Minutes

### Minute 1: Explore Scales
- Open ScaleChord on a MIDI track
- Click the **Scale Selector** dropdown
- Try different scales: Major, Minor, Blues, Pentatonic, etc.
- Play a note - hear the scale mode applied

### Minute 2: Change Root Note
- Click **Root Note** selector (currently C)
- Try different roots: F, G, Bb, etc.
- Play notes - the scale transposes automatically

### Minute 3: Experiment with Voicing
- Select **Voicing Style** dropdown
- Try "Compact", "Spread", "Jazz"
- Notice how the same notes harmonize differently

### Minute 4: Monitor Performance
- Look at **Performance Dashboard** (right panel)
- Green = good performance (< 5% CPU)
- Yellow = moderate (5-25% CPU)
- Red = high CPU usage (> 25%)
- Typically stays in green zone

### Minute 5: Save Your Setup
- Create a preset by clicking **Preset Browser**
- Click **Save New Preset**
- Name it something memorable
- Now you can reload this setup instantly

---

## 🎼 Core Concepts

### 48 Professional Scales
ScaleChord includes:
- 7 Major/Minor modes
- 12 Blues variations
- 8 Pentatonic flavors
- 10 Exotic scales (Arabic, Japanese, Hindu, etc.)
- 11 Advanced harmonic scales

### Smart Harmonization
Every note you play automatically:
1. **Detects scale context** (real-time detection)
2. **Selects optimal voicing** (spreads notes naturally)
3. **Applies voice leading** (smooth note transitions)
4. **Adds harmonic effects** (optional reverb/delay)

### Real-Time Monitoring
Performance Dashboard shows:
- **CPU Load**: Current processor usage
- **Voice Count**: Active simultaneous voices
- **Tension Score**: Harmonic complexity
- **Memory**: Plugin RAM usage

---

## 🎛️ Core Controls Explained

| Control | Purpose | Range | Notes |
|---------|---------|-------|-------|
| Scale Selector | Choose musical scale | 48 options | C Major is default |
| Root Note | Transpose scale | C-B (12 notes) | Changes all voicings |
| Voicing Style | Harmony spread | 3 modes | "Spread" = most voices |
| Sustain Time | Note hold duration | 0-4000ms | 500ms typical |
| Effects Chain | Add sound effects | 6 available | Optional enhancement |
| Performance Level | Dashboard display | Auto-scaling | Read-only monitor |

---

## 🚀 Next Steps

### To Learn More
- Read `RELEASE_PARAMETER_REFERENCE.md` for detailed control descriptions
- Read `RELEASE_ADVANCED_FEATURES.md` for power user techniques
- Check online tutorials at [link to be added]

### Common Workflows

**Jazz Musicians**:
1. Select a jazz scale (Dorian, Mixolydian, etc.)
2. Enable Jazz Reharmonizer effect
3. Adjust tension slider for reharmonization intensity
4. Create preset and use across your session

**Electronic Music**:
1. Use Minor Pentatonic or Blues scale
2. Increase sustain time to 2-3 seconds
3. Add reverb/delay effects
4. Create progression presets for your track

**Film Scoring**:
1. Use exotic scales (Arab, Hindu, etc.)
2. Adjust voicing for rich harmonic texture
3. Monitor CPU to ensure smooth playback
4. Export your MIDI with harmonies applied

---

## 📧 Support & Feedback

- **Issues/Bugs**: Report on GitHub Issues page
- **Feature Requests**: GitHub Discussions
- **Performance Issues**: Check Performance Dashboard first
- **Installation Help**: See Troubleshooting section above

---

## 🎉 Enjoy ScaleChord!

You now have one of the most powerful harmonic MIDI processors available. The real power comes from experimentation - try different scales, voicings, and effects combinations to discover new musical possibilities.

**Happy composing!** 🎵
