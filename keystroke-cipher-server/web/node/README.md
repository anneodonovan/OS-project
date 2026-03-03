# KeyCipher Node.js Bridge

Connects your React frontend to the C backend and kernel module.

## Setup

```bash
npm install
npm run dev        # development with auto-reload
npm start          # production
```

## How to connect your React app

Install socket.io-client in your React project:
```bash
npm install socket.io-client axios
```

Then in your React app:
```javascript
import { io } from 'socket.io-client';
import axios from 'axios';

const socket = io('http://localhost:3001');
const API    = 'http://localhost:3001/api';

// Live stats pushed from Node every 500ms
socket.on('stats', (stats) => {
    setIncomingUsed(stats.incoming_used);
    setIncomingFree(stats.incoming_free);
    setSemaphoreCount(stats.chatroom_free);
});

// A message was read/decrypted - update inbox
socket.on('message_read', ({ id, plaintext }) => {
    setMessages(prev => prev.map(m => m.id === id ? { ...m, plaintext } : m));
});

// READ ALL was clicked - replace entire inbox
socket.on('flush', ({ messages }) => {
    setMessages(messages);
});

// Send a direct message
const sendMessage = async (targetIp, message) => {
    await axios.post(`${API}/send`, { target_ip: targetIp, message });
};

// Read one message (decrypt)
const readMessage = async (id) => {
    await axios.post(`${API}/read/${id}`);
};

// Read all messages
const readAll = async () => {
    await axios.post(`${API}/read/all`);
};

// Send chatroom message
const sendChatroom = async (message) => {
    await axios.post(`${API}/send/chatroom`, { message });
};
```

## Ports
- Node bridge:   http://localhost:3001
- C API backend: http://localhost:8080 (internal only)
- React dev:     http://localhost:5173 (if using Vite)
