import { useState } from "react";

interface MessageCardProps {
  sender: string;
  timestamp: string;
  encrypted: string;
  plaintext: string;
}

const MessageCard = ({ sender, timestamp, encrypted, plaintext }: MessageCardProps) => {
  const [decrypted, setDecrypted] = useState(false);

  return (
    <div className="bg-card border border-border rounded-sm p-3 flex items-start justify-between gap-3">
      <div className="flex-1 min-w-0">
        <div className="flex items-center gap-2 mb-1.5">
          <span className="text-[10px] text-muted-foreground">{sender}</span>
          <span className="text-[10px] text-muted-foreground">·</span>
          <span className="text-[10px] text-muted-foreground">{timestamp}</span>
        </div>
        <div className="relative">
          <p
            className={`text-xs leading-relaxed break-all transition-opacity duration-300 ${
              decrypted ? "opacity-0 absolute" : "opacity-100"
            } text-muted-foreground`}
          >
            {encrypted}
          </p>
          <p
            className={`text-xs leading-relaxed break-all transition-opacity duration-300 ${
              decrypted ? "opacity-100" : "opacity-0 absolute"
            } text-foreground`}
          >
            {plaintext}
          </p>
        </div>
      </div>
      <button
        onClick={() => setDecrypted(!decrypted)}
        className="shrink-0 text-[10px] border border-border text-muted-foreground hover:text-primary hover:border-primary px-2 py-1 rounded-sm transition-colors"
      >
        {decrypted ? "HIDE" : "READ"}
      </button>
    </div>
  );
};

export default MessageCard;
