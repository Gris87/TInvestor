package com.griscom.tinvestor_notifier.services

import androidx.arch.core.executor.testing.InstantTaskExecutorRule
import kotlinx.coroutines.ExperimentalCoroutinesApi
import kotlinx.coroutines.test.advanceTimeBy
import kotlinx.coroutines.test.advanceUntilIdle
import kotlinx.coroutines.test.runCurrent
import kotlinx.coroutines.test.runTest
import org.junit.Rule
import org.junit.Test
import org.junit.runner.RunWith
import org.robolectric.Robolectric
import org.robolectric.RobolectricTestRunner
import kotlin.time.Duration.Companion.seconds

@RunWith(RobolectricTestRunner::class)
class SyncServiceUnitTest {
    @get:Rule
    val instantRule = InstantTaskExecutorRule()

    @Test
    fun onStartCommand() =
        runTest {
            val serviceController = Robolectric.buildService(SyncService::class.java)
            serviceController.create().startCommand(0, 0).get()
            serviceController.destroy()
        }

    @Test
    fun onBind() =
        runTest {
            val serviceController = Robolectric.buildService(SyncService::class.java)
            serviceController.create().bind()
        }

    @Test
    @OptIn(ExperimentalCoroutinesApi::class)
    fun startInScope() =
        runTest {
            val serviceController = Robolectric.buildService(SyncService::class.java)
            val service = serviceController.create().get()

            val job =
                service.startInScope(
                    backgroundScope,
                    5.seconds,
                )

            runCurrent()
            advanceTimeBy(5.seconds)

            service.isTerminatedForTesting = true

            advanceUntilIdle()

            job.join()
        }
}
