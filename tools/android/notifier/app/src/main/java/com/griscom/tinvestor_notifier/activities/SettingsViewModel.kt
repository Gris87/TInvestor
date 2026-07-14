package com.griscom.tinvestor_notifier.activities

import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.griscom.tinvestor_notifier.datastore.DataStoreManager
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asStateFlow
import kotlinx.coroutines.flow.collectLatest
import kotlinx.coroutines.launch

class SettingsViewModel(
    private val dataStoreManager: DataStoreManager,
) : ViewModel() {
    private val _serverAddress = MutableStateFlow("localhost")
    private val _serverPort = MutableStateFlow(8041)
    private val _isShowNotifications = MutableStateFlow(true)
    private val _filter = MutableStateFlow(listOf("system", "portfolio", "huge_sell", "dividends"))

    val serverAddress: StateFlow<String> = _serverAddress.asStateFlow()
    val serverPort: StateFlow<Int> = _serverPort.asStateFlow()
    val isShowNotifications: StateFlow<Boolean> = _isShowNotifications.asStateFlow()
    val filter: StateFlow<List<String>> = _filter.asStateFlow()

    init {
        viewModelScope.launch {
            dataStoreManager.serverAddress.collectLatest { value ->
                _serverAddress.value = value
            }
            dataStoreManager.serverPort.collectLatest { value ->
                _serverPort.value = value
            }
            dataStoreManager.isShowNotifications.collectLatest { value ->
                _isShowNotifications.value = value
            }
            dataStoreManager.filter.collectLatest { value ->
                _filter.value = value
            }
        }
    }

    fun updateServerAddress(value: String) {
        _serverAddress.value = value

        viewModelScope.launch {
            dataStoreManager.setServerAddress(value)
        }
    }

    fun updateServerPort(value: Int) {
        _serverPort.value = value

        viewModelScope.launch {
            dataStoreManager.setServerPort(value)
        }
    }

    fun updateShowNotifications(value: Boolean) {
        _isShowNotifications.value = value

        viewModelScope.launch {
            dataStoreManager.setShowNotifications(value)
        }
    }

    fun updateFilter(value: List<String>) {
        _filter.value = value

        viewModelScope.launch {
            dataStoreManager.setFilter(value)
        }
    }
}
