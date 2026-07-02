package com.griscom.tinvestor_notifier.db

import androidx.room.ColumnInfo
import androidx.room.Entity
import androidx.room.PrimaryKey
import com.griscom.tinvestor_notifier.utils.api_client.Notification

@Entity(tableName = "notifications")
class NotificationEntity {
    @PrimaryKey(autoGenerate = true)
    @ColumnInfo(name = "id")
    var id: Int = 0

    @ColumnInfo(name = "timestamp")
    var timestamp: Long = 0

    @ColumnInfo(name = "type")
    var type: String = ""

    @ColumnInfo(name = "text")
    var text: String = ""

    @ColumnInfo(name = "data")
    var data: String = ""

    constructor() {}

    constructor(timestamp: Long, type: String, text: String, data: String) {
        this.timestamp = timestamp
        this.type = type
        this.text = text
        this.data = data
    }

    constructor(notification: Notification) {
        timestamp = notification.timestamp
        type = notification.type
        text = notification.text
        data = notification.data
    }
}
