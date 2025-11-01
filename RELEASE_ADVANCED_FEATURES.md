# ScaleChord Advanced Features Guide - v1.0.0

**Master guide for power users and advanced techniques**

---

## 📖 Table of Contents

1. **Jazz Reharmonizer Deep Dive**
2. **Voice Leading Mastery**
3. **Preset Management & Organization**
4. **Performance Dashboard Analysis**
5. **MIDI Routing & Channel Configuration**
6. **Effects Chain Customization**
7. **Real-Time Parameter Automation**
8. **Advanced Workflow Techniques**
9. **Performance Optimization**
10. **Troubleshooting Advanced Issues**

---

## 🎷 Jazz Reharmonizer Deep Dive

The Jazz Reharmonizer is an advanced feature that intelligently reinterprets chord progressions using jazz harmony principles.

### How It Works

**Standard Mode (Reharmonizer Off)**:
```
Input: C note
Scale: C Major
Output: C, E, G, B (standard C major triad + 7th)
```

**Jazz Mode (Reharmonizer On, 50% intensity)**:
```
Input: C note
Scale: C Mixolydian
Output: C, Eb, G, B, D (C13sus b9 jazz reharmonization)
```

### Reharmonization Levels

**0% - Disabled**
- Pure scale voicing
- Harmonic integrity maintained
- Most predictable sound
- Least CPU intensive

**25% - Light Reharmonization**
- Subtle alterations
- Maintains core harmony
- Adds slight complexity
- Almost no CPU impact
- Best for: Subtle enhancement

**50% - Moderate Reharmonization**
- Noticeable jazz harmony
- Recognizable voicing changes
- Rich harmonic texture
- Minimal CPU impact
- Best for: Jazz improvisation

**75% - Strong Reharmonization**
- Significant harmonic transformation
- Complex jazz voicings
- May sound unfamiliar on simple inputs
- Slight CPU increase
- Best for: Advanced jazz

**100% - Aggressive Reharmonization**
- Maximum harmonic reinterpretation
- Complex, avant-garde voicings
- Dramatic sound transformation
- Noticeable CPU load
- Best for: Experimental music

### Jazz Reharmonization Algorithm

The reharmonizer uses:
1. **Scale Context Analysis**: Detects current harmonic mode
2. **Chord Type Detection**: Identifies implied chord function
3. **Voice Leading Optimization**: Maintains smooth transitions
4. **Jazz Extension Addition**: Adds 7ths, 9ths, 11ths, 13ths
5. **Upper Structure Triads**: Complex jazz voicings

### Musical Theory Behind It

**Extension Application**:
- Dominant: Add b9, #9, b13 alterations
- Minor: Add maj7 or natural 9ths
- Major: Add upper extensions
- Diminished: Symmetric structures

**Tension Management**:
- Lower intensity = consonant extensions
- Higher intensity = dissonant alterations
- Always respects scale context
- Maintains voice leading integrity

### Best Scale/Mode Combinations

**Dorian Mode**:
- Natural minor feel with raised 6th
- Perfect for minor jazz
- Works great with reharmonizer
- Try: 50% intensity for best results

**Mixolydian Mode**:
- Dominant 7 character
- Blues-jazz hybrid
- Excellent with reharmonizer
- Try: 25-50% for blues-fusion

**Harmonic Minor**:
- Classical jazz foundation
- Minor with raised 7th
- Advanced harmonic possibilities
- Try: 75% for complex jazz

**Harmonic Major**:
- Sophisticated harmonic flavor
- Major with raised 3rd, 5th
- Film score quality
- Try: 50% for cinematic effect

### Live Jazz Performance Tips

1. **Start Simple**: Begin at 0%, gradually increase
2. **Match Band**: Sync reharmonizer to band's complexity
3. **Real-Time Control**: Automate intensity changes in real-time
4. **Layer Effect**: Stack two instances at different intensities
5. **Combine Scales**: Switch between Dorian/Mixolydian in phrases

