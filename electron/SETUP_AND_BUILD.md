# ScaleChord Electron App - Setup & Build Guide

## 📋 Table of Contents

1. [Project Overview](#project-overview)
2. [Prerequisites](#prerequisites)
3. [Development Setup](#development-setup)
4. [Building & Packaging](#building--packaging)
5. [Distribution](#distribution)
6. [Platform-Specific Setup](#platform-specific-setup)
7. [Development Workflow](#development-workflow)
8. [CI/CD Integration](#cicd-integration)

---

## Project Overview

### Architecture

```
ScaleChord Electron App
├── Main Process (Node.js)
│   ├── Window Management
│   ├── File Operations
│   ├── Plugin Installation
│   └── IPC Communication
├── Renderer Process (Chromium)
│   ├── UI Components
│   ├── User Interactions
│   ├── Status Updates
│   └── Notifications
└── VST3 Plugin Bundle
    └── Pre-compiled binary
```

### Technology Stack

| Component | Technology | Version |
|-----------|-----------|---------|
| Framework | Electron | Latest |
| Build Tool | Electron Builder | Latest |
| Runtime | Node.js | 14+ |
| Package Manager | npm | 6+ |
| UI Framework | Vanilla JS + CSS3 | - |
| Bundler | Webpack (via Electron) | - |

---

## Prerequisites

### System Requirements

**All Platforms:**
- Node.js 14.0.0 or higher
- npm 6.0.0 or higher
- Git
- 500 MB free disk space

**macOS Specific:**
- macOS 10.13 or higher
- Xcode Command Line Tools: `xcode-select --install`

**Windows Specific:**
- Windows 7 SP1 or higher
- Visual Studio Build Tools (optional, for native modules)

**Linux Specific:**
- Ubuntu 18.04 or higher
- Build essentials: `sudo apt-get install build-essential`
- libssl-dev: `sudo apt-get install libssl-dev`

### Installation Verification

```bash
# Check Node.js
node --version     # Should output v14.0.0 or higher

# Check npm
npm --version      # Should output 6.0.0 or higher

# Check Git
git --version      # Should output version info
```

---

## Development Setup

### 1. Clone Repository

```bash
# Clone the repo
git clone https://github.com/scalechord/plugin.git
cd plugin/electron

# Or navigate to existing directory
cd /workspaces/production-works/electron
```

### 2. Install Dependencies

```bash
# Install all dependencies
npm install

# Verify installation
npm list electron
npm list electron-builder
```

**Expected Output:**
```
├── electron@latest
└── electron-builder@latest
```

### 3. Verify Plugin Binary

```bash
# Check if plugin exists
ls -la vst3_plugin/ScaleChord.vst3

# If not present, copy from build directory
cp /workspaces/production-works/plugin/vst3_build/ScaleChordPlugin_artefacts/Release/VST3/ScaleChord.vst3 vst3_plugin/
```

### 4. Run Development Mode

```bash
# Start development server with DevTools
npm run dev

# App should launch with:
# - Auto-reload on file changes
# - DevTools open for debugging
# - Console logging enabled
```

---

## Building & Packaging

### Development Build

```bash
# Build current platform only
npm run build

# Output: Generated binaries in dist/ folder
```

### Production Build (Recommended)

```bash
# Build for all platforms
npm run build:all

# Or build specific platforms
npm run build:mac
npm run build:win
npm run build:linux
```

### Build Output Structure

```
dist/
├── mac/
│   ├── ScaleChord-1.0.0.dmg           # macOS disk image
│   └── ScaleChord-1.0.0-mac.zip       # macOS portable
├── win/
│   ├── ScaleChord Setup 1.0.0.exe     # Windows installer
│   └── ScaleChord 1.0.0 portable.exe  # Windows portable
└── linux/
    ├── scalechord-1.0.0.AppImage      # Linux AppImage
    └── scalechord_1.0.0_amd64.deb     # Linux Debian package
```

### Build Configuration (package.json)

```json
{
  "build": {
    "appId": "com.scalechord.plugin",
    "productName": "ScaleChord Plugin",
    "directories": {
      "buildResources": "assets"
    },
    "files": [
      "main.js",
      "preload.js",
      "renderer.js",
      "styles.css",
      "index.html",
      "assets/**/*",
      "vst3_plugin/**/*"
    ]
  }
}
```

### Build Options

| Option | Effect |
|--------|--------|
| `-mwl` | Build for Mac, Windows, Linux |
| `-m` | Build for macOS only |
| `-w` | Build for Windows only |
| `-l` | Build for Linux only |
| `--publish=always` | Publish after build |
| `--publish=never` | Skip publishing |

---

## Distribution

### GitHub Releases

#### Setup GitHub Token

```bash
# Create GitHub personal access token at:
# https://github.com/settings/tokens

# Set environment variable
export GH_TOKEN=your_github_token

# Or add to ~/.bashrc / ~/.zshrc
echo 'export GH_TOKEN=your_token' >> ~/.bashrc
```

#### Create Release

```bash
# Build and publish to GitHub
npm run build:all -- --publish=always

# Creates release with:
# - Installers for all platforms
# - Release notes
# - Auto-update data
```

### Manual Distribution

```bash
# Build without publishing
npm run build:all

# Manually upload files from dist/ to:
# - GitHub Releases
# - Website download page
# - Package repositories (apt, homebrew, etc.)
```

### Update Server Setup

For auto-updates, configure update server:

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

## Platform-Specific Setup

### macOS Setup

#### Code Signing

```bash
# Create signing certificate
# 1. Go to developer.apple.com
# 2. Create Developer ID Certificate
# 3. Export as .p12 file

# Set up environment
export APPLE_ID="your-apple-id@example.com"
export APPLE_ID_PASSWORD="your-app-password"
export APPLE_TEAM_ID="XXXXXXXXXX"

# Build with signing
npm run build:mac
```

#### Notarization

```bash
# Apple requires notarization for Gatekeeper bypass

# Add to package.json:
{
  "build": {
    "mac": {
      "hardenedRuntime": true,
      "gatekeeperAssess": false,
      "notarize": {
        "teamId": "XXXXXXXXXX"
      }
    }
  }
}

# Build
npm run build:mac
# Notarization happens automatically
```

#### Distribution Options

- **App Store:** Submit `.app` to Mac App Store
- **Direct Download:** Distribute `.dmg` from website
- **Homebrew:** Submit to Homebrew for `brew install scalechord`

### Windows Setup

#### Code Signing

```bash
# Create code signing certificate
# 1. Purchase from certificate authority
# 2. Export as .pfx file

# Set up environment
set WIN_CERT_FILE=path\to\certificate.pfx
set WIN_CERT_PASSWORD=password

# Or on PowerShell
$env:WIN_CERT_FILE = "path\to\certificate.pfx"
$env:WIN_CERT_PASSWORD = "password"

# Build
npm run build:win
```

#### NSIS Installer Configuration

```json
{
  "build": {
    "nsis": {
      "oneClick": false,
      "allowToChangeInstallationDirectory": true,
      "createDesktopShortcut": true,
      "createStartMenuShortcut": true,
      "shortcutName": "ScaleChord Plugin"
    }
  }
}
```

#### Distribution Options

- **Windows Store:** Submit to Microsoft Store
- **Direct Download:** Distribute `.exe` from website
- **Chocolatey:** Submit to Chocolatey for `choco install scalechord`

### Linux Setup

#### AppImage Configuration

```json
{
  "build": {
    "linux": {
      "target": ["AppImage"],
      "desktop": {
        "Name": "ScaleChord Plugin",
        "Comment": "Intelligent MIDI voicing",
        "Categories": "Audio;AudioEditing;"
      }
    }
  }
}
```

#### Debian Package Configuration

```json
{
  "build": {
    "deb": {
      "depends": ["libssl1.1"],
      "category": "sound"
    }
  }
}
```

#### Distribution Options

- **Direct Download:** Distribute `.AppImage`
- **Debian Repository:** Submit `.deb` to repository
- **Snap Store:** Package as snap
- **Flatpak:** Package as flatpak

---

## Development Workflow

### File Structure

```
electron/
├── main.js                     # Main process
├── preload.js                  # IPC bridge
├── renderer.js                 # Renderer process
├── index.html                  # UI markup
├── styles.css                  # Styling
├── package.json                # Dependencies
├── vst3_plugin/                # Plugin binary
├── assets/                     # Icons and resources
└── dist/                       # Build output (generated)
```

### Making Changes

#### Update UI

```bash
# 1. Edit index.html (structure)
vim index.html

# 2. Edit styles.css (styling)
vim styles.css

# 3. Test in dev mode
npm run dev

# 4. Reload browser (Ctrl+R or Cmd+R)
```

#### Update Frontend Logic

```bash
# 1. Edit renderer.js
vim renderer.js

# 2. Test in dev mode (auto-reloads)
npm run dev

# 3. Check browser DevTools for errors
```

#### Update Backend Logic

```bash
# 1. Edit main.js
vim main.js

# 2. Stop dev mode (Ctrl+C)
# 3. Restart dev mode
npm run dev
```

#### Update Dependencies

```bash
# Add new package
npm install package-name

# Remove package
npm uninstall package-name

# Update all packages
npm update

# Check security issues
npm audit

# Fix security issues
npm audit fix
```

### Testing Changes

```bash
# Manual testing
npm run dev

# Test in production mode
npm run build
./dist/ScaleChord-*/ScaleChord      # Run built app

# Test on different platforms
npm run build:all
# Test each build manually
```

---

## CI/CD Integration

### GitHub Actions Setup

Create `.github/workflows/build.yml`:

```yaml
name: Build and Release

on:
  push:
    tags:
      - 'v*'

jobs:
  build:
    runs-on: ${{ matrix.os }}
    strategy:
      matrix:
        os: [ubuntu-latest, windows-latest, macos-latest]
    
    steps:
      - uses: actions/checkout@v2
      
      - uses: actions/setup-node@v2
        with:
          node-version: '14'
      
      - run: npm install
      
      - run: npm run build:all
      
      - uses: softprops/action-gh-release@v1
        with:
          files: dist/**/*
        env:
          GITHUB_TOKEN: ${{ secrets.GITHUB_TOKEN }}
```

### Local Testing

```bash
# Simulate CI build
npm ci          # Clean install
npm run build   # Build
npm test        # Run tests (if configured)
```

---

## Troubleshooting

### Development Issues

#### App Won't Start

```bash
# Clear npm cache
npm cache clean --force

# Reinstall dependencies
rm -rf node_modules package-lock.json
npm install

# Run again
npm run dev
```

#### DevTools Won't Open

```bash
# DevTools should open automatically in dev mode
# If not:
1. Press Ctrl+Shift+I (Windows/Linux) or Cmd+Option+I (macOS)
2. Check main.js has `mainWindow.webContents.openDevTools()`
```

#### Plugin Binary Not Found

```bash
# Verify location
ls vst3_plugin/ScaleChord.vst3

# If missing, copy from build:
cp ../plugin/vst3_build/.../ScaleChord.vst3 vst3_plugin/
```

### Build Issues

#### Dependency Conflicts

```bash
npm install --legacy-peer-deps
```

#### Platform-Specific Issues

**macOS:**
```bash
# Install Command Line Tools
xcode-select --install
```

**Windows:**
```bash
# Install Visual Studio Build Tools
# https://visualstudio.microsoft.com/downloads/
```

**Linux:**
```bash
sudo apt-get install build-essential libssl-dev
```

#### Out of Memory During Build

```bash
# Increase Node memory
NODE_OPTIONS=--max_old_space_size=4096 npm run build:all
```

---

## Performance Optimization

### Bundle Size

Target: Keep under 150 MB total

```bash
# Analyze bundle size
npm list

# Remove unused dependencies
npm audit

# Update electron-builder config
{
  "build": {
    "artifactName": "${productName}-${version}.${ext}",
    "asar": true  # Archive for smaller size
  }
}
```

### Startup Time

Target: Under 2 seconds

```bash
# Profile startup
time npm run dev

# Optimize main.js for faster initialization
# Load modules only when needed
```

### Runtime Memory

Target: Under 200 MB idle

```bash
# Check memory usage
# macOS: Activity Monitor
# Windows: Task Manager
# Linux: top, htop
```

---

## Release Checklist

- [ ] Update version in package.json
- [ ] Update CHANGELOG.md
- [ ] Update documentation
- [ ] Run tests
- [ ] Build for all platforms
- [ ] Test each platform installer
- [ ] Sign binaries (if required)
- [ ] Create GitHub release
- [ ] Upload installers
- [ ] Update website
- [ ] Announce on social media
- [ ] Post release notes on Discord

---

## Version Increment Guide

### Semantic Versioning

Format: `MAJOR.MINOR.PATCH`

- **MAJOR:** Breaking changes
- **MINOR:** New features (backward compatible)
- **PATCH:** Bug fixes

### Update Version

```bash
# In package.json
"version": "1.0.0"  → "1.1.0"  (new feature)
"version": "1.0.0"  → "1.0.1"  (bug fix)
"version": "1.0.0"  → "2.0.0"  (breaking change)

# Also update in:
# - Electron app code (if hardcoded)
# - Documentation
# - CHANGELOG
```

---

## Resources

- **Electron Docs:** https://www.electronjs.org/docs
- **Electron Builder:** https://www.electron.build
- **GitHub Actions:** https://github.com/features/actions
- **Code Signing:** https://www.electron.build/code-signing

---

## Quick Commands Reference

```bash
# Setup
npm install                    # Install dependencies
npm run dev                    # Run development mode

# Building
npm run build                  # Build current platform
npm run build:all              # Build all platforms
npm run build:mac              # Build macOS
npm run build:win              # Build Windows
npm run build:linux            # Build Linux
npm run pack                   # Create distributable (no installer)

# Cleanup
rm -rf node_modules dist      # Remove generated files
npm cache clean --force        # Clear npm cache

# Utilities
npm list                       # List dependencies
npm audit                      # Check security
npm update                     # Update dependencies
npm outdated                   # Check for updates
```

---

## Support & Contact

- **Issues:** https://github.com/scalechord/plugin/issues
- **Discussions:** https://github.com/scalechord/plugin/discussions
- **Email:** dev@scalechord.example.com
- **Discord:** https://discord.gg/scalechord

---

**Last Updated:** November 1, 2025  
**Electron Version:** Latest  
**Status:** ✅ Production Ready
