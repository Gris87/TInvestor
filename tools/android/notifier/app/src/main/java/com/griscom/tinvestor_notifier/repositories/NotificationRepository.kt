package com.griscom.tinvestor_notifier.db

import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.launch

class NotificationRepository(
    private val userDao: NotificationDao,
) {
    private val coroutineScope = CoroutineScope(Dispatchers.Main)

    val notificationsList: Flow<List<NotificationEntity>> = userDao.getNotifications()

    fun insertNotifications(notifications: List<NotificationEntity>) {
        coroutineScope.launch(Dispatchers.IO) {
            userDao.insertNotifications(notifications)
        }
    }
}