### Recording Reharmonized Parts

1. **Record MIDI**: Capture your playing with reharmonizer enabled
2. **Lock Settings**: Save preset of reharmonizer state
3. **Stem Export**: Export harmonies to separate track
4. **Layer Processing**: Blend original + reharmonized versions
5. **Final Mix**: Use subtle blend for natural result

---

## 🎼 Voice Leading Mastery

Voice leading is the art of moving between chords smoothly without large jumps. ScaleChord implements sophisticated voice leading algorithms.

### Voice Leading Fundamentals

**What Is Voice Leading?**
- Smooth movement between harmony notes
- Minimal distance traveled by each voice
- Natural-sounding progressions
- Classical composition technique

**ScaleChord Implementation**:
- Real-time optimization of voice positions
- Algorithm considers multiple voices simultaneously
- Balances smoothness with voice independence
- Prevents parallel voice errors

### Voice Leading Smoothness Setting (0-100%)

**0% - No Optimization**
- Any voicing allowed
- Maximum voice independence
- Can sound jumpy or disjunct
- Highest CPU efficiency
- Best for: Staccato, rhythmic passages

**25% - Light Smoothing**
- Minor voice movement optimization
- Balance between smoothness and freedom
- Good for: Electronic music, modern composition

**50% - Balanced Smoothing**
- Moderate optimization
- Smooth without sounding artificial
- Good for: General use, most styles

**75% - Strong Smoothing** (Default)
- Significant smoothness optimization
- Natural, musicianly progressions
- Good for: Jazz, classical, film score

**100% - Maximum Smoothing**
- Aggressive voice leading optimization
- Most smooth possible movement
- Can reduce voice independence slightly
- Good for: Complex chords, dense harmony

### Voice Leading Algorithm Details

**Five-Part Process**:
1. **Melodic Outline**: Main melody preserved
2. **Chord Spacing**: Optimal octave positioning
3. **Voice Distribution**: Balanced across range
4. **Smoothness Calculation**: Minimal total movement
5. **Voice Independence**: Maintains individuality

**Constraints Applied**:
- No parallel perfect intervals (classical)
- Voice crossing allowed but minimized
- Outer voices most important
- Inner voices flexible
- Range maintained throughout

### Practical Voice Leading Examples

**Example 1: C Major → G Major**

Without voice leading:
```
C-E-G-C → G-B-D-G   (large jumps)
```

With 75% voice leading:
```
C-E-G-C → G-E-B-D   (smooth movement)
```

**Example 2: Jazz ii-V-I Progression**

Standard voicing:
```
Dmi7 → G7alt → CMaj7
Large voice jumps, disjunct
```

With voice leading at 100%:
```
Dmi7 → G7alt → CMaj7
Smooth voice movement throughout
```

### Combining with Jazz Reharmonizer

**Powerful Combination**:
1. Set voice leading to 85%
2. Set reharmonizer to 50%
3. Use Dorian/Mixolydian scale
4. Play smooth progressions
5. Get sophisticated jazz result

**Result**: Complex yet smooth, complex yet playable

### Tips for Best Voice Leading

1. **Match Scale**: Voice leading works best with coherent scales
2. **Consistent Root**: Keep root note stable
3. **Smooth Playing**: Smooth input = smooth output
4. **Avoid Extreme Jumps**: Large input jumps override voice leading
5. **Combine with Sustain**: Sustain time interacts with perception

---

## 💾 Preset Management & Organization

### Understanding Presets

A preset captures:
- Scale selection
- Root note
- Voicing style
- Sustain time
- Effects chain status
- All effect parameters
- Jazz reharmonizer intensity
- Voice leading smoothness

### Preset Browser Interface

Located in right panel of ScaleChord UI:

