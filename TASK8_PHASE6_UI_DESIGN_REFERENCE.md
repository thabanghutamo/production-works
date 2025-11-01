# ScaleChord Plugin UI Design - Complete Reference (Phase 6 Final)

## UI Layout Architecture

### High-Level Structure
```
┌─────────────────────────────────────────────────────────────────┐
│                     ScaleChord Plugin UI                         │
│                     1000px × 700px                              │
├─────────────────────────────────────────────────────────────────┤
│  [LEFT PANEL]   │         [CENTER PANEL]      │   [RIGHT PANEL]  │
│   30% (300px)   │          40% (400px)         │    30% (300px)   │
├─────────────────────────────────────────────────────────────────┤
│ PARAMETERS     │        SPECTRUM              │    METRICS       │
│   (Sliders)    │     (32-Band Display)        │  (Performance)   │
│                │       (Peak Overlay)         │                  │
└─────────────────────────────────────────────────────────────────┘
```

---

## Detailed Panel Designs

### LEFT PANEL - PARAMETERS (300px × 700px)
```
┌─────────────────────────────────────────┐
│ ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓ │ ← Blue Accent Bar (3px)
│                                         │
│  ■ PARAMETERS ■                         │ ← Title (15pt bold, white)
│  ▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔  │ ← Separator (1px)
│                                         │
│  [K] ●         [S] ●                   │ ← Slider pairs
│  Key   Voicing Scale    Octave         │ ← Parameter labels
│                                         │
│  [A] ●         [D] ●                   │ ← Row 2
│  Attack   Decay Sustain  Release       │
│                                         │
│  [E] ●         [R] ●                   │ ← Row 3
│  Effect  Reverb  Delay   Chorus        │
│                                         │
│  ┌─────────────────────────────────────┤ ← 3px border
│  │                                     │
│                                         │
│                                         │
└─────────────────────────────────────────┘

Colors:
• Background: #1a1a1a (dark)
• Border: #2d2d2d (3px)
• Accent Bar: #0099ff (bright blue)
• Text: #ffffff (white)
• Slider Thumb: #0099ff (bright blue)
• Text Secondary: #888888 (gray)
```

**Features**:
- 12 rotary sliders in 2 columns × 6 rows
- 10px padding from edges
- 50px slider width/height
- 20px label height
- Professional spacing (50px between rows)
- Color-coded by function (Key, Scale, Effects)

---

### CENTER PANEL - SPECTRUM (400px × 700px)
```
┌─────────────────────────────────────────────┐
│ ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓ │ ← Green Accent Bar (3px)
│                                             │
│  ■ SPECTRUM ■                              │ ← Title (15pt bold, white)
│  ▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔  │ ← Separator (1px)
│                                             │
│          ┌────────────────────────────┐    │
│          │ ▌  ▌  ▌  ▌  ▌  ▌  ▌  ▌    │    │ ← 32-band visualization
│          │ ▌▌ ▌▌ ▌▌ ▌▌ ▌▌ ▌▌ ▌▌▌    │    │
│          │ ▌▌ ▌▌ ▌▌ ▌▌ ▌▌ ▌▌▌▌▌▌    │    │ Cyan  Green  Red
│          │ ▌▌▌▌▌▌▌▌▌▌▌▌▌▌▌▌▌▌▌▌▌▌  │    │ gradient colors
│          │ ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓  │    │
│          │ ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓  │    │ Peak hold lines
│          │ ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓  │    │ (2.5px thickness)
│          │ ▀ ▀ ▀ ▀ ▀ ▀ ▀ ▀ ▀ ▀ ▀ ▀▀ │    │
│          └────────────────────────────┘    │
│          20Hz            10kHz          20kHz │ ← Frequency scale
│                                             │
│  ┌─────────────────────────────────────────┤ ← 3px border
│  │                                         │
└─────────────────────────────────────────────┘

Colors:
• Background: #1a1a1a (dark)
• Border: #2d2d2d (3px)
• Accent Bar: #00ff00 (green/SpectrumMid)
• Band Colors: Cyan (#00ffff) → Green (#00ff00) → Red (#ff0000)
• Peak Color: Matches band color (highlighted)
• Band Borders: 1px individual borders for clarity
```

**Features**:
- 32 frequency bands (logarithmic distribution)
- Cyan→Green→Red smooth color gradient
- Peak hold with 30-frame decay (500ms at 60 FPS)
- Individual band borders for visual definition
- 10px padding from edges
- 85% usable height
- Responsive to audio content (simulated bass-heavy)

---

