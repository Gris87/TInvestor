package com.griscom.tinvestor_notifier.services

import android.Manifest
import android.app.Application
import android.app.NotificationManager
import android.content.Context
import androidx.arch.core.executor.testing.InstantTaskExecutorRule
import androidx.room.Room
import androidx.test.core.app.ApplicationProvider
import com.griscom.tinvestor_notifier.datastore.DataStoreManager
import com.griscom.tinvestor_notifier.db.NotificationRoomDatabase
import com.griscom.tinvestor_notifier.repositories.NotificationRepository
import kotlinx.coroutines.ExperimentalCoroutinesApi
import kotlinx.coroutines.flow.first
import kotlinx.coroutines.test.advanceTimeBy
import kotlinx.coroutines.test.advanceUntilIdle
import kotlinx.coroutines.test.runCurrent
import kotlinx.coroutines.test.runTest
import mockwebserver3.MockResponse
import mockwebserver3.MockWebServer
import okhttp3.tls.HandshakeCertificates
import okhttp3.tls.HeldCertificate
import org.junit.After
import org.junit.Assert
import org.junit.Before
import org.junit.Rule
import org.junit.Test
import org.junit.runner.RunWith
import org.robolectric.Robolectric
import org.robolectric.RobolectricTestRunner
import org.robolectric.Shadows
import java.net.InetAddress
import kotlin.time.Duration.Companion.seconds

@RunWith(RobolectricTestRunner::class)
class SyncServiceUnitTest {
    @get:Rule
    val instantRule = InstantTaskExecutorRule()

    private lateinit var db: NotificationRoomDatabase
    private lateinit var mockWebServer: MockWebServer

    @Before
    fun setup() {
        val context = ApplicationProvider.getApplicationContext<Context>()

        db =
            Room
                .inMemoryDatabaseBuilder(
                    context,
                    NotificationRoomDatabase::class.java,
                ).allowMainThreadQueries()
                .build()

        val localhostCertificate =
            HeldCertificate
                .Builder()
                .addSubjectAlternativeName(InetAddress.getByName("localhost").getCanonicalHostName())
                .build()

        val serverCertificates =
            HandshakeCertificates
                .Builder()
                .heldCertificate(localhostCertificate)
                .build()

        mockWebServer =
            MockWebServer().apply {
                useHttps(serverCertificates.sslSocketFactory())
            }
        mockWebServer.start()
    }

    @After
    fun tearDown() {
        db.close()
        mockWebServer.close()
    }

    @Test
    fun onStartCommand() =
        runTest {
            val serviceController = Robolectric.buildService(SyncService::class.java)
            serviceController.create().startCommand(0, 0).get()
            serviceController.destroy()
        }

    @Test
    fun onBind() =
        runTest {
            val serviceController = Robolectric.buildService(SyncService::class.java)
            serviceController.create().bind()
        }

    @Test
    @OptIn(ExperimentalCoroutinesApi::class)
    fun startInScope() =
        runTest {
            val serviceController = Robolectric.buildService(SyncService::class.java)
            val service = serviceController.create().get()

            val job =
                service.startInScope(
                    backgroundScope,
                    5.seconds,
                )

            runCurrent()
            advanceTimeBy(5.seconds)

            service.isTerminatedForTesting = true

            advanceUntilIdle()

            job.join()
        }

    @Test
    fun fetchData() =
        runTest {
            val serviceController = Robolectric.buildService(SyncService::class.java)
            val service = serviceController.create().get()

            val context = ApplicationProvider.getApplicationContext<Context>()
            val dataStore = DataStoreManager(context)
            val repository = NotificationRepository(db.notificationDao())
            val notificationManager =
                context.getSystemService(NotificationManager::class.java) as NotificationManager

            dataStore.setShowNotifications(false)
            service.fetchData(dataStore, repository, notificationManager)

            dataStore.setShowNotifications(true)
            service.fetchData(dataStore, repository, notificationManager)

            dataStore.setServerPort(mockWebServer.port)

            val mockResponse =
                MockResponse
                    .Builder()
                    .code(200)
                    .body(
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
                    ).addHeader("Content-Type", "application/json; charset=utf-8")
                    .build()
            mockWebServer.enqueue(mockResponse)

            Shadows
                .shadowOf(ApplicationProvider.getApplicationContext<Application>())
                .grantPermissions(Manifest.permission.POST_NOTIFICATIONS)
            service.fetchData(dataStore, repository, notificationManager)

            val req = mockWebServer.takeRequest()
            Assert.assertEquals("GET /notifications?from=10 HTTP/2", req.requestLine)

            val notifications = repository.notificationsListReversed.first()

            Assert.assertEquals(7, notifications.size)

            Assert.assertEquals(1704056760000, notifications[0].timestamp)
            Assert.assertEquals("pulse_sell", notifications[0].type)
            Assert.assertEquals("GGGGG", notifications[0].text)
            Assert.assertEquals("", notifications[0].data)

            Assert.assertEquals(1704056700000, notifications[1].timestamp)
            Assert.assertEquals("pulse_buy", notifications[1].type)
            Assert.assertEquals("FFFFF", notifications[1].text)
            Assert.assertEquals("", notifications[1].data)

            Assert.assertEquals(1704056640000, notifications[2].timestamp)
            Assert.assertEquals("pulse_neutral", notifications[2].type)
            Assert.assertEquals("EEEEE", notifications[2].text)
            Assert.assertEquals("", notifications[2].data)

            Assert.assertEquals(1704056580000, notifications[3].timestamp)
            Assert.assertEquals("dividends", notifications[3].type)
            Assert.assertEquals("DDDDD", notifications[3].text)
            Assert.assertEquals("", notifications[3].data)

            Assert.assertEquals(1704056520000, notifications[4].timestamp)
            Assert.assertEquals("huge_sell", notifications[4].type)
            Assert.assertEquals("CCCCC", notifications[4].text)
            Assert.assertEquals("", notifications[4].data)

            Assert.assertEquals(1704056460000, notifications[5].timestamp)
            Assert.assertEquals("portfolio", notifications[5].type)
            Assert.assertEquals("BBBBB", notifications[5].text)
            Assert.assertEquals("", notifications[5].data)

            Assert.assertEquals(1704056400000, notifications[6].timestamp)
            Assert.assertEquals("system", notifications[6].type)
            Assert.assertEquals("AAAAA", notifications[6].text)
            Assert.assertEquals("Some log", notifications[6].data)
        }
}
