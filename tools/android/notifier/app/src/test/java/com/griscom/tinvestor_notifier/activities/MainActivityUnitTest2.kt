package com.griscom.tinvestor_notifier.activities

import android.content.Context
import androidx.activity.ComponentActivity
import androidx.compose.foundation.lazy.LazyListState
import androidx.compose.ui.test.junit4.createAndroidComposeRule
import androidx.compose.ui.test.onNodeWithTag
import androidx.compose.ui.test.onRoot
import androidx.compose.ui.test.performClick
import androidx.compose.ui.test.performTextReplacement
import androidx.room.Room
import androidx.test.core.app.ApplicationProvider
import com.dropbox.differ.SimpleImageComparator
import com.github.takahirom.roborazzi.RobolectricDeviceQualifiers
import com.github.takahirom.roborazzi.RoborazziOptions
import com.github.takahirom.roborazzi.RoborazziRule
import com.github.takahirom.roborazzi.captureRoboImage
import com.griscom.tinvestor_notifier.db.NotificationDao
import com.griscom.tinvestor_notifier.db.NotificationEntity
import com.griscom.tinvestor_notifier.db.NotificationRoomDatabase
import com.griscom.tinvestor_notifier.ui.theme.TInvestorNotifierTheme
import kotlinx.coroutines.flow.first
import kotlinx.coroutines.runBlocking
import kotlinx.coroutines.test.runTest
import org.junit.After
import org.junit.Assert
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
class MainActivityUnitTest2 {
    @get:Rule
    val composeTestRule = createAndroidComposeRule<ComponentActivity>()

    @get:Rule
    val roborazziRule =
        RoborazziRule(
            options =
                RoborazziRule.Options(
                    roborazziOptions =
                        RoborazziOptions(
                            compareOptions =
                                RoborazziOptions.CompareOptions(
                                    changeThreshold = 0.01F,
                                    imageComparator =
                                        SimpleImageComparator(
                                            maxDistance = 0.007F,
                                            vShift = 2,
                                            hShift = 2,
                                        ),
                                ),
                        ),
                ),
        )

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
    fun dynamic_light_theme() =
        runTest {
            composeTestRule.setContent {
                TInvestorNotifierTheme(darkTheme = false, dynamicColor = true) {
                    ConversationContent(notificationDao)
                }
            }

            fillWithTestData()

            composeTestRule.onRoot().captureRoboImage(
                "MainActivityUnitTest/dynamic_light_theme/01_finish.png",
            )
        }

    @Test
    fun dynamic_dark_theme() =
        runTest {
            composeTestRule.setContent {
                TInvestorNotifierTheme(darkTheme = true, dynamicColor = true) {
                    ConversationContent(notificationDao)
                }
            }

            fillWithTestData()

            composeTestRule.onRoot().captureRoboImage(
                "MainActivityUnitTest/dynamic_dark_theme/01_finish.png",
            )
        }

    @Test
    fun light_theme() =
        runTest {
            composeTestRule.setContent {
                TInvestorNotifierTheme(darkTheme = false, dynamicColor = false) {
                    ConversationContent(notificationDao)
                }
            }

            fillWithTestData()

            composeTestRule.onRoot().captureRoboImage(
                "MainActivityUnitTest/light_theme/01_finish.png",
            )
        }

    @Test
    fun dark_theme() =
        runTest {
            composeTestRule.setContent {
                TInvestorNotifierTheme(darkTheme = true, dynamicColor = false) {
                    ConversationContent(notificationDao)
                }
            }

            fillWithTestData()

            composeTestRule.onRoot().captureRoboImage(
                "MainActivityUnitTest/dark_theme/01_finish.png",
            )
        }

    @Test
    fun display_notifications() =
        runTest {
            val listState = LazyListState()

            composeTestRule.setContent {
                TInvestorNotifierTheme {
                    ConversationContent(notificationDao, listState)
                }
            }

            composeTestRule.onRoot().captureRoboImage("MainActivityUnitTest/display_notifications/01_start.png")

            fillWithTestData()

            composeTestRule.onRoot().captureRoboImage("MainActivityUnitTest/display_notifications/02_notifications_added.png")

            composeTestRule.runOnIdle {
                runBlocking {
                    listState.scrollToItem(index = 4)
                }
            }

            composeTestRule.onRoot().captureRoboImage("MainActivityUnitTest/display_notifications/03_scroll_up.png")

            composeTestRule.onNodeWithTag("scroll_to_bottom_button").performClick()

            composeTestRule.onRoot().captureRoboImage("MainActivityUnitTest/display_notifications/04_finish.png")
        }

    @Test
    fun notification_log_button() =
        runTest {
            composeTestRule.setContent {
                TInvestorNotifierTheme {
                    ConversationContent(notificationDao)
                }
            }

            composeTestRule.onRoot().captureRoboImage("MainActivityUnitTest/notification_log_button/01_start.png")

            fillWithTestData()

            composeTestRule.onRoot().captureRoboImage("MainActivityUnitTest/notification_log_button/02_notifications_added.png")

            composeTestRule.onNodeWithTag("notification_3_log_button").performClick()

            composeTestRule.onRoot().captureRoboImage("MainActivityUnitTest/notification_log_button/03_finish.png")
        }

    @Test
    fun top_bar_action_search() =
        runTest {
            composeTestRule.setContent {
                TInvestorNotifierTheme {
                    ConversationContent(notificationDao)
                }
            }

            composeTestRule.onRoot().captureRoboImage("MainActivityUnitTest/top_bar_action_search/01_start.png")

            fillWithTestData()

            composeTestRule.onRoot().captureRoboImage("MainActivityUnitTest/top_bar_action_search/02_notifications_added.png")

            composeTestRule.onNodeWithTag("top_bar_action_search").performClick()

            composeTestRule.onRoot().captureRoboImage("MainActivityUnitTest/top_bar_action_search/03_search_clicked.png")

            composeTestRule.onNodeWithTag("search_field").performTextReplacement("AAAAA")

            composeTestRule.onRoot().captureRoboImage("MainActivityUnitTest/top_bar_action_search/04_search_01.png")

            composeTestRule.onNodeWithTag("search_field").performTextReplacement("CCCCC")

            composeTestRule.onRoot().captureRoboImage("MainActivityUnitTest/top_bar_action_search/05_search_02.png")

            composeTestRule.onNodeWithTag("top_bar_action_search").performClick()

            composeTestRule.onRoot().captureRoboImage("MainActivityUnitTest/top_bar_action_search/06_finish.png")
        }

    suspend fun fillWithTestData() {
        notificationDao.insertNotifications(
            listOf(
                NotificationEntity(
                    1704056400000,
                    "portfolio",
                    "Very very long text. I don't care if you read it\n".repeat(100),
                    "",
                ),
                NotificationEntity(1704232800000, "system", "AAAAA", "Some log"),
                NotificationEntity(1704236400000, "portfolio", "BBBBB", ""),
                NotificationEntity(1704240000000, "huge_sell", "CCCCC", ""),
                NotificationEntity(1704315600000, "dividends", "DDDDD", ""),
                NotificationEntity(1704319200000, "pulse_neutral", "EEEEE", ""),
                NotificationEntity(1704402000000, "pulse_buy", "FFFFF", ""),
                NotificationEntity(1704402600000, "pulse_sell", "GGGGG", ""),
            ),
        )

        val notifications = notificationDao.getNotificationsReversed().first()
        Assert.assertEquals(8, notifications.size)
    }
}
