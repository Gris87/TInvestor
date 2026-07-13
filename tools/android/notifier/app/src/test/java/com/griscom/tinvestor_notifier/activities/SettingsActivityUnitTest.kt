package com.griscom.tinvestor_notifier.activities

import android.content.Context
import androidx.compose.ui.test.junit4.createAndroidComposeRule
import androidx.compose.ui.test.onNodeWithTag
import androidx.compose.ui.test.onRoot
import androidx.compose.ui.test.performClick
import androidx.compose.ui.test.performTextReplacement
import androidx.test.core.app.ApplicationProvider
import com.github.takahirom.roborazzi.RobolectricDeviceQualifiers
import com.github.takahirom.roborazzi.captureRoboImage
import com.griscom.tinvestor_notifier.datastore.DataStoreManager
import kotlinx.coroutines.flow.first
import kotlinx.coroutines.test.runTest
import org.junit.Assert
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
            val context = ApplicationProvider.getApplicationContext<Context>()
            val dataStore = DataStoreManager(context)

            Assert.assertEquals("localhost", dataStore.serverAddress.first())
            Assert.assertEquals(8041, dataStore.serverPort.first())
            Assert.assertEquals(true, dataStore.isShowNotifications.first())
            Assert.assertEquals(listOf("system", "portfolio", "huge_sell", "dividends"), dataStore.filter.first())

            composeTestRule.onRoot().captureRoboImage("SettingsActivityUnitTest/update_preferences/01_start.png")

            composeTestRule.onNodeWithTag("server_address_field").performTextReplacement("yandex.ru")
            composeTestRule.waitForIdle()
            // Assert.assertEquals("yandex.ru", dataStore.serverAddress.first())

            composeTestRule.onRoot().captureRoboImage("SettingsActivityUnitTest/update_preferences/02_text_fields_updated_01.png")

            composeTestRule.onNodeWithTag("server_address_field").performTextReplacement("localhost")
            composeTestRule.waitForIdle()
            // Assert.assertEquals("localhost", dataStore.serverAddress.first())

            composeTestRule.onRoot().captureRoboImage("SettingsActivityUnitTest/update_preferences/03_text_fields_updated_02.png")

            composeTestRule.onNodeWithTag("server_port_field").performTextReplacement("8888")
            composeTestRule.waitForIdle()
            // Assert.assertEquals(8888, dataStore.serverPort.first())

            composeTestRule.onRoot().captureRoboImage("SettingsActivityUnitTest/update_preferences/04_text_fields_updated_03.png")

            composeTestRule.onNodeWithTag("server_port_field").performTextReplacement("8041")
            composeTestRule.waitForIdle()
            // Assert.assertEquals(8041, dataStore.serverPort.first())

            composeTestRule.onRoot().captureRoboImage("SettingsActivityUnitTest/update_preferences/05_text_fields_updated_04.png")

            composeTestRule.onNodeWithTag("show_notifications_switch").performClick()
            composeTestRule.waitForIdle()
            // Assert.assertEquals(false, dataStore.isShowNotifications.first())

            composeTestRule.onRoot().captureRoboImage("SettingsActivityUnitTest/update_preferences/06_show_notifications_toggled_01.png")

            composeTestRule.onNodeWithTag("show_notifications_switch").performClick()
            composeTestRule.waitForIdle()
            // Assert.assertEquals(true, dataStore.isShowNotifications.first())

            composeTestRule.onRoot().captureRoboImage("SettingsActivityUnitTest/update_preferences/07_show_notifications_toggled_02.png")

            composeTestRule.onRoot().captureRoboImage("SettingsActivityUnitTest/update_preferences/08_finish.png")
        }
}
