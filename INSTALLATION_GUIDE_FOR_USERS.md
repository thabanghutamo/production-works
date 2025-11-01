# 📦 ScaleChord VST3 Plugin - Installation Guide for Users

## ✨ The Easy Way (Recommended)

Your ScaleChord application now **automatically installs the VST3 plugin** when you run it!

### No configuration needed. Just follow these simple steps:

---

## 🖥️ **Installation Steps** (All Platforms)

### **Step 1: Download the Installer**
- Download: `ScaleChord Setup 1.0.0.exe` (Windows)
- Or: `ScaleChord Setup 1.0.0.dmg` (macOS)
- Or: `scalechord-1.0.0.AppImage` (Linux)

### **Step 2: Run the Installer**

#### **Windows:**
```
Double-click: ScaleChord Setup 1.0.0.exe
Click "Next" through the installation
Click "Finish" to complete
```

#### **macOS:**
```
Double-click: ScaleChord Setup 1.0.0.dmg
Drag ScaleChord to Applications folder
Double-click ScaleChord from Applications
```

#### **Linux:**
```
Make executable:
  chmod +x scalechord-1.0.0.AppImage

Double-click to run, or:
  ./scalechord-1.0.0.AppImage
```

### **Step 3: Launch the Application**
- Windows: Click "ScaleChord" in Start Menu or Desktop
- macOS: Open Applications → ScaleChord
- Linux: Click ScaleChord in applications menu

### **Step 4: Wait for Automatic Plugin Installation**
The application will:
- ✅ Check if the VST3 plugin is installed
- ✅ If not found, automatically install it
- ✅ Show installation status
- ✅ Complete in seconds

**That's it!** The plugin is now installed. 🎉

---

## 🎵 Using the Plugin in Your DAW

### **Step 1: Open Your DAW**
Supported DAWs:
- Ableton Live
- FL Studio
- Logic Pro
- Studio One
- Cubase
- Reaper
- Any VST3-compatible DAW

### **Step 2: Rescan Plugins** (may be automatic)

#### **Ableton Live:**
```
Preferences → File Folder → Plugin Sources
Reload all plugins button (or restart)
```

#### **FL Studio:**
```
Options → Manage Plug-ins → Refresh
```

#### **Studio One:**
```
Studio One → Options → Plugins → Re-scan
```

#### **Cubase:**
```
Devices → Plugin Manager
Scan for new plugins
```

#### **Logic Pro:**
```
System Preferences → Audio Plug-in Manager
Rescan plug-ins
```

### **Step 3: Find ScaleChord**
Look for: **"ScaleChord" or "Scale Chord"** in your VST3 plugins

### **Step 4: Create a New MIDI Track**
- Add a new instrument track
- Insert ScaleChord as a plugin
- Start using it!

---

## 🔍 Where Is the Plugin Installed?

The application automatically installs the plugin to the correct location:

### **Windows:**
```
%AppData%\Local\Programs\Common Files\VST3\ScaleChord.vst3
```

### **macOS:**
```
~/Library/Audio/Plug-Ins/VST3/ScaleChord.vst3
```

### **Linux:**
```
~/.vst3/ScaleChord.vst3
```

---

## ✅ How to Verify Installation

### **From ScaleChord App:**

1. Launch ScaleChord application
2. Look for "Plugin Status" section
3. Should show: ✓ Plugin Installed
4. Installation location will be displayed

### **Manual Check:**

#### **Windows:**
```powershell
dir "%AppData%\Local\Programs\Common Files\VST3\ScaleChord.vst3"
```

#### **macOS:**
```bash
ls ~/Library/Audio/Plug-Ins/VST3/ScaleChord.vst3
```

#### **Linux:**
```bash
ls ~/.vst3/ScaleChord.vst3
```

If you see the plugin folder, it's installed! ✅

---

## 🎯 Quick Start After Installation

### **1. Open your DAW**
- Ableton, FL Studio, Logic Pro, Studio One, Cubase, Reaper, etc.

### **2. Create MIDI track**
- Add new instrument/MIDI track

### **3. Insert ScaleChord**
- Click "+" to add plugin
- Search for "ScaleChord"
- Click to insert

### **4. Play MIDI**
- Play any MIDI notes
- ScaleChord automatically:
  - Analyzes your input
  - Generates intelligent voicings
  - Applies voice leading
  - Outputs full chords

### **5. Explore Parameters**
In the ScaleChord window:
- **Scale**: Choose from 13+ scales
- **Voice Leading**: Select algorithm
- **Parameters**: Fine-tune the effect

