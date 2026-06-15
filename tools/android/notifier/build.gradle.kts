// Top-level build file where you can add configuration options common to all sub-projects/modules.
plugins {
    alias(libs.plugins.android.application) apply false
    alias(libs.plugins.kotlin.compose) apply false
}

android {
    buildTypes {
        debug {
            // Enable coverage for unit tests
            enableUnitTestCoverage = true
            // Enable coverage for instrumentation tests
            enableAndroidTestCoverage = true
        }
    }
}