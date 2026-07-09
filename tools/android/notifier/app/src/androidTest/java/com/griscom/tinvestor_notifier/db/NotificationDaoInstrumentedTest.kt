package com.griscom.tinvestor_notifier.db

import android.content.Context
import androidx.arch.core.executor.testing.InstantTaskExecutorRule
import androidx.room.Room
import androidx.test.core.app.ApplicationProvider
import androidx.test.ext.junit.runners.AndroidJUnit4
import kotlinx.coroutines.flow.first
import kotlinx.coroutines.test.runTest
import org.junit.After
import org.junit.Assert
import org.junit.Before
import org.junit.Rule
import org.junit.Test
import org.junit.runner.RunWith

@RunWith(AndroidJUnit4::class)
class NotificationDaoInstrumentedTest {
    @get:Rule
    val instantRule = InstantTaskExecutorRule()

    private lateinit var db: NotificationRoomDatabase
    private lateinit var notificationDao: NotificationDao

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
        notificationDao = db.notificationDao()
    }

    @After
    fun tearDown() {
        db.close()
    }

    @Test
    fun insertNotifications_and_getNotificationsReversed() =
        runTest {
            notificationDao.insertNotifications(
                listOf(
                    NotificationEntity(1704056400000, "system", "AAAAA", "Some log"),
                    NotificationEntity(1704056460000, "portfolio", "BBBBB", ""),
                    NotificationEntity(1704056520000, "huge_sell", "CCCCC", ""),
                    NotificationEntity(1704056580000, "dividends", "DDDDD", ""),
                    NotificationEntity(1704056640000, "pulse_neutral", "EEEEE", ""),
                    NotificationEntity(1704056700000, "pulse_buy", "FFFFF", ""),
                    NotificationEntity(1704056760000, "pulse_sell", "GGGGG", ""),
                ),
            )

            val notifications = notificationDao.getNotificationsReversed().first()

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

    @Test
    fun getLastNotificationTimestamp() =
        runTest {
            Assert.assertEquals(null, notificationDao.getLastNotificationTimestamp())

            notificationDao.insertNotifications(
                listOf(
                    NotificationEntity(1704056400000, "system", "AAAAA", "Some log"),
                    NotificationEntity(1704056460000, "portfolio", "BBBBB", ""),
                    NotificationEntity(1704056520000, "huge_sell", "CCCCC", ""),
                    NotificationEntity(1704056580000, "dividends", "DDDDD", ""),
                    NotificationEntity(1704056640000, "pulse_neutral", "EEEEE", ""),
                    NotificationEntity(1704056700000, "pulse_buy", "FFFFF", ""),
                    NotificationEntity(1704056760000, "pulse_sell", "GGGGG", ""),
                ),
            )

            Assert.assertEquals(1704056760000, notificationDao.getLastNotificationTimestamp())
        }
}
