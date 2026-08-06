#!/usr/bin/env bash
# Idempotent cloud-agent install for STAEXE (sim + Mega compile tools).
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT"

echo "==> Node deps (sim)"
(cd sim && npm install)

echo "==> Build C++ choreography sim"
(cd sim && make clean && make)

echo "==> Arduino CLI (Mega verify)"
if ! command -v arduino-cli >/dev/null 2>&1; then
  curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh \
    | BINDIR="$HOME/.local/bin" sh
  export PATH="$HOME/.local/bin:$PATH"
fi
mkdir -p "$HOME/.arduino15"
arduino-cli config init --overwrite 2>/dev/null || true
arduino-cli core update-index
arduino-cli core install arduino:avr

echo "==> Mega compile smoke-check"
bash "$ROOT/scripts/verify-mega.sh"

echo "cloud-install done"