```
┌─────────────────────────┐
│ PRESET BROWSER          │
├─────────────────────────┤
│ Category: [▼ Jazz]      │
├─────────────────────────┤
│ [Dorian Blues]          │
│ [Mixo Fusion]           │
│ [Minor Reharmon]        │
├─────────────────────────┤
│ [Load] [Save] [Delete]  │
└─────────────────────────┘
```

### Organizing Presets by Category

**Recommended Categories**:

1. **Jazz**
   - Dorian Walking Bass
   - Mixolydian Blues
   - Harmonic Minor Altered
   - Reharmonization Intense

2. **Film Score**
   - Ambient Pad
   - Dramatic Sweep
   - Exotic Texture
   - Cinematic Reverb

3. **Electronic**
   - Synth Lead
   - Bass Layer
   - Pad Synth
   - Glitch Effect

4. **World Music**
   - Arabic Exotic
   - Japanese Pentatonic
   - Hindu Raga
   - Gypsy Minor

5. **Experimental**
   - Polyharmonic
   - Microtonal
   - Atonal Texture
   - Generative

### Creating Effective Presets

**Step 1: Define Purpose**
- What musical context?
- What emotional quality?
- What is it combined with?

**Step 2: Set Parameters**
- Choose appropriate scale
- Find optimal voicing
- Set sustain for context
- Adjust effects balance

**Step 3: Test Thoroughly**
- Try full playing range
- Test with various note velocities
- Verify CPU usage
- Check harmonic balance

**Step 4: Name Descriptively**
- Descriptive name: "Dorian Jazz Trio"
- Include scale if obvious: "C Mixo Blues"
- Include intensity if complex: "Reharmon 75%"
- Include intended DAW: "Ableton Pad"

### Preset Naming Conventions

**Format**: [Emotion/Style] [Scale] [Effect]

Examples:
- "Smooth Dorian Clean"
- "Bright Mixo With Reverb"
- "Dark Minor Phrygian Ambient"
- "Aggressive Blues Distorted"
- "Subtle Exotic Arab Light"

### Quick Save/Load Slots

Located below Preset Browser:

```
┌─ QUICK ACCESS ─────┐
│ [1] [2] [3] [4]    │
│ [5] [6] [7] [8]    │
└────────────────────┘
```

**Usage**:
- Click to load stored preset
- Hold-click to save current state
- 8 slots for instant access
- Typical allocation:
  - Slot 1: Default/Safe
  - Slots 2-4: Main variations
  - Slots 5-8: Experimental

### Exporting Presets

Preset files stored as:
```
~/.scalechord/presets/category/name.preset
```

**To Share Presets**:
1. Export preset file from browser
2. Send to another user
3. They import into preset browser
4. Settings automatically load

**Best Practice**:
- Document preset contents
- Include recommended DAWs
- Note CPU requirements
- Suggest complementary effects

---

## 📊 Performance Dashboard Analysis

The Performance Dashboard provides real-time metrics about your plugin's operation.

### Dashboard Sections

**Left Section: CPU Metrics**
```
┌─────────────────────┐
│ CPU: 3.2%           │
│ ▓░░░░░░░░░░░░░░░░░ │
│ Peak: 5.1%          │
│ Avg: 2.8%           │
└─────────────────────┘
```

- Current CPU load (instantaneous)
- Visual bar gauge (0-100%)
- Peak value in current session
- Average over last 10 seconds

**Middle Section: Voice Metrics**
```
┌─────────────────────┐
│ Voices: 12/128      │
│ Active: 4           │
│ Sustain: 3          │
│ Tension: 42%        │
└─────────────────────┘
```

- Total voices used / maximum available
- Currently active (sounding) voices
- Voices in sustain/release
- Harmonic complexity (0-100%)

**Right Section: Memory & Health**
```
┌─────────────────────┐
│ Memory: 47MB        │
│ Health: ✓ GOOD      │
│ Status: PLAYING     │
│ Issues: None        │
└─────────────────────┘
```

