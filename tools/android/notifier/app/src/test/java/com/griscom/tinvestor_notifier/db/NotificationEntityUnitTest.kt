package com.griscom.tinvestor_notifier.db

import com.griscom.tinvestor_notifier.utils.api_client.Notification
import kotlinx.coroutines.test.runTest
import org.junit.Assert
import org.junit.Test

class NotificationEntityUnitTest {
    @Test
    fun constructor_NotificationEntity_default() =
        runTest {
            val n = NotificationEntity()

            Assert.assertEquals(0, n.timestamp)
            Assert.assertEquals("", n.type)
            Assert.assertEquals("", n.text)
            Assert.assertEquals("", n.data)
        }

    @Test
    fun constructor_NotificationEntity_with_parameters() =
        runTest {
            val n = NotificationEntity(1704056400000, "system", "AAAAA", "Some log")

            Assert.assertEquals(1704056400000, n.timestamp)
            Assert.assertEquals("system", n.type)
            Assert.assertEquals("AAAAA", n.text)
            Assert.assertEquals("Some log", n.data)
        }

    @Test
    fun constructor_NotificationEntity_from_Notification() =
        runTest {
            val n = NotificationEntity(Notification(1704056400000, "system", "AAAAA", "Some log"))

            Assert.assertEquals(1704056400000, n.timestamp)
            Assert.assertEquals("system", n.type)
            Assert.assertEquals("AAAAA", n.text)
            Assert.assertEquals("Some log", n.data)
        }
}
