/**
 * server.js - Node.js bridge between React frontend and C backend API
 *
 * This server:
 * 1. Serves the React frontend (or proxies to React dev server)
 * 2. Forwards REST API calls to the C backend (userspace/api/)
 * 3. Uses Socket.IO to push live updates to the React frontend
 *    so the frontend doesn't need to poll - updates are pushed
 *
 * Architecture:
 *   React <──WebSocket──> Node.js <──HTTP──> C API server
 *                                <──/proc──> /proc/keycipher/stats (direct read)
 */

const express    = require('express');
const http       = require('http');
const { Server } = require('socket.io');
const cors       = require('cors');
const axios      = require('axios');
const fs         = require('fs');

const app        = express();
const httpServer = http.createServer(app);
const io         = new Server(httpServer, { cors: { origin: '*' } });

const C_BACKEND_URL = 'http://localhost:8080'; /* C api_server.c port  */
const PROC_STATS    = '/proc/keycipher/stats';
const PORT          = 3001;

app.use(cors());
app.use(express.json());

/* ─── REST API Routes (proxied to C backend) ────────────────────────── */

/**
 * GET /api/stats
 * - read /proc/keycipher/stats directly from filesystem (faster than HTTP)
 * - parse each line into a key-value object
 * - return as JSON to React frontend
 * - also emit 'stats' event over Socket.IO so all connected clients update
 *
 * TODO:
 * - fs.readFile(PROC_STATS)
 * - parse lines: "incoming_used: 3" → { incoming_used: 3 }
 * - res.json(parsedStats)
 * - io.emit('stats', parsedStats)
 */
app.get('/api/stats', async (req, res) => {
    res.json({ message: 'TODO: read /proc/keycipher/stats' });
});

/**
 * GET /api/messages
 * - forward request to C backend GET /api/messages
 * - return JSON list of encrypted messages in incoming FIFO
 *
 * TODO:
 * - const response = await axios.get(`${C_BACKEND_URL}/api/messages`)
 * - res.json(response.data)
 */
app.get('/api/messages', async (req, res) => {
    res.json({ messages: [] });
});

/**
 * GET /api/chatroom
 * - forward to C backend GET /api/chatroom
 * - includes current semaphore count for live counter in UI
 *
 * TODO:
 * - axios.get to C backend
 * - res.json(response.data)
 */
app.get('/api/chatroom', async (req, res) => {
    res.json({ messages: [], semaphore_count: 0 });
});

/**
 * POST /api/read/:id
 * - forward to C backend POST /api/read/:id
 * - C backend triggers ioctl on /dev/keycipher_in for this message
 * - kernel decrypts and frees one FIFO slot
 * - emit 'message_read' and updated 'stats' over Socket.IO
 *   so all browser windows update immediately without polling
 *
 * TODO:
 * - axios.post to C backend
 * - io.emit('message_read', { id: req.params.id, plaintext })
 * - io.emit('stats', updatedStats)
 * - res.json(response.data)
 */
app.post('/api/read/:id', async (req, res) => {
    res.json({ message: 'TODO: trigger ioctl read' });
});

/**
 * POST /api/read/all
 * - forward to C backend POST /api/read/all
 * - C backend triggers KEYCIPHER_FLUSH_IN ioctl
 * - all messages decrypted, all slots freed, all blocked peers unblock
 * - emit 'flush' event over Socket.IO with all decrypted messages
 *
 * TODO:
 * - axios.post to C backend
 * - io.emit('flush', { messages: decryptedMessages })
 * - io.emit('stats', { incoming_used: 0, incoming_free: 16, ... })
 * - res.json(response.data)
 */
app.post('/api/read/all', async (req, res) => {
    res.json({ message: 'TODO: trigger KEYCIPHER_FLUSH_IN' });
});

/**
 * POST /api/send
 * - forward to C backend POST /api/send
 * - body: { target_ip, message }
 * - C backend writes to /dev/keycipher_out (kernel encrypts)
 * - emit 'message_sent' event over Socket.IO with encrypted preview
 *
 * TODO:
 * - axios.post to C backend with req.body
 * - io.emit('message_sent', { target_ip, encrypted_preview })
 * - res.json(response.data)
 */
app.post('/api/send', async (req, res) => {
    res.json({ message: 'TODO: forward to C backend send' });
});

/**
 * POST /api/send/chatroom
 * - forward to C backend POST /api/send/chatroom
 * - body: { message }
 * - emit 'chatroom_message' over Socket.IO with encrypted preview
 *
 * TODO:
 * - axios.post to C backend
 * - io.emit('chatroom_message', { sender: 'me', encrypted_preview })
 * - res.json(response.data)
 */
app.post('/api/send/chatroom', async (req, res) => {
    res.json({ message: 'TODO: forward chatroom send' });
});

/* ─── Socket.IO - push live stats to React every 500ms ─────────────── */

/**
 * statsPollingLoop - reads /proc/keycipher/stats every 500ms
 * and emits 'stats' to all connected Socket.IO clients
 * This drives the live buffer bar and semaphore counter in the React UI
 *
 * TODO:
 * setInterval(() => {
 *   fs.readFile(PROC_STATS, 'utf8', (err, data) => {
 *     if (err) return;
 *     const stats = parseStats(data);   // "key: val\n" → { key: val }
 *     io.emit('stats', stats);
 *   });
 * }, 500);
 */
function statsPollingLoop() {
    /* TODO: implement 500ms polling loop */
}

/**
 * parseStats - parse /proc/keycipher/stats text into a JS object
 * - split by newline
 * - each line: "incoming_used: 3" → key="incoming_used", val=3
 * - return { incoming_used: 3, incoming_free: 13, ... }
 *
 * TODO: implement line parsing
 */
function parseStats(rawText) {
    /* TODO: implement stats parser */
    return {};
}

io.on('connection', (socket) => {
    /**
     * TODO:
     * - log socket.id connected
     * - immediately emit current stats to this new client
     * - socket.on('disconnect', () => log disconnected)
     */
    console.log('React client connected:', socket.id);
});

/* ─── Start Server ───────────────────────────────────────────────────── */
httpServer.listen(PORT, () => {
    console.log(`KeyCipher Node bridge running on port ${PORT}`);
    statsPollingLoop();
});
