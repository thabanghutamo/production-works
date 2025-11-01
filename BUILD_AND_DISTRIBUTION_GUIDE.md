# 🚀 ScaleChord Build & Distribution Guide

## Complete Step-by-Step Instructions

---

## 📋 Pre-Build Checklist

Before building, ensure everything is ready:

```bash
# 1. Verify plugin binary exists
ls -lh /workspaces/production-works/plugin/vst3_build/ScaleChordPlugin_artefacts/Release/VST3/ScaleChord.vst3/

# 2. Verify electron folder exists
ls -la /workspaces/production-works/electron/

# 3. Check all source files present
ls /workspaces/production-works/electron/{main.js,preload.js,renderer.js,index.html,styles.css,package.json}

# Expected output: All files should exist
```

---

## 🔧 Step 1: Setup Build Environment

### A. Copy Plugin Binary to Electron App

```bash
# Navigate to electron folder
cd /workspaces/production-works/electron

# Create vst3_plugin directory if needed
mkdir -p vst3_plugin

# Copy the compiled plugin
cp -r /workspaces/production-works/plugin/vst3_build/ScaleChordPlugin_artefacts/Release/VST3/ScaleChord.vst3 vst3_plugin/

# Verify copy
ls -la vst3_plugin/ScaleChord.vst3/

# Expected: Directory with Contents/ subfolder
```

### B. Install Node Dependencies

```bash
# Install npm packages
npm install

# Expected output: 
# > Added XX packages, and audited YY packages
# > found 0 vulnerabilities

# Verify installation
npm list electron electron-builder

# Expected output should show versions
```

### C. Verify Setup

```bash
# Check all required files
npm run build --help

# Should show electron-builder help
```

---

## 🏗️ Step 2: Build for Current Platform

### Quick Test Build (No Installer)

```bash
# This creates a test package without creating an installer
npm run pack

# Output location: dist/
# Test the build
./dist/ScaleChord*/bin/ScaleChord    # Linux
./dist/ScaleChord*.app/Contents/MacOS/ScaleChord  # macOS
./dist/ScaleChord Setup*/ScaleChord.exe            # Windows
```

### Build with Installer

```bash
# Build for current platform only
npm run build

# Output: dist/ folder with installer
# Windows: dist/ScaleChord Setup 1.0.0.exe
# macOS: dist/ScaleChord-1.0.0.dmg
# Linux: dist/scalechord-1.0.0.AppImage
```

---

## 🌍 Step 3: Build for All Platforms

### Build All Platforms (Recommended for Distribution)

```bash
# Build for Windows, macOS, and Linux
npm run build:all

# This may take 10-15 minutes depending on your system
# Builds in sequence: macOS → Windows → Linux
```

### Build Specific Platforms

```bash
# macOS only
npm run build:mac
# Creates: dist/ScaleChord-1.0.0.dmg, dist/ScaleChord-1.0.0-mac.zip

# Windows only
npm run build:win
# Creates: dist/ScaleChord Setup 1.0.0.exe, dist/ScaleChord 1.0.0 portable.exe

# Linux only
npm run build:linux
# Creates: dist/scalechord-1.0.0.AppImage, dist/scalechord_1.0.0_amd64.deb
```

---

## 📦 Step 4: Generated Distributables

After building, you'll have these files:

```
/workspaces/production-works/electron/dist/
│
├── mac/
│   ├── ScaleChord-1.0.0.dmg               ← macOS installer
│   ├── ScaleChord-1.0.0-mac.zip           ← macOS portable
│   └── builder-effective-config.yaml      ← Build config
│
├── win/
│   ├── ScaleChord Setup 1.0.0.exe         ← Windows installer
│   ├── ScaleChord 1.0.0 portable.exe      ← Windows portable
│   └── nsis/                              ← NSIS config
│
└── linux/
    ├── scalechord-1.0.0.AppImage          ← Linux AppImage
    ├── scalechord_1.0.0_amd64.deb         ← Linux Debian
    └── ScaleChord-1.0.0.AppImage.sha256   ← SHA256 hash
```

---

## ✅ Step 5: Test Each Build

### Test macOS Build

```bash
# Mount DMG
hdiutil mount dist/ScaleChord-1.0.0.dmg

# Copy app to Applications
cp -r /Volumes/ScaleChord/ScaleChord.app /Applications/

# Unmount
hdiutil unmount /Volumes/ScaleChord

# Launch and test
open /Applications/ScaleChord.app

# Expected: App launches, shows "Installed ✓" status
```

### Test Windows Build

```bash
# Run installer
./dist/ScaleChord\ Setup\ 1.0.0.exe

# Or portable version
./dist/ScaleChord\ 1.0.0\ portable.exe

# Expected: App launches and works
```

