#!/usr/bin/env bash
# Package a flash-ready STAEXE sketch after a successful Mega verify.
#
# Produces:
#   dist/STAEXE/STAEXE.ino   (+ required headers/sources)
#   dist/STAEXE-firmware.zip
#   dist/memory-report.txt
#   dist/STAEXE.hex          (when available)
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT"

export PATH="$HOME/.local/bin:${PATH:-}"

DIST="$ROOT/dist"
SKETCH="$DIST/STAEXE"
STAGED="$ROOT/build/sketch/STAEXE"

echo "==> Verify Mega compile first"
bash "$ROOT/scripts/verify-mega.sh"

echo "==> Copying verified sketch to dist/"
rm -rf "$SKETCH"
mkdir -p "$DIST"
cp -R "$STAGED" "$SKETCH"

REPORT="$DIST/memory-report.txt"
{
  echo "STAEXE firmware export — $(date -u +%Y-%m-%dT%H:%M:%SZ)"
  echo "Open folder: dist/STAEXE/  (board: Arduino Mega 2560)"
  echo
  if [[ -f "$ROOT/build/mega/memory-summary.txt" ]]; then
    cat "$ROOT/build/mega/memory-summary.txt"
  fi
  echo
  echo "Binaries:"
  find "$ROOT/build" -name '*.hex' 2>/dev/null | head -20 || true
} | tee "$REPORT"

HEX="$(find "$ROOT/build" -name 'STAEXE.ino.hex' 2>/dev/null | head -1 || true)"
if [[ -z "$HEX" ]]; then
  HEX="$(find "$ROOT/build" -name '*.hex' 2>/dev/null | head -1 || true)"
fi
if [[ -n "$HEX" ]]; then
  cp "$HEX" "$DIST/STAEXE.hex"
  echo "Copied $HEX → dist/STAEXE.hex"
fi

(cd "$DIST" && rm -f STAEXE-firmware.zip && zip -qr STAEXE-firmware.zip STAEXE memory-report.txt $( [[ -f STAEXE.hex ]] && echo STAEXE.hex ))
echo "Wrote $DIST/STAEXE-firmware.zip"
echo "Export ready: open dist/STAEXE/STAEXE.ino in Arduino IDE"
