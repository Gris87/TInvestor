package com.griscom.tinvestor_notifier.activities

import android.content.Context
import androidx.activity.ComponentActivity
import androidx.compose.ui.test.junit4.createAndroidComposeRule
import androidx.compose.ui.test.onRoot
import androidx.room.Room
import androidx.test.core.app.ApplicationProvider
import com.dropbox.differ.SimpleImageComparator
import com.github.takahirom.roborazzi.RobolectricDeviceQualifiers
import com.github.takahirom.roborazzi.RoborazziOptions
import com.github.takahirom.roborazzi.RoborazziRule
import com.github.takahirom.roborazzi.captureRoboImage
import com.griscom.tinvestor_notifier.db.NotificationEntity
import com.griscom.tinvestor_notifier.db.NotificationRoomDatabase
import com.griscom.tinvestor_notifier.repositories.NotificationRepository
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
class LogDisplayActivityUnitTest2 {
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
                                            vShift = 1,
                                            hShift = 1,
                                        ),
                                ),
                        ),
                ),
        )

    private lateinit var db: NotificationRoomDatabase
    private lateinit var repository: NotificationRepository

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
        repository = NotificationRepository(db.notificationDao())

        runBlocking {
            fillWithTestData()
        }
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
                    LogDisplayContent(repository, 1)
                }
            }

            composeTestRule.onRoot().captureRoboImage(
                "LogDisplayActivityUnitTest/dynamic_light_theme/01_finish.png",
            )
        }

    @Test
    fun dynamic_dark_theme() =
        runTest {
            composeTestRule.setContent {
                TInvestorNotifierTheme(darkTheme = true, dynamicColor = true) {
                    LogDisplayContent(repository, 1)
                }
            }

            composeTestRule.onRoot().captureRoboImage(
                "LogDisplayActivityUnitTest/dynamic_dark_theme/01_finish.png",
            )
        }

    @Test
    fun light_theme() =
        runTest {
            composeTestRule.setContent {
                TInvestorNotifierTheme(darkTheme = false, dynamicColor = false) {
                    LogDisplayContent(repository, 1)
                }
            }

            composeTestRule.onRoot().captureRoboImage(
                "LogDisplayActivityUnitTest/light_theme/01_finish.png",
            )
        }

    @Test
    fun dark_theme() =
        runTest {
            composeTestRule.setContent {
                TInvestorNotifierTheme(darkTheme = true, dynamicColor = false) {
                    LogDisplayContent(repository, 1)
                }
            }

            composeTestRule.onRoot().captureRoboImage(
                "LogDisplayActivityUnitTest/dark_theme/01_finish.png",
            )
        }

    suspend fun fillWithTestData() {
        repository.insertNotifications(
            listOf(
                NotificationEntity(
                    1704056400000,
                    "system",
                    "AAAAA",
                    "There are a lot of letters AAAAA. Please ignore them and pay attention to letters BBBBB.\n".repeat(50),
                ),
            ),
        )

        val notifications = repository.notificationsListReversed.first()
        Assert.assertEquals(1, notifications.size)
    }
}
