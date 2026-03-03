const stats = [
  { label: "Messages Sent", value: "1,247" },
  { label: "Messages Received", value: "3,891" },
  { label: "Keys Rotated", value: "42" },
  { label: "Avg Latency", value: "23ms" },
  { label: "Uptime", value: "99.7%" },
  { label: "Active Peers", value: "3" },
];

const StatsView = () => {
  return (
    <div className="grid grid-cols-2 md:grid-cols-3 gap-2">
      {stats.map((stat) => (
        <div key={stat.label} className="bg-card border border-border rounded-sm p-4 text-center">
          <span className="text-2xl font-bold text-foreground">{stat.value}</span>
          <span className="text-[10px] text-muted-foreground uppercase tracking-wider block mt-1">
            {stat.label}
          </span>
        </div>
      ))}
    </div>
  );
};

export default StatsView;
