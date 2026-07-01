package com.griscom.tinvestor_notifier.utils.api_client

import com.griscom.tinvestor_notifier.R
import com.griscom.tinvestor_notifier.data.MessageType
import io.ktor.client.HttpClient
import io.ktor.client.engine.mock.MockEngine
import io.ktor.client.engine.mock.respond
import io.ktor.client.plugins.contentnegotiation.ContentNegotiation
import io.ktor.http.HttpHeaders
import io.ktor.http.HttpStatusCode
import io.ktor.http.headersOf
import io.ktor.serialization.kotlinx.json.json
import io.ktor.utils.io.ByteReadChannel
import kotlinx.coroutines.runBlocking
import org.junit.Assert
import org.junit.Test

class ApiClientUnitTest {
    @Test
    fun getMessages() {
        runBlocking {
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
                        headers = headersOf(HttpHeaders.ContentType, "application/json"),
                    )
                }

            val httpClient =
                HttpClient(mockEngine) {
                    install(ContentNegotiation) {
                        json()
                    }
                }

            val apiClient = ApiClient(httpClient)
            val messages = apiClient.getMessages()

            Assert.assertEquals(7, messages.size)

            Assert.assertEquals("2024-01-01 00:00:00", messages[0].timestamp)
            Assert.assertEquals(MessageType.SYSTEM, messages[0].messageType)
            Assert.assertEquals(R.string.message_type_system, messages[0].messageTypeString)
            Assert.assertEquals("AAAAA", messages[0].text)
            Assert.assertEquals("Some log", messages[0].data)

            Assert.assertEquals("2024-01-01 00:01:00", messages[1].timestamp)
            Assert.assertEquals(MessageType.PORTFOLIO, messages[1].messageType)
            Assert.assertEquals(R.string.message_type_portfolio, messages[1].messageTypeString)
            Assert.assertEquals("BBBBB", messages[1].text)
            Assert.assertEquals("", messages[1].data)

            Assert.assertEquals("2024-01-01 00:02:00", messages[2].timestamp)
            Assert.assertEquals(MessageType.HUGE_SELL, messages[2].messageType)
            Assert.assertEquals(R.string.message_type_huge_sell, messages[2].messageTypeString)
            Assert.assertEquals("CCCCC", messages[2].text)
            Assert.assertEquals("", messages[2].data)

            Assert.assertEquals("2024-01-01 00:03:00", messages[3].timestamp)
            Assert.assertEquals(MessageType.DIVIDENDS, messages[3].messageType)
            Assert.assertEquals(R.string.message_type_dividends, messages[3].messageTypeString)
            Assert.assertEquals("DDDDD", messages[3].text)
            Assert.assertEquals("", messages[3].data)

            Assert.assertEquals("2024-01-01 00:04:00", messages[4].timestamp)
            Assert.assertEquals(MessageType.PULSE_NEUTRAL, messages[4].messageType)
            Assert.assertEquals(R.string.message_type_pulse_neutral, messages[4].messageTypeString)
            Assert.assertEquals("EEEEE", messages[4].text)
            Assert.assertEquals("", messages[4].data)

            Assert.assertEquals("2024-01-01 00:05:00", messages[5].timestamp)
            Assert.assertEquals(MessageType.PULSE_BUY, messages[5].messageType)
            Assert.assertEquals(R.string.message_type_pulse_buy, messages[5].messageTypeString)
            Assert.assertEquals("FFFFF", messages[5].text)
            Assert.assertEquals("", messages[5].data)

            Assert.assertEquals("2024-01-01 00:06:00", messages[6].timestamp)
            Assert.assertEquals(MessageType.PULSE_SELL, messages[6].messageType)
            Assert.assertEquals(R.string.message_type_pulse_sell, messages[6].messageTypeString)
            Assert.assertEquals("GGGGG", messages[6].text)
            Assert.assertEquals("", messages[6].data)
        }
    }
}
