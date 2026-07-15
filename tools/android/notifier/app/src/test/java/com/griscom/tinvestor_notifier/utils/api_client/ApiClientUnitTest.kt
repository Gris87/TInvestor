package com.griscom.tinvestor_notifier.utils.api_client

import androidx.arch.core.executor.testing.InstantTaskExecutorRule
import io.ktor.client.HttpClient
import io.ktor.client.engine.mock.MockEngine
import io.ktor.client.engine.mock.respond
import io.ktor.client.plugins.contentnegotiation.ContentNegotiation
import io.ktor.http.HttpHeaders
import io.ktor.http.HttpStatusCode
import io.ktor.http.headersOf
import io.ktor.serialization.kotlinx.json.json
import io.ktor.utils.io.ByteReadChannel
import kotlinx.coroutines.test.runTest
import org.junit.Assert
import org.junit.Rule
import org.junit.Test
import org.junit.runner.RunWith
import org.robolectric.RobolectricTestRunner

@RunWith(RobolectricTestRunner::class)
class ApiClientUnitTest {
    @get:Rule
    val instantRule = InstantTaskExecutorRule()

    @Test
    fun constructor_Notification() =
        runTest {
            val n = Notification(1704056400000, "system", "AAAAA", "Some log")

            Assert.assertEquals(1704056400000, n.timestamp)
            Assert.assertEquals("system", n.type)
            Assert.assertEquals("AAAAA", n.text)
            Assert.assertEquals("Some log", n.data)
        }

    @Test
    fun constructor_NotificationsResponse() =
        runTest {
            val r = NotificationsResponse(listOf(Notification(1704056400000, "system", "AAAAA", "Some log")))

            Assert.assertEquals(1704056400000, r.notifications[0].timestamp)
            Assert.assertEquals("system", r.notifications[0].type)
            Assert.assertEquals("AAAAA", r.notifications[0].text)
            Assert.assertEquals("Some log", r.notifications[0].data)
        }

    @Test
    fun getMessages() =
        runTest {
            val mockEngine =
                MockEngine {
                    respond(
                        content =
                            ByteReadChannel(
                                """
                                {
                                    "notifications": [
                                        {"timestamp": 1704056400000, "type": "system", "text": "AAAAA", "data": "Some log"},
                                        {"timestamp": 1704056460000, "type": "portfolio", "text": "BBBBB", "data": ""},
                                        {"timestamp": 1704056520000, "type": "huge_sell", "text": "CCCCC", "data": ""},
                                        {"timestamp": 1704056580000, "type": "dividends", "text": "DDDDD", "data": ""},
                                        {"timestamp": 1704056640000, "type": "pulse_neutral", "text": "EEEEE", "data": ""},
                                        {"timestamp": 1704056700000, "type": "pulse_buy", "text": "FFFFF", "data": ""},
                                        {"timestamp": 1704056760000, "type": "pulse_sell", "text": "GGGGG", "data": ""}
                                    ]
                                }
                                """.trimIndent(),
                            ),
                        status = HttpStatusCode.OK,
                        headers = headersOf(HttpHeaders.ContentType, "application/json; charset=utf-8"),
                    )
                }

            val httpClient =
                HttpClient(mockEngine) {
                    install(ContentNegotiation) {
                        json()
                    }
                }

            val apiClient = ApiClient(httpClient)
            val notifications = apiClient.getNotifications("localhost", 8041, 0)

            Assert.assertEquals(7, notifications.size)

            Assert.assertEquals(1704056400000, notifications[0].timestamp)
            Assert.assertEquals("system", notifications[0].type)
            Assert.assertEquals("AAAAA", notifications[0].text)
            Assert.assertEquals("Some log", notifications[0].data)

            Assert.assertEquals(1704056460000, notifications[1].timestamp)
            Assert.assertEquals("portfolio", notifications[1].type)
            Assert.assertEquals("BBBBB", notifications[1].text)
            Assert.assertEquals("", notifications[1].data)

            Assert.assertEquals(1704056520000, notifications[2].timestamp)
            Assert.assertEquals("huge_sell", notifications[2].type)
            Assert.assertEquals("CCCCC", notifications[2].text)
            Assert.assertEquals("", notifications[2].data)

            Assert.assertEquals(1704056580000, notifications[3].timestamp)
            Assert.assertEquals("dividends", notifications[3].type)
            Assert.assertEquals("DDDDD", notifications[3].text)
            Assert.assertEquals("", notifications[3].data)

            Assert.assertEquals(1704056640000, notifications[4].timestamp)
            Assert.assertEquals("pulse_neutral", notifications[4].type)
            Assert.assertEquals("EEEEE", notifications[4].text)
            Assert.assertEquals("", notifications[4].data)

            Assert.assertEquals(1704056700000, notifications[5].timestamp)
            Assert.assertEquals("pulse_buy", notifications[5].type)
            Assert.assertEquals("FFFFF", notifications[5].text)
            Assert.assertEquals("", notifications[5].data)

            Assert.assertEquals(1704056760000, notifications[6].timestamp)
            Assert.assertEquals("pulse_sell", notifications[6].type)
            Assert.assertEquals("GGGGG", notifications[6].text)
            Assert.assertEquals("", notifications[6].data)
        }

    @Test
    fun getMessages_failed_to_parse() =
        runTest {
            val mockEngine =
                MockEngine {
                    respond(
                        content =
                            ByteReadChannel(
                                "{Bad content ::::: 555",
                            ),
                        status = HttpStatusCode.OK,
                        headers = headersOf(HttpHeaders.ContentType, "application/json; charset=utf-8"),
                    )
                }

            val httpClient =
                HttpClient(mockEngine) {
                    install(ContentNegotiation) {
                        json()
                    }
                }

            val apiClient = ApiClient(httpClient)
            val notifications = apiClient.getNotifications("localhost", 8041, 0)

            Assert.assertEquals(0, notifications.size)
        }
}
