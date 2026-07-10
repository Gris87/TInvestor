package com.griscom.tinvestor_notifier.services

import android.Manifest
import android.annotation.SuppressLint
import android.app.Notification
import android.app.NotificationChannel
import android.app.NotificationManager
import android.app.PendingIntent
import android.app.Service
import android.content.Intent
import android.content.pm.PackageManager
import android.content.pm.ServiceInfo
import android.os.IBinder
import androidx.core.app.ActivityCompat
import com.griscom.tinvestor_notifier.R
import com.griscom.tinvestor_notifier.activities.MainActivity
import com.griscom.tinvestor_notifier.datastore.DataStoreManager
import com.griscom.tinvestor_notifier.db.NotificationEntity
import com.griscom.tinvestor_notifier.db.NotificationRoomDatabase
import com.griscom.tinvestor_notifier.repositories.NotificationRepository
import com.griscom.tinvestor_notifier.utils.api_client.ApiClient
import io.ktor.client.HttpClient
import io.ktor.client.engine.okhttp.OkHttp
import io.ktor.client.plugins.contentnegotiation.ContentNegotiation
import io.ktor.serialization.kotlinx.json.json
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.Job
import kotlinx.coroutines.SupervisorJob
import kotlinx.coroutines.delay
import kotlinx.coroutines.flow.first
import kotlinx.coroutines.isActive
import kotlinx.coroutines.launch
import kotlinx.coroutines.runBlocking
import java.security.SecureRandom
import java.security.cert.X509Certificate
import javax.net.ssl.SSLContext
import javax.net.ssl.X509TrustManager
import kotlin.time.Duration
import kotlin.time.Duration.Companion.minutes

private const val SYNC_CHANNEL_ID = "SYNC_CHANNEL_ID"
private const val MESSAGES_CHANNEL_ID = "MESSAGES_CHANNEL_ID"

private val INTERVAL = 1.minutes

class SyncService : Service() {
    private val serviceJob = SupervisorJob()
    private val serviceScope = CoroutineScope(Dispatchers.IO + serviceJob)

    var isTerminatedForTesting = false

    private val httpClient =
        HttpClient(OkHttp) {
            engine {
                val trustManager =
                    @SuppressLint("CustomX509TrustManager")
                    object : X509TrustManager {
                        @SuppressLint("TrustAllX509TrustManager")
                        override fun checkClientTrusted(
                            chain: Array<out X509Certificate>?,
                            authType: String?,
                        ) {}

                        @SuppressLint("TrustAllX509TrustManager")
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
        startInScope(serviceScope, INTERVAL)

        return START_STICKY
    }

    fun startInScope(
        scope: CoroutineScope,
        interval: Duration,
    ): Job =
        scope.launch {
            val dataStore =
                DataStoreManager(applicationContext)

            val syncChannel =
                NotificationChannel(
                    SYNC_CHANNEL_ID,
                    getString(R.string.channel_service_running),
                    NotificationManager.IMPORTANCE_NONE,
                ).apply {
                    description = getString(R.string.channel_description_service_running)
                }
            val messagesChannel =
                NotificationChannel(
                    MESSAGES_CHANNEL_ID,
                    getString(R.string.channel_messages),
                    NotificationManager.IMPORTANCE_HIGH,
                ).apply {
                    description = getString(R.string.channel_description_messages)
                }

            val notificationManager =
                getSystemService(NotificationManager::class.java) as NotificationManager
            notificationManager.createNotificationChannel(syncChannel)
            notificationManager.createNotificationChannel(messagesChannel)

            val applicationIntent =
                Intent(applicationContext, MainActivity::class.java).setFlags(
                    Intent.FLAG_ACTIVITY_NEW_TASK or Intent.FLAG_ACTIVITY_CLEAR_TASK,
                )
            val pendingIntent: PendingIntent =
                PendingIntent.getActivity(
                    applicationContext,
                    0,
                    applicationIntent,
                    PendingIntent.FLAG_IMMUTABLE,
                )

            val notification =
                Notification
                    .Builder(applicationContext, SYNC_CHANNEL_ID)
                    .setSmallIcon(R.mipmap.ic_launcher)
                    .setContentTitle(getString(R.string.app_name))
                    .setContentText(getString(R.string.service_running))
                    .setContentIntent(pendingIntent)
                    .build()

            val uniqueNotificationId = (System.currentTimeMillis() and 0xfffffff).toInt()
            startForeground(
                uniqueNotificationId,
                notification,
                ServiceInfo.FOREGROUND_SERVICE_TYPE_DATA_SYNC,
            )

            val repository =
                NotificationRepository(
                    NotificationRoomDatabase.getInstance(application).notificationDao(),
                )

            while (isActive && !isTerminatedForTesting) {
                fetchData(repository, dataStore, notificationManager)
                delay(interval)
            }
        }

    private fun fetchData(
        repository: NotificationRepository,
        dataStore: DataStoreManager,
        notificationManager: NotificationManager,
    ) {
        runBlocking {
            val serverAddress = dataStore.serverAddress.first()
            val serverPort = dataStore.serverPort.first()
            val isShowNotifications = dataStore.isShowNotifications.first()
            val filter = dataStore.filter.first()

            val lastNotificationTimestamp =
                repository.getLastNotificationTimestamp()

            val notifications = apiClient.getNotifications(serverAddress, serverPort, lastNotificationTimestamp + 10)

            val dbNotifications =
                mutableListOf<NotificationEntity>().apply {
                    addAll(notifications.map { NotificationEntity(it) })
                }

            repository.insertNotifications(dbNotifications)

            if (!isShowNotifications) {
                return@runBlocking
            }

            if (ActivityCompat.checkSelfPermission(
                    applicationContext,
                    Manifest.permission.POST_NOTIFICATIONS,
                ) != PackageManager.PERMISSION_GRANTED
            ) {
                return@runBlocking
            }

            val applicationIntent =
                Intent(applicationContext, MainActivity::class.java).setFlags(
                    Intent.FLAG_ACTIVITY_NEW_TASK or Intent.FLAG_ACTIVITY_CLEAR_TASK,
                )
            val pendingIntent: PendingIntent =
                PendingIntent.getActivity(applicationContext, 0, applicationIntent, PendingIntent.FLAG_IMMUTABLE)

            for (n in notifications) {
                if (n.type in filter) {
                    val notification =
                        Notification
                            .Builder(applicationContext, MESSAGES_CHANNEL_ID)
                            .setSmallIcon(R.mipmap.ic_launcher)
                            .setContentTitle(getString(R.string.app_name))
                            .setContentText(n.text)
                            .setStyle(
                                Notification
                                    .BigTextStyle()
                                    .bigText(n.text),
                            ).setContentIntent(pendingIntent)
                            .setAutoCancel(true)
                            .build()

                    val uniqueNotificationId = (System.currentTimeMillis() and 0xfffffff).toInt()
                    notificationManager.notify(uniqueNotificationId, notification)
                }
            }
        }
    }

    override fun onDestroy() {
        serviceJob.cancel()

        super.onDestroy()
    }

    override fun onBind(intent: Intent?): IBinder? = null
}
