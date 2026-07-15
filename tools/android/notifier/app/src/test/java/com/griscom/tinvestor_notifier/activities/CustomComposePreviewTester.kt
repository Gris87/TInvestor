package com.griscom.tinvestor_notifier.activities

import com.dropbox.differ.SimpleImageComparator
import com.github.takahirom.roborazzi.AndroidComposePreviewTester
import com.github.takahirom.roborazzi.ComposePreviewTester
import com.github.takahirom.roborazzi.ComposePreviewTester.TestParameter.JUnit4TestParameter.AndroidPreviewJUnit4TestParameter
import com.github.takahirom.roborazzi.ExperimentalRoborazziApi

@OptIn(ExperimentalRoborazziApi::class)
class CustomComposePreviewTester :
    ComposePreviewTester<AndroidPreviewJUnit4TestParameter> by AndroidComposePreviewTester(
        capturer = { parameter ->
            val customOptions =
                parameter.roborazziOptions.copy(
                    compareOptions =
                        parameter.roborazziOptions.compareOptions.copy(
                            // Set custom comparison threshold (0.0 = exact match, 1.0 = ignore differences)
                            imageComparator = SimpleImageComparator(maxDistance = 0.007F),
                        ),
                )
            AndroidComposePreviewTester.DefaultCapturer().capture(
                parameter.copy(roborazziOptions = customOptions),
            )
        },
    )
