import { useState } from "react";

const fakeEncrypt = (text: string) => {
  if (!text) return "";
  return Array.from(text)
    .map((c) => c.charCodeAt(0).toString(16).padStart(2, "0"))
    .join("");
};

const ComposeArea = () => {
  const [message, setMessage] = useState("");

  return (
    <div className="bg-card border border-border rounded-sm p-3 flex flex-col gap-2">
      <textarea
        value={message}
        onChange={(e) => setMessage(e.target.value)}
        placeholder="Type message..."
        rows={4}
        className="w-full bg-background border border-border rounded-sm p-2 text-xs text-foreground placeholder:text-muted-foreground resize-none focus:outline-none focus:border-primary transition-colors"
      />
      {message && (
        <p className="text-[10px] text-muted-foreground break-all leading-relaxed">
          {fakeEncrypt(message)}
        </p>
      )}
      <button className="w-full bg-primary text-primary-foreground py-2 rounded-sm text-xs tracking-wide hover:opacity-90 transition-opacity">
        SEND
      </button>
    </div>
  );
};

export default ComposeArea;
