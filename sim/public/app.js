/**
 * STAEXE PIXI renderer — matches the physical hexagonal sculpture.
 *
 * Frame (from photos + hex.ino):
 *   - Pointy-top hexagon, dark radial struts hub → 6 vertices
 *   - 6 fixed LED tubes on the outer perimeter struts
 *   - 12 motors in pairs at the vertices; each holds an LED tube
 *     hanging straight down at rest (firmware 0°)
 *
 * Strips 0..11 = motor arms (rotate with steppers)
 * Strips 12..17 = perimeter struts (fixed)
 */
import { Application, Container, Graphics, Text } from '/pixi/pixi.mjs';

const LED_STRIPS = 18;
const LEDS_PER_STRIP = 70;
const MOTOR_COUNT = 12;
const VERTEX_COUNT = 6;
const MOTORS_PER_VERTEX = 2;
const MOTOR_STRIP_COUNT = MOTOR_COUNT;

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

/**
 * One glowing tube per strip (photos show diffused tubes, not pixels).
 * @type {{ g: Graphics, x0: number, y0: number, x1: number, y1: number }[]}
 */
const strutTubes = [];
/**
 * @type {{
 *   wrap: Container,
 *   arm: Container,
 *   tube: Graphics,
 *   baseAngle: number,
 *   tubeStart: number,
 *   tubeEnd: number,
 *   tubeW: number,
 * }[]}
 */
const motors = [];

/** @type {number} */
let tubeW = 6;

