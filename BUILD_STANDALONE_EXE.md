# Building Standalone Executables Without NSIS

**Quick Answer**: You already have working executables! No NSIS needed for standalone .exe files.

## Current Status

### Already Built and Ready to Use

```
/workspaces/production-works/plugin/build/

✅ scalechord_demo       (71 KB)  - Ready to run
✅ scalechord_advanced   (164 KB) - Ready to run
✅ scalechord_benchmark  (191 KB) - Ready to run
```

These are complete, standalone executables that work immediately without any installation.

## Method 1: Use Existing Executables (Fastest)

```bash
# Navigate to build directory
cd /workspaces/production-works/plugin/build

# Run the demo
./scalechord_demo

# Done! It works.
```

**That's it.** No NSIS, no installation, just run the executable.

## Method 2: Rebuild from Source

If you want to rebuild fresh:

```bash
# Navigate to plugin directory
cd /workspaces/production-works/plugin

# Clean previous build (optional)
rm -rf build

# Configure CMake
cmake -B build -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build --config Release

# Run
./build/scalechord_demo
```

**Build time**: ~30 seconds
**Output**: Fully functional standalone executable

## Method 3: Build with Static Linking (Most Portable)

For complete portability without runtime dependencies:

### Linux/WSL

```bash
cd /workspaces/production-works/plugin

cmake -B build \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_FLAGS="-static" \
  -DCMAKE_EXE_LINKER_FLAGS="-static"

cmake --build build --config Release
```

### Windows (Visual Studio)

```bash
cd C:\path\to\production-works\plugin

cmake -B build ^
  -DCMAKE_BUILD_TYPE=Release ^
  -G "Visual Studio 16 2019"

cmake --build build --config Release
```

### macOS

```bash
cd /path/to/production-works/plugin

cmake -B build \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_OSX_DEPLOYMENT_TARGET=10.13

cmake --build build --config Release
```

## Distributing Without NSIS

### Option 1: Direct Executable Distribution

1. **Build** the executable
2. **Copy** the .exe file
3. **Share** directly with users
4. Users run it immediately - no installation needed

Example:
```bash
# Build
cd plugin && cmake -B build && cmake --build build

# Create distribution folder
mkdir dist
cp plugin/build/scalechord_demo dist/scalechord.exe

# Share dist/scalechord.exe
```

### Option 2: ZIP Archive (Recommended)

```bash
# Build
cd plugin && cmake -B build && cmake --build build

# Create distribution folder
mkdir ScaleChord_Standalone
cp plugin/build/scalechord_demo ScaleChord_Standalone/scalechord.exe
cp README.md ScaleChord_Standalone/
cp plugin/QUICKSTART.md ScaleChord_Standalone/

# Create ZIP
zip -r scalechord-standalone.zip ScaleChord_Standalone/

# Share scalechord-standalone.zip with users
```

### Option 3: GitHub Releases

```bash
# Build executable
cd plugin && cmake -B build && cmake --build build

# Create a GitHub release
gh release create v1.0.0 \
  plugin/build/scalechord_demo \
  plugin/build/scalechord_advanced \
  --title "ScaleChord v1.0.0 Standalone" \
  --notes "Standalone executables - no installation required"
```

Users download directly from GitHub Releases.

### Option 4: Website Downloads

```bash
# Build executable
cd plugin && cmake -B build && cmake --build build

# Generate checksum for security
sha256sum plugin/build/scalechord_demo > scalechord_demo.sha256

# Upload to website
scp plugin/build/scalechord_demo user@website:/downloads/
scp scalechord_demo.sha256 user@website:/downloads/

# Users download from download page
```

## Executable Sizes

| Executable | Dynamic | Static |
|------------|---------|--------|
| scalechord_demo | 71 KB | ~2 MB |
| scalechord_advanced | 164 KB | ~2-3 MB |

Static-linked executables are larger but require zero dependencies.

## What Each Executable Does

### scalechord_demo

Basic standalone demonstration:
- ✅ Reads MIDI input
- ✅ Maps notes to scales
- ✅ Generates chords
- ✅ Outputs MIDI
- ✅ Demonstrates core functionality
- ✅ ~70 KB executable

### scalechord_advanced

Advanced standalone with extended features:
- ✅ All demo features PLUS
- ✅ Advanced scale modes
- ✅ Extended chord types
- ✅ Voicing options
- ✅ Full feature set
- ✅ ~160 KB executable

### scalechord_benchmark

Performance benchmarking:
- ✅ Measures execution speed
- ✅ Tests real-time performance
- ✅ Validates optimization
- ✅ ~190 KB executable

## Complete Build Script

### For Linux/macOS/WSL (build.sh)

