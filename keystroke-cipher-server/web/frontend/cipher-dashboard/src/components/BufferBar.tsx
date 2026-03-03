interface BufferBarProps {
  percentage: number;
  slots: number;
  totalSlots: number;
}

const BufferBar = ({ percentage, slots, totalSlots }: BufferBarProps) => {
  return (
    <div className="bg-card border border-border rounded-sm p-3">
      <div className="flex items-center justify-between mb-1.5">
        <span className="text-[10px] text-muted-foreground uppercase tracking-wider">Buffer</span>
        <span className="text-xs text-foreground">{percentage}%</span>
      </div>
      <div className="w-full h-1.5 bg-muted rounded-sm overflow-hidden">
        <div
          className="h-full bg-primary transition-all duration-500"
          style={{ width: `${percentage}%` }}
        />
      </div>
      <span className="text-[10px] text-muted-foreground mt-1.5 block">
        {slots} / {totalSlots} slots
      </span>
    </div>
  );
};

export default BufferBar;