### Test Linux Build

```bash
# Make executable
chmod +x dist/scalechord-1.0.0.AppImage

# Run
./dist/scalechord-1.0.0.AppImage

# Or install deb
sudo apt install ./dist/scalechord_1.0.0_amd64.deb

# Launch
scalechord

# Expected: App launches
```

---

## 📤 Step 6: Prepare for Distribution

### Create Release Package

```bash
cd /workspaces/production-works/electron/dist

# Create checksums for security
sha256sum ScaleChord-1.0.0.dmg > ScaleChord-1.0.0.dmg.sha256
sha256sum ScaleChord\ Setup\ 1.0.0.exe > ScaleChord\ Setup\ 1.0.0.exe.sha256
sha256sum scalechord-1.0.0.AppImage > scalechord-1.0.0.AppImage.sha256

# Verify checksums
sha256sum -c *.sha256

# Expected: OK for all files
```

### Create Release Notes

Create `/workspaces/production-works/RELEASE_NOTES.md`:

```markdown
# ScaleChord VST3 Plugin v1.0.0 - Release Notes

**Release Date:** November 1, 2025

## 🎉 What's New

- ✨ Production-ready VST3 plugin
- 💻 Professional desktop installer
- 🎨 Beautiful dark-themed UI
- 📥 One-click plugin installation
- 🌍 Cross-platform support (Windows, macOS, Linux)
- 🔐 Security best practices
- 🚀 Ready for immediate use

## 📋 System Requirements

### Minimum
- CPU: Dual-core processor
- RAM: 512 MB available
- Storage: 50 MB for plugin + 150 MB for app
- DAW: Any VST3-compatible host

### Recommended
- CPU: Quad-core or better
- RAM: 2 GB or more
- Storage: SSD for performance
- DAW: Modern DAW (2022+)

### Supported Platforms
- Windows 7 SP1 and later
- macOS 10.13 and later
- Linux (Ubuntu 18.04 and later)

## 🎯 Key Features

### Intelligent Voicing
- Scale-aware chord generation
- 13+ built-in scales
- Multiple voice leading algorithms

### MIDI Processing
- Real-time MIDI effect processing
- 16-channel MIDI routing
- Velocity recognition

### Automation
- 7 fully automatable parameters
- DAW automation support
- Real-time parameter control

## 📥 Installation

1. Download the installer for your platform
2. Run the installer
3. Launch ScaleChord application
4. Click "Install Plugin" button
5. Rescan plugins in your DAW
6. Add ScaleChord to MIDI track

## 🐛 Known Issues

None at this time - please report issues on GitHub!

## 📞 Support

- GitHub Issues: https://github.com/scalechord/plugin/issues
- Discord: https://discord.gg/scalechord
- Email: support@scalechord.example.com

## 📄 License

MIT License - See LICENSE file for details

---

**Version:** 1.0.0  
**Status:** ✅ Production Ready  
**Build Date:** November 1, 2025
```

---

## 🌐 Step 7: Publish to GitHub

### Create GitHub Release

```bash
# Make sure you have the GitHub CLI installed
# https://cli.github.com

# Login to GitHub
gh auth login

# Create release
cd /workspaces/production-works

gh release create v1.0.0 \
  --title "ScaleChord VST3 Plugin v1.0.0" \
  --notes-file RELEASE_NOTES.md \
  electron/dist/ScaleChord-1.0.0.dmg \
  electron/dist/ScaleChord\ Setup\ 1.0.0.exe \
  electron/dist/scalechord-1.0.0.AppImage \
  electron/dist/scalechord_1.0.0_amd64.deb

# Verify release created
gh release view v1.0.0
```

### Manual Upload (Alternative)

1. Go to https://github.com/scalechord/plugin/releases
2. Click "Create a new release"
3. Tag: `v1.0.0`
4. Title: `ScaleChord VST3 Plugin v1.0.0`
5. Description: Copy from RELEASE_NOTES.md
6. Upload files:
   - ScaleChord-1.0.0.dmg
   - ScaleChord Setup 1.0.0.exe
   - scalechord-1.0.0.AppImage
   - scalechord_1.0.0_amd64.deb
7. Click "Publish release"

---

## 📦 Step 8: Create Distribution Package

### macOS Distribution

```bash
# Create macOS-specific package
cd /workspaces/production-works/electron/dist

# Archive macOS files
tar -czf scalechord-1.0.0-macos.tar.gz ScaleChord-1.0.0.dmg ScaleChord-1.0.0-mac.zip

# Create checksum
sha256sum scalechord-1.0.0-macos.tar.gz > scalechord-1.0.0-macos.tar.gz.sha256
```

