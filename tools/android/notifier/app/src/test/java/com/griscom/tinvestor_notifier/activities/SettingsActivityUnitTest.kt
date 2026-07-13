package com.griscom.tinvestor_notifier.activities

import androidx.compose.ui.test.junit4.createAndroidComposeRule
import androidx.compose.ui.test.onNodeWithTag
import androidx.compose.ui.test.onRoot
import androidx.compose.ui.test.performClick
import androidx.compose.ui.test.performTextReplacement
import com.github.takahirom.roborazzi.RobolectricDeviceQualifiers
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
class SettingsActivityUnitTest {
    @get:Rule
    val composeTestRule = createAndroidComposeRule<SettingsActivity>()

    @Test
    fun update_preferences() =
        runTest {
            composeTestRule.onRoot().captureRoboImage("SettingsActivityUnitTest/update_preferences/01_start.png")

            composeTestRule.onNodeWithTag("server_address_field").performTextReplacement("yandex.ru")
            composeTestRule.onNodeWithTag("server_port_field").performTextReplacement("8888")

            composeTestRule.onRoot().captureRoboImage("SettingsActivityUnitTest/update_preferences/02_text_fields_updated.png")

            composeTestRule.onNodeWithTag("show_notifications_switch").performClick()

            composeTestRule.onRoot().captureRoboImage("SettingsActivityUnitTest/update_preferences/03_finish.png")
        }
}
