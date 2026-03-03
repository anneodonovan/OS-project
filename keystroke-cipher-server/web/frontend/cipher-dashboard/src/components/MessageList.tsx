import { useState } from "react";
import MessageCard from "./MessageCard";

const mockMessages = [
  {
    id: 1,
    sender: "node-alpha",
    timestamp: "2026-03-03 14:22:07",
    encrypted: "a3f8c1d9e2b4a7c6f0d1e5b8a9c3f2d7e6b0a4c8f1d5e9b2a6c0f3d7e1b5a8",
    plaintext: "Rendezvous at checkpoint delta. Confirm receipt.",
  },
  {
    id: 2,
    sender: "node-bravo",
    timestamp: "2026-03-03 14:23:41",
    encrypted: "d7e1b5a8c3f2d9e6b0a4c8f1d5e9b2a6c0f3a3f8c1d9e2b4a7c6f0d1e5b8a9",
    plaintext: "Receipt confirmed. ETA 15 minutes.",
  },
  {
    id: 3,
    sender: "node-charlie",
    timestamp: "2026-03-03 14:25:12",
    encrypted: "b2a6c0f3d7e1b5a8c3f2d9e6b0a4c8f1d5e9a3f8c1d9e2b4a7c6f0d1e5b8a9c3",
    plaintext: "Key rotation complete. New cipher active on all nodes.",
  },
  {
    id: 4,
    sender: "node-alpha",
    timestamp: "2026-03-03 14:27:55",
    encrypted: "f0d1e5b8a9c3f2d7e6b0a4c8f1d5e9b2a6c0f3d7e1b5a8a3f8c1d9e2b4a7c6",
    plaintext: "Buffer overflow detected on relay-3. Rerouting traffic.",
  },
];

const MessageList = () => {
  const [allDecrypted, setAllDecrypted] = useState(false);

  return (
    <div className="flex flex-col gap-2">
      {mockMessages.map((msg) => (
        <MessageCard
          key={msg.id}
          sender={msg.sender}
          timestamp={msg.timestamp}
          encrypted={msg.encrypted}
          plaintext={msg.plaintext}
        />
      ))}
      <button
        onClick={() => setAllDecrypted(!allDecrypted)}
        className="w-full border border-border text-muted-foreground hover:text-primary hover:border-primary py-2 rounded-sm text-xs tracking-wide transition-colors mt-1"
      >
        READ ALL
      </button>
    </div>
  );
};

export default MessageList;
