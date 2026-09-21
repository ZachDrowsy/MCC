#pragma once

#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <ESPmDNS.h>
#include <Preferences.h>
#include "web_ui.h"

static WebServer server(80);
static DNSServer dnsServer;
static Preferences preferences;

static const char* SETUP_WIFI_NAME = "MCC-Setup";
static const char* HOST_NAME = "microclimatecontroller";

// Most ESP32 Dev boards use GPIO 0 for the BOOT button.
static const int WIFI_RESET_PIN = 0;
static const unsigned long WIFI_RESET_HOLD_MS = 5000;

static bool setupMode = false;
static unsigned long wifiResetPressStart = 0;

static String getSavedSSID()
{
    preferences.begin("wifi", true);
    String ssid = preferences.getString("ssid", "");
    preferences.end();
    return ssid;
}

static String getSavedPassword()
{
    preferences.begin("wifi", true);
    String password = preferences.getString("password", "");
    preferences.end();
    return password;
}

static void saveWiFi(const String& ssid, const String& password)
{
    preferences.begin("wifi", false);
    preferences.putString("ssid", ssid);
    preferences.putString("password", password);
    preferences.end();
}

static void clearSavedWiFi()
{
    preferences.begin("wifi", false);
    preferences.clear();
    preferences.end();
}

static void beginWiFiResetButton()
{
    pinMode(WIFI_RESET_PIN, INPUT_PULLUP);
}

static void handleWiFiResetButton()
{
    const bool pressed = digitalRead(WIFI_RESET_PIN) == LOW;

    if (pressed)
    {
        if (wifiResetPressStart == 0)
            wifiResetPressStart = millis();

        if (millis() - wifiResetPressStart >= WIFI_RESET_HOLD_MS)
        {
            Serial.println("BOOT held for 5 seconds: clearing saved Wi-Fi.");
            clearSavedWiFi();
            delay(300);
            ESP.restart();
        }
    }
    else
    {
        wifiResetPressStart = 0;
    }
}

static String buildSetupPage()
{
    String page = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>MCC Setup</title>
<style>
body{font-family:Arial,sans-serif;background:#f4f6f4;margin:0;padding:20px}
.container{max-width:420px;margin:auto;padding:25px;background:white;border-radius:18px;box-shadow:0 4px 18px rgba(0,0,0,.12)}
select,input,button{width:100%;box-sizing:border-box;padding:12px;margin-top:10px;font-size:16px}
button{background:#2f7d4a;color:white;border:none;border-radius:10px}
</style>
</head>
<body>
<div class="container">
<h2>MicroClimateController Setup</h2>
<p>Select the Wi-Fi network this controller should use.</p>
<form action="/save-wifi" method="POST">
<select name="ssid" required>
)rawliteral";

    int networkCount = WiFi.scanNetworks();
    if (networkCount == 0)
    {
        page += "<option value=''>No networks found</option>";
    }
    else
    {
        for (int i = 0; i < networkCount; ++i)
        {
            String ssid = WiFi.SSID(i);
            page += "<option value='" + ssid + "'>" + ssid + "</option>";
        }
    }

    page += R"rawliteral(
</select>
<input type="password" name="password" placeholder="Wi-Fi password">
<button type="submit">Connect Controller</button>
</form>
</div>
</body>
</html>
)rawliteral";

    return page;
}

static void redirectToSetupPortal()
{
    String setupURL = "http://" + WiFi.softAPIP().toString() + "/";

    server.sendHeader("Location", setupURL, true);
    server.send(302, "text/plain", "");
}


static void startSetupMode()
{
    setupMode = true;

    Serial.println();
    Serial.println("Starting MCC setup mode...");

    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(SETUP_WIFI_NAME);

    IPAddress setupIP = WiFi.softAPIP();

    Serial.print("Connect phone to: ");
    Serial.println(SETUP_WIFI_NAME);

    Serial.print("Setup IP: ");
    Serial.println(setupIP);

    // Redirect all DNS requests to the ESP32 while in setup mode.
    dnsServer.start(53, "*", setupIP);

    // Main setup page.
    server.on("/", HTTP_GET, []()
    {
        server.send(200, "text/html", buildSetupPage());
    });

    // Save selected Wi-Fi credentials.
    server.on("/save-wifi", HTTP_POST, []()
    {
        String ssid = server.arg("ssid");
        String password = server.arg("password");

        if (ssid.length() == 0)
        {
            server.send(
                400,
                "text/plain",
                "No Wi-Fi network selected."
            );

            return;
        }

        saveWiFi(ssid, password);

        server.send(
            200,
            "text/html",
            "<h2>Wi-Fi Saved</h2>"
            "<p>The controller is restarting.</p>"
            "<p>Reconnect your phone to your normal Wi-Fi.</p>"
            "<p>Then open:</p>"
            "<h3>http://mcc.local</h3>"
        );

        delay(2000);
        ESP.restart();
    });

    // --------------------------------------------------
    // CAPTIVE PORTAL DETECTION
    // --------------------------------------------------
    // Phones automatically request these addresses when they join Wi-Fi.
    // Redirecting them to the ESP32 setup page makes the network behave
    // like a hotel/guest Wi-Fi captive portal.

    // Apple / iPhone / iPad / macOS
    server.on("/hotspot-detect.html", HTTP_GET, redirectToSetupPortal);
    server.on("/library/test/success.html", HTTP_GET, redirectToSetupPortal);

    // Android
    server.on("/generate_204", HTTP_GET, redirectToSetupPortal);
    server.on("/gen_204", HTTP_GET, redirectToSetupPortal);

    // Windows
    server.on("/connecttest.txt", HTTP_GET, redirectToSetupPortal);
    server.on("/ncsi.txt", HTTP_GET, redirectToSetupPortal);

    // Any other requested page also gets sent to the setup page.
    server.onNotFound(redirectToSetupPortal);

    server.begin();

    Serial.println("Setup website started.");
}


static bool connectToSavedWiFi()
{
    String ssid = getSavedSSID();
    String password = getSavedPassword();

    if (ssid.length() == 0)
        return false;

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 30)
    {
        delay(500);
        ++attempts;
    }

    return WiFi.status() == WL_CONNECTED;
}

static void startMainWebsite()
{
    setupMode = false;

    if (MDNS.begin(HOST_NAME))
        Serial.println("Open: http://mcc.local");

    server.on("/", HTTP_GET, []()
    {
        server.send_P(200, "text/html", WEB_UI);
    });

    server.on("/reset-wifi", HTTP_GET, []()
    {
        clearSavedWiFi();
        server.send(200, "text/html", "<h2>Wi-Fi erased.</h2><p>Restarting...</p>");
        delay(1500);
        ESP.restart();
    });

    server.begin();
}

void startMCCNetwork()
{
    beginWiFiResetButton();

    if (connectToSavedWiFi())
        startMainWebsite();
    else
        startSetupMode();
}

void handleMCCNetwork()
{
    handleWiFiResetButton();

    if (setupMode)
        dnsServer.processNextRequest();

    server.handleClient();
}
