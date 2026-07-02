package com.griscom.tinvestor_notifier.data

import com.griscom.tinvestor_notifier.R

enum class MessageType {
    SYSTEM,
    PORTFOLIO,
    HUGE_SELL,
    DIVIDENDS,
    PULSE_NEUTRAL,
    PULSE_BUY,
    PULSE_SELL,
}

val messageTypeToTypeMap =
    mapOf(
        "system" to MessageType.SYSTEM,
        "portfolio" to MessageType.PORTFOLIO,
        "huge_sell" to MessageType.HUGE_SELL,
        "dividends" to MessageType.DIVIDENDS,
        "pulse_neutral" to MessageType.PULSE_NEUTRAL,
        "pulse_buy" to MessageType.PULSE_BUY,
        "pulse_sell" to MessageType.PULSE_SELL,
    )

val messageTypeToStringMap =
    mapOf(
        "system" to R.string.message_type_system,
        "portfolio" to R.string.message_type_portfolio,
        "huge_sell" to R.string.message_type_huge_sell,
        "dividends" to R.string.message_type_dividends,
        "pulse_neutral" to R.string.message_type_pulse_neutral,
        "pulse_buy" to R.string.message_type_pulse_buy,
        "pulse_sell" to R.string.message_type_pulse_sell,
    )

data class Message(
    var timestamp: String,
    var messageType: MessageType,
    var messageTypeString: Int,
    var text: String,
    var data: String,
)
