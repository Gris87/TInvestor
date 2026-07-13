package com.griscom.tinvestor_notifier.datastore

import android.content.Context
import androidx.arch.core.executor.testing.InstantTaskExecutorRule
import androidx.test.core.app.ApplicationProvider
import kotlinx.coroutines.flow.first
import kotlinx.coroutines.test.runTest
import org.junit.Assert
import org.junit.Before
import org.junit.Rule
import org.junit.Test
import org.junit.runner.RunWith
import org.robolectric.RobolectricTestRunner

@RunWith(RobolectricTestRunner::class)
class DataStoreManagerUnitTest {
    @get:Rule
    val instantRule = InstantTaskExecutorRule()

    private lateinit var dataStore: DataStoreManager

    @Before
    fun setup() {
        val context = ApplicationProvider.getApplicationContext<Context>()
        dataStore = DataStoreManager(context)
    }

    @Test
    fun serverAddress() =
        runTest {
            Assert.assertEquals("localhost", dataStore.serverAddress.first())
            dataStore.setServerAddress("yandex.ru")
            Assert.assertEquals("yandex.ru", dataStore.serverAddress.first())
            dataStore.setServerAddress("localhost")
            Assert.assertEquals("localhost", dataStore.serverAddress.first())
        }

    @Test
    fun serverPort() =
        runTest {
            Assert.assertEquals(8041, dataStore.serverPort.first())
            dataStore.setServerPort(8888)
            Assert.assertEquals(8888, dataStore.serverPort.first())
            dataStore.setServerPort(8041)
            Assert.assertEquals(8041, dataStore.serverPort.first())
        }

    @Test
    fun isShowNotifications() =
        runTest {
            Assert.assertEquals(true, dataStore.isShowNotifications.first())
            dataStore.setShowNotifications(false)
            Assert.assertEquals(false, dataStore.isShowNotifications.first())
            dataStore.setShowNotifications(true)
            Assert.assertEquals(true, dataStore.isShowNotifications.first())
        }

    @Test
    fun filter() =
        runTest {
            Assert.assertEquals(listOf("system", "portfolio", "huge_sell", "dividends"), dataStore.filter.first())
            dataStore.setFilter(listOf("portfolio"))
            Assert.assertEquals(listOf("portfolio"), dataStore.filter.first())
            dataStore.setFilter(listOf("system", "portfolio", "huge_sell", "dividends"))
            Assert.assertEquals(listOf("system", "portfolio", "huge_sell", "dividends"), dataStore.filter.first())
        }
}
