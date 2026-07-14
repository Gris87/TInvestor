package com.griscom.tinvestor_notifier.db

import android.content.Context
import androidx.arch.core.executor.testing.InstantTaskExecutorRule
import androidx.test.core.app.ApplicationProvider
import kotlinx.coroutines.test.runTest
import org.junit.Rule
import org.junit.Test
import org.junit.runner.RunWith
import org.robolectric.RobolectricTestRunner

@RunWith(RobolectricTestRunner::class)
class NotificationRoomDatabaseUnitTest {
    @get:Rule
    val instantRule = InstantTaskExecutorRule()

    @Test
    fun getInstance() =
        runTest {
            val context = ApplicationProvider.getApplicationContext<Context>()

            val db = NotificationRoomDatabase.getInstance(context)
            db.close()
        }
}
