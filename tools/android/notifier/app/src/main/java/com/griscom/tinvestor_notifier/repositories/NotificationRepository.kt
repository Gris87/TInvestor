package com.griscom.tinvestor_notifier.repositories

import com.griscom.tinvestor_notifier.db.NotificationDao
import com.griscom.tinvestor_notifier.db.NotificationEntity
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.withContext

class NotificationRepository(
    private val userDao: NotificationDao,
) {
    val notificationsListReversed: Flow<List<NotificationEntity>> = userDao.getNotificationsReversed()

    suspend fun insertNotifications(notifications: List<NotificationEntity>) {
        userDao.insertNotifications(notifications)
    }

    suspend fun getLastNotificationTimestamp(): Long {
        return withContext(Dispatchers.IO) {
            return@withContext userDao.getLastNotificationTimestamp() ?: 0
        }
    }
}
