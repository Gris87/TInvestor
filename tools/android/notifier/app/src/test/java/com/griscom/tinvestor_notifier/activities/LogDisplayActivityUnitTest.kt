package com.griscom.tinvestor_notifier.activities

import android.content.Context
import android.content.Intent
import androidx.activity.ComponentActivity
import androidx.compose.ui.test.junit4.AndroidComposeTestRule
import androidx.compose.ui.test.onRoot
import androidx.test.core.app.ApplicationProvider
import androidx.test.ext.junit.rules.ActivityScenarioRule
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

inline fun <reified A : ComponentActivity> createAndroidComposeRuleWithIntent(
    intentFactory: (Context) -> Intent,
): AndroidComposeTestRule<ActivityScenarioRule<A>, A> {
    val context = ApplicationProvider.getApplicationContext<Context>()
    val intent = intentFactory(context)
    val activityRule = ActivityScenarioRule<A>(intent)

    return AndroidComposeTestRule(
        activityRule = activityRule,
        activityProvider = { scenarioRule -> scenarioRule.getActivity() },
    )
}

fun <A : ComponentActivity> ActivityScenarioRule<A>.getActivity(): A {
    var activity: A? = null
    scenario.onActivity { activity = it }
    return activity ?: throw IllegalStateException("Activity was not initialized in ActivityScenarioRule!")
}

@RunWith(RobolectricTestRunner::class)
@GraphicsMode(GraphicsMode.Mode.NATIVE)
@Config(qualifiers = RobolectricDeviceQualifiers.Pixel9ProXL)
class LogDisplayActivityUnitTest {
    @get:Rule
    val composeTestRule =
        createAndroidComposeRuleWithIntent<LogDisplayActivity> { context ->
            Intent(context, LogDisplayActivity::class.java).apply {
                putExtra("LOG_KEY", "There are a lot of letters AAAAA. Please ignore them and pay attention to letters BBBBB.\n".repeat(50))
            }
        }

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
            composeTestRule.onRoot().captureRoboImage("LogDisplayActivityUnitTest/init_view/01_finish.png")
        }

    @Test
    fun dark_theme() =
        runTest {
            RuntimeEnvironment.setQualifiers("+night")

            composeTestRule.onRoot().captureRoboImage("LogDisplayActivityUnitTest/dark_theme/01_finish.png")
        }
}
