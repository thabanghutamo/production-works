# ScaleChord Parameter Reference - v1.0.0

**Complete guide to all 12 parameters and their effects**

---

## Overview

ScaleChord exposes 12 core parameters for real-time control of the harmonic processing engine. All parameters are:
- ✅ Real-time adjustable (no clicks or pops)
- ✅ Automatable in DAWs supporting APVTS
- ✅ Safe to change during playback
- ✅ With sensible defaults and ranges

---

## 🎛️ Core Parameters

### 1. Scale Selection
**Parameter ID**: `scaleSelect`  
**Type**: Enumerated (48 options)  
**Default**: C Major  
**Range**: 48 scales

#### Available Scales:

**Major Modes (7)**
- Major (Ionian)
- Dorian
- Phrygian
- Lydian
- Mixolydian
- Natural Minor (Aeolian)
- Locrian

**Blues Variations (12)**
- Blues Major
- Blues Minor
- Pentatonic Blues
- Minor Pentatonic Blues
- Blues Dominant
- Blues Dorian
- [Additional 6 blues variations]

**Pentatonic Flavors (8)**
- Major Pentatonic
- Minor Pentatonic
- Pentatonic #4
- Pentatonic b2
- Egyptian Pentatonic
- Japanese Pentatonic
- [Additional 2 pentatonic variations]

**Exotic/World Scales (10)**
- Arabic (Double Harmonic)
- Japanese (Hirajoshi)
- Hindu
- Gypsy (Hungarian Minor)
- Neapolitan
- Harmonic Minor
- Melodic Minor
- [Additional 3 exotic scales]

**Advanced Harmonic Scales (11)**
- Harmonic Major
- Neapolitan Major
- Neapolitan Minor
- Russian Minor
- Hungarian Major
- Messiaen Mode
- [Additional 5 advanced scales]

#### Usage Notes:
- Real-time selectable during playback
- Each scale has unique harmonic character
- Jazz musicians: Try Dorian, Mixolydian, Phrygian Dominant
- Electronic producers: Try Minor Pentatonic, Blues modes
- World music: Exotic scales for global sounds

---

### 2. Root Note Selection
**Parameter ID**: `rootNote`  
**Type**: Enumerated (12 options)  
**Default**: C  
**Range**: C, C#, D, D#, E, F, F#, G, G#, A, A#, B

#### Usage:
- Transpose entire scale to any root note
- All voicings automatically adjust
- Affects all harmonies in real-time
- Common sequence: C → F → Bb → Eb (jazz blues)

#### Tips:
- Use to match song key
- Try different roots with same scale for variation
- Automation: Slowly sweep root for psychoacoustic effects

---

### 3. Voicing Style
**Parameter ID**: `voicingMode`  
**Type**: Enumerated (3 options)  
**Default**: Balanced  
**Range**: Compact, Balanced, Spread

#### Modes:

**Compact**
- Voices stay close together
- More cohesive, blended sound
- Best for: Legato playing, smooth progressions
- Example use: Film scoring, ambient

**Balanced** (Default)
- Optimal voice spacing
- Natural, musicianly sound
- Best for: General use, all music styles
- Example use: Most production scenarios

**Spread**
- Voices separated across keyboard
- Rich, orchestral sound
- More CPU (but still < 5%)
- Best for: Dense harmonies, orchestral effects
- Example use: Cinematic music, classical

#### Voice Count by Mode:
- **Compact**: 2-3 additional voices per note
- **Balanced**: 3-4 additional voices per note (default)
- **Spread**: 4-5 additional voices per note

#### Tips:
- Change voicing style to add variety without changing notes
- Compact mode = more CPU headroom
- Spread mode = richer textures

---

### 4. Sustain Time
**Parameter ID**: `sustainTime`  
**Type**: Linear (milliseconds)  
**Default**: 500  
**Range**: 0 - 4000 ms

#### Effect:
- Controls how long harmony notes sustain after MIDI note release
- 0 ms = harmony ends when note ends (staccato)
- 500 ms = short decay (default, natural)
- 2000 ms = long tail (pad-like)
- 4000 ms = very long sustain (cinematic)

#### Usage Scenarios:
- **0 ms**: Staccato legato, tight rhythmic control
- **100-300 ms**: Bright, articulate playing
- **500 ms** (default): Natural sustain
- **1000+ ms**: Ambient textures, pad-like effect

#### Tips:
- Lower values = more rhythmic definition
- Higher values = smoother, dreamier sound
- Automate for dynamic interest
- Watch CPU with high sustain + spread voicing

---

### 5. Effects Chain Toggle
**Parameter ID**: `effectsEnabled`  
**Type**: Boolean  
**Default**: Off  
**Range**: On/Off

#### Effect:
- Enables/disables entire effects chain
- When OFF: Pure harmonies only
- When ON: Harmonies + 6 effects

