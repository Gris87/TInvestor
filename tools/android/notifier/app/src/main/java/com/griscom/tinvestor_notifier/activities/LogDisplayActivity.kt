package com.griscom.tinvestor_notifier.activities

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.ui.tooling.preview.Preview
import com.griscom.tinvestor_notifier.ui.theme.TInvestorNotifierTheme

class LogDisplayActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()

        setContent {
            TInvestorNotifierTheme {
                LogDisplayContent()
            }
        }
    }
}

@Composable
fun LogDisplayContent() {
}

@Preview
@Composable
fun LogDisplayContentPreview() {
    TInvestorNotifierTheme {
        LogDisplayContent()
    }
}