### RIGHT PANEL - METRICS (300px × 700px)
```
┌─────────────────────────────────────────┐
│ ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓ │ ← Green Accent Bar (3px)
│                                         │
│  ■ METRICS ■                            │ ← Title (15pt bold, white)
│  ▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔  │ ← Separator (1px)
│                                         │
│  Voices: 8/16                          │ ← Metric (12pt, gray)
│                                         │ ← 28px spacing
│  CPU: 3.2%                             │ ← Metric (12pt, gray)
│                                         │ ← 28px spacing
│  Latency: 0.85 ms                      │ ← Metric (12pt, gray)
│                                         │ ← 28px spacing
│  Health: 92%                           │ ← Metric (12pt bold, green)
│                                         │
│  ┌──────────────────────────────────┐  │
│  │         ◆ EXCELLENT ◆            │  │ ← Status bar (health-colored)
│  │                                  │  │
│  └──────────────────────────────────┘  │
│                                         │
│  ┌─────────────────────────────────────┤ ← 3px border
│  │                                     │
│                                         │
└─────────────────────────────────────────┘

Colors (Dynamic based on Health Score):
• Background: #1a1a1a (dark)
• Border: #2d2d2d (3px)
• Accent Bar: #44ff44 (green/HealthGood)
• Text Primary: #ffffff (white)
• Text Secondary: #888888 (gray)
• Status Background Options:
  - Health > 80%: #44ff44 (green/HealthGood)
  - Health 40-80%: #ffaa00 (orange/HealthWarning)
  - Health < 40%: #ff4444 (red/HealthBad)
• Status Text: #1a1a1a (dark, contrasting with background)
```

**Features**:
- Real-time performance metrics
- Voice count: 0-16 (simulated dynamic)
- CPU usage: 2-5% typical range
- Latency: 0.8ms typical
- Health score: 0-100% (calculated)
- Color-coded status (green/orange/red)
- Highlighted status bar with background color
- 15px left padding for metrics
- 28px vertical spacing between metrics
- Status bar: 50px height, centered text

---

## Color Theme Reference

### Complete 12-Color Theme

```
╔══════════════════════════════════════════════════════════════╗
║                   ScaleChord Color Theme                     ║
╚══════════════════════════════════════════════════════════════╝

PRIMARY BACKGROUND COLORS
┌─────────────────────────────────────────────────────────────┐
│ Background:  #1a1a1a (26, 26, 26)                           │
│ ■ Very dark gray - Primary panel fill                       │
│                                                             │
│ Panel:       #2d2d2d (45, 45, 45)                           │
│ ■ Dark gray - Borders and separators                        │
└─────────────────────────────────────────────────────────────┘

INTERACTIVE ELEMENTS
┌─────────────────────────────────────────────────────────────┐
│ SliderThumb: #0099ff (0, 153, 255)                          │
│ ■ Bright blue - Parameter sliders, left accent bar         │
└─────────────────────────────────────────────────────────────┘

SPECTRUM VISUALIZATION
┌─────────────────────────────────────────────────────────────┐
│ SpectrumLow: #00ffff (0, 255, 255)                          │
│ ■ Cyan - Low frequencies (bass)                            │
│                                                             │
│ SpectrumMid: #00ff00 (0, 255, 0)                            │
│ ■ Green - Mid frequencies, center accent bar               │
│                                                             │
│ SpectrumHigh: #ff0000 (255, 0, 0)                           │
│ ■ Red - High frequencies (treble)                          │
└─────────────────────────────────────────────────────────────┘

TEXT COLORS
┌─────────────────────────────────────────────────────────────┐
│ TextPrimary:   #ffffff (255, 255, 255)                      │
│ ■ White - Titles, primary labels                           │
│                                                             │
│ TextSecondary: #888888 (136, 136, 136)                      │
│ ■ Gray - Secondary labels, metrics                         │
└─────────────────────────────────────────────────────────────┘

HEALTH INDICATORS
┌─────────────────────────────────────────────────────────────┐
│ HealthGood:    #44ff44 (68, 255, 68)                        │
│ ■ Bright green - Health > 80%, right accent bar            │
│                                                             │
│ HealthWarning: #ffaa00 (255, 170, 0)                        │
│ ■ Orange - Health 40-80%                                   │
│                                                             │
│ HealthBad:     #ff4444 (255, 68, 68)                        │
│ ■ Bright red - Health < 40%                                │
└─────────────────────────────────────────────────────────────┘

USAGE SUMMARY
┌──────────────────┬───────────────────────────────────────┐
│ Color            │ Primary Uses                          │
├──────────────────┼───────────────────────────────────────┤
│ Background       │ Panel backgrounds                     │
│ Panel            │ Borders (3px), separators (1px)       │
│ SliderThumb      │ Slider controls, left accent bar      │
│ SpectrumLow      │ Spectrum bass, gradient start        │
│ SpectrumMid      │ Spectrum mids, center accent bar     │
│ SpectrumHigh     │ Spectrum highs, gradient end         │
│ TextPrimary      │ Titles, headers                       │
│ TextSecondary    │ Labels, secondary text                │
│ HealthGood       │ Health good, status highlight, accent │
│ HealthWarning    │ Health warning, status highlight      │
│ HealthBad        │ Health bad, status highlight          │
└──────────────────┴───────────────────────────────────────┘
```

