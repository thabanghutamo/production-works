# MIDI Routing & DAW Integration Guide

This guide shows how to use ScaleChord in your DAW (FL Studio, Ableton, Logic, etc.) once you've built the VST3 plugin.

## 🎯 Overview

ScaleChord sits between your MIDI controller/keyboard and your synth plugin. It transforms incoming notes into scale-quantized chords in real time.

```
MIDI Input
    ↓
[ScaleChord Plugin]
    ↓
MIDI Output (sent to synth)
    ↓
Synth Plugin (e.g., Serum, Wavetable, Sylenth1)
    ↓
Audio Output
```

## 📌 Setup Steps (Generic DAW)

### Step 1: Add ScaleChord to Your Track

1. Create a new **instrument track** or **MIDI track**
2. Insert **ScaleChord** as the first plugin on the track
3. Do **NOT** disable "MIDI Thru" or "Sidechain" (we need MIDI to flow through)

### Step 2: Configure ScaleChord

In the ScaleChord plugin window:
- **Root Note**: Select your musical key (e.g., C, D, Em)
- **Scale**: Choose Major, Minor, Pentatonic, Dorian, etc.
- **Voicing**: Triad, Seventh, or Open
- **Attack/Decay/Sustain/Release**: Set your envelope (or leave defaults)
- **Output Channel**: Important! See below.

### Step 3: Route Output to Synth

ScaleChord outputs MIDI on a configurable channel. Your synth must listen on that channel.

#### Option A: Same Track (Easiest)
- Insert a **synth plugin** after ScaleChord
- Set ScaleChord **Output Channel** to 1 (or any single channel)
- The synth will automatically receive the chord MIDI from ScaleChord

#### Option B: Separate Synth Track (For Mixing Flexibility)
- Create a second track with your synth plugin
- In ScaleChord, set **Output Channel** to a unique number (e.g., 2)
- In the synth track, set its **MIDI Input Channel** to match (e.g., 2)
- Route ScaleChord's MIDI output to the synth track (DAW-specific)

### Step 4: Arm & Test

1. Arm the ScaleChord track for MIDI input
2. Connect your MIDI controller/keyboard
3. Play a note on your keyboard
4. You should hear a chord from the synth

## 🎛️ DAW-Specific Instructions

### FL Studio

1. **Channel Mixer**: Create a new mixer track
2. **Insert ScaleChord** as an instrument
3. Set **ScaleChord Output Channel** to 1
4. Insert a **Synth** (e.g., 3x Osc, Sytrus) *on the same track* after ScaleChord
5. **Key Binding**: Assign your MIDI keyboard to send to the ScaleChord track
6. Play notes → Synth receives chord MIDI

**Tip**: Use FL Studio's **Arpeggiator** plugin after the synth to further tweak the chord rhythms.

### Ableton Live

1. Create a new **MIDI Track**
2. Drop ScaleChord into the **MIDI Effect Rack**
3. Add a **Synth** (Wavetable, Operator, Sampler) to the same track
4. Set **Monitor** to "In" so MIDI flows through
5. Set ScaleChord **Output Channel** to 1
6. Arm the track and play your keyboard

**Tip**: Save your config as an **Ableton Preset** (`.adv` file) for quick recall.

### Logic Pro

1. Create a new **Software Instrument Track**
2. Load ScaleChord as an **Audio Unit (AU)** effect (post-instrument if possible, or use MIDI Effects)
3. Insert your **Synth** (EXS24, Sculpture, Komplete Instruments)
4. Configure MIDI routing: Keyboard → ScaleChord → Synth
5. Set ScaleChord **Output Channel** to match the synth's input

**Tip**: Use Logic's **Environment** window for advanced MIDI routing.

### Cubase / Nuendo

1. Create an **Instrument Track** with **Scalehorb** as a MIDI insert
2. Add your **Synth** below it (VST Instrument or rack)
3. Enable **MIDI Thru**
4. Set ScaleChord **Output Channel** accordingly
5. Route MIDI from keyboard to the track

**Tip**: Use Cubase's **MIDI Monitor** to visualize note mapping in real time.

### Studio One

1. Insert a new **Instrument** track
2. Load ScaleChord as a **Realtime MIDI Plugin** (Studio One terminology)
3. Add a **VST Synth** on the same track
4. Set **MIDI Thru** enabled
5. Route keyboard MIDI to the track

### Reaper

1. Insert a new **MIDI Track**
2. Load ScaleChord as a **JSFX** or **VST MIDI Plugin** (Route mode: "Thru")
3. Insert a **ReaSampler**, **Synth1**, or any VSTi on the same track
4. Enable **MIDI receives from** → set track
5. Arm & play

## ⚙️ Parameter Reference

Once ScaleChord is loaded, you'll see these parameters:

