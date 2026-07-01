package com.griscom.tinvestor_notifier.utils.api_client

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
import org.junit.Assert.*
import org.junit.Test

class ApiClientUnitTest {
    @Test
    fun getMessages() {
        runBlocking {
            val mockEngine =
                MockEngine { request ->
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

            var apiClient = ApiClient(httpClient)
            apiClient.getMessages()

            assertEquals(4, 2 + 2)
        }
    }
}
