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
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.SupervisorJob
import kotlinx.coroutines.delay
import kotlinx.coroutines.isActive
import kotlinx.coroutines.launch
import kotlin.time.Duration.Companion.minutes

private const val TAG = "SyncService"
private const val CHANNEL_ID = "SYNC_CHANNEL_ID"
private const val FOREGROUND_ID = 1

private val INTERVAL = 1.minutes

class SyncService : Service() {
    private val serviceJob = SupervisorJob()
    private val serviceScope = CoroutineScope(Dispatchers.IO + serviceJob)

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
            while (isActive) {
                fetchData()
                delay(INTERVAL)
            }
        }

        return START_STICKY
    }

    private fun fetchData() {
        Log.d(TAG, "fetchData")
    }

    override fun onDestroy() {
        serviceJob.cancel()

        super.onDestroy()
    }

    override fun onBind(intent: Intent?): IBinder? = null
}