| Parameter | Range | Notes |
|-----------|-------|-------|
| **Root Note** | C–B | Musical key (C, C#, D, ..., B) |
| **Scale** | Dropdown | Major, Minor, Pentatonic, Dorian, Mixolydian |
| **Voicing** | Dropdown | Triad, Seventh, Open |
| **Attack** | 0–1000 ms | How fast envelope starts |
| **Decay** | 0–1000 ms | How fast it drops from peak |
| **Sustain** | 0–100% | Held level after decay |
| **Release** | 0–1000 ms | How long sound decays after note-off |
| **Velocity Scale** | 0–100% | Incoming velocity magnitude (0% = constant 64, 100% = full range) |
| **Output Channel** | 1–16 | Which MIDI channel to send chords on |
| **Octave Offset** | –2 to +2 | Shift voicing up/down octaves |

## 🎹 Playing Tips

### Live Performance

1. **Pre-Program Your Presets**
   - Set Key, Scale, Voicing before the show
   - Save them in your DAW as snapshots or presets
   - Trigger them via MIDI foot pedal or hotkey

2. **One-Handed Playing**
   - Left hand: play single notes on keyboard
   - ScaleChord generates chords
   - Right hand: free for other controls (effects, volume)

3. **Smooth Transitions**
   - Use longer Release times (300–500 ms) for legato feel
   - Use shorter Release (50–100 ms) for punchy, percussive chords

### Music Production

1. **Quick Composition**
   - Set Major scale with Seventh voicing
   - Play any melody; chords are auto-generated
   - Export MIDI or record audio

2. **Layering**
   - Create multiple ScaleChord instances on different tracks
   - Each with different scales or voicings
   - Synth 1: Triads (synth pad)
   - Synth 2: Sevenths (strings)
   - Synth 3: Open voicing (sparse strings)
   - Blend them for rich, complex harmonies

3. **Chord Lock**
   - Some DAWs allow you to "freeze" or "lock" MIDI output
   - Once locked, further tune or re-arrange manually

## 🐛 Troubleshooting

### Q: I'm not hearing any sound
- [ ] Is the synth plugin volume up?
- [ ] Is MIDI monitor showing incoming notes?
- [ ] Is **Monitor** set to "In" or "Auto" in your DAW?
- [ ] Check MIDI output channel in ScaleChord matches synth input

### Q: Chords sound wrong
- [ ] Verify you selected the correct **Root Note**
- [ ] Try a different **Scale** (e.g., Major vs. Minor)
- [ ] Check **Octave Offset**—try –1 or +1
- [ ] Make sure synth is on the right MIDI channel

### Q: Latency/delay between pressing and hearing
- [ ] Increase DAW buffer size (latency trade-off)
- [ ] Check if synth has heavy reverb/delay
- [ ] ScaleChord itself is <1 ms; latency likely from synth/OS

### Q: Plugin won't load in my DAW
- [ ] Verify you built ScaleChord as **VST3** (not just library)
- [ ] Copy .vst3 file to DAW's plugin folder:
  - Windows: `C:\Program Files\Common Files\VST3\`
  - Mac: `/Library/Audio/Plug-Ins/VST3/`
  - Linux: `~/.vst3/`
- [ ] Rescan plugins in DAW preferences
- [ ] Check DAW is looking for 64-bit plugins (if on 64-bit OS)

## 📖 Examples

### Example 1: Jam Session (Live Performance)

```
Key:     D Major
Scale:   Major
Voicing: Triad
Output:  Serum (on same track)

You play: D E F# G A B C#
Serum hears: D-F#-A, E-G#-B, F#-A-C#, G-B-D, ...
→ Instant D Major jam progression!
```

### Example 2: Deep House Production

```
Key:     A Minor
Scale:   Minor Pentatonic
Voicing: Open (spread)
Output:  Xfer Wavetable

You play: A C E G
Wavetable hears: A-C-E (octave spread), C-E-G (octave spread), ...
→ Rich, spacious minor chord progressions for production
```

### Example 3: Jazz Improv

```
Key:     F Major
Scale:   Dorian
Voicing: Seventh
Output:  Native Instruments Komplete Instruments

You play: F G A B C D E
Komplete hears: F-A-C-E, G-B-D-F, A-C-E-G, ... (7th voicings)
→ Jazz-appropriate quartal harmonies, ready to layer over bass
```

## 🎓 Advanced: MIDI Automation

In most DAWs, ScaleChord's parameters can be automated:

1. Record parameter changes over time
2. DAW records the parameter movement
3. On playback, parameters smoothly interpolate

**Example**: Automate **Scale** from Major → Minor at bar 16 for emotional shift

---

**Next**: Try it! Load ScaleChord in your DAW, pick a scale, and play. Enjoy effortless harmonies.
