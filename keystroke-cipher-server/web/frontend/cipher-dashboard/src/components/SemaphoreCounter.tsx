interface SemaphoreCounterProps {
  count: number;
}

const SemaphoreCounter = ({ count }: SemaphoreCounterProps) => {
  const isZero = count === 0;

  return (
    <div className="flex flex-col items-center justify-center py-16">
      <span
        className={`text-7xl font-bold tabular-nums transition-colors ${
          isZero ? "text-destructive" : "text-foreground"
        }`}
      >
        {count}
      </span>
      <span className="text-xs text-muted-foreground uppercase tracking-widest mt-3">
        Semaphore Count
      </span>
    </div>
  );
};

export default SemaphoreCounter;
