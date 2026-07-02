package com.griscom.tinvestor_notifier.db

import androidx.lifecycle.LiveData
import androidx.room.Dao
import androidx.room.Insert
import androidx.room.Query

@Dao
interface NotificationDao {
    @Query("SELECT * FROM notifications")
    fun getNotifications(): LiveData<List<NotificationEntity>>

    @Insert
    fun insertNotifications(notifications: List<NotificationEntity>)
}
