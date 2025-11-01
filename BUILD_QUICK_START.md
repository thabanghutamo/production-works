# 🚀 Quick Build & Distribution - START HERE

## ⚡ 5-Minute Express Build

### Option 1: Interactive Menu (Easiest)

```bash
cd /workspaces/production-works
./build_and_distribute.sh

# Then select option: 6 (Full build + distribution)
```

### Option 2: Command Line (Fastest)

```bash
cd /workspaces/production-works
./build_and_distribute.sh full
```

Both options will:
- ✅ Verify all requirements
- ✅ Setup build environment
- ✅ Build for all platforms (Win/Mac/Linux)
- ✅ Create checksums
- ✅ Generate release notes
- ✅ List all outputs
- ✅ Publish to GitHub (if configured)

---

## 📊 What Gets Built

After running the build script, you'll have:

```
dist/
├── macOS:
│   ├── ScaleChord-1.0.0.dmg             ← For Mac users
│   └── ScaleChord-1.0.0-mac.zip         ← Portable Mac version
│
├── Windows:
│   ├── ScaleChord Setup 1.0.0.exe       ← For Windows users
│   └── ScaleChord 1.0.0 portable.exe    ← Portable Windows
│
├── Linux:
│   ├── scalechord-1.0.0.AppImage        ← For Linux users
│   └── scalechord_1.0.0_amd64.deb       ← Debian package
│
└── Security:
    ├── checksums.txt                    ← SHA256 hashes
    ├── dmg.sha256, exe.sha256, etc.     ← Individual checksums
    └── RELEASE_NOTES_v1.0.0.md          ← Release notes
```

---

## 🎯 Build Options Explained

| Option | Time | Use When |
|--------|------|----------|
| **1 - Verify** | <1 min | Troubleshooting setup issues |
| **2 - Setup Only** | 2 min | Manual build later |
| **3 - Current Platform** | 5 min | Testing on your OS only |
| **4 - Specific Platform** | 5 min | Building for one platform |
| **5 - All Platforms** | 15 min | Full distribution build |
| **6 - Full Release** | 20 min | ⭐ Ready to distribute |

---

## 📦 Step-by-Step Release Process

### Step 1: Build Everything

```bash
./build_and_distribute.sh full
```

Takes ~15-20 minutes. Creates all installers.

### Step 2: Test Installers (Optional)

Test on your machine or download and test:
- macOS: Double-click `.dmg`
- Windows: Run `.exe`
- Linux: `./scalechord-1.0.0.AppImage`

### Step 3: Upload to GitHub

**Automatic (if GitHub CLI installed):**
```bash
# Already done if you selected option 6
# Release page: https://github.com/your-repo/releases/tag/v1.0.0
```

**Manual:**
1. Go to https://github.com/your-repo/releases
2. Click "Create a new release"
3. Tag: `v1.0.0`
4. Upload files from `dist/` folder
5. Copy content from `RELEASE_NOTES_v1.0.0.md`
6. Publish

### Step 4: Announce!

**Template:**
```
🎉 ScaleChord VST3 Plugin v1.0.0 is LIVE!

Features:
✨ Intelligent MIDI voicing
🎼 13+ scales
🎸 4 voice leading modes
🌍 Cross-platform (Win/Mac/Linux)

Download: https://github.com/your-repo/releases/tag/v1.0.0
```

---

## ⚙️ Command Reference

### Full Build (Recommended)
```bash
./build_and_distribute.sh full
```

### Just Verify Setup
```bash
./build_and_distribute.sh verify
```

### Setup Environment
```bash
./build_and_distribute.sh setup
```

### Build Current Platform
```bash
./build_and_distribute.sh build-current
```

### Build Specific Platform
```bash
./build_and_distribute.sh build-platform mac
./build_and_distribute.sh build-platform win
./build_and_distribute.sh build-platform linux
```

### Build All Platforms
```bash
./build_and_distribute.sh build-all
```

---

## 📂 File Locations

### After Build, Find Files Here:
```
/workspaces/production-works/electron/dist/
```

### Detailed Build Log:
```
/workspaces/production-works/build.log
```

