import { useState, useEffect } from 'react'

export default function useStats() {
  const [stats, setStats] = useState(null)

  useEffect(() => {
    // poll GET /api/stats every 500ms
    // response: { incoming_used, incoming_free, outgoing_used, outgoing_free,
    //             chatroom_slots_free, total_sent, total_received, total_decrypted }
    // setStats(data) on each response
    // return clearInterval cleanup on unmount
  }, [])

  return stats
}