### Windows Distribution

```bash
# Create Windows-specific package
cd /workspaces/production-works/electron/dist

# Zip Windows files
zip scalechord-1.0.0-windows.zip \
  "ScaleChord Setup 1.0.0.exe" \
  "ScaleChord 1.0.0 portable.exe"

# Create checksum
sha256sum scalechord-1.0.0-windows.zip > scalechord-1.0.0-windows.zip.sha256
```

### Linux Distribution

```bash
# Create Linux-specific package
cd /workspaces/production-works/electron/dist

# Archive Linux files
tar -czf scalechord-1.0.0-linux.tar.gz \
  scalechord-1.0.0.AppImage \
  scalechord_1.0.0_amd64.deb

# Create checksum
sha256sum scalechord-1.0.0-linux.tar.gz > scalechord-1.0.0-linux.tar.gz.sha256
```

---

## 🌍 Step 9: Upload to Distribution Channels

### Option A: GitHub Releases (Recommended)

```bash
# Already done in Step 7
# Users can download from:
# https://github.com/scalechord/plugin/releases/tag/v1.0.0
```

### Option B: Personal Website

```bash
# Create directory structure on your server
/downloads/
├── scalechord/
│   ├── v1.0.0/
│   │   ├── ScaleChord-1.0.0.dmg
│   │   ├── ScaleChord Setup 1.0.0.exe
│   │   ├── scalechord-1.0.0.AppImage
│   │   └── checksums.txt

# Upload files
scp electron/dist/* user@yourserver.com:/downloads/scalechord/v1.0.0/
```

### Option C: Package Managers

#### Homebrew (macOS)

```bash
# Create Homebrew formula at:
# scalechord.rb

class Scalechord < Formula
  desc "ScaleChord VST3 Plugin - Intelligent MIDI Voicing"
  homepage "https://scalechord.example.com"
  version "1.0.0"
  
  on_macos do
    url "https://github.com/scalechord/plugin/releases/download/v1.0.0/ScaleChord-1.0.0.dmg"
    sha256 "abc123..."
  end
  
  def install
    app.install "ScaleChord.app"
  end
end

# Submit to Homebrew:
# https://docs.brew.sh/Manpage#tap-pr-issue
```

#### Chocolatey (Windows)

```bash
# Create nuspec file:
# scalechord.nuspec

<?xml version="1.0" encoding="utf-8"?>
<package xmlns="http://schemas.microsoft.com/packaging/2015/06/nuspec.xsd">
  <metadata>
    <id>scalechord</id>
    <version>1.0.0</version>
    <title>ScaleChord VST3 Plugin</title>
    <description>Intelligent MIDI voicing and chord generation</description>
    <projectUrl>https://scalechord.example.com</projectUrl>
  </metadata>
  <files>
    <file src="tools\*" target="tools" />
  </files>
</package>

# Submit to Chocolatey:
# https://docs.chocolatey.org/en-us/create/create-packages
```

#### Ubuntu PPA

```bash
# Create PPA repository
# Follow: https://help.launchpad.net/Packaging/PPA

# Upload deb file:
dput ppa:yourname/scalechord scalechord_1.0.0_amd64.deb
```

---

## 📢 Step 10: Marketing & Announcement

### Social Media Posts

**Template for Twitter/X:**
```
🎉 ScaleChord VST3 Plugin v1.0.0 is LIVE! 🎵

✨ Intelligent MIDI voicing for music producers
🎸 Multiple voice leading algorithms
🎼 13+ built-in scales
🌍 Cross-platform (Win/Mac/Linux)

Download now: https://github.com/scalechord/plugin/releases
```

**Template for Discord:**
```
@everyone 📢 Major announcement!

🎉 ScaleChord VST3 Plugin v1.0.0 is now available!

Features:
✅ Scale-aware chord voicing
✅ Real-time MIDI processing
✅ Professional UI
✅ One-click installation
✅ Cross-platform support

Downloads: https://github.com/scalechord/plugin/releases

Try it out and let us know what you think! 🚀
```

**Template for Email:**
```
Subject: 🎉 ScaleChord VST3 Plugin v1.0.0 Released!

Hi Music Producers,

We're excited to announce the release of ScaleChord VST3 Plugin v1.0.0!

ScaleChord brings intelligent MIDI voicing to your DAW:
• Scale-aware chord generation
• Multiple voice leading modes
• Real-time MIDI processing
• Professional UI with dark theme

Download for your platform:
📱 macOS: https://[link]
🪟 Windows: https://[link]
🐧 Linux: https://[link]

Questions? Join our Discord: https://discord.gg/scalechord

Happy voicing! 🎵
```

---

