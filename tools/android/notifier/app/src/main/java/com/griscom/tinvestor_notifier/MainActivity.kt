package com.griscom.tinvestor_notifier

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.PaddingValues
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.items
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.Icon
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Surface
import androidx.compose.material3.Text
import androidx.compose.material3.TopAppBar
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import com.griscom.tinvestor_notifier.data.Message
import com.griscom.tinvestor_notifier.data.MessageType
import com.griscom.tinvestor_notifier.ui.theme.TInvestorNotifierTheme

private val ChatBubbleShape = RoundedCornerShape(4.dp, 20.dp, 20.dp, 20.dp)

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()

        val exampleMessages = listOf(
            Message("2026-01-01 10:00:00", MessageType.SYSTEM, R.string.message_type_system,"Hello"),
            Message("2026-01-01 11:00:00", MessageType.PORTFOLIO, R.string.message_type_portfolio,"My"),
            Message("2026-01-01 12:00:00", MessageType.HUGE_SELL, R.string.message_type_huge_sell,"Dear"),
            Message("2026-01-02 10:00:00", MessageType.DIVIDENDS, R.string.message_type_dividends,"Friend"),
            Message("2026-01-03 10:00:00", MessageType.PULSE_NEUTRAL, R.string.message_type_pulse_neutral,"Are"),
            Message("2026-01-03 11:00:00", MessageType.PULSE_BUY, R.string.message_type_pulse_buy,"You"),
            Message("2026-01-04 10:00:00", MessageType.PULSE_SELL, R.string.message_type_pulse_sell,"Clever")
        )

        setContent {
            TInvestorNotifierTheme {
                ConversationContent(exampleMessages)
            }
        }
    }
}

@Composable
fun ConversationContent(messages: List<Message>) {
    Scaffold(
        topBar = { TopBar() },
        modifier = Modifier.fillMaxSize()
    ) { innerPadding ->
        ScrollContent(innerPadding, messages)
    }
}

@Composable
fun TopBar() {
    @OptIn(ExperimentalMaterial3Api::class)
    TopAppBar(
        title = { Text(text = stringResource(R.string.app_name), fontSize = 24.sp) },
        actions={
            Icon(
                painter = painterResource(R.drawable.ic_search),
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
fun ScrollContent(innerPadding: PaddingValues, messages: List<Message>) {
    LazyColumn(modifier = Modifier.padding(innerPadding)) {
        items(messages) { message ->
            MessageItem(message)
        }
    }
}

@Composable
fun MessageItem(message: Message) {
    Surface(
        color = MaterialTheme.colorScheme.surfaceVariant,
        shape = ChatBubbleShape,
        modifier = Modifier.padding(8.dp)
    ) {
        Column(
            modifier = Modifier
                .fillMaxWidth()
                .padding(8.dp)
        ) {
            Row(
                horizontalArrangement = Arrangement.SpaceBetween,
                modifier = Modifier.fillMaxWidth()
            ) {
                Text(
                    text = message.timestamp,
                    fontSize = 12.sp,
                    color = MaterialTheme.colorScheme.onSurface.copy(alpha = 0.6f)
                )
                Text(
                    text = stringResource(message.messageTypeString),
                    fontSize = 12.sp,
                    color = MaterialTheme.colorScheme.onSurface.copy(alpha = 0.6f)
                )
            }
            Text(
                text = message.text,
                fontSize = 16.sp,
                modifier = Modifier
                    .fillMaxWidth()
                    .padding(top = 8.dp)
            )
        }
    }
}

@Preview
@Composable
fun ConversationContentPreview() {
    val exampleMessages = listOf(
        Message("2026-01-01 10:00:00", MessageType.SYSTEM, R.string.message_type_system,"Hello"),
        Message("2026-01-01 11:00:00", MessageType.PORTFOLIO, R.string.message_type_portfolio,"My"),
        Message("2026-01-01 12:00:00", MessageType.HUGE_SELL, R.string.message_type_huge_sell,"Dear"),
        Message("2026-01-02 10:00:00", MessageType.DIVIDENDS, R.string.message_type_dividends,"Friend"),
        Message("2026-01-03 10:00:00", MessageType.PULSE_NEUTRAL, R.string.message_type_pulse_neutral,"Are"),
        Message("2026-01-03 11:00:00", MessageType.PULSE_BUY, R.string.message_type_pulse_buy,"You"),
        Message("2026-01-04 10:00:00", MessageType.PULSE_SELL, R.string.message_type_pulse_sell,"Clever")
    )

    TInvestorNotifierTheme {
        ConversationContent(exampleMessages)
    }
}

@Preview
@Composable
fun MessageItemPreview() {
    val exampleMessage = Message(
        "2026-01-04 10:00:00",
        MessageType.PULSE_SELL,
        R.string.message_type_pulse_sell,
        "\uD83D\uDD34 Probably need to sell\n\u26A0 Attention! Text found in Pulse post: court\n{EUTR} expects for a court today!"
    )

    TInvestorNotifierTheme {
        MessageItem(exampleMessage)
    }
}
