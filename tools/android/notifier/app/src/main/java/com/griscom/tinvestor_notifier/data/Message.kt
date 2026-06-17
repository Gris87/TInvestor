package com.griscom.tinvestor_notifier.data

enum class MessageType {
    SYSTEM,
    PORTFOLIO,
    HUGE_SELL,
    DIVIDENDS,
    PULSE_NEUTRAL,
    PULSE_BUY,
    PULSE_SELL
}

data class Message(
    var timestamp: String,
    var messageType: MessageType,
    var text: String
)
