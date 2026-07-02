package com.griscom.tinvestor_notifier.services

import android.app.Notification
import android.app.NotificationChannel
import android.app.NotificationManager
import android.app.Service
import android.content.Intent
import android.content.pm.ServiceInfo
import android.os.IBinder
import android.util.Log
import com.griscom.tinvestor_notifier.R
import com.griscom.tinvestor_notifier.db.NotificationEntity
import com.griscom.tinvestor_notifier.db.NotificationRepository
import com.griscom.tinvestor_notifier.db.NotificationRoomDatabase
import com.griscom.tinvestor_notifier.utils.api_client.ApiClient
import io.ktor.client.HttpClient
import io.ktor.client.engine.okhttp.OkHttp
import io.ktor.client.plugins.contentnegotiation.ContentNegotiation
import io.ktor.serialization.kotlinx.json.json
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.SupervisorJob
import kotlinx.coroutines.delay
import kotlinx.coroutines.isActive
import kotlinx.coroutines.launch
import kotlinx.coroutines.runBlocking
import java.security.SecureRandom
import java.security.cert.X509Certificate
import javax.net.ssl.SSLContext
import javax.net.ssl.X509TrustManager
import kotlin.time.Duration.Companion.minutes

private const val TAG = "SyncService"
private const val CHANNEL_ID = "SYNC_CHANNEL_ID"
private const val FOREGROUND_ID = 1

private val INTERVAL = 1.minutes

class SyncService : Service() {
    private val serviceJob = SupervisorJob()
    private val serviceScope = CoroutineScope(Dispatchers.IO + serviceJob)

    private val httpClient =
        HttpClient(OkHttp) {
            engine {
                val trustManager =
                    object : X509TrustManager {
                        override fun checkClientTrusted(
                            chain: Array<out X509Certificate>?,
                            authType: String?,
                        ) {}

                        override fun checkServerTrusted(
                            chain: Array<out X509Certificate>?,
                            authType: String?,
                        ) {}

                        override fun getAcceptedIssuers(): Array<X509Certificate> = arrayOf()
                    }

                val sslContext =
                    SSLContext.getInstance("SSL").apply {
                        init(null, arrayOf(trustManager), SecureRandom())
                    }

                // OkHttp engine allows configuration via its native OkHttpClient.Builder
                config {
                    sslSocketFactory(sslContext.socketFactory, trustManager)
                    hostnameVerifier { _, _ -> true }
                }
            }

            install(ContentNegotiation) {
                json()
            }
        }
    private val apiClient = ApiClient(httpClient)

    override fun onStartCommand(
        intent: Intent?,
        flags: Int,
        startId: Int,
    ): Int {
        val channel =
            NotificationChannel(
                CHANNEL_ID,
                getString(R.string.service_running),
                NotificationManager.IMPORTANCE_LOW,
            )
        val manager = getSystemService(NotificationManager::class.java)
        manager?.createNotificationChannel(channel)

        val notification =
            Notification
                .Builder(this, CHANNEL_ID)
                .setSmallIcon(R.mipmap.ic_launcher)
                .setContentTitle(getString(R.string.app_name))
                .setContentText(getString(R.string.service_running))
                .build()

        startForeground(FOREGROUND_ID, notification, ServiceInfo.FOREGROUND_SERVICE_TYPE_DATA_SYNC)

        serviceScope.launch {
            val repository = NotificationRepository(NotificationRoomDatabase.getInstance(application).NotificationDao())

            while (isActive) {
                fetchData(repository)
                delay(INTERVAL)
            }
        }

        return START_STICKY
    }

    private fun fetchData(repository: NotificationRepository) {
        Log.d(TAG, "fetchData")

        runBlocking {
            val notifications = apiClient.getNotifications()

            val dbNotifications = mutableListOf<NotificationEntity>()

            for (notification in notifications) {
                dbNotifications.add(NotificationEntity(notification))
            }

            repository.insertNotifications(dbNotifications)
        }
    }

    override fun onDestroy() {
        serviceJob.cancel()

        super.onDestroy()
    }

    override fun onBind(intent: Intent?): IBinder? = null
}