#### 6 Available Effects (in chain order):
1. **Reverb**: Room simulation (wet/dry control)
2. **Delay**: Echo effect (time/feedback control)
3. **Chorus**: Modulation effect (depth/rate control)
4. **Distortion**: Harmonic saturation (amount control)
5. **EQ**: 3-band equalizer (low/mid/high control)
6. **Compression**: Dynamic control (ratio/threshold control)

#### Usage:
- OFF: Clean, transparent harmonization
- ON: Enhanced sound with spatial effects

#### Tips:
- Start with effects OFF to hear pure harmonies
- Gradually add effects for enhancement
- Use reverb for space, delay for rhythmic interest
- Distortion adds edge and character

---

### 6. Performance Level
**Parameter ID**: `performanceLevel`  
**Type**: Read-Only Display (0-100%)  
**Default**: N/A  
**Range**: 0-100%

#### Display:
- Shows current CPU load percentage
- Real-time updated
- Green: < 20% (excellent)
- Yellow: 20-50% (good)
- Red: > 50% (demanding)

#### What Affects This:
- Voicing style (Spread = more CPU)
- Sustain time (longer = more CPU)
- Effects enabled (effects add 5-15%)
- Number of simultaneous voices

#### Tips:
- Monitor during production
- If red: Reduce voicing style or disable effects
- Typical: 2-5% green zone
- Safe operation: Stays yellow

---

## 🎼 Advanced Parameters (Phase 5-6)

### 7. Jazz Reharmonizer Intensity
**Parameter ID**: `reharmonizeIntensity`  
**Type**: Linear (0-100)  
**Default**: 0 (disabled)  
**Range**: 0-100%

#### Effect:
- Controls jazz reharmonization strength
- 0 = disabled (pure scale voicing)
- 25-50% = subtle reharmonization
- 75-100% = aggressive jazz harmony

#### When to Use:
- Jazz improvisation
- Reharmonizing standard progressions
- Adding complexity to simple melodies

#### Tips:
- Start at 0 for standard voicing
- Gradually increase for harmonic richness
- Works best with Dorian/Mixolydian scales

---

### 8. Voice Leading Smoothness
**Parameter ID**: `voiceLeadingSmooth`  
**Type**: Linear (0-100)  
**Default**: 75  
**Range**: 0-100%

#### Effect:
- Controls voice leading algorithm aggression
- 0% = no smoothing (any voicing possible)
- 50% = balanced smoothing
- 100% = maximum smooth voice movement

#### Performance:
- 0-50%: Slightly more CPU efficient
- 75%: Balanced (default)
- 100%: Most CPU intensive

#### Musical Effect:
- Lower = more active voice movement
- Higher = smoother, more connected

#### Tips:
- Jazz: Try 75-100% for smooth flow
- Electronic: Try 25-50% for more movement
- Classical: Try 85%+ for legit voice leading

---

### 9. Tension Analysis
**Parameter ID**: `tensionLevel`  
**Type**: Read-Only Display (0-100)  
**Default**: N/A  
**Range**: 0-100%

#### Display:
- Shows harmonic complexity/tension
- Real-time updated
- Low (0-30%): Simple, stable harmony
- Medium (30-70%): Rich, active harmony
- High (70-100%): Complex, avant-garde

#### What Affects This:
- Scale choice (exotic = higher tension)
- Jazz reharmonizer intensity
- Voicing density
- Voice leading smoothness

#### Use For:
- Understanding harmonic texture
- Adjusting complexity intentionally
- Creating dynamic interest

---

### 10. MIDI Channel Input
**Parameter ID**: `midiChannel`  
**Type**: Enumerated (16 options)  
**Default**: All (Omni)  
**Range**: Omni, 1-16

#### Effect:
- Filters which MIDI channel triggers harmonies
- Omni = all channels
- 1-16 = specific channel only

#### Usage:
- Omni (default): Most common
- Channel-specific: Advanced routing scenarios

#### Tips:
- Leave on Omni for normal use
- Use channel filtering for complex arrangements
- Allows different scales on different channels

---

### 11. MIDI Octave Offset
**Parameter ID**: `octaveOffset`  
**Type**: Integer  
**Default**: 0  
**Range**: -2 to +2 octaves

#### Effect:
- Transposes all output notes up/down
- 0 = no transposition
- -1 = one octave lower
- +1 = one octave higher

#### Usage:
- Stack harmonies across multiple instances
- Create more octaves of harmony
- Adjust for instrument tuning

#### Tips:
- Use on parallel instances for layering
- -1: Lower harmonies, +0: Root, +1: Upper harmonies
- Creates 3-octave harmony stack

---

### 12. Preset Quick Access
**Parameter ID**: `presetSelect`  
**Type**: Enumerated (8 slots)  
**Default**: Empty  
**Range**: 8 quick-save slots

#### Effect:
- Instant recall of 8 favorite presets
- Save current configuration to slot
- Load with single click/button press

#### Slots:
- Slot 1-8: Custom presets
- Each saves: Scale, Root, Voicing, Sustain, Effects config
- Instant switching during performance

