package com.griscom.tinvestor_notifier.activities

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.rememberScrollState
import androidx.compose.foundation.text.selection.SelectionContainer
import androidx.compose.foundation.verticalScroll
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.material3.TopAppBar
import androidx.compose.runtime.Composable
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import com.griscom.tinvestor_notifier.R
import com.griscom.tinvestor_notifier.db.NotificationRoomDatabase
import com.griscom.tinvestor_notifier.repositories.NotificationRepository
import com.griscom.tinvestor_notifier.ui.theme.TInvestorNotifierTheme
import my.nanihadesuka.compose.ColumnScrollbar
import my.nanihadesuka.compose.ScrollbarSettings

class LogDisplayActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()

        val repository =
            NotificationRepository(NotificationRoomDatabase.getInstance(application).notificationDao())

        val notificationId = intent.getIntExtra("NOTIFICATION_ID", 1)

        setContent {
            TInvestorNotifierTheme {
                LogDisplayContent(repository, notificationId)
            }
        }
    }
}

@Composable
fun LogDisplayContent(
    repository: NotificationRepository,
    notificationId: Int,
) {
    var text by remember { mutableStateOf("") }

    LaunchedEffect(Unit) {
        val notification = repository.getNotificationById(notificationId)
        text = notification.data
    }

    LogDisplayContentInternal(text)
}

@Composable
fun LogDisplayContentInternal(text: String) {
    val scrollState = rememberScrollState()

    Scaffold(
        topBar = {
            @OptIn(ExperimentalMaterial3Api::class)
            TopAppBar(
                title = { Text(text = stringResource(R.string.log), fontSize = 24.sp) },
            )
        },
        modifier = Modifier.fillMaxSize(),
    ) { innerPadding ->
        ColumnScrollbar(
            state = scrollState,
            settings = ScrollbarSettings.Default,
        ) {
            Column(modifier = Modifier.fillMaxWidth().verticalScroll(scrollState).padding(innerPadding)) {
                SelectionContainer {
                    Text(
                        text = text,
                        fontSize = 16.sp,
                        modifier = Modifier.padding(16.dp),
                    )
                }
            }
        }
    }
}

@Preview
@Composable
fun LogDisplayContentInternalPreview() {
    TInvestorNotifierTheme {
        LogDisplayContentInternal(
            """
            This is a multiline text block in Jetpack Compose.
            You can long-press anywhere on this paragraph to bring up
            the native selection handles and copy the text.
            It automatically wraps lines based on the available width.
            """.trimIndent().repeat(50),
        )
    }
}