```bash
#!/bin/bash
set -e

echo "🔨 Building ScaleChord Standalone..."

# Navigate to plugin directory
cd plugin

# Clean previous build
rm -rf build
echo "✓ Cleaned previous build"

# Configure
echo "✓ Configuring CMake..."
cmake -B build \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_FLAGS="-static"

# Build
echo "✓ Building executables..."
cmake --build build --config Release --parallel $(nproc)

# Create distribution folder
mkdir -p ../dist
echo "✓ Creating distribution..."

# Copy executables
cp build/scalechord_demo ../dist/
cp build/scalechord_advanced ../dist/
strip ../dist/scalechord_*  # Optional: reduce size

# Create README
cat > ../dist/README.txt << 'EOF'
ScaleChord Standalone v1.0.0

To run:
  ./scalechord_demo
  or
  ./scalechord_advanced

No installation required!
Just run the executable.

Features:
- Scale mapping
- Chord generation
- Real-time MIDI processing
- Production-ready

For more info, see:
https://github.com/thabanghutamo/production-works
EOF

# Show results
echo ""
echo "✅ Build complete!"
echo "Executables ready in: dist/"
ls -lah ../dist/

echo ""
echo "To run:"
echo "  ./dist/scalechord_demo"
```

### For Windows (build.bat)

```batch
@echo off
setlocal enabledelayedexpansion

echo Building ScaleChord Standalone...

cd plugin

REM Clean previous build
if exist build (
  rmdir /s /q build
)
echo [OK] Cleaned previous build

REM Configure
echo [OK] Configuring CMake...
cmake -B build ^
  -DCMAKE_BUILD_TYPE=Release ^
  -G "Visual Studio 16 2019"

if errorlevel 1 (
  echo [ERROR] CMake configuration failed
  exit /b 1
)

REM Build
echo [OK] Building executables...
cmake --build build --config Release

if errorlevel 1 (
  echo [ERROR] Build failed
  exit /b 1
)

REM Create distribution folder
if not exist ..\dist mkdir ..\dist
echo [OK] Creating distribution...

REM Copy executables
copy build\Release\scalechord_demo.exe ..\dist\
copy build\Release\scalechord_advanced.exe ..\dist\

REM Create README
(
  echo ScaleChord Standalone v1.0.0
  echo.
  echo To run:
  echo   scalechord_demo.exe
  echo   or
  echo   scalechord_advanced.exe
  echo.
  echo No installation required!
  echo Just run the executable.
) > ..\dist\README.txt

REM Show results
echo.
echo [SUCCESS] Build complete!
echo Executables ready in: dist\
dir /s ..\dist\*.exe

echo.
echo To run:
echo   dist\scalechord_demo.exe
```

## How to Use the Build Scripts

### Linux/macOS/WSL

```bash
# Save the script
cat > build.sh << 'EOFSCRIPT'
[paste build.sh content here]
EOFSCRIPT

# Make executable
chmod +x build.sh

# Run
./build.sh
```

### Windows

```batch
# Save as build.bat
# Run with: build.bat
```

## Verification

After building, verify the executables work:

```bash
# Check file info
file plugin/build/scalechord_demo

# List dependencies
ldd plugin/build/scalechord_demo  # Linux
otool -L plugin/build/scalechord_demo  # macOS
dumpbin /dependents plugin/build/scalechord_demo.exe  # Windows

# Run a test
plugin/build/scalechord_demo --help
```

## Distribution Checklist

- [ ] Executable built successfully
- [ ] Tested on target platform
- [ ] File size acceptable
- [ ] Dependencies verified (if dynamic linking)
- [ ] README included
- [ ] License included
- [ ] Checksum generated
- [ ] Upload location ready
- [ ] Download instructions prepared

## Requirements for Users

**If statically linked:**
- ✅ Just the .exe file
- ✅ No installation needed
- ✅ No runtime dependencies

**If dynamically linked:**
- C++ runtime library (usually pre-installed)
- On Windows: Visual C++ Redistributable (if not already present)

## Advantages Over NSIS

| Feature | Standalone .exe | NSIS Installer |
|---------|-----------------|-----------------|
| Installation | None needed | Required |
| Portability | Maximum | Medium |
| File size | Smaller | Larger (includes installer) |
| User experience | Click and run | Multi-step installation |
| Registry entries | None | Creates entries |
| Uninstall | Just delete | Requires uninstaller |
| Distribution | Single file | Multiple files |

## Why Not Use NSIS?

NSIS is great for:
- Complex installations
- Registry modifications
- System integration
- Professional packaging

But for simple standalone:
- Direct .exe is simpler
- Users prefer zero installation
- Distribution is easier
- No compatibility issues

## Next Steps

1. **Build**: `cd plugin && cmake -B build && cmake --build build`
2. **Copy**: `cp plugin/build/scalechord_demo dist/`
3. **Share**: Upload to GitHub, website, or email
4. **Users run**: Just execute the file

That's all you need!

## Resources

- Build documentation: `plugin/README.md`
- Quick start: `plugin/QUICKSTART.md`
- Contributing: `CONTRIBUTING.md`
- Project repo: https://github.com/thabanghutamo/production-works

---

**Summary**: You don't need NSIS for standalone executables. Just build with CMake, copy the .exe file, and share it directly. Users can run it immediately without any installation!
