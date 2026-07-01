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
    suspend fun getMessages() {
        val notificationsResponse: NotificationsResponse =
            httpClient.get("https://88.218.66.61:8041/notifications").body()

        print(notificationsResponse)
    }
}
