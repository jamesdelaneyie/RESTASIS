/**
 * STAEXE PIXI renderer — 18 mirrored LED strips + 12 motors.
 * Frames come from the C++ choreography process over WebSocket.
 */
import { Application, Container, Graphics, Text } from '/pixi/pixi.mjs';

const LED_STRIPS = 18;
const LEDS_PER_STRIP = 70;
const MOTOR_COUNT = 12;

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

const ledLayer = new Container();
const motorLayer = new Container();
root.addChild(ledLayer);
root.addChild(motorLayer);

/** @type {Graphics[]} */
const ledDots = [];
/** @type {{ wrap: Container, arm: Graphics, radius: number }[]} */
const motors = [];
/** @type {number} */
let ledRadius = 3;

function layout() {
  const w = app.renderer.width;
  const h = app.renderer.height;
  ledLayer.removeChildren();
  motorLayer.removeChildren();
  ledDots.length = 0;
  motors.length = 0;

  const marginX = 48;
  const top = 24;
  const ledBlockH = h * 0.55;
  const cellW = (w - marginX * 2) / LEDS_PER_STRIP;
  const cellH = ledBlockH / LED_STRIPS;
  ledRadius = Math.max(1.5, Math.min(cellW, cellH) * 0.38);

  for (let s = 0; s < LED_STRIPS; s++) {
    for (let i = 0; i < LEDS_PER_STRIP; i++) {
      const g = new Graphics();
      const x = marginX + i * cellW + cellW * 0.5;
      const y = top + s * cellH + cellH * 0.5;
      g.circle(0, 0, ledRadius).fill({ color: 0x1a2a22 });
      g.x = x;
      g.y = y;
      ledLayer.addChild(g);
      ledDots.push(g);
    }
  }

  const cx = w * 0.5;
  const cy = top + ledBlockH + (h - ledBlockH - top) * 0.55;
  const ringR = Math.min(w, h - ledBlockH) * 0.28;
  const bodyR = Math.max(10, ringR * 0.12);
  const armLen = bodyR * 1.6;

  for (let m = 0; m < MOTOR_COUNT; m++) {
    const angle = (m / MOTOR_COUNT) * Math.PI * 2 - Math.PI / 2;
    const mx = cx + Math.cos(angle) * ringR;
    const my = cy + Math.sin(angle) * ringR;

    const body = new Graphics();
    body.circle(0, 0, bodyR).fill({ color: 0x24352c }).stroke({ width: 2, color: 0x3ecf8e, alpha: 0.7 });

    const hub = new Graphics();
    hub.circle(0, 0, bodyR * 0.22).fill({ color: 0xd7e6dc });

    const arm = new Graphics();
    arm.moveTo(0, 0).lineTo(armLen, 0).stroke({ width: 3, color: 0x3ecf8e, cap: 'round' });

    const label = new Text({
      text: String(m + 1),
      style: { fill: 0x7f9a8a, fontSize: 11, fontFamily: 'IBM Plex Sans, sans-serif' },
    });
    label.anchor.set(0.5);
    label.y = bodyR + 12;

    const wrap = new Container();
    wrap.x = mx;
    wrap.y = my;
    wrap.addChild(body, arm, hub, label);
    motorLayer.addChild(wrap);
    motors.push({ wrap, arm, radius: bodyR });
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
  /* Mirrored strips: same 70 RGB triples drawn on every row. */
  for (let s = 0; s < LED_STRIPS; s++) {
    for (let i = 0; i < LEDS_PER_STRIP; i++) {
      const o = i * 3;
      const r = latest.leds[o];
      const g = latest.leds[o + 1];
      const b = latest.leds[o + 2];
      const color = (r << 16) | (g << 8) | b;
      const lit = r | g | b;
      const dot = ledDots[s * LEDS_PER_STRIP + i];
      dot.clear();
      if (lit) {
        dot.circle(0, 0, ledRadius).fill({ color, alpha: 0.95 });
      } else {
        dot.circle(0, 0, ledRadius).fill({ color: 0x1a2a22, alpha: 0.9 });
      }
    }
  }

  for (let m = 0; m < MOTOR_COUNT; m++) {
    motors[m].arm.rotation = (latest.motors[m] * Math.PI) / 180;
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
