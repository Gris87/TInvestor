package com.griscom.tinvestor_notifier.db

import androidx.arch.core.executor.testing.InstantTaskExecutorRule
import androidx.test.platform.app.InstrumentationRegistry
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
            val appContext = InstrumentationRegistry.getInstrumentation().targetContext

            val db = NotificationRoomDatabase.getInstance(appContext)
            db.close()
        }
}