### Release Notes:
```
/workspaces/production-works/RELEASE_NOTES_v1.0.0.md
```

---

## ✅ Pre-Build Checklist

Before you build, verify:

```bash
# 1. Plugin binary exists
ls -lh /workspaces/production-works/plugin/vst3_build/ScaleChordPlugin_artefacts/Release/VST3/ScaleChord.vst3/

# 2. Electron files exist
ls /workspaces/production-works/electron/{main.js,preload.js,renderer.js,index.html,styles.css,package.json}

# 3. Build script is executable
ls -l /workspaces/production-works/build_and_distribute.sh
```

All should exist. If any are missing, something went wrong.

---

## 🐛 Troubleshooting

### "npm: command not found"
```bash
# Need to install Node.js
# https://nodejs.org
```

### "Permission denied" on build script
```bash
chmod +x /workspaces/production-works/build_and_distribute.sh
```

### Build fails: "EACCES" or permission errors
```bash
# Try with elevated permissions
sudo ./build_and_distribute.sh full
```

### Build successful but no files in dist/
```bash
# Check if build actually ran
ls -lah /workspaces/production-works/electron/dist/

# If empty, check npm build errors
npm run build -- --help
```

---

## 📊 Build Progress Example

```
❯ ./build_and_distribute.sh full

╔════════════════════════════════════════════╗
║   ScaleChord Build & Distribution System   ║
╚════════════════════════════════════════════╝

✓ Node.js found: v18.0.0
✓ npm found: 8.0.0
✓ Plugin binary found
✓ Electron directory found

[... setup ...]

✓ macOS:  ScaleChord-1.0.0.dmg (95 MB)
✓ Windows: ScaleChord Setup 1.0.0.exe (125 MB)
✓ Linux:   scalechord-1.0.0.AppImage (150 MB)
✓ Linux:   scalechord_1.0.0_amd64.deb (85 MB)

Total build size: 455 MB

✓ BUILD COMPLETE!
✓ All files ready in: /workspaces/production-works/electron/dist
✓ Build log: /workspaces/production-works/build.log
```

---

## 🎁 What's in Each Installer

### macOS (ScaleChord-1.0.0.dmg)
- ScaleChord.app application
- Plugin bundle
- Auto-install instructions

### Windows (ScaleChord Setup 1.0.0.exe)
- ScaleChord application
- Plugin bundle
- Auto-install instructions
- Start menu shortcuts
- Add/Remove Programs entry

### Linux (scalechord-1.0.0.AppImage)
- Self-contained app bundle
- Works on any Linux distribution
- Just make executable and run

### Linux (scalechord_1.0.0_amd64.deb)
- Standard Debian package
- Install with: `sudo apt install ./file.deb`
- Adds to applications menu

---

## 📞 Next Steps

1. **Run the build:**
   ```bash
   cd /workspaces/production-works
   ./build_and_distribute.sh full
   ```

2. **Wait for completion (~20 minutes)**

3. **Check the results:**
   ```bash
   ls -lh /workspaces/production-works/electron/dist/
   ```

4. **Upload to GitHub:**
   ```bash
   # Installers are in dist/ folder
   # Follow GitHub steps above
   ```

5. **Share with users!**
   ```
   🎉 Download ScaleChord: https://github.com/your-repo/releases
   ```

---

## ⚡ TL;DR (Quick Version)

```bash
cd /workspaces/production-works
chmod +x build_and_distribute.sh
./build_and_distribute.sh full

# Wait ~20 minutes...

# Files ready in: electron/dist/
# Upload to GitHub Releases
# Done! 🚀
```

---

## 🎊 Success Indicators

After running the full build, you should see:

✅ All files in `dist/` folder  
✅ Multiple installers for different platforms  
✅ Checksums files (security verification)  
✅ Release notes file  
✅ No errors in build log  
✅ All file sizes reasonable (50-150 MB each)  

If you see all these, you're ready to distribute! 🚀

---

**Ready? Let's go! 🎯**

```bash
cd /workspaces/production-works && ./build_and_distribute.sh full
```