#### Usage:
- Save your favorite combinations
- Quick switching in live performance
- One-click workflow during composition

#### Tips:
- Slot 1: Default/safe setup
- Slots 2-4: Main variations
- Slots 5-8: Experimental/special effects

---

## 📊 Parameter Summary Table

| # | Parameter | Type | Default | Range | Notes |
|---|-----------|------|---------|-------|-------|
| 1 | Scale Selection | Enum | C Major | 48 scales | Core scale choice |
| 2 | Root Note | Enum | C | C-B | Transposition |
| 3 | Voicing Style | Enum | Balanced | 3 modes | Harmony density |
| 4 | Sustain Time | Linear | 500ms | 0-4000ms | Note decay |
| 5 | Effects Chain | Bool | Off | On/Off | Sound enhancement |
| 6 | Performance Level | Read-Only | N/A | 0-100% | CPU monitor |
| 7 | Jazz Reharmon. | Linear | 0 | 0-100% | Reharmonize intensity |
| 8 | Voice Leading | Linear | 75 | 0-100% | Smoothness |
| 9 | Tension Analysis | Read-Only | N/A | 0-100% | Complexity monitor |
| 10 | MIDI Channel | Enum | Omni | Omni/1-16 | Channel select |
| 11 | Octave Offset | Int | 0 | -2 to +2 | Transposition |
| 12 | Preset Quick Access | Enum | Empty | 8 slots | Preset recall |

---

## 🎯 Common Parameter Combinations

### Jazz Improv Setup
```
Scale: Dorian or Mixolydian
Root: Match song key
Voicing: Balanced
Sustain: 500ms
Jazz Reharmonizer: 50%
Voice Leading: 85%
Effects: Light reverb only
```

### Electronic Production
```
Scale: Minor Pentatonic or Blues
Root: Key of track
Voicing: Spread (for richness)
Sustain: 1000-2000ms
Reharmonizer: 25% (light)
Voice Leading: 50%
Effects: Reverb + Delay
```

### Film Score
```
Scale: Exotic (Arab/Hindu)
Root: Match composition
Voicing: Spread
Sustain: 2000-4000ms
Reharmonizer: 0% (pure scale)
Voice Leading: 100%
Effects: Reverb (high)
```

### Ambient/Pad
```
Scale: Major or Harmonic Minor
Root: Stable root
Voicing: Spread
Sustain: Maximum (4000ms)
Reharmonizer: 0%
Voice Leading: 100%
Effects: Reverb + Delay
```

---

## 🔄 Real-Time Automation

All parameters except read-only displays can be automated in your DAW:

### Safe Automation
- Scale/Root changes: Smooth transition
- Voicing: Instant change (no clicks)
- Sustain: Crossfade for smoothness
- Effects: Real-time change safe

### Automation Tips
- Slow parameter sweeps sound musical
- Quick changes can sound dramatic
- Combine multiple automation lanes
- Record real-time adjustments

### Example Automation Sequences
1. **Buildup**: Gradually increase sustain time
2. **Intensity**: Increase reharmonizer intensity + spread voicing
3. **Release**: Decrease sustain + reduce effects
4. **Key Change**: Instant root note change

---

## 🔍 Troubleshooting Parameters

### Parameter Won't Change
- Check MIDI channel routing (Parameter 10)
- Verify DAW sends automation data
- Check Parameter lock state in DAW

### Sound Suddenly Strange
- Check performance level (may be throttling)
- Verify scale/root selection
- Review voicing and sustain settings

### CPU Spiking
- Reduce voicing style (Spread → Balanced)
- Lower sustain time
- Disable effects chain temporarily
- Reduce voice leading smoothness

### Harmonies Sound Wrong
- Verify root note matches song key
- Try different scales
- Check voice leading smoothness setting
- Test with different MIDI inputs

---

## 💾 Saving Your Settings

### Quick Save (Preset Slots)
- Adjust all parameters to desired state
- Click Preset Slot 1-8 (hold to save)
- Instantly recall anytime

### Full Preset Save
- Use Preset Browser (right panel)
- Click "Save New Preset"
- Name your configuration
- Organize in categories

### Settings Export
- All settings stored in DAW session
- Reopens with same configuration
- Export presets for sharing

---

## ✨ Advanced Techniques

### Stacking Instances
1. Create two ScaleChord instances
2. Set octave offset: -1 and +1
3. Same scale/root on both
4. Get 3-octave harmony spread

### Parallel Processing
1. Create three instances
2. Set to different scales
3. Use DAW mixing to blend
4. Create polyharmonic textures

### Dynamic Scale Switching
1. Automate scale parameter over time
2. Create evolving harmonic landscape
3. Combine with sustain automation
4. Build complex harmonic motion

---

*ScaleChord v1.0.0 - Complete Parameter Reference*  
*All 12 parameters documented*  
*Ready for production use*
