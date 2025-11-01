# ScaleChord VST3 Plugin - Electron Desktop App

## Overview

The ScaleChord Electron application is a desktop wrapper for the VST3 plugin that provides:

- **Plugin Installation Manager** - One-click installation to DAWs
- **Cross-Platform Support** - Windows, macOS, and Linux
- **Beautiful UI** - Professional dark theme with intuitive controls
- **Real-Time Status** - Live plugin status and installation verification
- **Automatic Updates** - Built-in update checking and installation
- **Documentation Hub** - Quick access to guides and tutorials
- **Community Links** - Direct access to support channels

---

## Installation & Setup

### Prerequisites

- Node.js 14+ and npm
- Electron (installed via npm)
- The compiled ScaleChord.vst3 plugin binary

### Development Setup

```bash
# Clone the repository
git clone https://github.com/scalechord/plugin.git
cd plugin/electron

# Install dependencies
npm install

# Run in development mode
npm run dev

# Build for your platform
npm run build

# Build for all platforms (Windows, macOS, Linux)
npm run build:all
```

### Production Installation

**From Downloaded Installer:**

1. **macOS:**
   - Download `ScaleChord-1.0.0.dmg`
   - Double-click to mount
   - Drag ScaleChord to Applications
   - Launch from Applications folder

2. **Windows:**
   - Download `ScaleChord Setup 1.0.0.exe`
   - Run installer
   - Follow installation wizard
   - Launch from Start Menu

3. **Linux:**
   - Download `ScaleChord-1.0.0.AppImage` or `.deb`
   - AppImage: Make executable (`chmod +x`) and run
   - Deb: `sudo apt install ./ScaleChord-1.0.0.deb`

---

## File Structure

```
electron/
├── main.js                 # Main Electron process
├── preload.js             # Secure IPC bridge
├── renderer.js            # Frontend JavaScript
├── index.html             # Main UI
├── styles.css             # Styling (dark theme)
├── package.json           # Dependencies and config
├── assets/                # Icons and resources
│   ├── icon.png
│   ├── icon.icns
│   └── icon.ico
└── vst3_plugin/           # VST3 plugin binary
    └── ScaleChord.vst3
```

---

## Main Components

### 1. Main Process (main.js)

**Responsibilities:**
- Window management
- IPC request handling
- File operations
- Plugin installation
- System integration

**Key Functions:**
```javascript
createWindow()           // Create main app window
getVST3Path()           // Get platform-specific VST3 path
isPluginInstalled()     // Check installation status
copyFolderRecursive()   // Handle plugin file copying
createMenu()            // Setup application menu
```

### 2. Preload Script (preload.js)

**Purpose:** Secure bridge between renderer and main process

**Exposed APIs:**
```javascript
electronAPI.getPluginStatus()    // Get current plugin status
electronAPI.installPlugin()      // Install plugin to VST3 folder
electronAPI.getVST3Paths()       // Get system VST3 paths
electronAPI.getPluginInfo()      // Get plugin metadata
electronAPI.openFileDialog()     // File browser dialog
```

### 3. Renderer Process (renderer.js)

**Responsibilities:**
- UI interaction handling
- IPC communication
- Status updates
- Notifications
- Keyboard shortcuts

### 4. User Interface (index.html + styles.css)

**Sections:**
- Plugin Information Display
- Features Showcase
- Installation Methods (3 options)
- Parameter Reference Table
- System Requirements
- Support & Help Options

