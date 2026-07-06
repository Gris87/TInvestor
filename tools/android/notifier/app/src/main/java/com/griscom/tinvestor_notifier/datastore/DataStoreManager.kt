package com.griscom.tinvestor_notifier.datastore

import android.content.Context
import androidx.datastore.preferences.core.booleanPreferencesKey
import androidx.datastore.preferences.core.edit
import androidx.datastore.preferences.core.intPreferencesKey
import androidx.datastore.preferences.core.stringPreferencesKey
import androidx.datastore.preferences.preferencesDataStore
import kotlinx.coroutines.flow.map

val Context.preferencesDataStore by preferencesDataStore(name = "user_settings")

class DataStoreManager(
    val context: Context,
) {
    companion object {
        val SERVER_ADDRESS = stringPreferencesKey("SERVER_ADDRESS")
        val SERVER_PORT = intPreferencesKey("SERVER_PORT")
        val SHOW_NOTIFICATIONS = booleanPreferencesKey("SHOW_NOTIFICATIONS")
        val FILTER = stringPreferencesKey("FILTER")
    }

    val serverAddress =
        context.preferencesDataStore.data
            .map { preferences ->
                preferences[SERVER_ADDRESS] ?: "localhost"
            }

    val serverPort =
        context.preferencesDataStore.data
            .map { preferences ->
                preferences[SERVER_PORT] ?: 8041
            }

    val isShowNotifications =
        context.preferencesDataStore.data
            .map { preferences ->
                preferences[SHOW_NOTIFICATIONS] ?: true
            }

    val filter =
        context.preferencesDataStore.data
            .map { preferences ->
                preferences[FILTER] ?: "system;portfolio;huge_sell;dividends"
            }

    suspend fun setServerAddress(address: String) {
        context.preferencesDataStore.edit { preferences ->
            preferences[SERVER_ADDRESS] = address
        }
    }

    suspend fun setServerPort(port: Int) {
        context.preferencesDataStore.edit { preferences ->
            preferences[SERVER_PORT] = port
        }
    }

    suspend fun setShowNotifications(enabled: Boolean) {
        context.preferencesDataStore.edit { preferences ->
            preferences[SHOW_NOTIFICATIONS] = enabled
        }
    }

    suspend fun setFilter(filter: String) {
        context.preferencesDataStore.edit { preferences ->
            preferences[FILTER] = filter
        }
    }
}
