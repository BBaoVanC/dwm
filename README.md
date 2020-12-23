# bbaovanc's dwm fork

## Table of Contents

- [bbaovanc's dwm fork](#bbaovancs-dwm-fork)
  - [Table of Contents](#table-of-contents)
  - [Dependencies](#dependencies)
  - [Changes](#changes)
    - [Patches](#patches)
    - [Features and Tweaks](#features-and-tweaks)
    - [Window Rules](#window-rules)
  - [Installation](#installation)

## Dependencies

- [libxft-bgra](https://aur.archlinux.org/packages/libxft-bgra)
- [st](https://git.bbaovanc.com/bbaovanc/st)
- [dmenu](https://git.bbaovanc.com/bbaovanc/dmenu)
- [dwmblocks](https://git.bbaovanc.com/bbaovanc/dwmblocks) (optional, but highly recommended)
- Read through the [keybinds](docs/keybinds.md) and install any missing programs or replace/remove keybinds

## Changes

### Patches

- [autostart](https://dwm.suckless.org/patches/autostart)
- [fullgaps](https://dwm.suckless.org/patches/fullgaps)
- [actualfullscreen](https://dwm.suckless.org/patches/actualfullscreen)
- [swallow](https://dwm.suckless.org/patches/swallow)
- [fixborders](https://dwm.suckless.org/patches/alpha/)
- [pertag](https://dwm.suckless.org/patches/pertag)
- [systray](https://dwm.suckless.org/patches/systray)
- [dwmblocks-systray (dwmblocks support)](https://git.bbaovanc.com/bbaovanc/dwmblocks)

### Features and Tweaks

- [Nord theme](https://nordtheme.com)
- Use bold version of system monospace font
- Increase window borders to 4px
- [Add window rules](#window-rules)
- Don't force dmenu colors
- [Add keybinds](docs/keybinds.md)
- Enable color emoji support (will crash without libxft-bgra)

### Window Rules

- `st-floating` class: make it easy to make floating st windows)
- Thunderbird compose window: make floating
- Steam News: make floating
- Steam Friends List: make floating
- Dragon-drag-and-drop: make floating and don't swallow
- Clipboard Editor (from dotfiles): make floating
- Event Tester (xev): don't swallow

## Installation

1. Run `make`
2. Run `sudo make install`

By default, `dwm` will be installed to `/usr/local/bin` and manpages to `/usr/local/share/man/man1`
