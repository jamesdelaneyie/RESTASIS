/**
 * STAEXE PIXI renderer — hexagonal sculpture layout.
 *
 * Geometry (matches firmware strip count):
 *   6 radial arms × 2 LED strips each  = 12 arm strips
 *   6 outer struts (hexagon edges)     =  6 strut strips
 *                                      ——
 *                                        18 strips × 70 LEDs
 *   12 motors: 2 per arm (inner + outer hubs)
 *
 * Frames still send one mirrored 70-RGB buffer; every strip paints the same
 * pattern until independent strip buffers land in firmware.
 */
import { Application, Container, Graphics, Text } from '/pixi/pixi.mjs';

const LED_STRIPS = 18;
const LEDS_PER_STRIP = 70;
const MOTOR_COUNT = 12;
const ARM_COUNT = 6;
const STRIPS_PER_ARM = 2;
const ARM_STRIP_COUNT = ARM_COUNT * STRIPS_PER_ARM; // 12

const statusEl = document.getElementById('status');
const clockEl = document.getElementById('clock');
const stageEl = document.getElementById('stage');

const app = new Application();
await app.init({
  resizeTo: stageEl,
  backgroundAlpha: 0,
  antialias: true,
  preference: 'webgl',
});
stageEl.appendChild(app.canvas);

const root = new Container();
app.stage.addChild(root);

const structureLayer = new Container();
const ledLayer = new Container();
const motorLayer = new Container();
root.addChild(structureLayer);
root.addChild(ledLayer);
root.addChild(motorLayer);

/** @type {(Graphics|null)[]} */
const ledDots = [];
/** @type {{ wrap: Container, rotor: Graphics, baseAngle: number }[]} */
const motors = [];
/** @type {number} */
let ledRadius = 2.5;

function lerp(a, b, t) {
  return a + (b - a) * t;
}

function pointOnSegment(x0, y0, x1, y1, t) {
  return { x: lerp(x0, x1, t), y: lerp(y0, y1, t) };
}

/** Place `LEDS_PER_STRIP` dots along a segment. */
function placeStrip(x0, y0, x1, y1, stripIndex) {
  const inset = 0.04;
  for (let i = 0; i < LEDS_PER_STRIP; i++) {
    const t = inset + ((1 - inset * 2) * i) / (LEDS_PER_STRIP - 1);
    const p = pointOnSegment(x0, y0, x1, y1, t);
    const g = new Graphics();
    g.circle(0, 0, ledRadius).fill({ color: 0x3a5246 });
    g.x = p.x;
    g.y = p.y;
    ledLayer.addChild(g);
    ledDots[stripIndex * LEDS_PER_STRIP + i] = g;
  }
}

