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
import androidx.compose.ui.Modifier
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import com.griscom.tinvestor_notifier.R
import com.griscom.tinvestor_notifier.ui.theme.TInvestorNotifierTheme
import my.nanihadesuka.compose.ColumnScrollbar
import my.nanihadesuka.compose.ScrollbarSettings

class LogDisplayActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()

        val text = intent.getStringExtra("LOG_KEY") ?: ""

        setContent {
            TInvestorNotifierTheme {
                LogDisplayContent(text)
            }
        }
    }
}

@Composable
fun LogDisplayContent(text: String) {
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
fun LogDisplayContentPreview() {
    TInvestorNotifierTheme {
        LogDisplayContent(
            """
            This is a multiline text block in Jetpack Compose.
            You can long-press anywhere on this paragraph to bring up 
            the native selection handles and copy the text.
            It automatically wraps lines based on the available width.
            """.trimIndent().repeat(50),
        )
    }
}