**Theme:**
- Dark background (#1A1A1A)
- Neon green accents (#00FF00)
- Professional typography
- Responsive design

---

## Key Features

### 1. One-Click Installation

```javascript
// User clicks "Install Plugin" button
// App copies VST3 bundle to:
// - Linux: ~/.vst3/ScaleChord.vst3
// - macOS: ~/Library/Audio/Plug-Ins/VST3/ScaleChord.vst3
// - Windows: AppData/Local/Programs/Common Files/VST3/ScaleChord.vst3

await installPlugin();
```

### 2. Installation Path Display

Shows platform-specific VST3 paths:
- **User VST3** - Personal plugin folder
- **System VST3** - System-wide folder
- Copy buttons for easy clipboard access

### 3. DAW Integration Guide

Built-in instructions for popular DAWs:
- Ableton Live
- Studio One
- Cubase
- Logic Pro
- FL Studio

### 4. Plugin Parameter Reference

Interactive table showing:
- Parameter name
- Value range
- Default value
- Description
- Automation support

### 5. Real-Time Status

Live indicators showing:
- Plugin status (Ready/Installing/Error)
- Installation status
- Green dot animation for active status

### 6. System Requirements

Displays:
- Minimum requirements
- Recommended specs
- Supported platforms
- Performance metrics

---

## Building & Distribution

### Development Build

```bash
npm run dev
```

Runs the app with DevTools open for debugging.

### Production Build

```bash
# Build for current platform
npm run build

# Build for specific platforms
npm run build:mac      # macOS .dmg
npm run build:win      # Windows .exe
npm run build:linux    # Linux AppImage + .deb
```

### Build Configuration (package.json)

```json
{
  "build": {
    "appId": "com.scalechord.plugin",
    "productName": "ScaleChord Plugin",
    "mac": {
      "target": ["dmg", "zip"],
      "category": "public.app-category.music"
    },
    "win": {
      "target": ["nsis", "portable"]
    },
    "linux": {
      "target": ["AppImage", "deb"]
    }
  }
}
```

### Generated Installers

**macOS:**
- `ScaleChord-1.0.0.dmg` - Disk image installer
- `ScaleChord-1.0.0-mac.zip` - Portable archive

**Windows:**
- `ScaleChord Setup 1.0.0.exe` - NSIS installer
- `ScaleChord 1.0.0 portable.exe` - Portable executable

**Linux:**
- `scalechord-1.0.0.AppImage` - AppImage bundle
- `scalechord_1.0.0_amd64.deb` - Debian package

---

## IPC Communication

### Main → Renderer Messages

```javascript
// Plugin status update
ipcMain.handle('get-plugin-status', async () => {
  return {
    name: 'ScaleChord VST3 Plugin',
    version: '1.0.0',
    status: 'ready',
    installed: isPluginInstalled()
  };
});

// Installation result
ipcMain.handle('install-plugin', async () => {
  // Copy plugin to VST3 path
  // Return success/error
});
```

### Renderer → Main Messages

```javascript
// Request plugin status
const status = await window.electronAPI.getPluginStatus();

// Request installation
const result = await window.electronAPI.installPlugin();

// Get VST3 paths
const paths = await window.electronAPI.getVST3Paths();

// Open file browser
const result = await window.electronAPI.openFileDialog(options);
```

---

## Keyboard Shortcuts

| Shortcut | Action |
|----------|--------|
| `Ctrl/Cmd + Q` | Quit application |
| `Ctrl/Cmd + R` | Refresh plugin status |
| `Ctrl/Cmd + I` | Install plugin |
| `Tab` | Navigate UI (accessibility) |
| `Enter` | Activate focused button |

---

## Error Handling

### Installation Errors

```javascript
// Plugin binary not found
Error: "Plugin binary not found"

// Permission denied
Error: "Permission denied writing to VST3 folder"

// Disk full
Error: "Insufficient disk space"
```

### Recovery Actions

1. **Check Plugin Binary**
   - Verify `vst3_plugin/ScaleChord.vst3` exists
   - Check file permissions

2. **VST3 Folder Permissions**
   - Linux: Ensure `~/.vst3` is writable
   - macOS: Check folder permissions
   - Windows: Run as administrator if needed

3. **Disk Space**
   - Ensure 50+ MB free space
   - Clear cache if needed

---

## Platform-Specific Notes

### macOS

- Requires macOS 10.13+
- App must be codesigned for distribution
- VST3 path: `~/Library/Audio/Plug-Ins/VST3`
- Gatekeeper security warning on first run (expected)

**Code Signing:**
```bash
# Before building
export APPLE_ID="your-apple-id"
export APPLE_ID_PASSWORD="your-password"
export APPLE_TEAM_ID="your-team-id"

npm run build:mac
```

### Windows

- Requires Windows 7 SP1+
- Administrator privileges may be needed
- VST3 path: `AppData\Local\Programs\Common Files\VST3`
- SmartScreen warning on first run (expected)

**Code Signing:**
```bash
# Before building
export WIN_CERT_FILE="path/to/cert.pfx"
export WIN_CERT_PASSWORD="password"

npm run build:win
```

### Linux

- Tested on Ubuntu 18.04+
- VST3 path: `~/.vst3`
- AppImage requires FUSE for execution

**AppImage Permissions:**
```bash
chmod +x scalechord-1.0.0.AppImage
./scalechord-1.0.0.AppImage
```

---

## Auto-Update System

The app includes built-in update checking:

```javascript
// Check for updates every 24 hours
setInterval(checkForUpdates, 24 * 60 * 60 * 1000);

// Download and install updates
// Restart app to apply
```

**Update Server Configuration:**
```json
{
  "build": {
    "publish": {
      "provider": "github",
      "owner": "scalechord",
      "repo": "plugin"
    }
  }
}
```

---

## User Interface Features

### Dark Theme

- **Background:** #1A1A1A (deep gray)
- **Surface:** #2A2A2A (medium gray)
- **Primary Accent:** #00FF00 (neon green)
- **Secondary Accent:** #FFFF00 (neon yellow)
- **Tertiary Accent:** #00FFFF (cyan)

### Responsive Design

- Desktop: Full layout
- Tablet: Adjusted grid (2 columns)
- Mobile: Single column layout

### Accessibility Features

- Keyboard navigation support
- High contrast mode detection
- Reduced motion support
- Screen reader compatible
- Tab focus indicators

---

## Troubleshooting

### App Won't Start

```bash
# Check logs
npm run dev

# Verify Node.js installation
node --version
npm --version

# Reinstall dependencies
rm -rf node_modules package-lock.json
npm install
```

### Plugin Won't Install

1. Check plugin binary path:
   ```bash
   ls -la vst3_plugin/ScaleChord.vst3
   ```

2. Verify VST3 folder exists:
   ```bash
   # Linux/macOS
   mkdir -p ~/.vst3
   
   # Windows
   mkdir "%APPDATA%\Local\Programs\Common Files\VST3"
   ```

3. Check permissions:
   ```bash
   # Make writable
   chmod 755 ~/.vst3
   ```

### DAW Doesn't Recognize Plugin

1. Run installer again
2. Rescan plugins in DAW
3. Check VST3 path in DAW settings
4. Verify plugin binary integrity
5. Try restarting DAW

---

## Development Tips

### Adding New Features

1. **UI Changes:**
   - Edit `index.html` (structure)
   - Edit `styles.css` (styling)

2. **Logic Changes:**
   - Edit `renderer.js` (frontend logic)
   - Edit `main.js` (backend logic)

3. **IPC Communication:**
   - Add handler in `main.js`
   - Add caller in `renderer.js`
   - Expose in `preload.js`

### Debugging

```javascript
// In renderer.js
console.log('Debug message'); // Shows in DevTools

// In main.js
console.log('Debug message'); // Shows in terminal

// IPC communication
ipcMain.handle('debug', async () => console.log('IPC event'));
```

### Testing

```bash
# Manual testing
npm run dev

# Test on different platforms
npm run build:all

# Test installers
# Windows: Run .exe, check installation
# macOS: Run .dmg, check installation
# Linux: Run .AppImage, check installation
```

---

## Performance Optimization

### Bundle Size
- Current: ~150 MB (with Electron)
- Plugin only: ~5-8 MB

### Startup Time
- Target: <2 seconds
- Measured: ~1.5 seconds (typical)

### Memory Usage
- Idle: ~100 MB
- Active: ~150-200 MB

---

## Security Considerations

- **Context Isolation:** Enabled
- **Sandbox:** Enabled for renderer
- **IPC:** Whitelist-based API exposure
- **Remote Module:** Disabled
- **Dev Tools:** Disabled in production
- **Code Signing:** Supported for all platforms

---

## Release Checklist

- [ ] All tests passing
- [ ] Version number updated
- [ ] Changelog updated
- [ ] Build for all platforms
- [ ] Test on each platform
- [ ] Sign binaries
- [ ] Create GitHub release
- [ ] Upload installers
- [ ] Update website
- [ ] Announce release

---

## Support & Documentation

- **GitHub:** https://github.com/scalechord/plugin
- **Website:** https://scalechord.example.com
- **Discord:** https://discord.gg/scalechord
- **Issues:** https://github.com/scalechord/plugin/issues

---

## License

ScaleChord VST3 Plugin - Electron App
© 2025 ScaleChord Development
Licensed under MIT License

See LICENSE file for details.
