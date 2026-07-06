package com.griscom.tinvestor_notifier.activities

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.PaddingValues
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.text.KeyboardOptions
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Switch
import androidx.compose.material3.Text
import androidx.compose.material3.TextField
import androidx.compose.material3.TopAppBar
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.runtime.remember
import androidx.compose.runtime.rememberCoroutineScope
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.text.input.KeyboardType
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import androidx.lifecycle.compose.collectAsStateWithLifecycle
import com.griscom.tinvestor_notifier.R
import com.griscom.tinvestor_notifier.datastore.DataStoreManager
import com.griscom.tinvestor_notifier.ui.theme.TInvestorNotifierTheme
import kotlinx.coroutines.launch

class SettingsActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()

        setContent {
            TInvestorNotifierTheme {
                SettingsContent()
            }
        }
    }
}

@Composable
fun SettingsContent() {
    Scaffold(
        topBar = { SettingsTopBar() },
        modifier = Modifier.fillMaxSize(),
    ) { innerPadding ->
        ScrollContent(innerPadding)
    }
}

@Composable
fun SettingsTopBar() {
    @OptIn(ExperimentalMaterial3Api::class)
    TopAppBar(
        title = { Text(text = stringResource(R.string.settings), fontSize = 24.sp) },
    )
}

@Composable
fun ScrollContent(innerPadding: PaddingValues) {
    val context = LocalContext.current

    val dataStore =
        remember {
            DataStoreManager(context)
        }

    val serverAddress by dataStore.serverAddress.collectAsStateWithLifecycle("localhost")
    val serverPort by dataStore.serverPort.collectAsStateWithLifecycle(8041)
    val isShowNotifications by dataStore.isShowNotifications.collectAsStateWithLifecycle(true)

    val coroutineScope = rememberCoroutineScope()

    Column(modifier = Modifier.fillMaxSize().padding(innerPadding)) {
        TextField(
            value = serverAddress,
            label = { Text(text = stringResource(R.string.server_address), fontSize = 16.sp) },
            onValueChange = { newValue ->
                coroutineScope.launch {
                    dataStore.setServerAddress(newValue)
                }
            },
            modifier = Modifier.fillMaxWidth().padding(8.dp),
        )
        TextField(
            value = serverPort.toString(),
            label = { Text(text = stringResource(R.string.server_port), fontSize = 16.sp) },
            keyboardOptions = KeyboardOptions(keyboardType = KeyboardType.Number),
            onValueChange = { newValue ->
                coroutineScope.launch {
                    if (newValue.all { it.isDigit() }) {
                        dataStore.setServerPort(newValue.toIntOrNull() ?: 0)
                    }
                }
            },
            modifier = Modifier.fillMaxWidth().padding(8.dp),
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
        Text(
            text = stringResource(R.string.filter),
            fontSize = 20.sp,
            modifier = Modifier.fillMaxWidth().padding(8.dp),
        )
    }
}

@Preview
@Composable
fun SettingsContentPreview() {
    TInvestorNotifierTheme {
        SettingsContent()
    }
}
