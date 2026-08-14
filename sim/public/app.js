/**
 * STAEXE PIXI renderer — hexagonal sculpture layout.
 *
 * Geometry (matches firmware strip count):
 *   12 motor-arm LED strips (1 per motor, aimed at centre)  = strips 0..11
 *   6 outer strut LED strips (fixed hex edges)              = strips 12..17
 *                                                           ——
 *                                                             18 × 70 LEDs
 *   12 motors in 6 pairs, one pair at each hex vertex.
 *
 * Motor-arm strips are children of each motor's rotating arm, so they
 * swing with the stepper angle from the C++ choreography frames.
 */
import { Application, Container, Graphics, Text } from '/pixi/pixi.mjs';

const LED_STRIPS = 18;
const LEDS_PER_STRIP = 70;
const MOTOR_COUNT = 12;
const VERTEX_COUNT = 6;
const MOTORS_PER_VERTEX = 2;
const MOTOR_STRIP_COUNT = MOTOR_COUNT; // 12 — one strip per motor arm
const STRUT_STRIP_COUNT = VERTEX_COUNT; // 6

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
const strutLedLayer = new Container();
const motorLayer = new Container();
root.addChild(structureLayer);
root.addChild(strutLedLayer);
root.addChild(motorLayer);

/** @type {(Graphics|null)[]} strip-major LED graphics (motor arms + struts) */
const ledDots = [];
/**
 * @type {{
 *   wrap: Container,
 *   arm: Container,
 *   baseAngle: number,
 *   stripIndex: number,
 * }[]}
 */
const motors = [];
/** @type {number} */
let ledRadius = 2.5;

function lerp(a, b, t) {
  return a + (b - a) * t;
}

function pointOnSegment(x0, y0, x1, y1, t) {
  return { x: lerp(x0, x1, t), y: lerp(y0, y1, t) };
}

/** Fixed strut strip in world space (does not rotate). */
function placeFixedStrip(x0, y0, x1, y1, stripIndex, parent) {
  const inset = 0.06;
  for (let i = 0; i < LEDS_PER_STRIP; i++) {
    const t = inset + ((1 - inset * 2) * i) / (LEDS_PER_STRIP - 1);
    const p = pointOnSegment(x0, y0, x1, y1, t);
    const g = new Graphics();
    g.circle(0, 0, ledRadius).fill({ color: 0x3a5246 });
    g.x = p.x;
    g.y = p.y;
    parent.addChild(g);
    ledDots[stripIndex * LEDS_PER_STRIP + i] = g;
  }
}

/**
 * LED strip along a motor arm in local space.
 * Local +X points along the arm (toward centre when arm.rotation = baseAngle).
 */
function placeArmStrip(arm, stripIndex, armLen) {
  const inset = 0.08;
  const start = armLen * inset;
  const end = armLen * (1 - inset * 0.35);
  for (let i = 0; i < LEDS_PER_STRIP; i++) {
    const t = i / (LEDS_PER_STRIP - 1);
    const g = new Graphics();
    g.circle(0, 0, ledRadius).fill({ color: 0x3a5246 });
    g.x = lerp(start, end, t);
    g.y = 0;
    arm.addChild(g);
    ledDots[stripIndex * LEDS_PER_STRIP + i] = g;
  }
}