function layout() {
  const w = app.renderer.width;
  const h = app.renderer.height;
  structureLayer.removeChildren();
  strutLedLayer.removeChildren();
  motorLayer.removeChildren();
  strutTubes.length = 0;
  motors.length = 0;

  const cx = w * 0.5;
  const cy = h * 0.52;
  const R = Math.min(w, h) * 0.4;
  tubeW = Math.max(5, R * 0.028);

  /** Pointy-top hexagon (vertex at 12 o’clock), as in the install photos. */
  const verts = [];
  for (let i = 0; i < VERTEX_COUNT; i++) {
    const a = (i / VERTEX_COUNT) * Math.PI * 2 - Math.PI / 2;
    verts.push({ x: cx + Math.cos(a) * R, y: cy + Math.sin(a) * R, a });
  }

  const structure = new Graphics();

  /* Radial structural struts (cables/tubing, not LEDs). */
  for (let i = 0; i < VERTEX_COUNT; i++) {
    const v = verts[i];
    structure.moveTo(cx, cy).lineTo(v.x, v.y).stroke({
      width: Math.max(4, R * 0.018),
      color: 0x141a16,
      alpha: 0.95,
      cap: 'round',
    });
  }

  /* Dark perimeter frame under the LED tubes. */
  for (let i = 0; i < VERTEX_COUNT; i++) {
    const a = verts[i];
    const b = verts[(i + 1) % VERTEX_COUNT];
    structure.moveTo(a.x, a.y).lineTo(b.x, b.y).stroke({
      width: Math.max(3, R * 0.012),
      color: 0x101410,
      alpha: 0.9,
      cap: 'round',
    });
  }

  /* Central hub box. */
  const hub = Math.max(22, R * 0.1);
  structure.roundRect(cx - hub * 0.7, cy - hub * 0.55, hub * 1.4, hub * 1.1, 4)
    .fill({ color: 0x0c0e0c })
    .stroke({ width: 1.5, color: 0x2a332c, alpha: 0.8 });

  structureLayer.addChild(structure);

  /* Perimeter LED tubes, slightly inset so they sit on the hex edges. */
  const inset = Math.max(4, R * 0.012);
  for (let i = 0; i < VERTEX_COUNT; i++) {
    const a = verts[i];
    const b = verts[(i + 1) % VERTEX_COUNT];
    const mx = (a.x + b.x) * 0.5;
    const my = (a.y + b.y) * 0.5;
    const vx = cx - mx;
    const vy = cy - my;
    const vLen = Math.hypot(vx, vy) || 1;
    const ix = (vx / vLen) * inset;
    const iy = (vy / vLen) * inset;
    /* Shorten slightly so tubes don’t swallow the motor hubs. */
    const t0 = 0.08;
    const t1 = 0.92;
    const x0 = a.x + ix + (b.x - a.x) * t0;
    const y0 = a.y + iy + (b.y - a.y) * t0;
    const x1 = a.x + ix + (b.x - a.x) * t1;
    const y1 = a.y + iy + (b.y - a.y) * t1;
    const g = new Graphics();
    strutLedLayer.addChild(g);
    strutTubes[i] = { g, x0, y0, x1, y1 };
  }

  /*
   * Motor pairs at each vertex. Rest pose (firmware 0°): every arm hangs
   * straight down. Choreography then adds ±30°/±60° from that.
   */
  const bodyR = Math.max(7, R * 0.028);
  const pairSpread = Math.max(10, R * 0.045);
  const armLen = R * 0.88;
  const tubeStart = bodyR * 1.3;
  const tubeEnd = armLen * 0.96;

  for (let v = 0; v < VERTEX_COUNT; v++) {
    const vert = verts[v];
    const inward = Math.atan2(cy - vert.y, cx - vert.x);
    const tangent = inward + Math.PI / 2;

    for (let k = 0; k < MOTORS_PER_VERTEX; k++) {
      const mIndex = v * MOTORS_PER_VERTEX + k;
      const side = k === 0 ? -1 : 1;
      const mx = vert.x + Math.cos(tangent) * pairSpread * 0.5 * side;
      const my = vert.y + Math.sin(tangent) * pairSpread * 0.5 * side;
      const baseAngle = Math.PI / 2;

      const wrap = new Container();
      wrap.x = mx;
      wrap.y = my;

      const arm = new Container();
      arm.rotation = baseAngle;

      const beam = new Graphics();
      beam
        .moveTo(0, 0)
        .lineTo(tubeStart, 0)
        .stroke({ width: 2, color: 0x1a1f1c, cap: 'round' });
      arm.addChild(beam);

      const tube = new Graphics();
      arm.addChild(tube);

      const body = new Graphics();
      body.circle(0, 0, bodyR).fill({ color: 0x121612 }).stroke({
        width: 1.5,
        color: 0x3a4540,
        alpha: 0.9,
      });

      const label = new Text({
        text: String(mIndex + 1),
        style: { fill: 0x6a8074, fontSize: 10, fontFamily: 'IBM Plex Sans, sans-serif' },
      });
      label.anchor.set(0.5);
      label.x = Math.cos(inward + Math.PI) * (bodyR + 13);
      label.y = Math.sin(inward + Math.PI) * (bodyR + 13);

      wrap.addChild(arm, body, label);
      motorLayer.addChild(wrap);

      motors[mIndex] = {
        wrap,
        arm,
        tube,
        baseAngle,
        tubeStart,
        tubeEnd,
        tubeW,
      };
    }
  }
}

function paintTube(g, x0, y0, x1, y1, width, r, gv, b) {
  g.clear();
  const lit = r | gv | b;
  const color = lit ? (r << 16) | (gv << 8) | b : 0x1c2822;
  const glow = lit ? 0.35 : 0.08;
  const coreA = lit ? 1 : 0.55;
  g.moveTo(x0, y0).lineTo(x1, y1).stroke({
    width: width * 2.4,
    color,
    alpha: glow,
    cap: 'round',
  });
  g.moveTo(x0, y0).lineTo(x1, y1).stroke({
    width,
    color,
    alpha: coreA,
    cap: 'round',
  });
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
  const r = latest.leds[0];
  const g = latest.leds[1];
  const b = latest.leds[2];

  for (let i = 0; i < strutTubes.length; i++) {
    const t = strutTubes[i];
    if (!t) continue;
    paintTube(t.g, t.x0, t.y0, t.x1, t.y1, tubeW, r, g, b);
  }

  for (let m = 0; m < MOTOR_COUNT; m++) {
    const motor = motors[m];
    if (!motor) continue;
    motor.arm.rotation = motor.baseAngle + (latest.motors[m] * Math.PI) / 180;
    paintTube(motor.tube, motor.tubeStart, 0, motor.tubeEnd, 0, motor.tubeW, r, g, b);
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
