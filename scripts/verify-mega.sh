#!/usr/bin/env bash
# Compile STAEXE for Arduino Mega 2560 and enforce flash/SRAM budgets.
#
# Stages sources into build/sketch/STAEXE/ (Arduino requires matching
# folder/.ino names), compiles, then checks memory percentages.
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
SKETCH_DIR="$ROOT/build/sketch/STAEXE"
mkdir -p "$BUILD_DIR"

if ! command -v arduino-cli >/dev/null 2>&1; then
  echo "error: arduino-cli not found. Run scripts/cloud-install.sh first." >&2
  exit 1
fi

arduino-cli core install arduino:avr >/dev/null

echo "==> Staging sketch at $SKETCH_DIR"
rm -rf "$SKETCH_DIR"
mkdir -p "$SKETCH_DIR"

# Entry point must be STAEXE.ino inside STAEXE/
cp "$ROOT/hex.ino" "$SKETCH_DIR/STAEXE.ino"

# Firmware sources — exclude desktop sim, vendored Arduino core stubs, and
# junk that would collide with arduino:avr (arduino.h / pins_arduino.h).
# Copy headers + the .cpp units the sketch actually links.
shopt -s nullglob
for f in \
  "$ROOT"/staexe_*.h "$ROOT"/staexe_*.cpp \
  "$ROOT"/display_arduino.cpp "$ROOT"/motors_arduino.cpp \
  "$ROOT"/leds_setup.h "$ROOT"/LEDAnimations.h \
  "$ROOT"/sensors.h "$ROOT"/IR_sensor_setup.h "$ROOT"/MSGEQ_setup.h \
  "$ROOT"/startup_melody.h "$ROOT"/pitches.h "$ROOT"/routines.h \
  "$ROOT"/setup_motors.h \
  "$ROOT"/BasicStepperDriver.h "$ROOT"/BasicStepperDriver.cpp \
  "$ROOT"/MultiDriver.h "$ROOT"/MultiDriver.cpp \
  "$ROOT"/SyncDriver.h "$ROOT"/SyncDriver.cpp \
  "$ROOT"/FAB_LED.h \
  "$ROOT"/MSGEQ7.h "$ROOT"/MSGEQ7.hpp \
  "$ROOT"/IRremote.h "$ROOT"/IRremote.hpp "$ROOT"/IRremoteInt.h \
  "$ROOT"/IRProtocol.h "$ROOT"/IRFeedbackLED.hpp \
  "$ROOT"/IRReceive.hpp "$ROOT"/IRSend.hpp \
  "$ROOT"/LongUnion.h \
  "$ROOT"/TinyIRReceiver.h "$ROOT"/TinyIRReceiver.hpp \
  "$ROOT"/digitalWriteFast.h "$ROOT"/boarddefs.h \
  "$ROOT"/ir_*.hpp "$ROOT"/ac_LG.h "$ROOT"/ac_LG.hpp
do
  cp "$f" "$SKETCH_DIR/"
done

if [[ -d "$ROOT/private" ]]; then
  mkdir -p "$SKETCH_DIR/private"
  # Prefer private digitalWriteFast / IRTimer if present
  cp -R "$ROOT/private/." "$SKETCH_DIR/private/"
fi

echo "==> Compiling STAEXE for $BOARD"
set +e
OUT="$(arduino-cli compile \
  --fqbn "$BOARD" \
  --build-path "$BUILD_DIR" \
  --warnings more \
  --export-binaries \
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

find "$BUILD_DIR" "$SKETCH_DIR" -name '*.hex' 2>/dev/null | head -20 || true
