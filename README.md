# Level Complete Stats Card

A Geode mod for Geometry Dash. After you beat a level, it pops up a small
card — level name, attempts, time spent, a fun letter grade, and the date —
with a button to save it as a PNG so it's easy to drop into Discord.

Pure local data. No network calls, no backend.

## What's here

```
StatsCard/
├── mod.json              # Geode mod manifest + settings
├── CMakeLists.txt        # standard Geode build file
├── src/
│   ├── main.cpp           # hooks PlayLayer, times attempts, fires the card
│   ├── StatsCardPopup.hpp
│   └── StatsCardPopup.cpp # builds the card UI + PNG export
└── resources/             # empty for now — v1 only uses built-in GD assets
```

## Building it

You'll need:
- [Geode CLI](https://docs.geode-sdk.org/getting-started/) installed and on your PATH
- The Geode SDK checked out somewhere, with the `GEODE_SDK` environment
  variable pointing at it
- A C++ toolchain matching your platform (MSVC on Windows, Clang on macOS)

Then, from inside this folder:

```bash
geode build
```

To build **and** install straight into your GD mods folder for testing:

```bash
geode build --install
```

## Things worth double-checking before you build

I wrote this against my general knowledge of the Geode SDK's API shape, but
Geode's bindings shift between SDK/GD versions, so before it'll compile
clean you'll likely want to check:

- **`mod.json` → `gd` versions** — set these to whatever GD version you're
  actually targeting (`2.2074` is a placeholder).
- **`GJGameLevel` field names** — `m_levelName`, `m_creatorName`,
  `m_attempts` are the ones I expect to exist, but confirm against the
  bindings your installed SDK ships (there's a searchable list at
  `docs.geode-sdk.org` / the Geode Discord's `#dev-help`).
- **`CCRenderTexture::saveToFile`** — the PNG export in `onSaveImage`. This
  API is stable across most cocos2d-x forks but signatures do vary slightly.

None of this is unusual for a first Geode mod — the SDK docs and Discord
are quick to get unstuck in if the compiler flags a renamed member.

## Publishing to Vibemodded

Once it builds, `geode build` will spit out a `.geode` file. From there:

1. Push this repo to GitHub under `YallCode` (or `reyecode`, whichever you
   want as the canonical source)
2. Create a GitHub Release and attach the `.geode` file
3. Add an entry to your `Vibe-Index` repo pointing at that release download

Happy to help set up the `index.json` entry for Vibe-Index once you've got
a build — just say the word.
