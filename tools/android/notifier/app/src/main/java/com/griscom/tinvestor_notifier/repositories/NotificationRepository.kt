package com.griscom.tinvestor_notifier.repositories

import com.griscom.tinvestor_notifier.db.NotificationDao
import com.griscom.tinvestor_notifier.db.NotificationEntity
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.withContext

class NotificationRepository(
    private val notificationDao: NotificationDao,
) {
    val notificationsListReversed: Flow<List<NotificationEntity>> = notificationDao.getNotificationsReversed()

    suspend fun getNotificationById(notificationId: Int): NotificationEntity {
        return withContext(Dispatchers.IO) {
            return@withContext notificationDao.getNotificationById(notificationId) ?: NotificationEntity()
        }
    }

    suspend fun insertNotifications(notifications: List<NotificationEntity>) {
        notificationDao.insertNotifications(notifications)
    }

    suspend fun getLastNotificationTimestamp(): Long {
        return withContext(Dispatchers.IO) {
            return@withContext notificationDao.getLastNotificationTimestamp() ?: 0
        }
    }
}
