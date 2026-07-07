package com.griscom.tinvestor_notifier.activities

import android.Manifest
import android.app.NotificationManager
import android.content.Intent
import android.content.pm.PackageManager
import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.animation.AnimatedVisibility
import androidx.compose.animation.fadeIn
import androidx.compose.animation.fadeOut
import androidx.compose.foundation.Image
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.itemsIndexed
import androidx.compose.foundation.lazy.rememberLazyListState
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.material3.Button
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.Icon
import androidx.compose.material3.IconButton
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Surface
import androidx.compose.material3.Text
import androidx.compose.material3.TextField
import androidx.compose.material3.TopAppBar
import androidx.compose.runtime.Composable
import androidx.compose.runtime.collectAsState
import androidx.compose.runtime.derivedStateOf
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.rememberCoroutineScope
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.text.style.TextAlign
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import androidx.core.app.ActivityCompat
import com.griscom.tinvestor_notifier.R
import com.griscom.tinvestor_notifier.db.NotificationEntity
import com.griscom.tinvestor_notifier.db.NotificationRepository
import com.griscom.tinvestor_notifier.db.NotificationRoomDatabase
import com.griscom.tinvestor_notifier.ui.theme.TInvestorNotifierTheme
import kotlinx.coroutines.launch
import my.nanihadesuka.compose.LazyColumnScrollbar
import my.nanihadesuka.compose.ScrollbarSettings
import java.time.Instant
import java.time.ZoneId
import java.time.format.DateTimeFormatter

private const val REQUEST_CODE_POST_NOTIFICATIONS = 1

private val CHAT_BUBBLE_SHAPE = RoundedCornerShape(4.dp, 20.dp, 20.dp, 20.dp)

private val DATE_FORMATTER =
    DateTimeFormatter
        .ofPattern("yyyy-MM-dd")
        .withZone(ZoneId.systemDefault())
private val DATE_TIME_FORMATTER =
    DateTimeFormatter
        .ofPattern("yyyy-MM-dd HH:mm:ss")
        .withZone(ZoneId.systemDefault())

val MESSAGE_TYPE_TO_STRING_MAP =
    mapOf(
        "system" to R.string.message_type_system,
        "portfolio" to R.string.message_type_portfolio,
        "huge_sell" to R.string.message_type_huge_sell,
        "dividends" to R.string.message_type_dividends,
        "pulse_neutral" to R.string.message_type_pulse_neutral,
        "pulse_buy" to R.string.message_type_pulse_buy,
        "pulse_sell" to R.string.message_type_pulse_sell,
    )

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()

        if (ActivityCompat.checkSelfPermission(
                this,
                Manifest.permission.POST_NOTIFICATIONS,
            ) != PackageManager.PERMISSION_GRANTED
        ) {
            ActivityCompat.requestPermissions(this, arrayOf(Manifest.permission.POST_NOTIFICATIONS), REQUEST_CODE_POST_NOTIFICATIONS)
        }

        val repository = NotificationRepository(NotificationRoomDatabase.getInstance(application).notificationDao())

        setContent {
            TInvestorNotifierTheme {
                val notifications by repository.notificationsListReversed.collectAsState(initial = emptyList())

                ConversationContent(notifications)
            }
        }
    }

    override fun onResume() {
        super.onResume()

        val notificationManager = getSystemService(NotificationManager::class.java) as NotificationManager
        notificationManager.cancelAll()
    }
}

