package com.griscom.tinvestor_notifier.activities

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.rememberScrollState
import androidx.compose.foundation.selection.toggleable
import androidx.compose.foundation.text.KeyboardOptions
import androidx.compose.foundation.verticalScroll
import androidx.compose.material3.Checkbox
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Switch
import androidx.compose.material3.Text
import androidx.compose.material3.TextField
import androidx.compose.material3.TopAppBar
import androidx.compose.material3.TriStateCheckbox
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateListOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.rememberCoroutineScope
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.platform.testTag
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.semantics.Role
import androidx.compose.ui.state.ToggleableState
import androidx.compose.ui.text.input.KeyboardType
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import androidx.lifecycle.compose.collectAsStateWithLifecycle
import com.griscom.tinvestor_notifier.R
import com.griscom.tinvestor_notifier.datastore.DataStoreManager
import com.griscom.tinvestor_notifier.ui.theme.TInvestorNotifierTheme
import kotlinx.coroutines.launch

private val MESSAGE_TYPES_LIST = listOf("system", "portfolio", "huge_sell", "dividends", "pulse_neutral", "pulse_buy", "pulse_sell")

class SettingsActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()

        val viewModel = SettingsViewModel(DataStoreManager(this))

        setContent {
            TInvestorNotifierTheme {
                SettingsContent(viewModel)
            }
        }
    }
}

@Composable
fun SettingsContent(viewModel: SettingsViewModel) {
    val context = LocalContext.current

    val dataStore =
        remember {
            DataStoreManager(context)
        }

    val serverAddress by viewModel.serverAddress.collectAsStateWithLifecycle()
    val serverPort by viewModel.serverPort.collectAsStateWithLifecycle()
    val isShowNotifications by dataStore.isShowNotifications.collectAsStateWithLifecycle(true)
    val filter by dataStore.filter.collectAsStateWithLifecycle(listOf("system", "portfolio", "huge_sell", "dividends"))

    val filterStates =
        remember(filter) {
            mutableStateListOf<Boolean>().apply {
                addAll(MESSAGE_TYPES_LIST.map { it in filter })
            }
        }

    val parentState =
        when {
            filterStates.all { it } -> ToggleableState.On
            filterStates.none { it } -> ToggleableState.Off
            else -> ToggleableState.Indeterminate
        }

    val scrollState = rememberScrollState()

    val coroutineScope = rememberCoroutineScope()

    Scaffold(
        topBar = {
            @OptIn(ExperimentalMaterial3Api::class)
            TopAppBar(
                title = { Text(text = stringResource(R.string.settings), fontSize = 24.sp) },
            )
        },
        modifier = Modifier.fillMaxSize(),
    ) { innerPadding ->
        Column(modifier = Modifier.fillMaxWidth().verticalScroll(scrollState).padding(innerPadding)) {
            TextField(
                value = serverAddress,
                onValueChange = { newValue ->
                    viewModel.updateServerAddress(newValue)
                },
                label = { Text(text = stringResource(R.string.server_address), fontSize = 16.sp) },
                singleLine = true,
                modifier = Modifier.fillMaxWidth().padding(8.dp).testTag("server_address_field"),
            )

            TextField(
                value = serverPort.toString(),
                onValueChange = { newValue ->
                    if (newValue.all { it.isDigit() }) {
                        viewModel.updateServerPort(newValue.toIntOrNull() ?: 0)
                    }
                },
                label = { Text(text = stringResource(R.string.server_port), fontSize = 16.sp) },
                keyboardOptions = KeyboardOptions(keyboardType = KeyboardType.Number),
                singleLine = true,
                modifier = Modifier.fillMaxWidth().padding(8.dp).testTag("server_port_field"),
            )

            Row(
                verticalAlignment = Alignment.CenterVertically,
                modifier = Modifier.fillMaxWidth().padding(8.dp),
            ) {
                Text(
                    text = stringResource(R.string.show_notifications),
                    fontSize = 16.sp,
                    modifier = Modifier.padding(0.dp, 0.dp, 8.dp, 0.dp),
                )
                Switch(
                    checked = isShowNotifications,
                    onCheckedChange = { enabled ->
                        coroutineScope.launch {
                            dataStore.setShowNotifications(enabled)
                        }
                    },
                )
            }

            Row(
                horizontalArrangement = Arrangement.SpaceBetween,
                verticalAlignment = Alignment.CenterVertically,
                modifier = Modifier.fillMaxWidth().padding(8.dp),
            ) {
                Text(
                    text = stringResource(R.string.filter),
                    fontSize = 20.sp,
                )
                Row(
                    verticalAlignment = Alignment.CenterVertically,
                ) {
                    Text(
                        text = stringResource(R.string.select_all),
                        fontSize = 16.sp,
                    )
                    TriStateCheckbox(
                        state = parentState,
                        onClick = {
                            val newState = parentState != ToggleableState.On

                            filterStates.forEachIndexed { index, _ ->
                                filterStates[index] = newState
                            }

                            coroutineScope.launch {
                                dataStore.setFilter(if (newState) MESSAGE_TYPES_LIST else emptyList())
                            }
                        },
                    )
                }
            }

            filterStates.forEachIndexed { index, checked ->
                Row(
                    modifier =
                        Modifier
                            .fillMaxWidth()
                            .toggleable(
                                value = checked,
                                onValueChange = { isChecked ->
                                    filterStates[index] = isChecked

                                    val newFilter = mutableListOf<String>()

                                    filterStates.forEachIndexed { index, state ->
                                        if (state) {
                                            newFilter.add(MESSAGE_TYPES_LIST[index])
                                        }
                                    }

                                    coroutineScope.launch {
                                        dataStore.setFilter(newFilter)
                                    }
                                },
                                role = Role.Checkbox, // Changes to Role.Switch for a Switch
                            ),
                    verticalAlignment = Alignment.CenterVertically,
                ) {
                    Checkbox(
                        checked = checked,
                        onCheckedChange = null,
                        modifier = Modifier.padding(8.dp),
                    )
                    Text(
                        text =
                            stringResource(
                                MESSAGE_TYPE_TO_STRING_MAP.getOrDefault(MESSAGE_TYPES_LIST[index], R.string.message_type_system),
                            ),
                        fontSize = 16.sp,
                    )
                }
            }
        }
    }
}

@Preview
@Composable
fun SettingsContentPreview() {
    val viewModel = SettingsViewModel(DataStoreManager(LocalContext.current))

    TInvestorNotifierTheme {
        SettingsContent(viewModel)
    }
}
