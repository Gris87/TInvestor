package com.griscom.tinvestor_notifier.utils.api_client

import com.griscom.tinvestor_notifier.R
import com.griscom.tinvestor_notifier.data.Message
import com.griscom.tinvestor_notifier.data.MessageType
import io.ktor.client.HttpClient
import io.ktor.client.call.body
import io.ktor.client.request.get
import kotlinx.serialization.Serializable
import java.time.Instant
import java.time.ZoneId
import java.time.format.DateTimeFormatter

@Serializable
data class Notification(
    val timestamp: Long,
    val type: String,
    val text: String,
    val data: String,
)

@Serializable
data class NotificationsResponse(
    val notifications: List<Notification>,
)

class ApiClient(
    private val httpClient: HttpClient,
) {
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

    suspend fun getMessages(): List<Message> {
        val notificationsResponse: NotificationsResponse =
            httpClient.get("https://88.218.66.61:8041/notifications").body()

        val res = mutableListOf<Message>()

        for (notification in notificationsResponse.notifications) {
            val instant = Instant.ofEpochMilli(notification.timestamp)
            val formatter =
                DateTimeFormatter
                    .ofPattern("yyyy-MM-dd HH:mm:ss")
                    .withZone(ZoneId.systemDefault())
            res.add(
                Message(
                    formatter.format(instant),
                    messageTypeToTypeMap.getValue(notification.type),
                    messageTypeToStringMap.getValue(notification.type),
                    notification.text,
                    notification.data,
                ),
            )
        }

        return res
    }
}