@Composable
fun ConversationContent(notifications: List<NotificationEntity>) {
    val context = LocalContext.current

    var isSearchVisible by remember { mutableStateOf(false) }
    var searchText by remember { mutableStateOf("") }

    val listState = rememberLazyListState()

    val showButton by remember {
        derivedStateOf {
            listState.canScrollBackward
        }
    }

    val coroutineScope = rememberCoroutineScope()

    Scaffold(
        topBar = {
            @OptIn(ExperimentalMaterial3Api::class)
            TopAppBar(
                title = { Text(text = stringResource(R.string.app_name), fontSize = 24.sp) },
                actions = {
                    IconButton(onClick = {
                        isSearchVisible = !isSearchVisible
                        searchText = ""
                    }) {
                        Icon(
                            painter = painterResource(R.drawable.ic_search),
                            tint = MaterialTheme.colorScheme.onSurfaceVariant,
                            modifier =
                                Modifier
                                    .padding(8.dp)
                                    .height(24.dp),
                            contentDescription = stringResource(R.string.content_description_search),
                        )
                    }
                    IconButton(onClick = {
                        val intent = Intent(context, SettingsActivity::class.java)
                        context.startActivity(intent)
                    }) {
                        Icon(
                            painter = painterResource(R.drawable.ic_settings),
                            tint = MaterialTheme.colorScheme.onSurfaceVariant,
                            modifier =
                                Modifier
                                    .padding(8.dp)
                                    .height(24.dp),
                            contentDescription = stringResource(R.string.content_description_settings),
                        )
                    }
                },
            )
        },
        modifier = Modifier.fillMaxSize(),
    ) { innerPadding ->
        Box(modifier = Modifier.fillMaxSize().padding(innerPadding)) {
            Column(
                modifier = Modifier.fillMaxWidth(),
            ) {
                if (isSearchVisible) {
                    TextField(
                        value = searchText,
                        onValueChange = { searchText = it },
                        label = { Text(text = stringResource(R.string.search)) },
                        singleLine = true,
                        modifier = Modifier.fillMaxWidth(),
                    )
                }

                LazyColumnScrollbar(
                    state = listState,
                    settings = ScrollbarSettings.Default,
                ) {
                    LazyColumn(
                        state = listState,
                        reverseLayout = true,
                        modifier = Modifier.fillMaxSize(),
                    ) {
                        itemsIndexed(notifications) { index, notification ->
                            NotificationItem(notifications.getOrNull(index + 1), notification)
                        }
                    }
                }
            }

            AnimatedVisibility(
                visible = showButton,
                enter = fadeIn(),
                exit = fadeOut(),
                modifier =
                    Modifier
                        .align(Alignment.BottomCenter)
                        .padding(bottom = 16.dp),
            ) {
                Button(
                    onClick = {
                        coroutineScope.launch {
                            if (notifications.isNotEmpty()) {
                                listState.animateScrollToItem(0)
                            }
                        }
                    },
                ) {
                    Image(
                        painter = painterResource(R.drawable.ic_arrow_down),
                        modifier =
                            Modifier
                                .padding(8.dp)
                                .height(24.dp),
                        contentDescription = stringResource(R.string.content_description_scroll_down),
                    )
                }
            }
        }
    }
}

@Composable
fun NotificationItem(
    previousNotification: NotificationEntity?,
    notification: NotificationEntity,
) {
    val previousDate = if (previousNotification != null) DATE_FORMATTER.format(Instant.ofEpochMilli(previousNotification.timestamp)) else ""
    val currentDate = DATE_FORMATTER.format(Instant.ofEpochMilli(notification.timestamp))

    Surface(
        color = MaterialTheme.colorScheme.surfaceVariant,
        shape = CHAT_BUBBLE_SHAPE,
        modifier = Modifier.padding(8.dp),
    ) {
        Column(
            modifier =
                Modifier
                    .fillMaxWidth()
                    .padding(8.dp),
        ) {
            Row(
                horizontalArrangement = Arrangement.SpaceBetween,
                modifier = Modifier.fillMaxWidth(),
            ) {
                Text(
                    text = DATE_TIME_FORMATTER.format(Instant.ofEpochMilli(notification.timestamp)),
                    fontSize = 12.sp,
                    color = MaterialTheme.colorScheme.onSurface.copy(alpha = 0.6f),
                )
                Text(
                    text = stringResource(MESSAGE_TYPE_TO_STRING_MAP.getValue(notification.type)),
                    fontSize = 12.sp,
                    color = MaterialTheme.colorScheme.onSurface.copy(alpha = 0.6f),
                )
            }
            Text(
                text = notification.text,
                fontSize = 16.sp,
                modifier =
                    Modifier
                        .fillMaxWidth()
                        .padding(top = 8.dp),
            )
        }
    }

    if (previousDate != currentDate) {
        Text(
            text = currentDate,
            textAlign = TextAlign.Center,
            fontSize = 12.sp,
            color = MaterialTheme.colorScheme.onSurface.copy(alpha = 0.6f),
            modifier = Modifier.fillMaxWidth(),
        )
    }
}

@Preview
@Composable
fun ConversationContentPreview() {
    val exampleNotifications =
        listOf(
            NotificationEntity(1704402000000, "pulse_sell", "Clever", ""),
            NotificationEntity(1704319200000, "pulse_buy", "You", ""),
            NotificationEntity(1704315600000, "pulse_neutral", "Are", ""),
            NotificationEntity(1704240000000, "dividends", "Friend", ""),
            NotificationEntity(1704236400000, "huge_sell", "Dear", ""),
            NotificationEntity(1704232800000, "portfolio", "My", ""),
            NotificationEntity(1704056400000, "system", "Hello", "Some log"),
        )

    TInvestorNotifierTheme {
        ConversationContent(exampleNotifications)
    }
}

@Preview
@Composable
fun NotificationItemPreview() {
    val exampleNotification =
        NotificationEntity(
            1704056400000,
            "pulse_sell",
            "\uD83D\uDD34 Probably need to sell\n\u26A0 Attention! Text found in Pulse post: court\n{EUTR} expects for a court today!",
            "",
        )

    TInvestorNotifierTheme {
        NotificationItem(null, exampleNotification)
    }
}
