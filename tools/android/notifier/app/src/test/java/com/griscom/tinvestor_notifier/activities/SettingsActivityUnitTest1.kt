package com.griscom.tinvestor_notifier.activities

import androidx.compose.ui.test.junit4.createAndroidComposeRule
import androidx.compose.ui.test.onRoot
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
import org.robolectric.RuntimeEnvironment
import org.robolectric.annotation.Config
import org.robolectric.annotation.GraphicsMode

@RunWith(RobolectricTestRunner::class)
@GraphicsMode(GraphicsMode.Mode.NATIVE)
@Config(qualifiers = RobolectricDeviceQualifiers.Pixel9ProXL)
class SettingsActivityUnitTest1 {
    @get:Rule
    val composeTestRule = createAndroidComposeRule<SettingsActivity>()

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

    @Test
    fun init_view() =
        runTest {
            composeTestRule.onRoot().captureRoboImage("SettingsActivityUnitTest/init_view/01_finish.png")
        }

    @Test
    fun dark_theme() =
        runTest {
            RuntimeEnvironment.setQualifiers("+night")

            composeTestRule.onRoot().captureRoboImage("SettingsActivityUnitTest/dark_theme/01_finish.png")
        }
}
