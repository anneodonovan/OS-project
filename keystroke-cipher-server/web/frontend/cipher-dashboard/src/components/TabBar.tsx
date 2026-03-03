interface TabBarProps {
  activeTab: string;
  onTabChange: (tab: string) => void;
}

const tabs = ["Direct Messages", "Chatroom", "Stats"];

const TabBar = ({ activeTab, onTabChange }: TabBarProps) => {
  return (
    <div className="flex border-b border-border">
      {tabs.map((tab) => (
        <button
          key={tab}
          onClick={() => onTabChange(tab)}
          className={`px-4 py-2.5 text-xs tracking-wide transition-colors relative ${
            activeTab === tab
              ? "text-primary"
              : "text-muted-foreground hover:text-foreground"
          }`}
        >
          {tab}
          {activeTab === tab && (
            <span className="absolute bottom-0 left-0 right-0 h-px bg-primary" />
          )}
        </button>
      ))}
    </div>
  );
};

export default TabBar;
