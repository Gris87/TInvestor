package com.griscom.tinvestor_notifier.activities

import android.content.Context
import androidx.compose.ui.test.junit4.createAndroidComposeRule
import androidx.compose.ui.test.onNodeWithTag
import androidx.compose.ui.test.onRoot
import androidx.compose.ui.test.performClick
import androidx.test.core.app.ApplicationProvider
import com.github.takahirom.roborazzi.RobolectricDeviceQualifiers
import com.github.takahirom.roborazzi.captureRoboImage
import com.griscom.tinvestor_notifier.db.NotificationDao
import com.griscom.tinvestor_notifier.db.NotificationEntity
import com.griscom.tinvestor_notifier.db.NotificationRoomDatabase
import kotlinx.coroutines.launch
import kotlinx.coroutines.test.advanceUntilIdle
import kotlinx.coroutines.test.runTest
import org.junit.After
import org.junit.Before
import org.junit.Rule
import org.junit.Test
import org.junit.runner.RunWith
import org.robolectric.RobolectricTestRunner
import org.robolectric.annotation.Config
import org.robolectric.annotation.GraphicsMode

@RunWith(RobolectricTestRunner::class)
@GraphicsMode(GraphicsMode.Mode.NATIVE)
@Config(qualifiers = RobolectricDeviceQualifiers.Pixel9ProXL)
class MainActivityUnitTest {
    @get:Rule
    val composeTestRule = createAndroidComposeRule<MainActivity>()

    private lateinit var db: NotificationRoomDatabase
    private lateinit var notificationDao: NotificationDao

    @Before
    fun setup() {
        val context = ApplicationProvider.getApplicationContext<Context>()

        db = NotificationRoomDatabase.getInstance(context)
        notificationDao = db.notificationDao()
    }

    @After
    fun tearDown() {
        db.close()
    }

    @Test
    fun init_view() =
        runTest {
            composeTestRule.onRoot().captureRoboImage("MainActivityUnitTest/init_view/01_finish.png")
        }

    @Test
    fun display_notifications() =
        runTest {
            composeTestRule.onRoot().captureRoboImage("MainActivityUnitTest/display_notifications/01_start.png")

            launch {
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
            }
            advanceUntilIdle()

            composeTestRule.onRoot().captureRoboImage("MainActivityUnitTest/display_notifications/02_finish.png")
        }

    @Test
    fun top_bar_action_search() =
        runTest {
            composeTestRule.onRoot().captureRoboImage("MainActivityUnitTest/top_bar_action_search/01_start.png")

            composeTestRule.onNodeWithTag("top_bar_action_search").performClick()

            composeTestRule.onRoot().captureRoboImage("MainActivityUnitTest/top_bar_action_search/02_finish.png")
        }

    @Test
    fun top_bar_action_settings() =
        runTest {
            composeTestRule.onRoot().captureRoboImage("MainActivityUnitTest/top_bar_action_settings/01_start.png")

            composeTestRule.onNodeWithTag("top_bar_action_settings").performClick()

            composeTestRule.onRoot().captureRoboImage("MainActivityUnitTest/top_bar_action_settings/02_finish.png")
        }
}
