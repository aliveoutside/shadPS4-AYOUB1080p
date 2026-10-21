<!--
SPDX-FileCopyrightText: 2026 shadPS4-perf contributors
SPDX-License-Identifier: GPL-2.0-or-later
-->

# shadPS4 for the inFAMOUS Second Son PC port

This is a fork of [shadPS4](https://github.com/shadps4-emu/shadPS4) used as a git submodule by
the [inFAMOUS Second Son native PC port](https://github.com/Castlesea/infamous-second-son-pc-port).
The port builds its renderer (`libiss-gpu.so`) from this tree's video core and shader recompiler.
It does not use the emulator itself.

It is not meant to be built or used on its own. Build the port instead; its README explains how.

## History

1. [shadPS4](https://github.com/shadps4-emu/shadPS4) at `2338a06` (30 September 2026).
2. [weshayoub92/shadPS4-perf](https://github.com/weshayoub92/shadPS4-perf) (`5843748`): performance
   work for inFAMOUS Second Son, covering GPU readbacks, command processor threading, per-draw
   lookup caches and background shader compilation.
3. This fork: the changes the PC port needs on top of that, such as buffer and pipeline cache
   hooks, upload helpers and the descriptor and stage lookup caches.

## License

GPL-2.0-or-later, like shadPS4 (see `LICENSE`).

> Unofficial and not affiliated with Sony Interactive Entertainment, Sucker Punch Productions or the
> shadPS4 team. It contains no game files, keys, firmware or system modules.
