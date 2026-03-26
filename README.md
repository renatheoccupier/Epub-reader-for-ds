# IkuReader 6.5 Modern EPUB Mod

This project is an EPUB-focused Nintendo DS/DSi build of IkuReader.

Compared with the older codebase, this version is trimmed to EPUB reading and keeps image rendering inside EPUB files.

---

## Features

- Reads `.epub` books on Nintendo DS and DSi
- Renders JPEG images embedded in EPUB files
- File browser with cover-style preview
- Adjustable font, size, line gap, indent, theme, gamma, layout, and screen usage
- Bookmarks, table of contents, and in-book search

> [!WARNING]
> I don't test it directly in DS (cause i don't have it) so i may be lagging a little or even crash (i hope not)

## What Was Optimized Here

This repo now includes a few performance-focused changes:

- File browser previews are delayed slightly instead of decoding immediately on every cursor move
- Recent browser previews are cached, so revisiting the same book is faster
- EPUB image decoding now reuses the opened archive and caches recent decoded images
- Re-opening pages after some settings changes is smoother, especially in image-heavy books

## Quick Start

1. Build the project with `make`.
2. Copy `sandbox/data/ikureader` to `/data/ikureader` on your flashcart or SD card.
3. Copy the generated ROM from `sandbox/`:
   - `IkuReader-6.5_modern.nds` for DS flashcarts
   - `IkuReader-6.5_modern.dsi` for DSi-style setups that use `.dsi`
4. Put your books anywhere you like. `/books/` is convenient and is checked first by the browser.
5. Launch the ROM and open an EPUB.

> [!NOTE]
> If you don't want to touch the code then you can just download `Ikureader-6.5_modern.nds` or `IkuReader-6.5_modern.dis` and sandbox in [release](https://github.com/renatheoccupier/Epub-reader-for-ds/releases)
> Remember put the data in sandbox in the root (usually where you first see when open sd card)
> If you don't like or want add font remember prepare 3 file (or at lease copy it for 3 file) `font.ttf` for normal, `fontB.ttf` for bold, and `fontI.ttf` for italic (ex: `DroidSerif.ttf`, `DroidSerifB.ttf`, `DroidSerifI.ttf`)
> I highly recommend you download `tools.zip` or made yourself some tools to optimize epub files

## Controls

### File Browser

- D-pad Up/Down: move selection
- Right / `A`: open folder or preview a book
- Left / `Y`: go back to parent folder, or leave the browser from `/`
- Touch: select entries
- In the open prompt:
  - Right / `A`: open the selected book
  - Left / `Y`: keep browsing

### Reading

- Right side tap, Right / `A`: next page
- Left side tap, Left / `Y`: previous page
- `R` / `L`: line-by-line scroll
- Up / `X`: bookmarks and contents
- Down / `B`: reading settings
- `Select`: search
- Swipe left/right: page turn

> [!NOTE]
> If you used ikureader before or even not, don't worry it work quite simple and intuitive (i guess) oh and touch in screen work so you can using it if not get the button

## Build

Tested in this repo with:

- `make -C arm9`
- `make`

You need a working devkitPro / devkitARM setup with the Nintendo DS tools available in your environment.

## Project Layout

- `arm9/source/`: main reader code
- `include/`: headers
- `sandbox/data/ikureader/`: runtime data to copy to the device
- `tools/`: helper scripts for EPUB preparation

---

# Credits

- [awkitsune](https://github.com/awkitsune) / Vladimir Kosickij for the modern IkuReader update that this work builds on
- Chintoi for the original IkuReader codebase
- [renatheoccupier](https://github.com/renatheoccupier)

Reference sources for credit:

- `awkitsune/IkuReader`: <https://github.com/awkitsune/IkuReader>
- Original IkuReader release notes mentioning Chintoi: <https://www.dcemu.co.uk/content/88954-IkuReader-v0-043-0044?s=c71cd78a6fd5a633f9483b42687ad6ea>

---

# **Sorry if i missing or make wrong infomation hope it work well on your devices thank for reading that freaking long readme :v**
