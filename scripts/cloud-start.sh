#!/usr/bin/env bash
# Keep the PIXI simulator available for review while the agent runs.
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
export PATH="$HOME/.local/bin:${PATH:-}"
cd "$ROOT/sim"
# Detached start is handled by the environment runner; stay foreground if invoked alone.
exec npm start