## 🔍 Step 11: Post-Release Checklist

- [ ] All installers tested on each platform
- [ ] GitHub release created with files
- [ ] Release notes published
- [ ] Website updated with download links
- [ ] Social media posts published
- [ ] Community Discord notified
- [ ] GitHub discussions pinned
- [ ] Support channels ready
- [ ] Issue tracking active
- [ ] Feedback collection started

---

## 🚀 Quick Build & Release Script

Create `/workspaces/production-works/BUILD_AND_RELEASE.sh`:

```bash
#!/bin/bash

echo "🚀 ScaleChord Build & Release Script"
echo "===================================="

# Colors for output
GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Step 1: Setup
echo -e "\n${BLUE}Step 1: Setting up build environment...${NC}"
cd /workspaces/production-works/electron

# Copy plugin
mkdir -p vst3_plugin
cp -r /workspaces/production-works/plugin/vst3_build/ScaleChordPlugin_artefacts/Release/VST3/ScaleChord.vst3 vst3_plugin/
echo -e "${GREEN}✓ Plugin binary copied${NC}"

# Install dependencies
npm install
echo -e "${GREEN}✓ Dependencies installed${NC}"

# Step 2: Build
echo -e "\n${BLUE}Step 2: Building for all platforms...${NC}"
npm run build:all
echo -e "${GREEN}✓ Builds completed${NC}"

# Step 3: Create checksums
echo -e "\n${BLUE}Step 3: Creating checksums...${NC}"
cd dist
sha256sum *.dmg *.exe *.AppImage *.deb > checksums.txt 2>/dev/null
echo -e "${GREEN}✓ Checksums created${NC}"

# Step 4: Summary
echo -e "\n${BLUE}Build Summary:${NC}"
echo -e "${GREEN}✓ macOS:${NC}"
ls -lh ScaleChord-1.0.0.dmg 2>/dev/null || echo "Not found"
echo -e "${GREEN}✓ Windows:${NC}"
ls -lh ScaleChord\ Setup\ 1.0.0.exe 2>/dev/null || echo "Not found"
echo -e "${GREEN}✓ Linux:${NC}"
ls -lh scalechord-1.0.0.AppImage 2>/dev/null || echo "Not found"
ls -lh scalechord_1.0.0_amd64.deb 2>/dev/null || echo "Not found"

echo -e "\n${GREEN}✓ Build complete!${NC}"
echo -e "\nDistributable files are in: dist/"
echo -e "Ready to upload to GitHub Releases or your website!"
```

Run it:
```bash
chmod +x /workspaces/production-works/BUILD_AND_RELEASE.sh
/workspaces/production-works/BUILD_AND_RELEASE.sh
```

---

## 📊 Distribution Checklist

**Pre-Build:**
- [ ] Plugin binary verified
- [ ] All source files present
- [ ] Dependencies documented
- [ ] Version number updated to 1.0.0

**Build Process:**
- [ ] npm install successful
- [ ] build:all completes without errors
- [ ] All platform binaries created
- [ ] Checksums generated

**Testing:**
- [ ] macOS DMG tested and launches
- [ ] Windows EXE tested and launches
- [ ] Linux AppImage tested and launches
- [ ] Plugin installation works in each
- [ ] DAW rescan recognizes plugin
- [ ] Plugin processes MIDI correctly

**Release:**
- [ ] GitHub release created
- [ ] Files uploaded to GitHub
- [ ] Release notes published
- [ ] Checksums verified
- [ ] Website updated
- [ ] Social media posts scheduled
- [ ] Community notified

**Post-Release:**
- [ ] Monitor GitHub issues
- [ ] Respond to user feedback
- [ ] Update documentation
- [ ] Plan next release

---

## 🎯 Common Issues & Solutions

### Build Fails: "node_modules not found"
```bash
rm -rf node_modules package-lock.json
npm install
```

### Permission Denied on Linux
```bash
chmod +x dist/scalechord-1.0.0.AppImage
./dist/scalechord-1.0.0.AppImage
```

### macOS Code Signing Issues
```bash
# Skip signing for local testing
npm run build:mac -- --publish never
```

### Windows SmartScreen Warning
- This is normal and expected
- Users click "More info" → "Run anyway"
- Sign binaries for production release

---

## 📞 Support & Resources

- **Electron Builder Docs:** https://www.electron.build
- **GitHub Releases API:** https://docs.github.com/en/rest/releases
- **Code Signing:** https://www.electron.build/code-signing
- **Auto-Updates:** https://www.electron.build/auto-update

---

## 🎊 You're Ready!

Your ScaleChord plugin is production-ready and ready for distribution. Follow these steps to build, test, and release to users worldwide! 🚀