function layout() {
  const w = app.renderer.width;
  const h = app.renderer.height;
  structureLayer.removeChildren();
  ledLayer.removeChildren();
  motorLayer.removeChildren();
  ledDots.length = 0;
  motors.length = 0;

  const cx = w * 0.5;
  const cy = h * 0.52;
  const R = Math.min(w, h) * 0.38;
  const hubR = Math.max(14, R * 0.08);
  /* Sized to read as LED pixels along arms/struts, not vanish into the beams. */
  ledRadius = Math.max(2.2, Math.min(w, h) * 0.0055);

  /** Hexagon vertices (flat orientation starts at top). */
  const verts = [];
  for (let i = 0; i < ARM_COUNT; i++) {
    const a = (i / ARM_COUNT) * Math.PI * 2 - Math.PI / 2;
    verts.push({ x: cx + Math.cos(a) * R, y: cy + Math.sin(a) * R, a });
  }

  const structure = new Graphics();
  structure.circle(cx, cy, hubR).fill({ color: 0x1a2820 }).stroke({ width: 2, color: 0x3ecf8e, alpha: 0.45 });

  for (let i = 0; i < ARM_COUNT; i++) {
    const v = verts[i];
    const n = verts[(i + 1) % ARM_COUNT];
    structure.moveTo(cx, cy).lineTo(v.x, v.y).stroke({ width: 3, color: 0x2a3f34, alpha: 0.7, cap: 'round' });
    structure.moveTo(v.x, v.y).lineTo(n.x, n.y).stroke({ width: 2.5, color: 0x24352c, alpha: 0.65, cap: 'round' });
  }
  structureLayer.addChild(structure);

  /* Arm LED strips: two parallel runs per radial arm. */
  for (let arm = 0; arm < ARM_COUNT; arm++) {
    const v = verts[arm];
    const tangX = -(v.y - cy);
    const tangY = v.x - cx;
    const tLen = Math.hypot(tangX, tangY) || 1;
    const offset = Math.max(8, R * 0.036);
    const ox = (tangX / tLen) * offset;
    const oy = (tangY / tLen) * offset;

    const armStart = pointOnSegment(cx, cy, v.x, v.y, 0.12);
    const armEnd = pointOnSegment(cx, cy, v.x, v.y, 0.92);

    const stripA = arm * STRIPS_PER_ARM;
    placeStrip(armStart.x + ox, armStart.y + oy, armEnd.x + ox, armEnd.y + oy, stripA);
    placeStrip(armStart.x - ox, armStart.y - oy, armEnd.x - ox, armEnd.y - oy, stripA + 1);
  }

  /* Perimeter strut LED strips — inset slightly toward hub so they sit on the edge. */
  for (let i = 0; i < ARM_COUNT; i++) {
    const a = verts[i];
    const b = verts[(i + 1) % ARM_COUNT];
    const mx = (a.x + b.x) * 0.5;
    const my = (a.y + b.y) * 0.5;
    const inward = Math.max(5, R * 0.02);
    const vx = cx - mx;
    const vy = cy - my;
    const vLen = Math.hypot(vx, vy) || 1;
    const ix = (vx / vLen) * inward;
    const iy = (vy / vLen) * inward;
    placeStrip(a.x + ix, a.y + iy, b.x + ix, b.y + iy, ARM_STRIP_COUNT + i);
  }

  /* Motors: inner + outer hub on each arm. */
  const bodyR = Math.max(9, R * 0.045);
  const rotorLen = bodyR * 1.55;
  for (let arm = 0; arm < ARM_COUNT; arm++) {
    const v = verts[arm];
    const armAngle = Math.atan2(v.y - cy, v.x - cx);
    const baseAngle = armAngle + Math.PI / 2;
    for (let k = 0; k < 2; k++) {
      const t = k === 0 ? 0.38 : 0.72;
      const p = pointOnSegment(cx, cy, v.x, v.y, t);
      const mIndex = arm * 2 + k;

      const body = new Graphics();
      body.circle(0, 0, bodyR).fill({ color: 0x24352c }).stroke({ width: 2, color: 0x3ecf8e, alpha: 0.75 });

      const hub = new Graphics();
      hub.circle(0, 0, bodyR * 0.22).fill({ color: 0xd7e6dc });

      const rotor = new Graphics();
      rotor
        .moveTo(-rotorLen * 0.35, 0)
        .lineTo(rotorLen, 0)
        .stroke({ width: 3, color: 0x3ecf8e, cap: 'round' });
      rotor.rotation = baseAngle;

      const label = new Text({
        text: String(mIndex + 1),
        style: { fill: 0x7f9a8a, fontSize: 10, fontFamily: 'IBM Plex Sans, sans-serif' },
      });
      label.anchor.set(0.5);
      label.y = bodyR + 11;

      const wrap = new Container();
      wrap.x = p.x;
      wrap.y = p.y;
      wrap.addChild(body, rotor, hub, label);
      motorLayer.addChild(wrap);
      motors[mIndex] = { wrap, rotor, baseAngle };
    }
  }
}

layout();
window.addEventListener('resize', layout);

let latest = {
  t: 0,
  leds: new Uint8Array(LEDS_PER_STRIP * 3),
  motors: new Float32Array(MOTOR_COUNT),
};

function applyFrame(frame) {
  latest.t = frame.t;
  if (Array.isArray(frame.leds) && frame.leds.length >= LEDS_PER_STRIP * 3) {
    for (let i = 0; i < LEDS_PER_STRIP * 3; i++) latest.leds[i] = frame.leds[i] & 255;
  }
  if (Array.isArray(frame.motors) && frame.motors.length >= MOTOR_COUNT) {
    for (let i = 0; i < MOTOR_COUNT; i++) latest.motors[i] = frame.motors[i];
  }
  clockEl.textContent = `t=${Math.round(frame.t / 1000)}s`;
}

function paint() {
  for (let s = 0; s < LED_STRIPS; s++) {
    for (let i = 0; i < LEDS_PER_STRIP; i++) {
      const o = i * 3;
      const r = latest.leds[o];
      const g = latest.leds[o + 1];
      const b = latest.leds[o + 2];
      const color = (r << 16) | (g << 8) | b;
      const lit = r | g | b;
      const dot = ledDots[s * LEDS_PER_STRIP + i];
      if (!dot) continue;
      dot.clear();
      if (lit) {
        dot.circle(0, 0, ledRadius * 1.35).fill({ color, alpha: 0.28 });
        dot.circle(0, 0, ledRadius).fill({ color, alpha: 1 });
      } else {
        dot.circle(0, 0, ledRadius).fill({ color: 0x3a5246, alpha: 0.85 });
      }
    }
  }

  for (let m = 0; m < MOTOR_COUNT; m++) {
    const motor = motors[m];
    if (!motor) continue;
    motor.rotor.rotation = motor.baseAngle + (latest.motors[m] * Math.PI) / 180;
  }
}

app.ticker.add(paint);

function connect() {
  const proto = location.protocol === 'https:' ? 'wss' : 'ws';
  const ws = new WebSocket(`${proto}://${location.host}/frames`);
  ws.onopen = () => {
    statusEl.textContent = 'live';
    statusEl.dataset.live = '1';
  };
  ws.onclose = () => {
    statusEl.textContent = 'reconnecting…';
    statusEl.dataset.live = '0';
    setTimeout(connect, 1000);
  };
  ws.onerror = () => ws.close();
  ws.onmessage = (ev) => {
    try {
      applyFrame(JSON.parse(ev.data));
    } catch (_) {
      /* ignore malformed */
    }
  };
}

connect();
