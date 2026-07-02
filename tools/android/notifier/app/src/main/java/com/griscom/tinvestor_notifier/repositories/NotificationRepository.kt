package com.griscom.tinvestor_notifier.db

import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.withContext

class NotificationRepository(
    private val userDao: NotificationDao,
) {
    val notificationsList: Flow<List<NotificationEntity>> = userDao.getNotifications()

    suspend fun insertNotifications(notifications: List<NotificationEntity>) {
        userDao.insertNotifications(notifications)
    }

    suspend fun getLastNotificationTimestamp(): Long {
        return withContext(Dispatchers.IO) {
            return@withContext userDao.getLastNotificationTimestamp() ?: 0
        }
    }
}