function layout() {
  const w = app.renderer.width;
  const h = app.renderer.height;
  structureLayer.removeChildren();
  strutLedLayer.removeChildren();
  motorLayer.removeChildren();
  ledDots.length = 0;
  motors.length = 0;

  const cx = w * 0.5;
  const cy = h * 0.52;
  const R = Math.min(w, h) * 0.4;
  const hubR = Math.max(14, R * 0.07);
  ledRadius = Math.max(2.2, Math.min(w, h) * 0.0055);

  /** Hexagon vertices (pointy-top: first vertex at top). */
  const verts = [];
  for (let i = 0; i < VERTEX_COUNT; i++) {
    const a = (i / VERTEX_COUNT) * Math.PI * 2 - Math.PI / 2;
    verts.push({ x: cx + Math.cos(a) * R, y: cy + Math.sin(a) * R, a });
  }

  /* Structure: hub + outer hex (no fixed radial beams — arms carry LEDs). */
  const structure = new Graphics();
  structure.circle(cx, cy, hubR).fill({ color: 0x1a2820 }).stroke({ width: 2, color: 0x3ecf8e, alpha: 0.45 });
  for (let i = 0; i < VERTEX_COUNT; i++) {
    const a = verts[i];
    const b = verts[(i + 1) % VERTEX_COUNT];
    structure.moveTo(a.x, a.y).lineTo(b.x, b.y).stroke({
      width: 2.5,
      color: 0x24352c,
      alpha: 0.7,
      cap: 'round',
    });
  }
  structureLayer.addChild(structure);

  /* Fixed LED strips along outer struts. */
  for (let i = 0; i < VERTEX_COUNT; i++) {
    const a = verts[i];
    const b = verts[(i + 1) % VERTEX_COUNT];
    const mx = (a.x + b.x) * 0.5;
    const my = (a.y + b.y) * 0.5;
    const inward = Math.max(6, R * 0.022);
    const vx = cx - mx;
    const vy = cy - my;
    const vLen = Math.hypot(vx, vy) || 1;
    const ix = (vx / vLen) * inward;
    const iy = (vy / vLen) * inward;
    placeFixedStrip(
      a.x + ix,
      a.y + iy,
      b.x + ix,
      b.y + iy,
      MOTOR_STRIP_COUNT + i,
      strutLedLayer,
    );
  }

  /*
   * Motors: pairs at each hex vertex.
   * Offset along the local tangent so the pair sits on the edge, side by side.
   * Each motor's arm points toward the sculpture centre and carries one LED strip.
   */
  const bodyR = Math.max(9, R * 0.042);
  const pairSpread = Math.max(18, R * 0.08);
  const armLen = R * 0.72;

  for (let v = 0; v < VERTEX_COUNT; v++) {
    const vert = verts[v];
    const inward = Math.atan2(cy - vert.y, cx - vert.x); // toward centre
    const tangent = inward + Math.PI / 2;

    for (let k = 0; k < MOTORS_PER_VERTEX; k++) {
      const mIndex = v * MOTORS_PER_VERTEX + k;
      const side = k === 0 ? -1 : 1;
      const mx = vert.x + Math.cos(tangent) * pairSpread * 0.5 * side;
      const my = vert.y + Math.sin(tangent) * pairSpread * 0.5 * side;

      /* Slightly different rest aim so paired arms don't perfectly overlap. */
      const aimNudge = side * 0.12;
      const baseAngle = inward + aimNudge;

      const wrap = new Container();
      wrap.x = mx;
      wrap.y = my;

      const body = new Graphics();
      body
        .circle(0, 0, bodyR)
        .fill({ color: 0x24352c })
        .stroke({ width: 2, color: 0x3ecf8e, alpha: 0.8 });

      const hub = new Graphics();
      hub.circle(0, 0, bodyR * 0.22).fill({ color: 0xd7e6dc });

      /* Rotating arm: beam + LED strip (local +X = along arm toward centre). */
      const arm = new Container();
      arm.rotation = baseAngle;

      const beam = new Graphics();
      beam
        .moveTo(bodyR * 0.6, 0)
        .lineTo(armLen, 0)
        .stroke({ width: 3, color: 0x3ecf8e, alpha: 0.85, cap: 'round' });
      arm.addChild(beam);

      placeArmStrip(arm, mIndex, armLen);

      const label = new Text({
        text: String(mIndex + 1),
        style: { fill: 0x7f9a8a, fontSize: 10, fontFamily: 'IBM Plex Sans, sans-serif' },
      });
      label.anchor.set(0.5);
      /* Park label outside the hex so rotating arms don't cover it. */
      label.x = Math.cos(inward + Math.PI) * (bodyR + 14);
      label.y = Math.sin(inward + Math.PI) * (bodyR + 14);

      wrap.addChild(arm, body, hub, label);
      motorLayer.addChild(wrap);

      motors[mIndex] = {
        wrap,
        arm,
        baseAngle,
        stripIndex: mIndex,
      };
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

function paintLed(dot, r, g, b) {
  if (!dot) return;
  const color = (r << 16) | (g << 8) | b;
  const lit = r | g | b;
  dot.clear();
  if (lit) {
    dot.circle(0, 0, ledRadius * 1.35).fill({ color, alpha: 0.28 });
    dot.circle(0, 0, ledRadius).fill({ color, alpha: 1 });
  } else {
    dot.circle(0, 0, ledRadius).fill({ color: 0x3a5246, alpha: 0.85 });
  }
}

function paint() {
  /* Colours — mirrored buffer across all strips. */
  for (let s = 0; s < LED_STRIPS; s++) {
    for (let i = 0; i < LEDS_PER_STRIP; i++) {
      const o = i * 3;
      paintLed(
        ledDots[s * LEDS_PER_STRIP + i],
        latest.leds[o],
        latest.leds[o + 1],
        latest.leds[o + 2],
      );
    }
  }

  /* Motor arms (and their LED strips) rotate with choreography angles. */
  for (let m = 0; m < MOTOR_COUNT; m++) {
    const motor = motors[m];
    if (!motor) continue;
    motor.arm.rotation = motor.baseAngle + (latest.motors[m] * Math.PI) / 180;
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
