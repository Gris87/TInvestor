package com.griscom.tinvestor_notifier

import android.os.Build
import androidx.test.platform.app.InstrumentationRegistry
import org.junit.Assert
import org.junit.Test
import org.junit.runner.RunWith
import org.robolectric.RobolectricTestRunner
import org.robolectric.annotation.Config

@RunWith(RobolectricTestRunner::class)
@Config(application = TInvestorNotifierApplication::class, sdk = [Build.VERSION_CODES.BAKLAVA])
class TInvestorNotifierApplicationUnitTest {
    @Test
    fun useAppContext() {
        val appContext = InstrumentationRegistry.getInstrumentation().targetContext
        Assert.assertEquals("org.robolectric.default", appContext.packageName)
    }
}