---

## Typography Reference

### Font Sizes & Styles

```
┌────────────────────────────────────────────────────────┐
│ TYPOGRAPHY HIERARCHY                                   │
├────────────────────────────────────────────────────────┤
│                                                        │
│ Titles (Panel Headers)                                │
│ Size: 15pt                                            │
│ Weight: BOLD                                          │
│ Color: TextPrimary (#ffffff)                          │
│ Example: "PARAMETERS", "SPECTRUM", "METRICS"         │
│ ────────────────────────────────────────────────────  │
│                                                        │
│ Secondary Labels (Status Text)                        │
│ Size: 13pt                                            │
│ Weight: BOLD                                          │
│ Color: HealthGood/Warning/Bad (dynamic)               │
│ Example: "EXCELLENT", "OK", "STRESSED"               │
│ ────────────────────────────────────────────────────  │
│                                                        │
│ Parameter Labels                                      │
│ Size: 12pt                                            │
│ Weight: Regular                                       │
│ Color: TextPrimary (#ffffff)                          │
│ Example: "Key", "Scale", "Attack", "Reverb"         │
│ ────────────────────────────────────────────────────  │
│                                                        │
│ Metric Values                                         │
│ Size: 12pt                                            │
│ Weight: Regular (metrics) / BOLD (health)             │
│ Color: TextSecondary (#888888) for regular            │
│ Color: HealthGood/Warning/Bad for health             │
│ Example: "Voices: 8/16", "CPU: 3.2%", "Health: 92%" │
│ ────────────────────────────────────────────────────  │
│                                                        │
│ Minimum Font                                          │
│ Size: 11pt                                            │
│ Weight: Regular                                       │
│ Color: TextSecondary (#888888)                        │
│ Example: Secondary metrics labels                    │
│                                                        │
└────────────────────────────────────────────────────────┘
```

---

## Spacing System

### Consistent Margins & Padding

```
PANEL LAYOUT
┌─────────────────────────────────────────┐
│ ▓ 3px Top Accent Bar (Decorative)       │
│ ▔▔ 1px Separator Line                   │
│ • Title Section (40px height)           │
│ ▔▔ 1px Separator Line (below title)     │
│ • Content Area                          │
│   ├─ 10px left padding (PANEL_MARGIN)   │
│   ├─ 10px right padding                 │
│   ├─ 10px top padding                   │
│   └─ 10px bottom padding                │
│ • 3px Border (all sides)                │
└─────────────────────────────────────────┘

METRICS SPACING
┌──────────────────────────────────────┐
│ Voices: 8/16         ← 15px left pad │
│                      ← 28px spacing  │
│ CPU: 3.2%                           │
│                      ← 28px spacing  │
│ Latency: 0.85 ms                    │
│                      ← 28px spacing  │
│ Health: 92%                         │
│                                      │
│ ┌────────────────────────────────┐   │
│ │     ◆ EXCELLENT ◆              │   │ ← 5px margin all sides
│ └────────────────────────────────┘   │
└──────────────────────────────────────┘

SPECTRUM BANDS
• Band Width: (Area Width / 32) pixels
• Band Height: Dynamic based on frequency
• Max Height: 85% of spectrum area (15% reserved)
• Left Padding: 10px
• Right Padding: 10px
• Individual Band Borders: 1px
• Band Spacing: 1px

SLIDER LAYOUT (2 columns × 6 rows)
• Column 1: x = 5px, width = 40px
• Column 2: x = 100px, width = 40px
• Row Spacing: 50px
• Slider Size: 50px × 50px
• Label Size: 35px × 20px
```

---

## Responsive Behavior

### Aspect Ratio & Scaling

