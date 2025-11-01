# Quick Reference: Build Standalone EXE Without NSIS

## TL;DR

```bash
# Already built? Just use it!
./plugin/build/scalechord_demo

# Or rebuild fresh
cd plugin && cmake -B build && cmake --build build

# Copy and distribute
cp plugin/build/scalechord_demo dist/scalechord.exe
```

**That's it. No NSIS needed.**

---

## One-Liner Commands

### Build and Run (Linux/macOS/WSL)
```bash
cd /workspaces/production-works/plugin && cmake -B build && cmake --build build && ./build/scalechord_demo
```

### Build and Run (Windows PowerShell)
```powershell
cd C:\path\to\production-works\plugin; cmake -B build; cmake --build build --config Release; .\build\Release\scalechord_demo.exe
```

### Build with Static Linking
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-static" && cmake --build build
```

### Create ZIP Distribution
```bash
cd /workspaces/production-works && \
mkdir -p scalechord-dist && \
cp plugin/build/scalechord_demo scalechord-dist/scalechord.exe && \
cp README.md scalechord-dist/ && \
zip -r scalechord-standalone.zip scalechord-dist/
```

---

## File Locations

| File | Location | Size | Status |
|------|----------|------|--------|
| Demo executable | `plugin/build/scalechord_demo` | 71 KB | ✅ Ready |
| Advanced demo | `plugin/build/scalechord_advanced` | 164 KB | ✅ Ready |
| CMakeLists.txt | `plugin/CMakeLists.txt` | - | ✅ Configured |

---

## Distribution Methods

### Direct Download
```
User: Downloads scalechord.exe
User: Clicks to run
Done!
```

### GitHub Releases
```bash
gh release create v1.0.0 plugin/build/scalechord_demo
```

### Website
```bash
scp plugin/build/scalechord_demo user@website:/downloads/
```

### Email
```bash
zip scalechord.exe && mail -a scalechord.zip user@email.com
```

---

## Build Options

| Option | Command | Result | Speed |
|--------|---------|--------|-------|
| Use existing | `./plugin/build/scalechord_demo` | Works now | Instant |
| Fresh build | `cmake -B build && cmake --build build` | New binary | 30s |
| Static link | `-DCMAKE_CXX_FLAGS="-static"` | No dependencies | 30s |
| Release mode | `-DCMAKE_BUILD_TYPE=Release` | Optimized | 30s |

---

## What Users Need

### If Static Linked
- ✅ Just the .exe file
- ✅ No installation
- ✅ No dependencies
- ✅ Works immediately

### If Dynamic Linked
- .exe file
- C++ runtime (usually pre-installed)
- On Windows: Visual C++ Redistributable (if needed)

---

## Testing

```bash
# Check executable
file plugin/build/scalechord_demo

# List dependencies
ldd plugin/build/scalechord_demo        # Linux
otool -L plugin/build/scalechord_demo   # macOS
dumpbin /dependents scalechord_demo.exe # Windows

# Test it runs
plugin/build/scalechord_demo --help
```

---

## CMake Cheat Sheet

| Task | Command |
|------|---------|
| Configure | `cmake -B build` |
| Build | `cmake --build build` |
| Build Release | `cmake --build build --config Release` |
| Clean | `rm -rf build` |
| Parallel build | `cmake --build build --parallel $(nproc)` |
| Specific generator | `cmake -G "Visual Studio 16 2019" -B build` |

---

## VSCode Build Task

Add to `.vscode/tasks.json`:

```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "Build ScaleChord",
            "type": "shell",
            "command": "cmake",
            "args": [
                "--build",
                "${workspaceFolder}/plugin/build"
            ],
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "problemMatcher": [
                "$gcc"
            ]
        }
    ]
}
```

---

## Platform-Specific

### Windows
```batch
cd plugin
cmake -B build -G "Visual Studio 16 2019"
cmake --build build --config Release
.\build\Release\scalechord_demo.exe
```

### Linux
```bash
cd plugin
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/scalechord_demo
```

### macOS
```bash
cd plugin
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/scalechord_demo
```

### WSL
```bash
cd /mnt/c/path/to/production-works/plugin
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/scalechord_demo
```

---

## Size Optimization

### Reduce Binary Size
```bash
# Strip symbols
strip plugin/build/scalechord_demo

# Before: 71 KB → After: ~35 KB
```

### Static Link Optimization
```bash
cmake -B build \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_FLAGS="-O3 -s"  # Optimize and strip
```

---

## Troubleshooting

| Issue | Solution |
|-------|----------|
| CMake not found | Install CMake: `sudo apt install cmake` |
| Compiler not found | Install compiler: `sudo apt install build-essential` |
| Build fails | Clean: `rm -rf build` then rebuild |
| Executable won't run | Check: `file build/scalechord_demo` |
| Wrong architecture | Specify: `-DCMAKE_BUILD_TYPE=Release` |

---

## Documentation

| Document | Purpose |
|----------|---------|
| `BUILD_STANDALONE_EXE.md` | Full guide (you are here) |
| `plugin/README.md` | Project overview |
| `plugin/QUICKSTART.md` | Quick start guide |
| `plugin/BUILDING_VST3.md` | Building plugin version |
| `CONTRIBUTING.md` | Contributing guidelines |

---

## Key Points

1. **No NSIS needed** for simple standalone executables
2. **CMake does everything** - build, link, optimize
3. **Single file distribution** - copy .exe and go
4. **Users run immediately** - no installation steps
5. **Static linking optional** - but recommended for portability

---

## Next Steps

```bash
# 1. Build
cd /workspaces/production-works/plugin
cmake -B build && cmake --build build

# 2. Test
./build/scalechord_demo

# 3. Distribute
cp build/scalechord_demo ../dist/scalechord.exe

# 4. Share
# Upload to GitHub, website, or email
```

---

## Support

For issues or questions:
- GitHub Issues: https://github.com/thabanghutamo/production-works/issues
- Documentation: See `/workspaces/production-works/plugin/README.md`
- Quick start: See `/workspaces/production-works/plugin/QUICKSTART.md`

---

**You don't need NSIS for standalone executables. Just use CMake and copy the binary!**