- RAM consumed by this instance
- Overall system health indicator
- Current operation status
- Any warnings or issues

### Interpreting CPU Meters

**Green Zone (0-20%)**
- Excellent performance
- Room for additional effects
- Safe for layering multiple instances
- Normal operation

**Yellow Zone (20-50%)**
- Good performance
- Acceptable for most scenarios
- Limit additional effects
- Monitor carefully

**Orange Zone (50-75%)**
- Heavy processing
- Reduce complexity immediately
- Single instance only
- Limited headroom

**Red Zone (75-100%)**
- Overloaded
- Turn off effects or reduce voicing
- Switch to Compact voicing
- Emergency action required

### Tension Level Interpretation

**Low (0-30%)**
- Simple, stable harmony
- Consonant sounds
- Predictable progression
- Good for: Lullabies, ambient

**Medium (30-70%)**
- Rich, active harmony
- Mix of consonant/dissonant
- Engaging sound
- Good for: Most music styles

**High (70-100%)**
- Complex, dissonant harmony
- Advanced harmonic language
- Challenging sound
- Good for: Jazz, experimental

### Real-Time Monitoring Tips

1. **Watch During Recording**: Monitor CPU during performance
2. **Check Peak Values**: Identify problematic sections
3. **Assess Tension**: Confirm harmonic intention
4. **Verify Voice Count**: Ensure not exceeding MIDI limits
5. **Track Memory**: Watch for gradual leaks

### Performance Optimization Using Dashboard

If CPU is too high:
1. Check voicing style (switch Spread → Balanced)
2. Reduce sustain time (lowers voice count)
3. Disable effects temporarily
4. Reduce jazz reharmonizer intensity
5. Switch to lighter voice leading (lower %)

---

## 🎛️ MIDI Routing & Channel Configuration

### MIDI Channel Routing

**MIDI Channel Parameter**: Controls which MIDI channels trigger harmonies

**Options**:
- **Omni** (Default): All channels 1-16
- **Channel 1-16**: Specific single channel

### Advanced Routing Scenarios

**Scenario 1: Multi-Key Improvisation**
```
DAW Track 1: ScaleChord on Channel 1 (C Minor)
DAW Track 2: ScaleChord on Channel 2 (G Major)
DAW Track 3: ScaleChord on Channel 3 (Bb Major)

MIDI Keyboard sends:
- Left hand → Channel 1 (C Minor harmony)
- Middle → Channel 2 (G Major harmony)
- Right hand → Channel 3 (Bb Major harmony)
```

**Scenario 2: Layered Instances**
```
Instance 1: Channel Omni (All input routed)
Instance 2: Channel Omni (All input routed)
Settings: Different scales, both process same MIDI
Result: Harmonic polytonality
```

**Scenario 3: Drum-Triggered Harmony**
```
MIDI Drums: Kick drum notes → Channel 4
MIDI Melody: Melody notes → Channel 1
ScaleChord 1: Listen Channel 1 (melody harmony)
ScaleChord 2: Listen Channel 4 (drum sync effect)
Result: Dynamic rhythm-synced harmonies
```

### Octave Offset for Range Control

**Parameter**: Octave Offset (-2 to +2)

**Application**: Transpose all output harmonies

**Stacking Example**:
```
Instance 1: Octave Offset -1 (lower octave)
Instance 2: Octave Offset 0 (middle)
Instance 3: Octave Offset +1 (upper octave)

All three listen to same MIDI input
Result: 3-octave harmony spread across keyboard
```

### MIDI Velocity Sensitivity

**Current Implementation**:
- Dynamics affect envelope (Phase 6)
- Velocity shapes attack/release
- Higher velocity = faster attack
- Lower velocity = slower attack

**Creative Use**:
- Soft touch: Smooth, sustaining harmony
- Hard hit: Sharp, punchy harmony
- Velocity expression: Dynamic playing

---

## 🎛️ Effects Chain Customization

