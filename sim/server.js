/*********************************************************
 * STAEXE sim bridge — spawn C++ choreography, fan out
 * NDJSON frames to the PIXI page over WebSocket.
 *********************************************************/
const { spawn } = require('child_process');
const path = require('path');
const express = require('express');
const http = require('http');
const { WebSocketServer } = require('ws');

const PORT = process.env.PORT || 5173;
const TIME_SCALE = process.env.STAEXE_TIME_SCALE || '20';
const SIM_BIN = path.join(__dirname, 'staexe-sim');

const app = express();
app.use(express.static(path.join(__dirname, 'public')));
app.use('/pixi', express.static(path.join(__dirname, 'node_modules/pixi.js/dist')));

const server = http.createServer(app);
const wss = new WebSocketServer({ server, path: '/frames' });

let latest = null;
const clients = new Set();

wss.on('connection', (ws) => {
  clients.add(ws);
  if (latest) ws.send(latest);
  ws.on('close', () => clients.delete(ws));
});

function broadcast(line) {
  latest = line;
  for (const ws of clients) {
    if (ws.readyState === 1) ws.send(line);
  }
}

function startSim() {
  const child = spawn(SIM_BIN, [TIME_SCALE], {
    cwd: __dirname,
    stdio: ['ignore', 'pipe', 'inherit'],
  });

  let buffer = '';
  child.stdout.setEncoding('utf8');
  child.stdout.on('data', (chunk) => {
    buffer += chunk;
    let idx;
    while ((idx = buffer.indexOf('\n')) >= 0) {
      const line = buffer.slice(0, idx).trim();
      buffer = buffer.slice(idx + 1);
      if (line) broadcast(line);
    }
  });

  child.on('exit', (code) => {
    console.error(`[staexe-sim] exited (${code}); restarting in 1s`);
    setTimeout(startSim, 1000);
  });

  child.on('error', (err) => {
    console.error('[staexe-sim] failed to start:', err.message);
    console.error('Run `make` in sim/ first.');
  });
}

server.listen(PORT, () => {
  console.log(`STAEXE sim  http://localhost:${PORT}`);
  console.log(`time scale  ${TIME_SCALE}x (set STAEXE_TIME_SCALE to change)`);
  startSim();
});
