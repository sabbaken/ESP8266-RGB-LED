/* 
void ChangePalettePeriodically()
{
        if( selector   ==  0)   { SetupTotallyRandomPalette();               currentBlending = NOBLEND; }
        if( selector   ==  1)  { SetupTotallyRandomPalette();               currentBlending = LINEARBLEND; }
        if( selector   ==  2)  { SetupBlackAndWhiteStripedPalette();               currentBlending = NOBLEND; }
        if( selector   ==  3)  { SetupBlackAndWhiteStripedPalette();               currentBlending = LINEARBLEND; }
        if( selector   ==  4)  { currentPalette = RainbowColors_p;         currentBlending = LINEARBLEND; }
        if( selector   ==  5)  { currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;     }
        if( selector   ==  6)  { currentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND; }
        if( selector   ==  7)  { SetupPurpleAndGreenPalette();             currentBlending = LINEARBLEND; }
        if( selector   ==  8)  { SetupTotallyRandomPalette();              currentBlending = LINEARBLEND; }
        if( selector   ==  9)  { SetupBlackAndWhiteStripedPalette();       currentBlending = NOBLEND;     }
        if( selector   ==  10)  { SetupBlackAndWhiteStripedPalette();       currentBlending = LINEARBLEND; }
        if( selector   ==  11)  { currentPalette = CloudColors_p;           currentBlending = LINEARBLEND; }
        if( selector   ==  12)  { currentPalette = PartyColors_p;           currentBlending = LINEARBLEND; }
        if( selector   ==  13)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = NOBLEND;     }
        if( selector   ==  14)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = LINEARBLEND; }

}
 */


/*===========[Reciving data from MQTT]=============*/
/*void callback(char* topic, byte* payload, unsigned int length)
{

    String payload = pub.payload_string();
    if( String( pub.topic() ) == "LED/1/Color" ){ 
       one_color_all (payload.toInt() , 20 , 20 ,true);
    }
}

void conectMQTT(){
    if (WiFi.status() == WL_CONNECTED) {
        if (!client.connected()) {
            if (client.connect(MQTT::Connect("arduinoClient2").set_auth(mqtt_user, mqtt_pass))) {
    
    client.set_callback(callback);
    if (client.subscribe("LED/1/Color") == 1)
               { one_color_all(39, 255, 90, true);
        } else { one_color_all(255, 57, 43, true);      }
    }
}


}*/
