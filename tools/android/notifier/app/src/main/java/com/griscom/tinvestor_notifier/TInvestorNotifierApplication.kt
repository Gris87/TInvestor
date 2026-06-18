package com.griscom.tinvestor_notifier

import android.app.Application
import android.content.Intent
import com.griscom.tinvestor_notifier.services.SyncService

class TInvestorNotifierApplication: Application() {
    override fun onCreate() {
        super.onCreate()

        startForegroundService(Intent(this, SyncService::class.java))
    }
}
