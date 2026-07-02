package com.griscom.tinvestor_notifier.db

import android.content.Context
import androidx.room.Database
import androidx.room.Room
import androidx.room.RoomDatabase

@Database(entities = [(NotificationEntity::class)], version = 1)
abstract class NotificationRoomDatabase : RoomDatabase() {
    abstract fun notificationDao(): NotificationDao

    companion object {
        private var instance: NotificationRoomDatabase? = null

        fun getInstance(context: Context): NotificationRoomDatabase {
            synchronized(this) {
                var res = instance

                if (res == null) {
                    res =
                        Room
                            .databaseBuilder(
                                context.applicationContext,
                                NotificationRoomDatabase::class.java,
                                "notifications.db",
                            ).fallbackToDestructiveMigration(false)
                            .build()
                    instance = res
                }

                return res
            }
        }
    }
}