```
MINIMUM SIZE: 800 × 600
┌──────────────────────────────────────────┐
│ [L:240] [C:320] [R:240]                  │
│ 30%     40%      30%                      │
└──────────────────────────────────────────┘

RECOMMENDED SIZE: 1000 × 700
┌───────────────────────────────────────────────┐
│ [L:300] [C:400] [R:300]                       │
│ 30%     40%      30%                          │
└───────────────────────────────────────────────┘

LARGE SIZE: 1400 × 900
┌─────────────────────────────────────────────────────────┐
│ [L:420] [C:560] [R:420]                                 │
│ 30%     40%      30%                                    │
└─────────────────────────────────────────────────────────┘

PANEL WIDTH RATIOS (Fixed)
• Left Panel:   30% (Parameters)
• Center Panel: 40% (Spectrum)
• Right Panel:  30% (Metrics)

RESPONSIVE CALCULATIONS
┌──────────────────────────────────────────┐
│ Left Width  = (TotalWidth * 0.30)        │
│ Min Width   = 150px (enforced minimum)   │
│ Right Width = (TotalWidth * 0.30)        │
│ Min Width   = 150px (enforced minimum)   │
│ Center Width = TotalWidth - Left - Right │
└──────────────────────────────────────────┘
```

---

## Animation & Transitions

### Real-Time Updates (60 FPS)

```
SPECTRUM DECAY ALGORITHM
┌────────────────────────────────────────────────────────┐
│ Decay Factor:     0.85 (per frame)                     │
│ Rise Factor:      1.2 (when new data arrives)          │
│ Update Rate:      60 FPS (16ms per frame)              │
│ Peak Hold Time:   30 frames (500ms)                    │
│ Smooth Rendering: Linear interpolation                │
│                                                        │
│ Frame-by-frame calculation:                           │
│ spectrumBands[i] = (bands[i] * 0.85) + (data[i] * 1.2) │
│ Peak: Hold for 30 frames, then decay                  │
└────────────────────────────────────────────────────────┘

COLOR TRANSITIONS
┌────────────────────────────────────────────────────────┐
│ Spectrum Gradient:  Smooth interpolation               │
│   0.0 → 0.5: Cyan to Green                            │
│   0.5 → 1.0: Green to Red                             │
│                                                        │
│ Health Status Color: Instant update                    │
│   > 80%: Green (#44ff44)                              │
│   40-80%: Orange (#ffaa00)                            │
│   < 40%: Red (#ff4444)                                │
└────────────────────────────────────────────────────────┘

VISUAL FEEDBACK
┌────────────────────────────────────────────────────────┐
│ Metrics Update: Every 16ms (60 FPS)                    │
│ Spectrum Update: Every 16ms with decay algorithm       │
│ Peak Hold: 30-frame hold, then gradual decay           │
│ Status Bar: Instant color change on health change      │
│ Overall: Smooth, real-time visual feedback             │
└────────────────────────────────────────────────────────┘
```

---

## Visual Definition Elements

### Borders, Lines & Highlights

```
BORDER SYSTEM
┌──────────────────────────────────────────┐
│ ▓▓▓ All panel borders: 3px thickness    │
│ ▓▓▓ Color: #2d2d2d (Panel color)        │
│ ▓▓▓ Applied to all three main panels    │
└──────────────────────────────────────────┘

SEPARATOR LINES
┌──────────────────────────────────────────┐
│ Below panel title: 1px horizontal line   │
│ Color: #2d2d2d (Panel color)            │
│ Horizontal margin: 5px from edge         │
│ Placement: Below "PARAMETERS", etc.      │
└──────────────────────────────────────────┘

ACCENT BARS (Decorative)
┌──────────────────────────────────────────┐
│ Top of each panel: 3px solid bar         │
│ Left Panel:   #0099ff (SliderThumb)     │
│ Center Panel: #00ff00 (SpectrumMid)     │
│ Right Panel:  #44ff44 (HealthGood)      │
│ Thickness: 3px                          │
│ Purpose: Visual interest & panel ID      │
└──────────────────────────────────────────┘

BAND DEFINITION (Spectrum)
┌──────────────────────────────────────────┐
│ Each spectrum band has 1px border        │
│ Border color: Matches band fill color    │
│ Provides visual separation               │
│ Improves readability at high zoom        │
└──────────────────────────────────────────┘

STATUS HIGHLIGHT (Metrics)
┌──────────────────────────────────────────┐
│ Status bar background: Health color      │
│ Green (#44ff44) when health > 80%        │
│ Orange (#ffaa00) when 40-80%             │
│ Red (#ff4444) when health < 40%          │
│ 5px margin around status bar              │
│ Creates visual emphasis for status       │
└──────────────────────────────────────────┘
```

---

## Summary

**Phase 6 UI Design Complete** ✅

### Design System Implemented:
- ✅ 12-color professional theme
- ✅ Consistent 10px padding system
- ✅ 3px borders on all panels
- ✅ Responsive 3-panel layout (3:4:3)
- ✅ Professional typography (11-15pt)
- ✅ Color-coded health indicators
- ✅ Spectrum gradient visualization
- ✅ Real-time metrics display
- ✅ Decorative accent bars
- ✅ Visual definition elements

**Result**: Professionally polished UI ready for v1.0 release.
