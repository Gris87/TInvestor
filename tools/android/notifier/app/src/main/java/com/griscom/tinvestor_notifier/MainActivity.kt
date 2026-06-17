package com.griscom.tinvestor_notifier

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.PaddingValues
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.Icon
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.material3.TopAppBar
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import com.griscom.tinvestor_notifier.ui.theme.TInvestorNotifierTheme

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()

        setContent {
            TInvestorNotifierTheme {
                ConversationContent()
            }
        }
    }
}

@Composable
fun ConversationContent() {
    Scaffold(
        modifier = Modifier.fillMaxSize(),
        topBar = { TopBar() }
    ) { innerPadding ->
        ScrollContent(innerPadding)
    }
}

@Composable
fun TopBar() {
    @OptIn(ExperimentalMaterial3Api::class)
    TopAppBar(
        title = { Text(stringResource(R.string.app_name)) },
        actions={
            Icon(
                painterResource(R.drawable.ic_search),
                tint = MaterialTheme.colorScheme.onSurfaceVariant,
                modifier = Modifier
                    .padding(8.dp)
                    .height(24.dp),
                contentDescription = stringResource(R.string.content_description_search),
            )
        }
    )
}

@Composable
fun ScrollContent(innerPadding: PaddingValues) {
    Column(Modifier.padding(innerPadding)) {
        Text("fdfh")
    }
}

@Preview
@Composable
fun ConversationContentPreview() {
    TInvestorNotifierTheme {
        ConversationContent()
    }
}

@Preview
@Composable
fun TopBarPreview() {
    TInvestorNotifierTheme {
        TopBar()
    }
}

@Preview
@Composable
fun ScrollContentPreview() {
    TInvestorNotifierTheme {
        ScrollContent(PaddingValues())
    }
}
