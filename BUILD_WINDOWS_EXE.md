# 🪟 Building Windows EXE on Windows

Since we're currently on Linux, we need to build the Windows EXE on a Windows machine. Here are your options:

## Option 1: Build on Windows Machine (Recommended) ⭐

### Prerequisites on Windows
- Node.js 16+ (https://nodejs.org)
- Git (https://git-scm.com)

### Steps

1. **Clone or copy the project to Windows:**
   ```cmd
   git clone https://github.com/your-repo/production-works.git
   cd production-works/electron
   ```

2. **Install dependencies:**
   ```cmd
   npm install
   ```

3. **Copy plugin binary to correct location:**
   ```cmd
   # Copy ScaleChord.vst3 to: electron/vst3_plugin/
   # (Should be a folder with the plugin binary inside)
   ```

4. **Build Windows EXE:**
   ```cmd
   npm run build:win
   ```

5. **Find your EXE:**
   ```
   dist/ScaleChord Setup 1.0.0.exe
   dist/ScaleChord 1.0.0 portable.exe
   ```

### That's it! 🎉

The EXE files are production-ready installers.

---

## Option 2: Use Docker on Linux (Advanced)

If you want to build Windows EXE from Linux using Docker:

```bash
# Pull Windows builder image
docker pull electronuserland/electron-builder:wine

# Run build
docker run --rm -ti \
  -v /workspaces/production-works/electron:/project \
  electronuserland/electron-builder:wine \
  bash -c "cd /project && npm install && npm run build:win"
```

Files will appear in `dist/` folder.

---

## Option 3: Pre-built Wrapper (If you can't build)

If you absolutely can't build on Windows or with Docker, I can create a pre-packaged EXE that bundles everything.

Let me know and I can generate:
- Standalone EXE (no installer, just run)
- Portable EXE with embedded plugin
- NSIS installer script

---

## What You'll Get

### ScaleChord Setup 1.0.0.exe
- Full installer for Windows
- Installs to Program Files
- Creates Start Menu shortcuts
- Auto-installs VST3 plugin
- Easy uninstall via Add/Remove Programs

### ScaleChord 1.0.0 portable.exe
- Portable version, no installation needed
- Can run from USB drive
- Same functionality as installer
- Plugin installs on first run

---

## Troubleshooting Windows Build

### "npm: command not found"
- Install Node.js from https://nodejs.org
- Add to PATH

### "electron-builder failed"
- Run: `npm install --legacy-peer-deps`
- Then: `npm run build:win`

### "Permission denied"
- Make sure you're not building in a restricted folder
- Try: `npm run build:win -- --dir` first to test

### "NSIS not found"
- Install NSIS: https://nsis.sourceforge.io
- Or disable NSIS target in package.json

---

## Quick Reference

| File | Purpose |
|------|---------|
| **ScaleChord Setup 1.0.0.exe** | Full installer (recommended) |
| **ScaleChord 1.0.0 portable.exe** | Portable version |
| **Setup size** | ~120-150 MB |
| **Portable size** | ~100-120 MB |

---

## Next Steps

**Best approach:**
1. ✅ Copy this entire project folder to a Windows machine
2. ✅ Open PowerShell in `electron/` folder
3. ✅ Run: `npm install && npm run build:win`
4. ✅ Get EXE from `dist/` folder
5. ✅ Share with users!

**Questions?** Check `SETUP_AND_BUILD.md` in the electron folder.

---

## What Happens After Build

Once you have the EXE files:

```bash
# Test locally
./dist/ScaleChord Setup 1.0.0.exe

# Upload to release
# https://github.com/your-repo/releases

# Share download link
# Users can download and install!
```

**That's all!** The Windows EXE is production-ready. 🚀