ScaleChord includes 6 effects: Reverb, Delay, Chorus, Distortion, EQ, Compression

### Effects Chain Overview

```
Signal Path:
Input MIDI → Harmonizer → Reverb → Delay → Chorus → Distortion → EQ → Compression → Output
```

### Individual Effect Deep Dive

#### 1. Reverb (Spatial Effect)

**Parameters**:
- Room Size: 0-100 (small room to cathedral)
- Wet/Dry Mix: 0-100% (100% = pure reverb)
- Decay Time: 0.2-5.0 seconds

**Usage Tips**:
- Small rooms (20-40%): Jazz, bright sound
- Medium rooms (40-70%): General use
- Large spaces (70-100%): Film score, ambient
- Default: 50% room, 30% wet

**Best With**:
- Long sustain times (works with pad)
- Slow music (reverb decay time)
- Jazz (natural room simulation)

#### 2. Delay (Rhythmic Effect)

**Parameters**:
- Delay Time: 62.5ms to 4 seconds
- Feedback: 0-80% (0% = one repeat)
- Wet/Dry Mix: 0-100%
- Sync to Tempo: On/Off

**Usage Tips**:
- Quarter-note sync: Rhythmic delay (typical)
- Eighth-note: Busier rhythm
- Custom timing: Unique pocket
- Feedback 30-50%: Natural echo
- Feedback 60%+: Ambient texture

**Best With**:
- Moderate sustain (250-1000ms)
- Faster tempos (synced delays sound better)
- Electronic music (rhythmic precision)
- Ambient music (high feedback, long decay)

**Tempo Sync**:
If enabled, delay locks to DAW tempo:
- 1/4 = quarter note
- 1/8 = eighth note
- 1/16 = sixteenth note
- 1/3 = triplet eighth

#### 3. Chorus (Modulation Effect)

**Parameters**:
- Depth: 0-100% (modulation amount)
- Rate: 0.1-10 Hz (modulation speed)
- Mix: 0-100% wet

**Usage Tips**:
- Subtle (depth 20-30%, rate 0.5Hz): Chorus ensemble
- Moderate (depth 40-50%, rate 1-2Hz): Lush width
- Heavy (depth 70%+, rate 3-5Hz): Vibe effect
- Default: 40% depth, 1.5 Hz rate

**Best With**:
- Lead voices (adds width)
- Pad sounds (enhancement)
- Medium-long sustain times
- Slow to moderate tempo

