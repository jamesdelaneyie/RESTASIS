#!/usr/bin/env bash
# Compile firmware/STAEXE for Arduino Mega 2560 and enforce flash/SRAM budgets.
#
# Usage:  ./scripts/verify-mega.sh
# Env:    STAEXE_FLASH_MAX_PCT (default 90), STAEXE_SRAM_MAX_PCT (default 80)
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT"

export PATH="$HOME/.local/bin:${PATH:-}"

BOARD="${STAEXE_BOARD:-arduino:avr:mega}"
FLASH_MAX_PCT="${STAEXE_FLASH_MAX_PCT:-90}"
SRAM_MAX_PCT="${STAEXE_SRAM_MAX_PCT:-80}"
BUILD_DIR="${STAEXE_BUILD_DIR:-$ROOT/build/mega}"
SKETCH_DIR="$ROOT/firmware/STAEXE"
LIBS_DIR="$ROOT/firmware/lib"
mkdir -p "$BUILD_DIR"

if ! command -v arduino-cli >/dev/null 2>&1; then
  echo "error: arduino-cli not found. Run scripts/cloud-install.sh first." >&2
  exit 1
fi

arduino-cli core install arduino:avr >/dev/null

echo "==> Compiling STAEXE for $BOARD"
set +e
OUT="$(arduino-cli compile \
  --fqbn "$BOARD" \
  --libraries "$LIBS_DIR" \
  --build-path "$BUILD_DIR" \
  --warnings more \
  "$SKETCH_DIR" 2>&1)"
STATUS=$?
set -e
echo "$OUT"
if [[ $STATUS -ne 0 ]]; then
  echo "error: Mega compile failed (exit $STATUS)" >&2
  exit 1
fi

flash_line="$(echo "$OUT" | grep -E 'program storage space' | tail -1 || true)"
sram_line="$(echo "$OUT" | grep -E 'dynamic memory' | tail -1 || true)"

parse_pct() {
  echo "$1" | sed -n 's/.*(\([0-9][0-9]*\)%).*/\1/p' | head -1
}

flash_pct="$(parse_pct "$flash_line")"
sram_pct="$(parse_pct "$sram_line")"

if [[ -z "$flash_pct" || -z "$sram_pct" ]]; then
  echo "warning: could not parse memory percentages from arduino-cli output" >&2
  echo "flash_line=$flash_line"
  echo "sram_line=$sram_line"
else
  echo "==> Memory: flash ${flash_pct}% / sram ${sram_pct}%"
  if (( flash_pct > FLASH_MAX_PCT )); then
    echo "error: flash ${flash_pct}% exceeds budget ${FLASH_MAX_PCT}%" >&2
    exit 2
  fi
  if (( sram_pct > SRAM_MAX_PCT )); then
    echo "error: SRAM ${sram_pct}% exceeds budget ${SRAM_MAX_PCT}%" >&2
    exit 3
  fi
  echo "OK within budgets (flash≤${FLASH_MAX_PCT}%, sram≤${SRAM_MAX_PCT}%)"
fi

SUMMARY="$BUILD_DIR/memory-summary.txt"
{
  echo "board=$BOARD"
  echo "flash_line=$flash_line"
  echo "sram_line=$sram_line"
  echo "flash_pct=${flash_pct:-unknown}"
  echo "sram_pct=${sram_pct:-unknown}"
} > "$SUMMARY"
echo "Wrote $SUMMARY"

find "$BUILD_DIR" -name '*.hex' 2>/dev/null | head -20 || true