---

## 🆘 Troubleshooting

### **"Plugin not found in my DAW"**

**Solution 1: Rescan Plugins**
- Open your DAW settings
- Find "Rescan" or "Refresh Plugins"
- Restart DAW
- Look for "ScaleChord" again

**Solution 2: Check Installation**
1. Launch ScaleChord app
2. Check "Plugin Status" section
3. If not installed, it will auto-install
4. If failed, see "Installation Failed" section

**Solution 3: Manual Installation**
- Right-click ScaleChord app
- Choose "Run as Administrator" (Windows)
- App will attempt installation again

### **"Installation Failed"**

**Windows:**
- Run as Administrator
- Check disk space (need ~200 MB)
- Disable antivirus temporarily

**macOS:**
- May need to grant permissions
- Check folder permissions: `/Library/Audio/Plug-Ins/VST3`

**Linux:**
- Check folder exists: `~/.vst3`
- Grant write permissions: `chmod -R u+w ~/.vst3`

### **"Plugin crashes my DAW"**

1. Restart DAW
2. Rescan plugins (usually regenerates cache)
3. Try reinstalling by:
   - Uninstall plugin folder
   - Launch ScaleChord app again (auto-reinstalls)
   - Rescan in DAW

### **"Plugin found but not working"**

- Reload plugin in DAW
- Check that input is MIDI (not audio)
- Verify you have MIDI keyboard/input
- Check DAW is sending MIDI to the track

---

## 📖 Feature Overview

### **What ScaleChord Does:**

✨ **Intelligent Voicing**
- Analyzes input MIDI
- Generates smart chords
- Respects scale context

✨ **Voice Leading**
- Smooth note transitions
- Minimal interval jumps
- Professional orchestration

✨ **Multiple Scales**
- Major, Minor, Modes
- Blues, Pentatonic
- Jazz, Chromatic
- And more!

✨ **Full Automation**
- All parameters automatable
- Works with DAW automation
- Dynamic processing

✨ **Real-time**
- Low latency processing
- No noticeable delay
- Professional audio quality

---

## 🎮 Basic Usage

### **1. Play a Single Note**
- Send any MIDI note
- ScaleChord generates full voicing

### **2. Choose Your Scale**
- Select from 13+ scales
- Voicing respects scale notes

### **3. Adjust Voice Leading**
- 4 different algorithms
- Smooth vs. full revoicing

### **4. Tweak Parameters**
- Octave offset
- Voicing density
- Inversion type

---

## 💾 Uninstalling ScaleChord

### **Windows:**
1. Control Panel → Programs → Programs and Features
2. Find "ScaleChord Plugin"
3. Click "Uninstall"

### **macOS:**
1. Applications folder
2. Find "ScaleChord"
3. Drag to Trash

### **Linux:**
1. Applications menu
2. Right-click ScaleChord
3. Remove/Uninstall

**Note:** VST3 plugin will remain installed. To remove:

**Windows:**
```powershell
rmdir /s "%AppData%\Local\Programs\Common Files\VST3\ScaleChord.vst3"
```

**macOS:**
```bash
rm -rf ~/Library/Audio/Plug-Ins/VST3/ScaleChord.vst3
```

**Linux:**
```bash
rm -rf ~/.vst3/ScaleChord.vst3
```

---

## 📞 Support & Help

- **Documentation**: See QUICK_START.md
- **Issues**: Check troubleshooting section above
- **Feature requests**: Contact support@scalechord.example.com
- **Bug reports**: Report to issues@scalechord.example.com

---

## 🎊 You're All Set!

Everything is automatic. Just:
1. Download the installer
2. Run it
3. Launch ScaleChord
4. Plugin auto-installs
5. Use in your DAW!

**No technical knowledge needed.** 🚀

---

## ⚡ System Requirements

- **OS:** Windows 7+, macOS 10.13+, Linux (Ubuntu 18.04+)
- **RAM:** 512 MB minimum (2 GB recommended)
- **Storage:** 200 MB for installation
- **DAW:** Any VST3-compatible DAW
- **CPU:** Any modern processor

---

## 🎯 Next Steps

1. ✅ Download installer
2. ✅ Run installer
3. ✅ Launch ScaleChord app
4. ✅ Check plugin installed (automatic)
5. ✅ Open your DAW
6. ✅ Rescan plugins
7. ✅ Insert ScaleChord plugin
8. ✅ Start creating! 🎵

---

**Questions?** Everything should work automatically. If you have issues, refer to the Troubleshooting section above or contact support.

**Happy music making!** 🎉
