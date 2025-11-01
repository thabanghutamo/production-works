# 🎯 Get Your Windows EXE - Quick Guide

## 📌 Current Situation

You're on a **Linux system** (Ubuntu). Building Windows EXE on Linux requires Wine, which can be complicated.

**Easiest solution:** Build on a Windows machine (takes 5 minutes).

---

## ⚡ FASTEST WAY TO GET EXE (5 minutes on Windows)

### What You Need:
- Windows 10/11 computer
- Node.js 16+ installed (https://nodejs.org)
- PowerShell or Command Prompt

### Steps:

1. **Copy the project to Windows**
   - Download/copy this folder to your Windows machine
   - Or: `git clone https://github.com/your-repo/production-works.git`

2. **Open PowerShell in the `electron` folder**
   ```powershell
   cd C:\path\to\production-works\electron
   ```

3. **Run the builder script:**
   ```powershell
   .\build_windows.bat
   ```

4. **Wait ~3-5 minutes for the build to complete**

5. **Your EXE files are ready in `dist/` folder:**
   ```
   dist/ScaleChord Setup 1.0.0.exe          ← Installer
   dist/ScaleChord 1.0.0 portable.exe       ← Portable
   ```

**That's it! Upload to GitHub and share with users! 🚀**

---

## 🐳 Alternative: Use Docker (Advanced)

If you don't have Windows but have Docker:

```bash
# On Linux with Docker installed:
cd /workspaces/production-works

docker run --rm -ti \
  -v $(pwd)/electron:/project \
  electronuserland/electron-builder:wine \
  bash -c "cd /project && npm install --legacy-peer-deps && npm run build:win"

# Wait ~10 minutes...
# EXE appears in: electron/dist/
```

**Pros:** Works on Linux  
**Cons:** Slower, requires Docker

---

## 📦 What You're Building

### **ScaleChord Setup 1.0.0.exe** (Installer)
- Full installer for Windows
- Install to Program Files
- Adds Start Menu shortcuts
- Automatically installs VST3 plugin
- Remove via Control Panel / Add/Remove Programs
- **Size:** ~120 MB

### **ScaleChord 1.0.0 portable.exe** (Portable)
- No installation needed
- Can run from USB drive
- Can run from any folder
- Plugin installs on first run
- **Size:** ~100 MB

---

## 🔧 Files We've Created for You

In the `electron/` folder:

- **`build_windows.bat`** ← Use this on Windows! 
  - Double-click to build
  - Checks Node.js
  - Installs dependencies
  - Builds EXE
  - Shows results

- **`run_scalechord.bat`** ← Launcher script
  - Runs the app from built files
  - For testing before packaging

- **`package.json`** ← Already fixed
  - Ready to build

---

## ✅ Pre-build Checklist

Before building, ensure:

```cmd
REM On your Windows machine:

REM 1. Node.js installed?
node --version

REM 2. In the right directory?
cd electron
dir package.json

REM 3. VST3 plugin is present?
dir vst3_plugin
```

If all checks pass ✓, you're ready to build!

---

## 🎯 Quick Reference

| Need | How | Time |
|------|-----|------|
| **Installer EXE** | Run `build_windows.bat` on Windows | 5 min |
| **Portable EXE** | Same script, both created | 5 min |
| **From Linux (Docker)** | Use Docker command above | 10 min |
| **Manual build** | `npm install && npm run build:win` | 5 min |

---

## 📤 After You Get the EXE

1. **Test it**
   ```
   Double-click: ScaleChord Setup 1.0.0.exe
   ```

2. **Upload to GitHub Releases**
   ```
   https://github.com/your-repo/releases
   ```

3. **Share with users**
   ```
   Download: https://github.com/your-repo/releases/tag/v1.0.0
   ```

---

## 🆘 Troubleshooting

### "Node.js not found"
- Install from: https://nodejs.org
- Restart PowerShell after install
- Verify: `node --version`

### "npm: command not found"
- Same as above, or reinstall Node.js
- Choose "Add to PATH" during installation

### "Build fails with permission error"
- Run PowerShell as Administrator
- Then run the script again

### "Wine is required" error (on Linux)
- Use Docker method above instead
- Or build on Windows machine

### Build takes very long
- First build downloads Electron (~150 MB)
- Subsequent builds are faster
- Be patient, it's normal

---

## 💡 Pro Tips

**Speed up rebuilds:**
- npm cache is saved between builds
- Second build is ~2-3x faster
- Only rebuild when you change code

**Test portable version:**
```cmd
dist\ScaleChord 1.0.0 portable.exe
```

**Test installer:**
```cmd
dist\ScaleChord Setup 1.0.0.exe
```

**Create checksums (for security):**
```cmd
certutil -hashfile "ScaleChord Setup 1.0.0.exe" SHA256
```

---

## 🎊 Success Indicators

After running `build_windows.bat`, you should see:

✅ "Node.js found"  
✅ "npm found"  
✅ "vst3_plugin directory found"  
✅ "Dependencies installed"  
✅ "Build completed successfully! 🎉"  
✅ Files listed: `dist\ScaleChord Setup 1.0.0.exe`

If you see all these, you're done! 🚀

---

## 📞 Need Help?

1. Check `BUILD_WINDOWS_EXE.md` for detailed instructions
2. Check `SETUP_AND_BUILD.md` for development setup
3. Check `ELECTRON_APP_GUIDE.md` for app architecture

---

## ⏱️ Timeline

**With Windows machine:**
- Setup: 1-2 minutes (install Node.js if needed)
- Build: 5 minutes
- **Total: ~5 minutes** ⚡

**With Docker on Linux:**
- Pull image: 5 minutes (first time only)
- Build: 10 minutes
- **Total: ~10 minutes**

**Manual Windows machine:**
- Setup: Same as above
- Run: `npm install && npm run build:win`
- **Total: ~5 minutes**

---

## 🎯 Next Steps

1. **Recommended:** Go to a Windows machine
2. **Copy** the `electron/` folder
3. **Run** `build_windows.bat`
4. **Get** your EXE files!

**Questions?** Everything is documented. You got this! 💪
