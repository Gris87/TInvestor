package com.griscom.tinvestor_notifier.activities

import androidx.compose.ui.test.junit4.createAndroidComposeRule
import androidx.compose.ui.test.onNodeWithTag
import androidx.compose.ui.test.onRoot
import androidx.compose.ui.test.performClick
import com.dropbox.differ.SimpleImageComparator
import com.github.takahirom.roborazzi.RobolectricDeviceQualifiers
import com.github.takahirom.roborazzi.RoborazziOptions
import com.github.takahirom.roborazzi.RoborazziRule
import com.github.takahirom.roborazzi.captureRoboImage
import kotlinx.coroutines.test.runTest
import org.junit.Rule
import org.junit.Test
import org.junit.runner.RunWith
import org.robolectric.RobolectricTestRunner
import org.robolectric.annotation.Config
import org.robolectric.annotation.GraphicsMode

@RunWith(RobolectricTestRunner::class)
@GraphicsMode(GraphicsMode.Mode.NATIVE)
@Config(qualifiers = RobolectricDeviceQualifiers.Pixel9ProXL)
class MainActivityUnitTest1 {
    @get:Rule
    val composeTestRule = createAndroidComposeRule<MainActivity>()

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

    @Test
    fun init_view() =
        runTest {
            composeTestRule.onRoot().captureRoboImage("MainActivityUnitTest/init_view/01_finish.png")
        }

    @Test
    fun top_bar_action_settings() =
        runTest {
            composeTestRule.onRoot().captureRoboImage("MainActivityUnitTest/top_bar_action_settings/01_start.png")

            composeTestRule.onNodeWithTag("top_bar_action_settings").performClick()

            composeTestRule.onRoot().captureRoboImage("MainActivityUnitTest/top_bar_action_settings/02_finish.png")
        }
}
