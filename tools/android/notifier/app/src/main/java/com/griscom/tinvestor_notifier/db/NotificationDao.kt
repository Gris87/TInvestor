package com.griscom.tinvestor_notifier.db

import androidx.room.Dao
import androidx.room.Insert
import androidx.room.Query
import kotlinx.coroutines.flow.Flow

@Dao
interface NotificationDao {
    @Query("SELECT * FROM notifications ORDER BY id DESC")
    fun getNotificationsReversed(): Flow<List<NotificationEntity>>

    @Insert
    suspend fun insertNotifications(notifications: List<NotificationEntity>)

    @Query("SELECT MAX(timestamp) FROM notifications")
    suspend fun getLastNotificationTimestamp(): Long?
}
