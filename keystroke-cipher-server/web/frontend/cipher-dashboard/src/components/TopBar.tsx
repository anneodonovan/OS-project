interface PeerBadge {
  ip: string;
  status: "online" | "idle" | "offline";
}

const peers: PeerBadge[] = [
  { ip: "192.168.1.42", status: "online" },
  { ip: "10.0.0.17", status: "online" },
  { ip: "172.16.0.8", status: "idle" },
  { ip: "10.0.0.23", status: "offline" },
];

const statusColor: Record<string, string> = {
  online: "bg-success",
  idle: "bg-warning",
  offline: "bg-destructive",
};

const TopBar = () => {
  return (
    <div className="w-full bg-topbar border-b border-border px-4 py-3 flex items-center justify-between">
      <span className="text-sm font-semibold tracking-widest text-foreground uppercase">
        KeyCipher
      </span>
      <div className="flex items-center gap-2">
        {peers.map((peer) => (
          <div
            key={peer.ip}
            className="flex items-center gap-1.5 bg-secondary px-2.5 py-1 rounded-sm text-xs text-secondary-foreground"
          >
            <span className={`w-1.5 h-1.5 rounded-full ${statusColor[peer.status]}`} />
            <span>{peer.ip}</span>
          </div>
        ))}
      </div>
    </div>
  );
};

export default TopBar;
