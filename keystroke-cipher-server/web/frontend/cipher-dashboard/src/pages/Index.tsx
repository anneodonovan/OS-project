import { useState } from "react";
import TopBar from "@/components/TopBar";
import TabBar from "@/components/TabBar";
import MessageList from "@/components/MessageList";
import ComposeArea from "@/components/ComposeArea";
import BufferBar from "@/components/BufferBar";
import SemaphoreCounter from "@/components/SemaphoreCounter";
import StatsView from "@/components/StatsView";

const Index = () => {
  const [activeTab, setActiveTab] = useState("Direct Messages");

  return (
    <div className="min-h-screen flex flex-col bg-background">
      <TopBar />
      <TabBar activeTab={activeTab} onTabChange={setActiveTab} />

      <div className="flex-1 p-4">
        {activeTab === "Direct Messages" && (
          <div className="flex flex-col lg:flex-row gap-4 max-w-6xl mx-auto">
            <div className="flex-1 lg:flex-[2]">
              <MessageList />
            </div>
            <div className="lg:flex-1 flex flex-col gap-3">
              <ComposeArea />
              <BufferBar percentage={67} slots={134} totalSlots={200} />
            </div>
          </div>
        )}

        {activeTab === "Chatroom" && (
          <div className="flex flex-col lg:flex-row gap-4 max-w-6xl mx-auto">
            <div className="flex-1 lg:flex-[2] flex flex-col gap-4">
              <SemaphoreCounter count={7} />
              <MessageList />
            </div>
            <div className="lg:flex-1 flex flex-col gap-3">
              <ComposeArea />
              <BufferBar percentage={43} slots={86} totalSlots={200} />
            </div>
          </div>
        )}

        {activeTab === "Stats" && (
          <div className="max-w-4xl mx-auto">
            <StatsView />
          </div>
        )}
      </div>
    </div>
  );
};

export default Index;
