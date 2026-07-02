package com.griscom.tinvestor_notifier.db

import androidx.lifecycle.LiveData
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch

class NotificationRepository(
    private val userDao: NotificationDao,
) {
    private val coroutineScope = CoroutineScope(Dispatchers.Main)

    val notificationsList: LiveData<List<NotificationEntity>> = userDao.getNotifications()

    fun insertNotifications(notifications: List<NotificationEntity>) {
        coroutineScope.launch(Dispatchers.IO) {
            userDao.insertNotifications(notifications)
        }
    }
}