**Avoids**:
- Very short notes (doesn't have time to work)
- Very fast tempos (becomes fuzzy)
- Already stereo sources

#### 4. Distortion (Harmonic Saturation)

**Parameters**:
- Drive: 0-100% (saturation intensity)
- Tone: 0-100% (dark to bright)
- Mix: 0-100% wet

**Usage Tips**:
- Subtle (Drive 10-20%): Tape warmth
- Moderate (Drive 30-50%): Grit and edge
- Heavy (Drive 75%+): Aggressive sound
- Dark tone: Mellow saturation
- Bright tone: Crisp overtones

**Best With**:
- Lead sounds (adds character)
- Bass notes (adds punch)
- Rock/metal styles
- Short to medium sustain

**Creative Ideas**:
- Combine with reverb: Ambient distortion
- Combine with delay: Echoing distortion
- Automate drive: Dynamic effect

#### 5. EQ (3-Band Equalizer)

**Parameters**:
- Low Freq: 0-100% (50Hz-200Hz region)
- Mid Freq: 0-100% (500Hz-2kHz region)
- High Freq: 0-100% (8kHz-20kHz region)

**Usage Tips**:
- Boost low: Deeper, darker sound
- Boost mid: Warmth, presence
- Boost high: Clarity, brightness
- Cut (negative): Remove frequency range

**EQ Presets**:
- Bright: Low 40%, Mid 60%, High 80%
- Dark: Low 70%, Mid 40%, High 20%
- Warm: Low 60%, Mid 70%, High 40%
- Thin: Low 20%, Mid 40%, High 60%

**Best With**:
- All sounds (EQ is foundational)
- Problem frequencies (surgical cuts)
- Matching tone to DAW mix
- Compensating for room acoustics

#### 6. Compression (Dynamic Control)

**Parameters**:
- Ratio: 1:1 to 4:1 (compression amount)
- Threshold: 0-100% (compression start point)
- Release: 0.1-1.0 seconds

**Usage Tips**:
- Gentle (2:1 ratio, high threshold): Transparent
- Moderate (3:1, mid threshold): Smooth control
- Heavy (4:1, low threshold): Obvious effect
- Fast release: Punchiness
- Slow release: Smoothness

**Compression Presets**:
- Transparent: 1.5:1 ratio, high threshold, fast release
- Smooth: 2:1 ratio, mid threshold, medium release
- Glue: 3:1 ratio, mid threshold, slow release
- Aggressive: 4:1 ratio, low threshold, medium release

**Best With**:
- Controlling dynamic range
- Creating cohesion in mix
- Adding presence to notes
- Preventing clipping on peaks

### Effects Chain Workflow

**For Jazz Harmony**:
1. Reverb: 40% wet, medium room
2. Compression: 2:1, smooth glue
3. EQ: Warm preset
4. Disable: Delay, Chorus, Distortion

**For Ambient**:
1. Reverb: 60% wet, large room
2. Delay: Quarter-note sync, 40% feedback
3. Chorus: 50% depth, slow rate
4. Compression: Transparent, gentle
5. Disable: Distortion

**For Electronic**:
1. Delay: Synced to tempo
2. Distortion: Subtle drive for edge
3. EQ: Bright preset
4. Compression: Glue preset
5. Disable: Reverb, Chorus (add separately)

**For Film Score**:
1. Reverb: Large space, 50% wet
2. Delay: Quarter-note, 30% feedback
3. Compression: Smooth
4. EQ: Warm preset
5. Disable: Distortion, Chorus

---

## ⏱️ Real-Time Parameter Automation

### DAW Automation Basics

ScaleChord parameters can be automated in your DAW using APVTS (Audio Processor Value Tree State).

**Which Parameters Are Automatable**:
- ✅ Scale Selection
- ✅ Root Note
- ✅ Voicing Style
- ✅ Sustain Time
- ✅ All effect parameters
- ✅ Jazz Reharmonizer Intensity
- ✅ Voice Leading Smoothness
- ❌ Read-only (Performance, Tension, Preset Select)

### Automation Recording in DAW

**Ableton Live**:
1. Right-click parameter in ScaleChord UI
2. Select "Map to Macro/Automate"
3. Create automation envelope on track
4. Draw or record changes

**Logic Pro**:
1. Enable automation on track
2. Select ScaleChord from plugin list
3. Choose parameter to automate
4. Draw automation curve

**Reaper**:
1. Right-click track
2. Select "Add FX parameter to track"
3. Create automation envelope
4. Draw parameter changes

### Automatable Parameter Ideas

**Scale Switching Automation**:
- Create progression: Verse (Dorian) → Chorus (Major) → Bridge (Phrygian)
- Automate scale change at measure boundaries
- Use for key modulations

**Sustain Time Evolution**:
- Intro: Short sustain (500ms - staccato)
- Verse: Medium sustain (1000ms - smooth)
- Chorus: Long sustain (2000ms - pad-like)
- Outro: Fade sustain back to short

**Intensity Build-Up**:
- Start: Reharmonizer 0%, Light voice leading (25%)
- Middle: Reharmonizer 50%, Medium voice leading (75%)
- Peak: Reharmonizer 100%, Full voice leading (100%)
- Resolution: Fade back down

**Effect Envelope**:
- Intro: Reverb only (dry lead)
- Build: Add delay, increase reverb
- Peak: All effects engaged
- Outro: Reverb only, fade out

### Advanced Automation Techniques

**1. Parallel Automation**
```
Track 1: Scale = Dorian (static)
Track 2: Scale = Mixolydian (static)
Both Automated Root Note: C → D → E → F (same timing)
Result: Harmonic progression across both instances
```

**2. Offset Automation**
```
Instance 1: Jazz Reharmonizer at 0%
Instance 2: Jazz Reharmonizer at 50% (offset by 50% of Instance 1)
Result: Gradual intensity change
```

**3. Inverse Automation**
```
Sustain Time: 500ms → 2500ms (increases)
Reverb Wet: 70% → 20% (decreases)
Effect: Paradoxical texture evolution
```

### Automation Shapes & Curves

**Linear Ramp**:
- Smooth, even change over time
- Most musical for most parameters
- Use for: Steady parameter evolution

**Exponential Curve**:
- Fast start, slow end
- Use for: Dramatic effect entries
- Example: Effect intensity suddenly kicks in

**Logarithmic Curve**:
- Slow start, fast end
- Use for: Smooth exits
- Example: Effect fade-out at song end

**Step Function**:
- Sudden, no-interpolation changes
- Use for: Immediate scale/root changes
- Example: Verse to chorus transition

### Recording Live Automation

**Process**:
1. Enable automation recording on track
2. Play song while adjusting parameters in real-time
3. DAW records all movements
4. Edit/refine automation afterward
5. Lock automation for consistency

**Best For**:
- Expressive, natural changes
- One-off special moments
- Capturing inspired playing
- Adding human feel

---

## 🎯 Advanced Workflow Techniques

### Technique 1: Chord Progression Generation

**Goal**: Create a new chord progression using scale detection

**Process**:
1. Choose a scale (e.g., C Dorian)
2. Play a melody: C-D-E-C-Bb (5 notes)
3. ScaleChord generates harmony for each note
4. Record the output MIDI
5. Edit/refine the harmony MIDI
6. Layer under original melody

**Result**: Instant harmonic background

### Technique 2: Polyharmonic Textural Layer

**Goal**: Create complex polyharmonic texture (multiple scales at once)

**Process**:
1. Create 3 instances of ScaleChord
2. Instance 1: C Minor scale
3. Instance 2: C Phrygian scale (offset +1 octave)
4. Instance 3: C Blues scale (offset -1 octave)
5. Send same MIDI to all 3
6. Blend outputs using DAW mixer

**Result**: Complex, layered harmonic texture

### Technique 3: Harmonic Transition Effect

**Goal**: Smooth key change using automation

**Process**:
1. Automate root note change over 4 bars
2. Example: C → D → E → F (quarter note transition)
3. Smooth curve (not linear) for musicality
4. Combine with reverb + delay for effect
5. Peak effect at new key arrival

**Result**: Psychological key modulation

### Technique 4: Generative Harmony Based on MIDI Input

**Goal**: Create constantly evolving harmony

**Process**:
1. Enable Jazz Reharmonizer (50-75% intensity)
2. Set voice leading to 100%
3. Use exotic scale (Arab, Hindu)
4. Play steady, slow notes
5. Harmony constantly evolves
6. Record result as MIDI

**Result**: AI-like harmonic evolution

### Technique 5: Stacked Instance Arpeggiation

**Goal**: Create arpeggio effect across multiple instances

**Process**:
1. 4 instances, each octave offset: -2, -1, 0, +1
2. Each instance different delay time (1/4, 1/8, 1/16, 1/32)
3. Same input MIDI to all
4. Each delays and spaces harmonies differently
5. Blend with reverb/chorus

**Result**: Orchestral arpeggio effect

---

## ⚡ Performance Optimization

### CPU Reduction Strategies

**Strategy 1: Voicing Simplification**
- Spread → Balanced: ~20% CPU reduction
- Balanced → Compact: ~15% CPU reduction
- Total possible: ~35% reduction

**Strategy 2: Sustain Reduction**
- Each 1000ms sustain ≈ 5% CPU increase
- Cutting sustain by half ≈ 10% reduction
- Minimum sustain (0ms) minimum CPU

**Strategy 3: Effects Disabling**
- Disable all effects: ~10-15% reduction
- Each effect: ~2-3% reduction
- Reverb: ~3% (most expensive)
- Compression: ~1% (least expensive)

**Strategy 4: Voice Leading Reduction**
- 100% → 75%: ~5% reduction
- 75% → 50%: ~5% reduction
- 50% → 0%: ~10% reduction
- Total possible: ~20% reduction

**Strategy 5: Jazz Reharmonizer Reduction**
- 100% → 50%: ~10% reduction
- 50% → 0%: ~10% reduction
- Total possible: ~20% reduction
- Most expensive advanced feature

### Memory Optimization

**Current Memory**: ~50-100MB per instance

**Reduction Techniques**:
1. Lower sustain time (fewer sustained notes)
2. Reduce polyphony where possible
3. Disable unused effects
4. Single instance vs. multiple layers

**Memory Monitoring**:
- Dashboard shows real-time memory
- Watch for gradual increases (leaks)
- Report if memory grows without bound

### Real-Time Safety

**Always Safe (Zero Allocations)**:
- Parameter changes
- MIDI note input/output
- All rendering/processing
- Voice allocation

**Never Allocate**:
- Effect processing
- Envelope generation
- Voice leading calculation
- Reharmonization

**Result**: Guaranteed real-time safe operation

---

## 🐛 Troubleshooting Advanced Issues

### Issue: Harmonies Sound Wrong

**Diagnosis**:
1. Verify scale is correct for song key
2. Check root note matches song key
3. Try different voicing styles
4. Review voice leading settings

**Solutions**:
- Scale wrong: Change scale selection
- Root wrong: Correct root note
- Voicing bad: Try Balanced voicing
- Too smooth: Reduce voice leading %

### Issue: CPU Spiking Randomly

**Diagnosis**:
- Monitor CPU in dashboard
- Check which parameter changed
- Note the scale/voicing combination
- Look for automation curves

**Solutions**:
- Reduce sustain time
- Switch to Compact voicing
- Disable Jazz Reharmonizer
- Disable effects
- Reduce voice leading smoothness

### Issue: Harmonies Not Triggering

**Diagnosis**:
- Check MIDI channel routing
- Verify MIDI input reaches plugin
- Check octave offset is accessible
- Verify not in Preset Select mode

**Solutions**:
- Set MIDI channel to Omni
- Test with different keyboard
- Reset octave offset to 0
- Check DAW MIDI monitor

### Issue: Plugin Crashes on Certain Notes

**Diagnosis**:
- Identify problem note(s)
- Check scale at that note
- Look for edge case issues
- Monitor CPU during crash

**Solutions**:
- Try different scale
- Change voicing
- Update plugin to latest version
- Report as bug if reproducible

### Issue: Effects Sound Distorted

**Diagnosis**:
- Check effect settings
- Look for distortion parameter
- Check compression settings
- Verify not intentional

**Solutions**:
- Reduce effect intensity
- Lower input level to effect
- Check mix levels
- Disable problematic effect

---

## 📚 Additional Resources

### Theory Deep Dives
- Jazz Harmony course (recommended)
- Voice Leading exercises
- Scale studies
- Chord progression analysis

### DAW-Specific Guides
- Ableton Live automation documentation
- Logic Pro plugin guide
- Reaper VST documentation

### Music Concepts
- Harmonic analysis
- Chord extensions
- Voice leading principles
- Jazz improvisation

---

*ScaleChord v1.0.0 - Advanced Features Guide*  
*Master all power-user techniques*  
*Production-ready documentation*
