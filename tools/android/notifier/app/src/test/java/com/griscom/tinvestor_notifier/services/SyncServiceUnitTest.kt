package com.griscom.tinvestor_notifier.services

import android.Manifest
import android.app.Application
import android.app.NotificationManager
import android.content.Context
import androidx.arch.core.executor.testing.InstantTaskExecutorRule
import androidx.test.core.app.ApplicationProvider
import com.griscom.tinvestor_notifier.datastore.DataStoreManager
import com.griscom.tinvestor_notifier.db.NotificationRoomDatabase
import com.griscom.tinvestor_notifier.repositories.NotificationRepository
import kotlinx.coroutines.ExperimentalCoroutinesApi
import kotlinx.coroutines.test.advanceTimeBy
import kotlinx.coroutines.test.advanceUntilIdle
import kotlinx.coroutines.test.runCurrent
import kotlinx.coroutines.test.runTest
import org.junit.Rule
import org.junit.Test
import org.junit.runner.RunWith
import org.robolectric.Robolectric
import org.robolectric.RobolectricTestRunner
import org.robolectric.Shadows
import kotlin.time.Duration.Companion.seconds

@RunWith(RobolectricTestRunner::class)
class SyncServiceUnitTest {
    @get:Rule
    val instantRule = InstantTaskExecutorRule()

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
            val repository =
                NotificationRepository(
                    NotificationRoomDatabase.getInstance(context).notificationDao(),
                )
            val notificationManager =
                context.getSystemService(NotificationManager::class.java) as NotificationManager

            dataStore.setShowNotifications(false)
            service.fetchData(dataStore, repository, notificationManager)

            dataStore.setShowNotifications(true)
            service.fetchData(dataStore, repository, notificationManager)

            Shadows
                .shadowOf(ApplicationProvider.getApplicationContext<Application>())
                .grantPermissions(Manifest.permission.POST_NOTIFICATIONS)
            service.fetchData(dataStore, repository, notificationManager)
        }
}
