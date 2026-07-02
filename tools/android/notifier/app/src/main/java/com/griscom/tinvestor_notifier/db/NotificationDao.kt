package com.griscom.tinvestor_notifier.db

import androidx.room.Dao
import androidx.room.Insert
import androidx.room.Query
import kotlinx.coroutines.flow.Flow

@Dao
interface NotificationDao {
    @Query("SELECT * FROM notifications")
    fun getNotifications(): Flow<List<NotificationEntity>>

    @Insert
    fun insertNotifications(notifications: List<NotificationEntity>)
}
