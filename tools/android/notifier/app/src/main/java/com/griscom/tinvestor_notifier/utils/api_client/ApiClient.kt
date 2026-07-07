package com.griscom.tinvestor_notifier.utils.api_client

import io.ktor.client.HttpClient
import io.ktor.client.call.body
import io.ktor.client.request.get
import kotlinx.serialization.Serializable

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
    suspend fun getNotifications(
        serverAddress: String,
        serverPort: Int,
        from: Long,
    ): List<Notification> {
        var res: List<Notification> = emptyList()

        try {
            val notificationsResponse: NotificationsResponse =
                httpClient.get("https://$serverAddress:$serverPort/notifications?from=$from").body()

            res = notificationsResponse.notifications
        } catch (_: Exception) {
        }

        return res
    }
}
