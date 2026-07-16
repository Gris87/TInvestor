package com.griscom.tinvestor_notifier.activities

import androidx.compose.ui.test.junit4.createAndroidComposeRule
import androidx.compose.ui.test.onNodeWithTag
import androidx.compose.ui.test.onRoot
import androidx.compose.ui.test.performClick
import androidx.compose.ui.test.performTextReplacement
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
class SettingsActivityUnitTest {
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
                                            maxDistance = 0.07F,
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

    @Test
    fun update_preferences() =
        runTest {
            composeTestRule.onRoot().captureRoboImage("SettingsActivityUnitTest/update_preferences/01_start.png")

            composeTestRule.onNodeWithTag("server_address_field").performTextReplacement("yandex.ru")

            composeTestRule.onRoot().captureRoboImage("SettingsActivityUnitTest/update_preferences/02_text_fields_updated_01.png")

            composeTestRule.onNodeWithTag("server_address_field").performTextReplacement("localhost")

            composeTestRule.onRoot().captureRoboImage("SettingsActivityUnitTest/update_preferences/03_text_fields_updated_02.png")

            composeTestRule.onNodeWithTag("server_port_field").performTextReplacement("8888")

            composeTestRule.onRoot().captureRoboImage("SettingsActivityUnitTest/update_preferences/04_text_fields_updated_03.png")

            composeTestRule.onNodeWithTag("server_port_field").performTextReplacement("8041")

            composeTestRule.onRoot().captureRoboImage("SettingsActivityUnitTest/update_preferences/05_text_fields_updated_04.png")

            composeTestRule.onNodeWithTag("show_notifications_switch").performClick()

            composeTestRule.onRoot().captureRoboImage("SettingsActivityUnitTest/update_preferences/06_show_notifications_toggled_01.png")

            composeTestRule.onNodeWithTag("show_notifications_switch").performClick()

            composeTestRule.onRoot().captureRoboImage("SettingsActivityUnitTest/update_preferences/07_show_notifications_toggled_02.png")

            composeTestRule.onNodeWithTag("filter_select_all_checkbox").performClick()

            composeTestRule.onRoot().captureRoboImage("SettingsActivityUnitTest/update_preferences/08_filter_select_all_toggled_01.png")

            composeTestRule.onNodeWithTag("filter_select_all_checkbox").performClick()

            composeTestRule.onRoot().captureRoboImage("SettingsActivityUnitTest/update_preferences/09_filter_select_all_toggled_02.png")

            composeTestRule.onNodeWithTag("filter_system_checkbox").performClick()

            composeTestRule.onRoot().captureRoboImage("SettingsActivityUnitTest/update_preferences/10_filter_toggled_01.png")

            composeTestRule.onNodeWithTag("filter_portfolio_checkbox").performClick()

            composeTestRule.onRoot().captureRoboImage("SettingsActivityUnitTest/update_preferences/11_filter_toggled_02.png")

            composeTestRule.onNodeWithTag("filter_huge_sell_checkbox").performClick()

            composeTestRule.onRoot().captureRoboImage("SettingsActivityUnitTest/update_preferences/12_filter_toggled_03.png")

            composeTestRule.onNodeWithTag("filter_dividends_checkbox").performClick()

            composeTestRule.onRoot().captureRoboImage("SettingsActivityUnitTest/update_preferences/13_filter_toggled_04.png")

            composeTestRule.onNodeWithTag("filter_pulse_neutral_checkbox").performClick()

            composeTestRule.onRoot().captureRoboImage("SettingsActivityUnitTest/update_preferences/14_filter_toggled_05.png")

            composeTestRule.onNodeWithTag("filter_pulse_buy_checkbox").performClick()

            composeTestRule.onRoot().captureRoboImage("SettingsActivityUnitTest/update_preferences/15_filter_toggled_06.png")

            composeTestRule.onNodeWithTag("filter_pulse_sell_checkbox").performClick()

            composeTestRule.onRoot().captureRoboImage("SettingsActivityUnitTest/update_preferences/16_filter_toggled_07.png")

            composeTestRule.onNodeWithTag("filter_pulse_neutral_checkbox").performClick()
            composeTestRule.onNodeWithTag("filter_pulse_buy_checkbox").performClick()
            composeTestRule.onNodeWithTag("filter_pulse_sell_checkbox").performClick()

            composeTestRule.onRoot().captureRoboImage("SettingsActivityUnitTest/update_preferences/17_finish.png")
        }
}
