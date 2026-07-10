package com.griscom.tinvestor_notifier

import android.os.Build
import androidx.arch.core.executor.testing.InstantTaskExecutorRule
import androidx.test.platform.app.InstrumentationRegistry
import kotlinx.coroutines.test.runTest
import org.junit.Assert
import org.junit.Rule
import org.junit.Test
import org.junit.runner.RunWith
import org.robolectric.RobolectricTestRunner
import org.robolectric.annotation.Config

@RunWith(RobolectricTestRunner::class)
@Config(application = TInvestorNotifierApplication::class, sdk = [Build.VERSION_CODES.BAKLAVA])
class TInvestorNotifierApplicationUnitTest {
    @get:Rule
    val instantRule = InstantTaskExecutorRule()

    @Test
    fun useAppContext() =
        runTest {
            val appContext = InstrumentationRegistry.getInstrumentation().targetContext
            Assert.assertEquals("com.griscom.tinvestor_notifier", appContext.packageName)
        }
}
